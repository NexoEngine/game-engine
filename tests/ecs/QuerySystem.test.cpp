//// QuerySystem.test.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/04/2025
//  Description: Test file for the QuerySystem class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "QuerySystem.hpp"
#include "Coordinator.hpp"
#include "Access.hpp"
#include "../utils/comparison.hpp"
#include "SingletonComponent.hpp"
#include <memory>
#include <string>
#include <type_traits>

namespace nexo::ecs {

    // Define test components
    struct Position {
        float x, y, z;

        Position(float x = 0.0f, float y = 0.0f, float z = 0.0f)
            : x(x), y(y), z(z) {}

        bool operator==(const Position& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };

    struct Velocity {
        float vx, vy, vz;

        Velocity(float vx = 0.0f, float vy = 0.0f, float vz = 0.0f)
            : vx(vx), vy(vy), vz(vz) {}

        bool operator==(const Velocity& other) const {
            return vx == other.vx && vy == other.vy && vz == other.vz;
        }
    };

    struct Tag {
        std::string name;
        int category;

        Tag(const std::string& name = "", int category = 0)
            : name(name), category(category) {}

        bool operator==(const Tag& other) const {
            return name == other.name && category == other.category;
        }
    };

    // Define singleton components
    class GameSettings {
    public:
        bool debugMode = false;
        float gameSpeed = 1.0f;

        GameSettings() = default;
        GameSettings(bool debug, float speed) : debugMode(debug), gameSpeed(speed) {}

	    GameSettings(const GameSettings&) = delete;
		GameSettings& operator=(const GameSettings&) = delete;
    };

    // Test fixture
    class QuerySystemTest : public ::testing::Test {
    protected:
        std::shared_ptr<Coordinator> coordinator;
        std::vector<Entity> entities;

        void SetUp() override {
            // Initialize coordinator
            coordinator = std::make_shared<Coordinator>();
            coordinator->init();
            System::coord = coordinator;

            // Register components
            coordinator->registerComponent<Position>();
            coordinator->registerComponent<Velocity>();
            coordinator->registerComponent<Tag>();

            // Register singleton
            coordinator->registerSingletonComponent<GameSettings>(true, 2.0f);

            // Create test entities
            for (int i = 0; i < 5; ++i) {
                Entity entity = coordinator->createEntity();
                entities.push_back(entity);

                // Add components
                coordinator->addComponent(entity, Position(i * 1.0f, i * 2.0f, i * 3.0f));
                coordinator->addComponent(entity, Velocity(i * 0.5f, i * 1.0f, i * 1.5f));
                coordinator->addComponent(entity, Tag("Entity_" + std::to_string(i), i % 3));
            }

            // Create an entity with only Position and Tag
            Entity posTagEntity = coordinator->createEntity();
            coordinator->addComponent(posTagEntity, Position(10.0f, 20.0f, 30.0f));
            coordinator->addComponent(posTagEntity, Tag("PosTagOnly", 99));
            entities.push_back(posTagEntity);
        }

        void TearDown() override {
            // Clean up entities
            for (auto entity : entities) {
                coordinator->destroyEntity(entity);
            }

            // Reset coordinator
            System::coord = nullptr;
        }
    };

    // Test system classes

    // 1. System with read access to Position and write access to Velocity
    class MovementSystem : public QuerySystem<Read<Position>, Write<Velocity>> {
    public:
        void applyGravity(float gravity) {
            for (Entity entity : entities) {
                const Position& pos = getComponent<Position>(entity);
                Velocity& vel = getComponent<Velocity>(entity);

                // Apply gravity based on height
                vel.vy -= gravity * (pos.y / 10.0f);
            }
        }
    };

    // 2. System with singleton component access
    class PhysicsSystem : public QuerySystem<
        Read<Position>,
        Write<Velocity>,
        ReadSingleton<GameSettings>> {
    public:
        void updateVelocities() {
            const auto& settings = getSingleton<GameSettings>();

            for (Entity entity : entities) {
                const Position& pos = getComponent<Position>(entity);
                Velocity& vel = getComponent<Velocity>(entity);

                // Apply game speed scaling
                vel.vx *= settings.gameSpeed;
                vel.vy *= settings.gameSpeed;
                vel.vz *= settings.gameSpeed;
            }
        }
    };

    TEST_F(QuerySystemTest, SystemCreation) {
        auto system = coordinator->registerQuerySystem<MovementSystem>();
        ASSERT_NE(system, nullptr);

        // Verify system signature is set correctly
        Signature expectedSignature;
        expectedSignature.set(coordinator->getComponentType<Position>());
        expectedSignature.set(coordinator->getComponentType<Velocity>());

        EXPECT_EQ(system->getSignature(), expectedSignature);

        // Verify system's entity set gets populated
        EXPECT_EQ(system->entities.size(), 5); // Only first 5 entities have Position and Velocity
    }

    TEST_F(QuerySystemTest, ComponentAccess) {
        auto system = coordinator->registerQuerySystem<MovementSystem>();

        // Apply gravity (0.5)
        system->applyGravity(0.5f);

        // Verify velocities were modified
        for (size_t i = 0; i < 5; ++i) {
            Velocity& vel = coordinator->getComponent<Velocity>(entities[i]);

            // Expected: original vy - (gravity * height / 10.0f)
            float expectedVy = i * 1.0f - 0.5f * (i * 2.0f / 10.0f);
            EXPECT_FLOAT_EQ(vel.vy, expectedVy);

            // vx and vz should be unchanged
            EXPECT_FLOAT_EQ(vel.vx, i * 0.5f);
            EXPECT_FLOAT_EQ(vel.vz, i * 1.5f);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Singleton Component Tests
    //////////////////////////////////////////////////////////////////////////

    TEST_F(QuerySystemTest, SingletonComponentAccess) {
        auto system = coordinator->registerQuerySystem<PhysicsSystem>();

        // Update velocities using game speed from singleton
        system->updateVelocities();

        // Verify velocities were scaled by game speed (2.0)
        for (size_t i = 0; i < 5; ++i) {
            Velocity& vel = coordinator->getComponent<Velocity>(entities[i]);

            EXPECT_FLOAT_EQ(vel.vx, i * 0.5f * 2.0f);
            EXPECT_FLOAT_EQ(vel.vy, i * 1.0f * 2.0f);
            EXPECT_FLOAT_EQ(vel.vz, i * 1.5f * 2.0f);
        }

        // Update singleton and verify changes are reflected
        auto& settings = coordinator->getSingletonComponent<GameSettings>();
        settings.gameSpeed = 3.0f;

        // Reset velocities
        for (size_t i = 0; i < 5; ++i) {
        	auto &vel = coordinator->getComponent<Velocity>(entities[i]);
         	vel.vx = i * 0.5f;
          	vel.vy = i * 1.0f;
          	vel.vz = i * 1.5f;
        }

        system->updateVelocities();

        // Verify velocities were scaled by the new game speed (3.0)
        for (size_t i = 0; i < 5; ++i) {
            Velocity& vel = coordinator->getComponent<Velocity>(entities[i]);

            EXPECT_FLOAT_EQ(vel.vx, i * 0.5f * 3.0f);
            EXPECT_FLOAT_EQ(vel.vy, i * 1.0f * 3.0f);
            EXPECT_FLOAT_EQ(vel.vz, i * 1.5f * 3.0f);
        }
    }

    TEST_F(QuerySystemTest, EntityUpdates) {
        auto system = coordinator->registerQuerySystem<MovementSystem>();

        // Initially system should have 5 entities
        EXPECT_EQ(system->entities.size(), 5);

        // Remove a component from an entity
        coordinator->removeComponent<Velocity>(entities[0]);

        // Verify entity was removed from the system
        EXPECT_EQ(system->entities.size(), 4);

        // Add the component back
        coordinator->addComponent(entities[0], Velocity(99.0f, 99.0f, 99.0f));

        // Verify entity was added back to the system
        EXPECT_EQ(system->entities.size(), 5);

        // Apply gravity again and verify it works for all entities
        system->applyGravity(1.0f);

        // Check the newly re-added entity
        Velocity& vel = coordinator->getComponent<Velocity>(entities[0]);
        EXPECT_FLOAT_EQ(vel.vy, 99.0f - 1.0f * (0.0f / 10.0f));
    }

    TEST_F(QuerySystemTest, AccessingMissingComponent) {
        auto system = coordinator->registerQuerySystem<MovementSystem>();

        // Remove a component
        coordinator->removeComponent<Velocity>(entities[0]);

        // Trying to access the removed component should throw
        EXPECT_THROW(system->getComponent<Velocity>(entities[0]), InternalError);
    }

    TEST_F(QuerySystemTest, EmptySystem) {
        // Remove all relevant components
        for (size_t i = 0; i < 5; ++i) {
            coordinator->removeComponent<Velocity>(entities[i]);
        }

        auto system = coordinator->registerQuerySystem<MovementSystem>();

        // Verify system has no entities
        EXPECT_EQ(system->entities.size(), 0);

        // Operations on empty system should not crash
        EXPECT_NO_THROW(system->applyGravity(1.0f));
    }

    // Define a system with non-registered component
    struct Unregistered {
        int value = 0;
    };

    class SystemWithUnregisteredComponent : public QuerySystem<Read<Unregistered>> {
    };

    TEST_F(QuerySystemTest, UnregisteredComponentAccess) {
        // Creating system with unregistered component should fail
        EXPECT_THROW(coordinator->registerQuerySystem<SystemWithUnregisteredComponent>(), ComponentNotRegistered);
    }
}
