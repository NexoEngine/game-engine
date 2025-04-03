//// exampleAdvanced.cpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        03/04/2025
//  Description: Comprehensive example showcasing various ECS features
//               highlighting proper usage of group ownership
//
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <chrono>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include <random>
#include <iomanip>
#include <xmmintrin.h>
#include "ecs/Coordinator.hpp"

// ============================================================================
// Component Definitions
// ============================================================================

struct Transform {
    float x = 0.0f;
    float y = 0.0f;
    float rotation = 0.0f;
    float scale = 1.0f;
};

struct Physics {
    float velocityX = 0.0f;
    float velocityY = 0.0f;
    float mass = 1.0f;
    bool hasGravity = true;
};

struct Renderable {
    enum class Type { Circle, Rectangle, Triangle, Custom };
    Type type = Type::Rectangle;
    int zIndex = 0;
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};

struct AI {
    float detectionRadius = 100.0f;
    int behaviorType = 0;  // 0: patrol, 1: chase, 2: flee, etc.
    float reactionTime = 0.5f;
    float timeSinceLastDecision = 0.0f;
};

struct Health {
    int current = 100;
    int maximum = 100;
    float regenRate = 0.0f;
    bool isDead = false;
};

struct Tag {
    enum class Layer { Player, Enemy, Neutral, Projectile, Environment };
    Layer layer = Layer::Neutral;
    std::string name = "Entity";
    int id = 0;
};

struct Lifetime {
    float timeRemaining = 0.0f;
    bool shouldDestroy = false;
};

// Helper to format log messages
void log(const std::string& message) {
    std::cout << message << std::endl;
}

// ============================================================================
// System Implementations
// ============================================================================

// Example 1: Physics system, this demonstrates the use of groups in order to efficiently access multiple owned components together
class PhysicsSystem : public nexo::ecs::System {
public:
    PhysicsSystem() {
        // Create a group that owns both Transform and Physics
        // This is the correct way to handle frequently accessed components together
        movementGroup = coord->registerGroup<Transform, Physics>(nexo::ecs::get<>());
    }

    void update(float deltaTime) {
        const float GRAVITY = 9.8f;

        log("Physics update started for " + std::to_string(movementGroup->size()) + " entities");

        // ITERATION METHOD 1: Using range-based for loop
        // This demonstrates how to access multiple owned components efficiently
        for (auto [entity, transform, physics] : *movementGroup) {
            // Apply velocity to position
            transform.x += physics.velocityX * deltaTime;
            transform.y += physics.velocityY * deltaTime;

            // Apply gravity if enabled
            if (physics.hasGravity) {
                physics.velocityY += GRAVITY * physics.mass * deltaTime;
            }

            // Apply damping
            physics.velocityX *= 0.99f;
            physics.velocityY *= 0.99f;

            // Boundary checking
            if (transform.y < -100.0f) {
                transform.y = -100.0f;
                physics.velocityY = -physics.velocityY * 0.8f; // Bounce
            }
        }

        log("Physics update completed");
    }

    // Method to show different ways to access group data
    void showPerformanceExamples(float deltaTime) {
        log("Demonstrating different access methods for physics calculations:");
        const float GRAVITY = 9.8f;

        // METHOD 1: RANGE-BASED FOR LOOP (Already shown in update method)
        auto start1 = std::chrono::high_resolution_clock::now();
        for (auto [entity, transform, physics] : *movementGroup) {
            // Apply velocity to position
            transform.x += physics.velocityX * deltaTime;
            transform.y += physics.velocityY * deltaTime;

            // Apply gravity if enabled
            if (physics.hasGravity) {
                physics.velocityY += GRAVITY * physics.mass * deltaTime;
            }

            // Apply damping
            physics.velocityX *= 0.99f;
            physics.velocityY *= 0.99f;

            // Boundary checking
            if (transform.y < -100.0f) {
                transform.y = -100.0f;
                physics.velocityY = -physics.velocityY * 0.8f; // Bounce
            }
        }
        auto end1 = std::chrono::high_resolution_clock::now();

        // METHOD 2: USING EACH METHOD WITH LAMBDA
        auto start2 = std::chrono::high_resolution_clock::now();

        log("Method 2: Using each() with lambda");
        movementGroup->each([deltaTime, GRAVITY](nexo::ecs::Entity entity, Transform& transform, Physics& physics) {
            // Same physics calculations as above
            transform.x += physics.velocityX * deltaTime;
            transform.y += physics.velocityY * deltaTime;
            // Apply gravity if enabled
            if (physics.hasGravity) {
                physics.velocityY += GRAVITY * physics.mass * deltaTime;
            }

            // Apply damping
            physics.velocityX *= 0.99f;
            physics.velocityY *= 0.99f;

            // Boundary checking
            if (transform.y < -100.0f) {
                transform.y = -100.0f;
                physics.velocityY = -physics.velocityY * 0.8f; // Bounce
            }
        });

        auto end2 = std::chrono::high_resolution_clock::now();

        // METHOD 3: DIRECT SPAN ACCESS (Most efficient for tight loops)
        auto start3 = std::chrono::high_resolution_clock::now();

        log("Method 3: Using direct span access (most efficient)");
        auto transformSpan = movementGroup->get<Transform>();
        auto physicsSpan = movementGroup->get<Physics>();

        for (size_t i = 0; i < transformSpan.size(); i++) {
            transformSpan[i].x += physicsSpan[i].velocityX * deltaTime;
            transformSpan[i].y += physicsSpan[i].velocityY * deltaTime;

            if (physicsSpan[i].hasGravity) {
                physicsSpan[i].velocityY += GRAVITY * physicsSpan[i].mass * deltaTime;
            }

            // Apply damping
            physicsSpan[i].velocityX *= 0.99f;
            physicsSpan[i].velocityY *= 0.99f;
        }

        auto end3 = std::chrono::high_resolution_clock::now();

        // METHOD 4: SIMD PROCESSING (Further optimization with vectorization, not extremly pertinent for this case, but it demonstrates that it is possible)
        auto start4 = std::chrono::high_resolution_clock::now();

        log("Method 4: Using SIMD instructions for vectorized processing");

        transformSpan = movementGroup->get<Transform>();
        physicsSpan = movementGroup->get<Physics>();
        const size_t size = transformSpan.size();

        // Process in chunks of 4 elements using SSE instructions
        const __m128 dt = _mm_set1_ps(deltaTime);           // [dt, dt, dt, dt]
        const __m128 gravity = _mm_set1_ps(GRAVITY);        // [9.8, 9.8, 9.8, 9.8]
        const __m128 damping = _mm_set1_ps(0.99f);          // [0.99, 0.99, 0.99, 0.99]
        const __m128 bounce = _mm_set1_ps(-0.8f);           // [-0.8, -0.8, -0.8, -0.8]
        const __m128 floor = _mm_set1_ps(-100.0f);          // [-100, -100, -100, -100]

        // Process chunks of 4 elements at once
        size_t i = 0;
        for (; i + 3 < size; i += 4) {
            // Load 4 x and y positions
            __m128 posX = _mm_set_ps(
                transformSpan[i+3].x, transformSpan[i+2].x,
                transformSpan[i+1].x, transformSpan[i].x
            );

            __m128 posY = _mm_set_ps(
                transformSpan[i+3].y, transformSpan[i+2].y,
                transformSpan[i+1].y, transformSpan[i].y
            );

            // Load 4 x and y velocities
            __m128 velX = _mm_set_ps(
                physicsSpan[i+3].velocityX, physicsSpan[i+2].velocityX,
                physicsSpan[i+1].velocityX, physicsSpan[i].velocityX
            );

            __m128 velY = _mm_set_ps(
                physicsSpan[i+3].velocityY, physicsSpan[i+2].velocityY,
                physicsSpan[i+1].velocityY, physicsSpan[i].velocityY
            );

            // Load 4 mass values
            __m128 mass = _mm_set_ps(
                physicsSpan[i+3].mass, physicsSpan[i+2].mass,
                physicsSpan[i+1].mass, physicsSpan[i].mass
            );

            // Load 4 hasGravity flags (as floats: 1.0f for true, 0.0f for false)
            __m128 hasGravity = _mm_set_ps(
                physicsSpan[i+3].hasGravity ? 1.0f : 0.0f,
                physicsSpan[i+2].hasGravity ? 1.0f : 0.0f,
                physicsSpan[i+1].hasGravity ? 1.0f : 0.0f,
                physicsSpan[i].hasGravity ? 1.0f : 0.0f
            );

            // Calculate velocity * deltaTime
            __m128 deltaX = _mm_mul_ps(velX, dt);
            __m128 deltaY = _mm_mul_ps(velY, dt);

            // Update positions: pos += vel * dt
            posX = _mm_add_ps(posX, deltaX);
            posY = _mm_add_ps(posY, deltaY);

            // Apply gravity: vel_y += gravity * mass * dt * hasGravity
            __m128 gravityEffect = _mm_mul_ps(gravity, mass);       // gravity * mass
            gravityEffect = _mm_mul_ps(gravityEffect, dt);          // * dt
            gravityEffect = _mm_mul_ps(gravityEffect, hasGravity);  // * hasGravity
            velY = _mm_add_ps(velY, gravityEffect);                 // velY += ...

            // Apply damping: vel *= 0.99
            velX = _mm_mul_ps(velX, damping);
            velY = _mm_mul_ps(velY, damping);

            // Boundary check: if (posY < -100.0) { posY = -100.0; velY *= -0.8; }
            __m128 belowFloor = _mm_cmplt_ps(posY, floor);          // posY < floor?
            __m128 bounceVel = _mm_mul_ps(velY, bounce);            // velY * -0.8
            velY = _mm_or_ps(
                _mm_and_ps(belowFloor, bounceVel),                  // if below floor: use bounce vel
                _mm_andnot_ps(belowFloor, velY)                     // else: keep original vel
            );
            posY = _mm_or_ps(
                _mm_and_ps(belowFloor, floor),                      // if below floor: use floor value
                _mm_andnot_ps(belowFloor, posY)                     // else: keep original pos
            );

            // Store back the results
            float posXResult[4], posYResult[4], velXResult[4], velYResult[4];
            _mm_storeu_ps(posXResult, posX);
            _mm_storeu_ps(posYResult, posY);
            _mm_storeu_ps(velXResult, velX);
            _mm_storeu_ps(velYResult, velY);

            for (size_t j = 0; j < 4; j++) {
                transformSpan[i+j].x = posXResult[j];
                transformSpan[i+j].y = posYResult[j];
                physicsSpan[i+j].velocityX = velXResult[j];
                physicsSpan[i+j].velocityY = velYResult[j];
            }
        }

        // Process remaining elements normally
        for (; i < size; i++) {
            transformSpan[i].x += physicsSpan[i].velocityX * deltaTime;
            transformSpan[i].y += physicsSpan[i].velocityY * deltaTime;

            if (physicsSpan[i].hasGravity) {
                physicsSpan[i].velocityY += GRAVITY * physicsSpan[i].mass * deltaTime;
            }

            physicsSpan[i].velocityX *= 0.99f;
            physicsSpan[i].velocityY *= 0.99f;

            if (transformSpan[i].y < -100.0f) {
                transformSpan[i].y = -100.0f;
                physicsSpan[i].velocityY *= -0.8f;
            }
        }

        auto end4 = std::chrono::high_resolution_clock::now();

        // Display timing results
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();
        auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count();
        auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(end3 - start3).count();
        auto duration4 = std::chrono::duration_cast<std::chrono::microseconds>(end4 - start4).count();

        log("Performance comparison:");
        log("  Range-based for: " + std::to_string(duration1) + " μs");
        log("  each() method:   " + std::to_string(duration2) + " μs");
        log("  Direct span:     " + std::to_string(duration3) + " μs");
        log("  SIMD processing: " + std::to_string(duration4) + " μs");
    }

private:
    // This group properly owns both Transform and Physics together since they're
    // frequently accessed together in the physics update
    nexo::ecs::GroupAlias<
        nexo::ecs::OwnedComponents<Transform, Physics>,
        nexo::ecs::NonOwnedComponents<>
    > movementGroup = nullptr;
};

// Example 2: Render system demonstrating sorting and partitioning
class RenderSystem : public nexo::ecs::System {
public:
    RenderSystem() {
        // Create a group that owns Renderable but accesses Transform as non-owned
        // This is appropriate because rendering needs to sort/organize by Renderable properties
        // but Transform is already owned by the PhysicsSystem
        renderGroup = coord->registerGroup<Renderable>(nexo::ecs::get<Transform, Tag>());
    }

    void render() {
        log("Render pass started for " + std::to_string(renderGroup->size()) + " entities");

        // SORTING EXAMPLE: Sort by z-index for proper render order
        // This shows how owning the Renderable component allows efficient sorting
        renderGroup->sortBy<Renderable, int>(
            [](const Renderable& renderable) { return renderable.zIndex; }
        );

        // PARTITIONING EXAMPLE: Group by renderable type for batch rendering
        auto typePartitions = renderGroup->getPartitionView<Renderable, Renderable::Type>(
            [](const Renderable& renderable) { return renderable.type; }
        );

        log("Rendering in " + std::to_string(typePartitions.partitionCount()) + " batches");

        // Process each partition separately (batch rendering)
        auto keys = typePartitions.getPartitionKeys();
        for (auto type : keys) {
            std::string typeName;
            switch (type) {
                case Renderable::Type::Circle: typeName = "Circles"; break;
                case Renderable::Type::Rectangle: typeName = "Rectangles"; break;
                case Renderable::Type::Triangle: typeName = "Triangles"; break;
                case Renderable::Type::Custom: typeName = "Custom shapes"; break;
            }

            const auto* partition = typePartitions.getPartition(type);
            if (partition) {
                log("  Batch rendering " + std::to_string(partition->count) + " " + typeName);

                // Example of using each() on a partition for batch rendering
                typePartitions.each(type, [](nexo::ecs::Entity entity, Renderable& renderable, Transform& transform, Tag& tag) {
                    // Simulated rendering code
                    std::string typeName;
                    switch (renderable.type) {
                        case Renderable::Type::Circle: typeName = "Circle"; break;
                        case Renderable::Type::Rectangle: typeName = "Rectangle"; break;
                        case Renderable::Type::Triangle: typeName = "Triangle"; break;
                        case Renderable::Type::Custom: typeName = "Custom shape"; break;
                    }
                    std::cout << "    - Rendered " << typeName << " " << tag.name << " at ("
                              << transform.x << ", " << transform.y
                              << ") with z-index " << renderable.zIndex << "\n";
                });
            }
        }

        log("Render pass completed");
    }

private:
    nexo::ecs::GroupAlias<
        nexo::ecs::OwnedComponents<Renderable>,
        nexo::ecs::NonOwnedComponents<Transform, Tag>
    > renderGroup = nullptr;
};

// Example 3: AI and health system demonstrating component addition/removal
class AISystem : public nexo::ecs::System {
public:
    AISystem() {
        // AI and Health components are frequently accessed together in AI logic,
        // so they're owned by the same group
        aiGroup = coord->registerGroup<AI, Health>(nexo::ecs::get<Transform, Tag>());
    }

    void update(float deltaTime) {
        log("AI update started for " + std::to_string(aiGroup->size()) + " entities");

        // Keep track of entities that need component changes
        std::vector<nexo::ecs::Entity> entitiesToAddLifetime;
        std::vector<nexo::ecs::Entity> entitiesToSpawnProjectile;

        // Random number generation for AI decisions
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> posDist(-100.0, 100.0);
        std::uniform_int_distribution<> decisionDist(0, 10);

        // PARTITIONING EXAMPLE: Create partitions by behavior type
        auto behaviorPartitions = aiGroup->getPartitionView<AI, int>(
            [](const AI& ai) { return ai.behaviorType; }
        );

        // Process each behavior type batch separately
        for (int behaviorType = 0; behaviorType <= 2; behaviorType++) {
            std::string behaviorName;
            switch (behaviorType) {
                case 0: behaviorName = "Patrol"; break;
                case 1: behaviorName = "Chase"; break;
                case 2: behaviorName = "Flee"; break;
                default: behaviorName = "Unknown"; break;
            }

            const auto* partition = behaviorPartitions.getPartition(behaviorType);
            if (!partition) {
                continue;
            }

            log("  Processing " + std::to_string(partition->count) + " entities with " + behaviorName + " behavior");

            // Process entities with this behavior type
            behaviorPartitions.each(behaviorType, [&](
                nexo::ecs::Entity entity,
                AI& ai,
                Health& health,
                Transform& transform,
                Tag& tag
            ) {
                // Update AI decision timer
                ai.timeSinceLastDecision += deltaTime;

                // Only make decisions at specific intervals based on reaction time
                if (ai.timeSinceLastDecision >= ai.reactionTime) {
                    ai.timeSinceLastDecision = 0.0f;

                    // Change behavior based on health
                    if (health.current < health.maximum * 0.2f && ai.behaviorType != 2) {
                        log("    " + tag.name + " is low on health, switching to Flee behavior");
                        ai.behaviorType = 2; // Flee
                    }
                    else if (health.current > health.maximum * 0.8f && ai.behaviorType == 2) {
                        log("    " + tag.name + " has recovered, switching to Patrol behavior");
                        ai.behaviorType = 0; // Patrol
                    }

                    // Random decision to fire projectile
                    if (decisionDist(gen) == 0) {
                        entitiesToSpawnProjectile.push_back(entity);
                    }

                    // Apply health regeneration
                    if (health.regenRate > 0 && health.current < health.maximum) {
                        health.current += static_cast<int>(health.regenRate * deltaTime);
                        health.current = std::min(health.current, health.maximum);
                    }

                    // Check for death
                    if (health.current <= 0 && !health.isDead) {
                        health.isDead = true;
                        health.current = 0;
                        log("    " + tag.name + " has died!");

                        // Mark for adding lifetime component after iteration
                        entitiesToAddLifetime.push_back(entity);
                    }
                }
            });
        }

        // Process component changes after iteration to avoid invalidation
        for (auto entity : entitiesToAddLifetime) {
            if (!coord->entityHasComponent<Lifetime>(entity)) {
                log("  Adding Lifetime component to dead entity " +
                    coord->getComponent<Tag>(entity).name);
                coord->addComponent(entity, Lifetime{5.0f, false});
            }
        }

        // Process projectile spawning after iteration
        for (auto entity : entitiesToSpawnProjectile) {
            spawnProjectile(entity);
        }

        log("AI update completed");
    }

    void spawnProjectile(nexo::ecs::Entity source) {
        const auto& sourceTransform = coord->getComponent<Transform>(source);
        const auto& sourceTag = coord->getComponent<Tag>(source);

        nexo::ecs::Entity projectile = coord->createEntity();

        // Random projectile direction
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> velDist(-50.0f, 50.0f);

        // Add necessary components
        coord->addComponent(projectile, Transform{
            sourceTransform.x, sourceTransform.y, 0.0f, 0.5f
        });

        coord->addComponent(projectile, Physics{
            static_cast<float>(velDist(gen)),
            static_cast<float>(velDist(gen)),
            0.1f, true
        });

        coord->addComponent(projectile, Renderable{
            Renderable::Type::Circle, -1, 1.0f, 0.0f, 0.0f, 1.0f
        });

        coord->addComponent(projectile, Lifetime{3.0f, false});

        coord->addComponent(projectile, Tag{
            Tag::Layer::Projectile,
            sourceTag.name + "_projectile",
            static_cast<int>(projectile)
        });

        log("  " + sourceTag.name + " spawned a projectile");
    }

    void applyDamage(nexo::ecs::Entity target, int amount) {
        if (!coord->entityHasComponent<Health>(target)) {
            log("Cannot apply damage to entity without Health component");
            return;
        }

        auto& health = coord->getComponent<Health>(target);
        std::string entityName = "Unknown";

        if (coord->entityHasComponent<Tag>(target)) {
            entityName = coord->getComponent<Tag>(target).name;
        }

        health.current -= amount;
        log("Applied " + std::to_string(amount) + " damage to " + entityName +
            " (Health: " + std::to_string(health.current) + "/" +
            std::to_string(health.maximum) + ")");
    }

private:
    nexo::ecs::GroupAlias<
        nexo::ecs::OwnedComponents<AI, Health>,
        nexo::ecs::NonOwnedComponents<Transform, Tag>
    > aiGroup = nullptr;
};

// Example 4: Lifetime system for handling entity destruction
class LifetimeSystem : public nexo::ecs::System {
public:
    LifetimeSystem() {
        // This group owns the Lifetime component
        lifetimeGroup = coord->registerGroup<Lifetime>(nexo::ecs::get<Tag>());
    }

    void update(float deltaTime) {
        if (lifetimeGroup->size() == 0) {
            return;
        }

        log("Updating lifetime for " + std::to_string(lifetimeGroup->size()) + " entities");

        // Collect entities to be destroyed after iteration
        std::vector<nexo::ecs::Entity> entitiesToDestroy;

        lifetimeGroup->each([&](nexo::ecs::Entity entity, Lifetime& lifetime, Tag& tag) {
            lifetime.timeRemaining -= deltaTime;

            if (lifetime.timeRemaining <= 0.0f || lifetime.shouldDestroy) {
                log("  Marking " + tag.name + " for destruction");
                entitiesToDestroy.push_back(entity);
            }
        });

        // Destroy entities after iteration is complete
        for (auto entity : entitiesToDestroy) {
            log("  Destroying entity " + std::to_string(entity));
            coord->destroyEntity(entity);
        }
    }

private:
    nexo::ecs::GroupAlias<
        nexo::ecs::OwnedComponents<Lifetime>,
        nexo::ecs::NonOwnedComponents<Tag>
    > lifetimeGroup = nullptr;
};

// ============================================================================
// Main Program
// ============================================================================

int main() {
    // Initialize ECS Coordinator
    nexo::ecs::Coordinator coordinator;
    coordinator.init();

    log("ECS initialized");

    // Register all components
    coordinator.registerComponent<Transform>();
    coordinator.registerComponent<Physics>();
    coordinator.registerComponent<Renderable>();
    coordinator.registerComponent<AI>();
    coordinator.registerComponent<Health>();
    coordinator.registerComponent<Tag>();
    coordinator.registerComponent<Lifetime>();

    log("Components registered");

    // Register and set up the systems
    auto physicsSystem = coordinator.registerSystem<PhysicsSystem>();
    {
        nexo::ecs::Signature signature;
        signature.set(coordinator.getComponentType<Physics>());
        signature.set(coordinator.getComponentType<Transform>());
        coordinator.setSystemSignature<PhysicsSystem>(signature);
    }

    auto renderSystem = coordinator.registerSystem<RenderSystem>();
    {
        nexo::ecs::Signature signature;
        signature.set(coordinator.getComponentType<Transform>());
        signature.set(coordinator.getComponentType<Renderable>());
        signature.set(coordinator.getComponentType<Tag>());
        coordinator.setSystemSignature<RenderSystem>(signature);
    }

    auto aiSystem = coordinator.registerSystem<AISystem>();
    {
        nexo::ecs::Signature signature;
        signature.set(coordinator.getComponentType<AI>());
        signature.set(coordinator.getComponentType<Health>());
        signature.set(coordinator.getComponentType<Transform>());
        signature.set(coordinator.getComponentType<Tag>());
        coordinator.setSystemSignature<AISystem>(signature);
    }

    auto lifetimeSystem = coordinator.registerSystem<LifetimeSystem>();
    {
        nexo::ecs::Signature signature;
        signature.set(coordinator.getComponentType<Lifetime>());
        signature.set(coordinator.getComponentType<Tag>());
        coordinator.setSystemSignature<LifetimeSystem>(signature);
    }

    log("Systems registered");

    // ========================================================================
    // Entity Creation Examples
    // ========================================================================
    log("Creating entities...");

    // Create a player entity
    nexo::ecs::Entity player = coordinator.createEntity();
    coordinator.addComponent(player, Transform{0.0f, 0.0f, 0.0f, 1.5f});
    coordinator.addComponent(player, Physics{0.0f, 0.0f, 2.0f, true});
    coordinator.addComponent(player, Renderable{Renderable::Type::Triangle, 10, 0.0f, 0.0f, 1.0f, 1.0f});
    coordinator.addComponent(player, Health{150, 150, 1.0f});
    coordinator.addComponent(player, Tag{Tag::Layer::Player, "Player", 1});
    log("Created Player entity with ID: " + std::to_string(player));

    // Create some enemy entities with different behavior types
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> posDist(-100.0, 100.0);
    std::uniform_int_distribution<> zIndexDist(1, 20);
    std::uniform_int_distribution<> colorDist(0, 255);
    std::uniform_int_distribution<> behaviorDist(0, 2);

    std::vector<nexo::ecs::Entity> enemies;
    for (int i = 0; i < 10; ++i) {
        nexo::ecs::Entity enemy = coordinator.createEntity();

        float posX = static_cast<float>(posDist(gen));
        float posY = static_cast<float>(posDist(gen));
        int behaviorType = behaviorDist(gen);

        coordinator.addComponent(enemy, Transform{posX, posY});
        coordinator.addComponent(enemy, Physics{0.0f, 0.0f, 1.0f, true});
        coordinator.addComponent(enemy, Renderable{
            Renderable::Type::Rectangle,
            zIndexDist(gen),
            colorDist(gen) / 255.0f,
            colorDist(gen) / 255.0f,
            colorDist(gen) / 255.0f,
            1.0f
        });
        coordinator.addComponent(enemy, AI{
            50.0f + i * 5.0f,  // Detection radius
            behaviorType,      // Behavior type
            0.5f + i * 0.1f    // Reaction time
        });
        coordinator.addComponent(enemy, Health{80 + i * 5, 100, 0.5f});
        coordinator.addComponent(enemy, Tag{
            Tag::Layer::Enemy,
            "Enemy_" + std::to_string(i),
            100 + i
        });

        enemies.push_back(enemy);
        log("Created Enemy entity with ID: " + std::to_string(enemy));
    }

    // Create some environmental objects
    for (int i = 0; i < 5; ++i) {
        nexo::ecs::Entity environment = coordinator.createEntity();

        float posX = static_cast<float>(posDist(gen));
        float posY = static_cast<float>(posDist(gen));

        coordinator.addComponent(environment, Transform{posX, posY, 0.0f, 2.0f + i * 0.5f});
        coordinator.addComponent(environment, Renderable{
            i % 2 == 0 ? Renderable::Type::Circle : Renderable::Type::Custom,
            zIndexDist(gen),
            0.0f, 0.5f, 0.0f, 1.0f  // Green color
        });
        coordinator.addComponent(environment, Tag{
            Tag::Layer::Environment,
            "Environment_" + std::to_string(i),
            200 + i
        });

        log("Created Environment entity with ID: " + std::to_string(environment));
    }

    // ========================================================================
    // Performance Example
    // ========================================================================
    log("\n=== Performance Comparison of Access Methods ===");
    physicsSystem->showPerformanceExamples(0.016f);

    // ========================================================================
    // Simulation Update Loop
    // ========================================================================
    log("\n=== Starting Simulation ===");

    const float TIME_STEP = 0.016f;  // ~60 FPS
    for (int frame = 0; frame < 5; ++frame) {
        log("\n--- Frame " + std::to_string(frame) + " ---");

        // Update all systems
        physicsSystem->update(TIME_STEP);
        aiSystem->update(TIME_STEP);

        // Example: Apply damage to a random enemy every other frame
        if (frame % 2 == 0 && !enemies.empty()) {
            std::uniform_int_distribution<> enemyDist(0, static_cast<int>(enemies.size()) - 1);
            int targetIndex = enemyDist(gen);

            log("\n*** Combat Event ***");
            aiSystem->applyDamage(enemies[targetIndex], 30);
        }

        // Update lifetime system
        lifetimeSystem->update(TIME_STEP);

        // Render the scene last
        renderSystem->render();
    }

    log("\n=== Simulation Complete ===");
    return 0;
}
