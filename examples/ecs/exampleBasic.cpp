#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <random>
#include <iomanip>
#include "ecs/Coordinator.hpp"
#include "ecs/QuerySystem.hpp"
#include "ecs/GroupSystem.hpp"

// Component Definitions
struct Position {
    float x = 0.0f;
    float y = 0.0f;
};

struct Velocity {
    float x = 0.0f;
    float y = 0.0f;
};

void log(const std::string& message)
{
    std::cout << message << std::endl;
}

// This query system will increment the position component by the velocity component for each entity having both
// Position is marked as a write component and Velocity as a read component
// This enforces constness at compile time to prevent accidental modification of Velocity
// The query system induces a small performance overhead because of the indirection required to access the components
// (because the entities we are iterating on does not necessarily have contiguous components in memory)
// This should be used when you want to create a group system that does not own any components
class QueryBenchmarkSystem : public nexo::ecs::QuerySystem<
    nexo::ecs::Write<Position>,
    nexo::ecs::Read<Velocity>
> {
	public:
	    void runBenchmark()
	    {
			log("Running query system benchmarks with " + std::to_string(entities.size()) + " entities");
	        constexpr int NUM_ITERATIONS = 100;
	        auto queryTime = benchmarkQuery(NUM_ITERATIONS);
	        log("Query System: " + std::to_string(queryTime) + " milliseconds per iteration");
	    }

	private:
	    double benchmarkQuery(int numIterations)
	    {
	        auto start = std::chrono::high_resolution_clock::now();

	        for (int i = 0; i < numIterations; i++) {
	            for (nexo::ecs::Entity entity : entities) {
	                auto &position = getComponent<Position>(entity);
	                auto &velocity = getComponent<Velocity>(entity);

	                // This triggers a compiler error since Velocity is marked as read-only
	                //velocity.x += 1;

	                position.x += velocity.x;
	                position.y += velocity.y;
	            }
	        }

	        auto end = std::chrono::high_resolution_clock::now();
	        std::chrono::duration<double, std::milli> duration = end - start;
	        return duration.count() / numIterations;
	    }
};

// This a basic full-owning group system
// At startup, the system will automatically create a group of entities with Position and Velocity components
// Then we can safely iterate over the group and update the position of each entity
// Those system induces a huge overhead when you are adding/removing components or destroying entities often
// So make sure to use them wisely and avoid unnecessary operations.
// But in most case, those are blazingly fast
// If unsure, you can try both a query system and a group system to test out what is best for your use case !
class GroupBenchmarkSystem : public nexo::ecs::GroupSystem<
    nexo::ecs::Owned<
        nexo::ecs::Write<Position>,
        nexo::ecs::Read<Velocity>
    >,
    nexo::ecs::NonOwned<>
> {
	public:
	    void runBenchmark()
	    {
	        log("Running benchmarks with " + std::to_string(m_group->size()) + " entities");

	        constexpr int NUM_ITERATIONS = 100;

	        // Benchmark each approach
	        auto eachTime = benchmarkEach(NUM_ITERATIONS);
	        log("Each method: " + std::to_string(eachTime) + " milliseconds per iteration");

	        // Benchmark spans approach
	        auto spansTime = benchmarkSpans(NUM_ITERATIONS);
	        log("Spans method: " + std::to_string(spansTime) + " milliseconds per iteration");

	        // Benchmark iterator approach
	        auto iteratorTime = benchmarkIterator(NUM_ITERATIONS);
	        log("Iterator method: " + std::to_string(iteratorTime) + " milliseconds per iteration");
	    }

	private:
	    double benchmarkIterator(int numIterations)
	    {
	    	// Method 1: Using the iterator, slowest solution of the three but more verbose
	     	// Also not fully recommend since it does not enforce the constness for read-only components
	        auto start = std::chrono::high_resolution_clock::now();

	        for (int i = 0; i < numIterations; i++) {
	            for (const auto &[entity, position, velocity] : *m_group) {
	                position.x += velocity.x;
	                position.y += velocity.y;

	                // This is highly dangerous since velocity is read-only and this is not enforced at compile time,
	                // we advise to use it at user's discretion
	                // velocity.x += 1;
	            }
	        }

	        auto end = std::chrono::high_resolution_clock::now();
	        std::chrono::duration<double, std::milli> duration = end - start;
	        return duration.count() / numIterations;
	    }

	    double benchmarkEach(int numIterations)
		{
	    	// Method 2: Using the each method, faster than the iterator, but does not necessarly enforce constness if not mentionned
	        auto start = std::chrono::high_resolution_clock::now();

	        for (int i = 0; i < numIterations; i++) {
	            m_group->each([](nexo::ecs::Entity entity, Position& position, const Velocity& velocity) {
	                position.x += velocity.x;
	                position.y += velocity.y;

	                // This triggers a compilation error since we are using a const reference to velocity in the lambda function
	                // velocity.x += 1;
	            });

	            // But here, this would compile even though the user forgot to mention the const in the lambda function,
				// which can be problematic in multihreaded systems
	            // m_group->each([](nexo::ecs::Entity entity, Position& position, Velocity& velocity) {
	            //     position.x += velocity.x;
	            //     position.y += velocity.y;

	            //     velocity.x += 1;
	            // });
	        }

	        auto end = std::chrono::high_resolution_clock::now();
	        std::chrono::duration<double, std::milli> duration = end - start;
	        return duration.count() / numIterations;
	    }

	    double benchmarkSpans(int numIterations)
		{
	    	// Method 3: Using the spans directly, this is the fasted method of the three
	     	// Also, it automatically enforces constness on read-only components
	      	// This solution should be your preferred one
	        auto start = std::chrono::high_resolution_clock::now();

	        for (int i = 0; i < numIterations; i++) {
	            auto positionSpan = get<Position>();
	            // Constness is not enforced on the span itself since it is basically a non-owning view of the underlying data.
	            // But we consider it to be good practice to make more explicit that we are using read-only components
	            const auto velocitySpan = get<Velocity>();

	            size_t size = positionSpan.size();
	            for (size_t j = 0; j < size; ++j) {
	                auto& position = positionSpan[j];
	                auto& velocity = velocitySpan[j];

	                // Of course for more clarity you should write:
	                // const auto& velocity = velocitySpan[j];
	                // but here we are demonstrating that even if the user forgets to mark velocity as const,
	                // the compiler will raise an error for read-only components

	                // This triggers a compilation error since velocity is read-only even though we did not explicitly mark it as const
	                // velocity.x += 1;

	                position.x += velocity.x;
	                position.y += velocity.y;
	            }
	        }

	        auto end = std::chrono::high_resolution_clock::now();
	        std::chrono::duration<double, std::milli> duration = end - start;
	        return duration.count() / numIterations;
	    }
};

int main() {
    // Initialize ECS Coordinator
    nexo::ecs::Coordinator coordinator;
    coordinator.init();

    log("ECS initialized");

    // Register components
    coordinator.registerComponent<Position>();
    coordinator.registerComponent<Velocity>();

    log("Components registered");

    // Register benchmark systems
    auto queryBenchmarkSystem = coordinator.registerQuerySystem<QueryBenchmarkSystem>();
    auto groupBenchmarkSystem = coordinator.registerGroupSystem<GroupBenchmarkSystem>();

    log("Benchmark systems registered");

    // Create 5,000 entities for benchmarking
    log("Creating 5,000 entities for benchmarking...");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> velocityDist(-10.0, 10.0);

    std::vector<nexo::ecs::Entity> entities;
    entities.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        nexo::ecs::Entity entity = coordinator.createEntity();

        float velX = static_cast<float>(velocityDist(gen));
        float velY = static_cast<float>(velocityDist(gen));

        coordinator.addComponent(entity, Position{0.0f, 0.0f});
        coordinator.addComponent(entity, Velocity{velX, velY});

        entities.push_back(entity);
    }

    log("Created " + std::to_string(entities.size()) + " entities");

    // Run the benchmarks
    log("\n=== Starting QuerySystem Benchmark ===");
    queryBenchmarkSystem->runBenchmark();
    log("=== QuerySystem Benchmark Complete ===");

    log("\n=== Starting GroupSystem Benchmark ===");
    groupBenchmarkSystem->runBenchmark();
    log("=== GroupSystem Benchmark Complete ===");

    return 0;
}
