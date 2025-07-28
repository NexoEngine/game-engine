//// GroupSystem.test.cpp /////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/04/2025
//  Description: Test file for the GroupSystem class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "GroupSystem.hpp"
#include "Coordinator.hpp"
#include "Access.hpp"
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
    class GroupSystemTest : public ::testing::Test {
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

    // System with owned Position and read-only access to Velocity
    class PositionSystem : public GroupSystem<Owned<Write<Position>>, NonOwned<Read<Velocity>>> {
    public:
        void updatePositions() {
            auto positions = get<Position>();
            auto velocities = get<Velocity>();
            auto entities = getEntities();

            for (size_t i = 0; i < positions.size(); ++i) {
            	const auto &vel = velocities->get(entities[i]);
                positions[i].x += vel.vx;
                positions[i].y += vel.vy;
                positions[i].z += vel.vz;
            }
        }
    };

    // 2. System with read-only access to Position and Tag
    class ReadOnlySystem : public GroupSystem<Owned<Read<Position>>, NonOwned<Read<Tag>>> {
    public:
        int countEntitiesAboveThreshold(float threshold) {
            auto positions = get<Position>();
            auto tags = get<Tag>();
            int count = 0;

            for (size_t i = 0; i < positions.size(); ++i) {
                if (positions[i].x > threshold) {
                    count++;
                }
            }

            return count;
        }
    };

    // 3. System with singleton component access
    class SystemWithSingleton : public GroupSystem<
        Owned<Write<Position>>,
        NonOwned<Read<Velocity>>,
        ReadSingleton<GameSettings>> {
    public:
        void scaleVelocities() {
            auto positions = get<Position>();
            auto velocities = get<Velocity>();
            const auto entities = getEntities();
            const auto& settings = getSingleton<GameSettings>();

            for (size_t i = 0; i < positions.size(); ++i) {
            	const auto &vel = velocities->get(entities[i]);
                positions[i].x += vel.vx * settings.gameSpeed;
                positions[i].y += vel.vy * settings.gameSpeed;
                positions[i].z += vel.vz * settings.gameSpeed;
            }
        }
    };

    // 4. System with both read and write access to different components
    class MixedAccessSystem : public GroupSystem<
        Owned<Write<Position>, Read<Tag>>,
        NonOwned<Read<Velocity>>> {
    public:
        void updatePositionsByCategory(int category, float multiplier) {
            auto positions = get<Position>();
            auto velocities = get<Velocity>();
            auto tags = get<Tag>();
            auto entities = getEntities();

            for (size_t i = 0; i < positions.size(); ++i) {
                if (tags[i].category == category) {
                    positions[i].x += velocities->get(entities[i]).vx * multiplier;
                    positions[i].y += velocities->get(entities[i]).vy * multiplier;
                    positions[i].z += velocities->get(entities[i]).vz * multiplier;
                }
            }
        }
    };

    TEST_F(GroupSystemTest, SystemCreation) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();
        ASSERT_NE(system, nullptr);

        // Check static type checking for owned components
        EXPECT_TRUE(PositionSystem::isOwnedComponent<Position>());
        EXPECT_FALSE(PositionSystem::isOwnedComponent<Velocity>());
    }

    TEST_F(GroupSystemTest, WriteAccessToOwnedComponents) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Update positions
        system->updatePositions();

        // Verify changes were applied
        for (size_t i = 0; i < entities.size(); ++i) {
            Position& pos = coordinator->getComponent<Position>(entities[i]);
            EXPECT_FLOAT_EQ(pos.x, i * 1.0f + i * 0.5f);
            EXPECT_FLOAT_EQ(pos.y, i * 2.0f + i * 1.0f);
            EXPECT_FLOAT_EQ(pos.z, i * 3.0f + i * 1.5f);
        }
    }

    TEST_F(GroupSystemTest, ReadOnlyComponents) {
        auto system = coordinator->registerGroupSystem<ReadOnlySystem>();

        // Count entities above threshold
        int count = system->countEntitiesAboveThreshold(2.0f);

        // Verify count
        EXPECT_EQ(count, 2); // Entities 3 and 4 have x > 2.0

        // This would cause a compilation error if uncommented - verifying access control:
        // auto positions = system->get<Position>();
        // positions[0].x = 999.0f;  // Cannot modify a read-only component
    }

    //////////////////////////////////////////////////////////////////////////
    // Singleton Component Tests
    //////////////////////////////////////////////////////////////////////////

    TEST_F(GroupSystemTest, SingletonComponentAccess) {
        auto system = coordinator->registerGroupSystem<SystemWithSingleton>();

        // Test accessing singleton component
        system->scaleVelocities();

        // Verify changes reflect the game speed setting (2.0)
        for (size_t i = 0; i < entities.size(); ++i) {
            Position& pos = coordinator->getComponent<Position>(entities[i]);
            EXPECT_FLOAT_EQ(pos.x, i * 1.0f + i * 0.5f * 2.0f);
            EXPECT_FLOAT_EQ(pos.y, i * 2.0f + i * 1.0f * 2.0f);
            EXPECT_FLOAT_EQ(pos.z, i * 3.0f + i * 1.5f * 2.0f);
        }

        // Update singleton and verify changes are reflected
        auto& settings = coordinator->getSingletonComponent<GameSettings>();
        settings.gameSpeed = 3.0f;

        // Reset positions
        for (size_t i = 0; i < entities.size(); ++i) {
        	Position& pos = coordinator->getComponent<Position>(entities[i]);
         	pos.x = i * 1.0f;
         	pos.y = i * 2.0f;
         	pos.z = i * 3.0f;
        }

        system->scaleVelocities();

        // Verify changes reflect the updated game speed (3.0)
        for (size_t i = 0; i < entities.size(); ++i) {
            Position& pos = coordinator->getComponent<Position>(entities[i]);
            EXPECT_FLOAT_EQ(pos.x, i * 1.0f + i * 0.5f * 3.0f);
            EXPECT_FLOAT_EQ(pos.y, i * 2.0f + i * 1.0f * 3.0f);
            EXPECT_FLOAT_EQ(pos.z, i * 3.0f + i * 1.5f * 3.0f);
        }
    }

    TEST_F(GroupSystemTest, MixedAccessToComponents) {
        auto system = coordinator->registerGroupSystem<MixedAccessSystem>();

        // Update positions for category 1 with multiplier 2.0
        system->updatePositionsByCategory(1, 2.0f);

        // Verify changes for entities in category 1
        for (size_t i = 0; i < entities.size(); ++i) {
            Position& pos = coordinator->getComponent<Position>(entities[i]);
            Tag& tag = coordinator->getComponent<Tag>(entities[i]);

            if (tag.category == 1) {
                EXPECT_FLOAT_EQ(pos.x, i * 1.0f + i * 0.5f * 2.0f);
                EXPECT_FLOAT_EQ(pos.y, i * 2.0f + i * 1.0f * 2.0f);
                EXPECT_FLOAT_EQ(pos.z, i * 3.0f + i * 1.5f * 2.0f);
            } else {
                EXPECT_FLOAT_EQ(pos.x, i * 1.0f);
                EXPECT_FLOAT_EQ(pos.y, i * 2.0f);
                EXPECT_FLOAT_EQ(pos.z, i * 3.0f);
            }
        }
    }

    TEST_F(GroupSystemTest, EntityRetrieval) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Get entities
        auto systemEntities = system->getEntities();

        // Verify all entities are accessible
        EXPECT_EQ(systemEntities.size(), entities.size());

        // Verify the entities match
        std::vector<Entity> entityVec(systemEntities.begin(), systemEntities.end());
        std::sort(entityVec.begin(), entityVec.end());
        std::sort(entities.begin(), entities.end());

        for (size_t i = 0; i < entities.size(); ++i) {
            EXPECT_EQ(entityVec[i], entities[i]);
        }
    }

    TEST_F(GroupSystemTest, EntityRemoval) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Initial entity count
        EXPECT_EQ(system->getEntities().size(), entities.size());

        // Remove a component from an entity
        coordinator->removeComponent<Position>(entities[0]);

        // Verify entity was removed from the group
        EXPECT_EQ(system->getEntities().size(), entities.size() - 1);

        // Verify the right entity was removed
        bool found = false;
        for (auto entity : system->getEntities()) {
            if (entity == entities[0]) {
                found = true;
                break;
            }
        }
        EXPECT_FALSE(found);
    }

    TEST_F(GroupSystemTest, EmptyGroup) {
    	auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Remove all Position components
        for (auto entity : entities) {
            coordinator->removeComponent<Position>(entity);
        }

        // Verify empty group
        EXPECT_EQ(system->getEntities().size(), 0);

        // Verify component spans are empty
        auto positions = system->get<Position>();
        EXPECT_EQ(positions.size(), 0);

        // Test operations on empty group - should not crash
        system->updatePositions();
    }

    // Test with system that accesses non-registered component
    struct Unregistered {
        int value = 0;
    };

    class SystemWithUnregisteredComponent : public GroupSystem<Owned<Write<Unregistered>>> {
    };

    TEST_F(GroupSystemTest, UnregisteredComponentAccess) {
        // This should fail at runtime
        EXPECT_THROW(coordinator->registerGroupSystem<SystemWithUnregisteredComponent>(), ComponentNotRegistered);
    }
}
