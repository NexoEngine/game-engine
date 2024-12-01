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
#include "ecs/Coordinator.hpp"

struct Position {
    float x, y, z;
};

struct Velocity {
    float x, y, z;
};

class MovementSystem : public nexo::ecs::System {
    public:
        void update(float deltaTime) {
            for (auto entity : entities) {
                auto& position = coord->getComponent<Position>(entity);
                auto& velocity = coord->getComponent<Velocity>(entity);

                // Update position using velocity
                position.x += velocity.x * deltaTime;
                position.y += velocity.y * deltaTime;
                position.z += velocity.z * deltaTime;
            }
        }
};

int main() {
    // Initialize the ECS Coordinator
    nexo::ecs::Coordinator coordinator;
    coordinator.init();

    // Register components
    coordinator.registerComponent<Position>();
    coordinator.registerComponent<Velocity>();

    // Register and set up the MovementSystem
    auto movementSystem = coordinator.registerSystem<MovementSystem>();
    nexo::ecs::Signature movementSignature;
    movementSignature.set(coordinator.getComponentType<Position>(), true);
    movementSignature.set(coordinator.getComponentType<Velocity>(), true);
    coordinator.setSystemSignature<MovementSystem>(movementSignature);

    // Create entities and assign components
    auto entity1 = coordinator.createEntity();
    coordinator.addComponent(entity1, Position{0.0f, 0.0f, 0.0f});
    coordinator.addComponent(entity1, Velocity{1.0f, 0.0f, 0.0f});

    auto entity2 = coordinator.createEntity();
    coordinator.addComponent(entity2, Position{5.0f, 5.0f, 5.0f});
    coordinator.addComponent(entity2, Velocity{0.0f, -1.0f, 0.0f});

    // Simulate a game loop
    for (int frame = 0; frame < 10; ++frame) {
        std::cout << "Frame " << frame << ":\n";

        // Update the MovementSystem
        movementSystem->update(0.016f); // Assuming 60 FPS, so ~16ms per frame

        // Output entity positions
        for (auto entity : movementSystem->entities) {
            auto& position = coordinator.getComponent<Position>(entity);
            std::cout << "Entity " << entity << " Position: ("
                      << position.x << ", " << position.y << ", " << position.z << ")\n";
        }

        std::cout << "---------------------\n";
    }

    return 0;
}
