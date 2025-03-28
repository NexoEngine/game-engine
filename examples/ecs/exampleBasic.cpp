//// exampleBasic.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        27/11/2024
//  Description: Source file for the basic ecs example
//
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <chrono>
#include "ecs/Coordinator.hpp"

#include <cstdlib>
#include <iostream>
#include <random>
#include <mutex>

struct Position {
    float x;
    float y;
    float z;
};

struct Velocity {
    float x;
    float y;
    float z;
};


class MovementSystem : public nexo::ecs::System {
	public:

		MovementSystem()
		{
			positionArray = coord->getComponentArray<Position>();
			velocityArray = coord->getComponentArray<Velocity>();
		}

		void update(float deltaTime)
		{
            for (auto entity : entities) {
                auto& position = positionArray->getData(entity);
                auto& velocity = velocityArray->getData(entity);

                // Update position using velocity
                position.x += velocity.x * deltaTime;
                position.y += velocity.y * deltaTime;
                position.z += velocity.z * deltaTime;
            }
        }

    private:
    	std::shared_ptr<nexo::ecs::ComponentArray<Position>> positionArray;
    	std::shared_ptr<nexo::ecs::ComponentArray<Velocity>> velocityArray;
};

int main(int argc, char** argv) {
    // Check if the correct number of arguments is provided.
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <number_of_entities> <number_of_frames> <repeat_iterations>\n";
        return 1;
    }

    // Parse command-line arguments.
    int numEntities = std::stoi(argv[1]);
    int numFrames   = std::stoi(argv[2]);
    int repeatCount = std::stoi(argv[3]);

    std::cout << "Running with " << numEntities << " entities, "
              << numFrames << " frames per run, repeated " << repeatCount << " times.\n";

    // Initialize the ECS Coordinator.
    nexo::ecs::Coordinator coordinator;
    coordinator.init();

    // Register components.
    coordinator.registerComponent<Position>();
    coordinator.registerComponent<Velocity>();

    // Register and set up the MovementSystem.
    auto movementSystem = coordinator.registerSystem<MovementSystem>();
    nexo::ecs::Signature movementSignature;
    movementSignature.set(coordinator.getComponentType<Position>(), true);
    movementSignature.set(coordinator.getComponentType<Velocity>(), true);
    coordinator.setSystemSignature<MovementSystem>(movementSignature);

    auto setupStart = std::chrono::steady_clock::now();
    // Create entities and assign components.
    std::vector<nexo::ecs::Entity> createdEntities;
    createdEntities.reserve(numEntities);
    for (int i = 0; i < numEntities; i++) {
        nexo::ecs::Entity newEntity = coordinator.createEntity();
        coordinator.addComponent(newEntity, Position{0.0f, 0.0f, 0.0f});
        coordinator.addComponent(newEntity, Velocity{1.0f, 0.0f, 0.0f});
        createdEntities.push_back(newEntity);
    }
    auto setupEnd = std::chrono::steady_clock::now();
    auto setupDurationMicro = std::chrono::duration_cast<std::chrono::microseconds>(setupEnd - setupStart).count();

    std::cout << "Setup duration: " << setupDurationMicro << " microseconds" << std::endl;

    // Remove 25% of the entities randomly.
    int numToRemove = numEntities / 4;
    {
        // Create a random device and engine.
        std::random_device rd;
        std::mt19937 engine(rd());
        std::shuffle(createdEntities.begin(), createdEntities.end(), engine);
    }
    for (int i = 0; i < numToRemove; ++i) {
        nexo::ecs::Entity entityToRemove = createdEntities[i];
        coordinator.destroyEntity(entityToRemove);
    }
    std::cout << "Removed " << numToRemove << " entities randomly after setup.\n";

    // Run the update loop as specified by the command-line arguments.
    // We'll measure frame times in microseconds.
    long long overallDurationMicro = 0;
    for (int repeat = 0; repeat < repeatCount; ++repeat) {
        long long totalDurationMicro = 0;
        for (int frame = 0; frame < numFrames; ++frame) {
            auto frameStart = std::chrono::steady_clock::now();

            // Update the MovementSystem (assume a delta time of ~16ms per frame).
            movementSystem->update(0.016f);

            auto frameEnd = std::chrono::steady_clock::now();
            auto frameDurationMicro = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart).count();
            // Convert microseconds to milliseconds for display.
            double frameDurationMilli = frameDurationMicro / 1000.0;
            totalDurationMicro += frameDurationMicro;
        }
        double totalDurationMilli = totalDurationMicro / 1000.0;
        double averageDurationMilli = (totalDurationMicro / static_cast<double>(numFrames)) / 1000.0;
        std::cout << "Iteration " << repeat << ": total time to render " << numFrames
                  << " frames: " << totalDurationMilli << " milliseconds.\n";
        std::cout << "Iteration " << repeat << ": average frame duration: "
                  << averageDurationMilli << " milliseconds.\n\n";
        overallDurationMicro += totalDurationMicro;
    }

    // Overall per-frame average (in milliseconds).
    double overallAverageFrameMilli = (overallDurationMicro / static_cast<double>(repeatCount * numFrames)) / 1000.0;
    std::cout << "Overall average frame duration over all repeats: "
              << overallAverageFrameMilli << " milliseconds.\n";

    // Average total time to render numFrames frames across all repeats.
    double averageTotalTimeMilli = (overallDurationMicro / static_cast<double>(repeatCount)) / 1000.0;
    std::cout << "Average time to render " << numFrames << " frames: "
              << averageTotalTimeMilli << " milliseconds.\n";

    return 0;
}
