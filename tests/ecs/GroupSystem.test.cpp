//// GroupSystem.test.cpp /////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
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

    //////////////////////////////////////////////////////////////////////////
    // Edge Case Tests - Group Creation with Various Component Combinations
    //////////////////////////////////////////////////////////////////////////

    // System with multiple owned components
    class MultiOwnedSystem : public GroupSystem<Owned<Write<Position>, Write<Velocity>, Read<Tag>>> {
    public:
        int countAll() {
            auto entities = getEntities();
            return entities.size();
        }
    };

    TEST_F(GroupSystemTest, GroupCreationWithMultipleOwnedComponents) {
        auto system = coordinator->registerGroupSystem<MultiOwnedSystem>();
        ASSERT_NE(system, nullptr);

        // Verify all entities are in the group (all have Position, Velocity, Tag)
        EXPECT_EQ(system->getEntities().size(), entities.size());

        // Verify static type checking
        EXPECT_TRUE(MultiOwnedSystem::isOwnedComponent<Position>());
        EXPECT_TRUE(MultiOwnedSystem::isOwnedComponent<Velocity>());
        EXPECT_TRUE(MultiOwnedSystem::isOwnedComponent<Tag>());
    }

    // System with multiple non-owned components
    class MultiNonOwnedSystem : public GroupSystem<
        Owned<Write<Position>>,
        NonOwned<Read<Velocity>, Read<Tag>>> {
    public:
        void processEntities() {
            auto positions = get<Position>();
            auto velocities = get<Velocity>();
            auto tags = get<Tag>();
            auto entities = getEntities();

            for (size_t i = 0; i < positions.size(); ++i) {
                positions[i].x += velocities->get(entities[i]).vx;
            }
        }
    };

    TEST_F(GroupSystemTest, GroupCreationWithMultipleNonOwnedComponents) {
        auto system = coordinator->registerGroupSystem<MultiNonOwnedSystem>();
        ASSERT_NE(system, nullptr);

        // Verify type checking
        EXPECT_TRUE(MultiNonOwnedSystem::isOwnedComponent<Position>());
        EXPECT_FALSE(MultiNonOwnedSystem::isOwnedComponent<Velocity>());
        EXPECT_FALSE(MultiNonOwnedSystem::isOwnedComponent<Tag>());

        // Process entities
        system->processEntities();

        // Verify changes were applied
        for (size_t i = 0; i < entities.size(); ++i) {
            Position& pos = coordinator->getComponent<Position>(entities[i]);
            EXPECT_FLOAT_EQ(pos.x, i * 1.0f + i * 0.5f);
        }
    }

    // System with only read access
    class ReadOnlyMultiSystem : public GroupSystem<
        Owned<Read<Position>, Read<Velocity>, Read<Tag>>> {
    public:
        int sumCategories() {
            auto tags = get<Tag>();
            int sum = 0;
            for (size_t i = 0; i < tags.size(); ++i) {
                sum += tags[i].category;
            }
            return sum;
        }
    };

    TEST_F(GroupSystemTest, GroupCreationWithOnlyReadAccess) {
        auto system = coordinator->registerGroupSystem<ReadOnlyMultiSystem>();
        ASSERT_NE(system, nullptr);

        int sum = system->sumCategories();

        // Categories are: 0, 1, 2, 0, 1 -> sum = 4
        EXPECT_EQ(sum, 4);
    }

    //////////////////////////////////////////////////////////////////////////
    // Edge Case Tests - Entity Addition/Removal
    //////////////////////////////////////////////////////////////////////////

    TEST_F(GroupSystemTest, EntityAdditionDuringRuntime) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        size_t initialCount = system->getEntities().size();
        EXPECT_EQ(initialCount, 5);

        // Create new entity with required components
        Entity newEntity = coordinator->createEntity();
        coordinator->addComponent(newEntity, Position(10.0f, 20.0f, 30.0f));
        coordinator->addComponent(newEntity, Velocity(1.0f, 2.0f, 3.0f));

        // Verify entity was added to the group
        EXPECT_EQ(system->getEntities().size(), initialCount + 1);

        // Clean up
        coordinator->destroyEntity(newEntity);
    }

    TEST_F(GroupSystemTest, MultipleEntityRemoval) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Remove multiple components
        coordinator->removeComponent<Position>(entities[0]);
        coordinator->removeComponent<Position>(entities[1]);
        coordinator->removeComponent<Position>(entities[2]);

        // Verify entities were removed from the group
        EXPECT_EQ(system->getEntities().size(), 2);

        // Verify correct entities remain
        auto groupEntities = system->getEntities();
        std::set<Entity> remaining(groupEntities.begin(), groupEntities.end());
        EXPECT_TRUE(remaining.find(entities[3]) != remaining.end());
        EXPECT_TRUE(remaining.find(entities[4]) != remaining.end());
    }

    TEST_F(GroupSystemTest, EntityDestructionRemovesFromGroup) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        size_t initialCount = system->getEntities().size();

        // Destroy an entity
        coordinator->destroyEntity(entities[2]);

        // Verify entity was removed from the group
        EXPECT_EQ(system->getEntities().size(), initialCount - 1);

        // Verify destroyed entity is not in the group
        auto groupEntities = system->getEntities();
        for (auto entity : groupEntities) {
            EXPECT_NE(entity, entities[2]);
        }

        // Remove from test entities to avoid double-free in teardown
        entities.erase(entities.begin() + 2);
    }

    TEST_F(GroupSystemTest, AddComponentToEntityAlreadyInGroup) {
        // Create a simple component
        struct Health {
            int hp = 100;
        };

        coordinator->registerComponent<Health>();

        auto system = coordinator->registerGroupSystem<PositionSystem>();
        size_t initialCount = system->getEntities().size();

        // Add a component that doesn't affect group membership
        coordinator->addComponent(entities[0], Health{50});

        // Group size should remain the same
        EXPECT_EQ(system->getEntities().size(), initialCount);
    }

    //////////////////////////////////////////////////////////////////////////
    // Edge Case Tests - Component Access Edge Cases
    //////////////////////////////////////////////////////////////////////////

    TEST_F(GroupSystemTest, AccessOwnedComponentSpan) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Get owned component span
        auto positions = system->get<Position>();

        // Verify span size matches group size
        EXPECT_EQ(positions.size(), entities.size());

        // Modify components through span
        for (size_t i = 0; i < positions.size(); ++i) {
            positions[i].x = 100.0f + i;
        }

        // Verify changes were applied
        for (size_t i = 0; i < entities.size(); ++i) {
            Position& pos = coordinator->getComponent<Position>(entities[i]);
            EXPECT_FLOAT_EQ(pos.x, 100.0f + i);
        }
    }

    TEST_F(GroupSystemTest, AccessNonOwnedComponentArray) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Get non-owned component array
        auto velocities = system->get<Velocity>();

        // Verify we can read components
        auto entities = system->getEntities();
        for (size_t i = 0; i < entities.size(); ++i) {
            const auto& vel = velocities->get(entities[i]);
            EXPECT_FLOAT_EQ(vel.vx, i * 0.5f);
        }
    }

    TEST_F(GroupSystemTest, IterateOverEmptyGroupComponents) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Remove all entities
        for (auto entity : entities) {
            coordinator->removeComponent<Position>(entity);
        }

        // Get components from empty group
        auto positions = system->get<Position>();
        auto velocities = system->get<Velocity>();

        // Verify empty spans/arrays
        EXPECT_EQ(positions.size(), 0);
        EXPECT_EQ(system->getEntities().size(), 0);
    }

    //////////////////////////////////////////////////////////////////////////
    // Edge Case Tests - Multiple Groups with Overlapping Components
    //////////////////////////////////////////////////////////////////////////

    class VelocityTagSystem : public GroupSystem<
        Owned<Write<Velocity>>,
        NonOwned<Read<Tag>>> {
    public:
        void scaleVelocities(float factor) {
            auto velocities = get<Velocity>();
            for (size_t i = 0; i < velocities.size(); ++i) {
                velocities[i].vx *= factor;
                velocities[i].vy *= factor;
                velocities[i].vz *= factor;
            }
        }
    };

    TEST_F(GroupSystemTest, MultipleGroupsWithOverlappingComponents) {
        auto positionSystem = coordinator->registerGroupSystem<PositionSystem>();
        auto velocitySystem = coordinator->registerGroupSystem<VelocityTagSystem>();

        ASSERT_NE(positionSystem, nullptr);
        ASSERT_NE(velocitySystem, nullptr);

        // Both systems should have same entities (all have Position, Velocity, Tag)
        EXPECT_EQ(positionSystem->getEntities().size(), entities.size());
        EXPECT_EQ(velocitySystem->getEntities().size(), entities.size());

        // Modify through velocity system
        velocitySystem->scaleVelocities(2.0f);

        // Verify changes are visible in coordinator
        for (size_t i = 0; i < entities.size(); ++i) {
            Velocity& vel = coordinator->getComponent<Velocity>(entities[i]);
            EXPECT_FLOAT_EQ(vel.vx, i * 0.5f * 2.0f);
        }
    }

    TEST_F(GroupSystemTest, DifferentGroupsWithPartiallyOverlappingEntities) {
        // Create position+velocity system first
        auto positionVelocitySystem = coordinator->registerGroupSystem<PositionSystem>();

        // Initially all entities have both components
        EXPECT_EQ(positionVelocitySystem->getEntities().size(), 5);

        // Remove Velocity from some entities
        coordinator->removeComponent<Velocity>(entities[0]);
        coordinator->removeComponent<Velocity>(entities[2]);

        // Position+Velocity system should now have fewer entities
        EXPECT_EQ(positionVelocitySystem->getEntities().size(), 3);

        // Verify correct entities remain in the group
        auto groupEntities = positionVelocitySystem->getEntities();
        std::set<Entity> remaining(groupEntities.begin(), groupEntities.end());
        EXPECT_TRUE(remaining.find(entities[1]) != remaining.end());
        EXPECT_TRUE(remaining.find(entities[3]) != remaining.end());
        EXPECT_TRUE(remaining.find(entities[4]) != remaining.end());
    }

    //////////////////////////////////////////////////////////////////////////
    // Edge Case Tests - Single Entity in Group
    //////////////////////////////////////////////////////////////////////////

    TEST_F(GroupSystemTest, SingleEntityInGroup) {
        // Remove all but one entity
        for (size_t i = 1; i < entities.size(); ++i) {
            coordinator->removeComponent<Position>(entities[i]);
        }

        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Verify single entity
        EXPECT_EQ(system->getEntities().size(), 1);

        // Get components
        auto positions = system->get<Position>();
        EXPECT_EQ(positions.size(), 1);

        // Modify component
        positions[0].x = 999.0f;

        // Verify change
        Position& pos = coordinator->getComponent<Position>(entities[0]);
        EXPECT_FLOAT_EQ(pos.x, 999.0f);
    }

    TEST_F(GroupSystemTest, SingleEntityOperations) {
        // Remove all but one entity
        for (size_t i = 1; i < entities.size(); ++i) {
            coordinator->removeComponent<Position>(entities[i]);
        }

        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Update positions (should handle single entity correctly)
        system->updatePositions();

        // Verify update was applied
        Position& pos = coordinator->getComponent<Position>(entities[0]);
        EXPECT_FLOAT_EQ(pos.x, 0.0f);  // 0 + 0
        EXPECT_FLOAT_EQ(pos.y, 0.0f);  // 0 + 0
        EXPECT_FLOAT_EQ(pos.z, 0.0f);  // 0 + 0
    }

    //////////////////////////////////////////////////////////////////////////
    // Edge Case Tests - Singleton Component Combinations
    //////////////////////////////////////////////////////////////////////////

    class MultipleSingletonSystem : public GroupSystem<
        Owned<Write<Position>>,
        NonOwned<Read<Velocity>>,
        ReadSingleton<GameSettings>> {
    public:
        void processWithSingleton() {
            const auto& settings = getSingleton<GameSettings>();
            auto positions = get<Position>();

            for (size_t i = 0; i < positions.size(); ++i) {
                if (settings.debugMode) {
                    positions[i].x += 10.0f;
                }
            }
        }
    };

    TEST_F(GroupSystemTest, SingletonComponentWithMultipleAccess) {
        auto system = coordinator->registerGroupSystem<MultipleSingletonSystem>();

        // Access singleton
        const auto& settings = system->getSingleton<GameSettings>();
        EXPECT_TRUE(settings.debugMode);
        EXPECT_FLOAT_EQ(settings.gameSpeed, 2.0f);

        // Process with singleton
        system->processWithSingleton();

        // Verify changes
        for (size_t i = 0; i < entities.size(); ++i) {
            Position& pos = coordinator->getComponent<Position>(entities[i]);
            EXPECT_FLOAT_EQ(pos.x, i * 1.0f + 10.0f);
        }
    }

    TEST_F(GroupSystemTest, SingletonChangeReflectedInAllSystems) {
        auto system1 = coordinator->registerGroupSystem<SystemWithSingleton>();
        auto system2 = coordinator->registerGroupSystem<MultipleSingletonSystem>();

        // Modify singleton
        auto& settings = coordinator->getSingletonComponent<GameSettings>();
        settings.debugMode = false;
        settings.gameSpeed = 5.0f;

        // Both systems should see the change
        const auto& settings1 = system1->getSingleton<GameSettings>();
        const auto& settings2 = system2->getSingleton<GameSettings>();

        EXPECT_FALSE(settings1.debugMode);
        EXPECT_FLOAT_EQ(settings1.gameSpeed, 5.0f);
        EXPECT_FALSE(settings2.debugMode);
        EXPECT_FLOAT_EQ(settings2.gameSpeed, 5.0f);
    }

    //////////////////////////////////////////////////////////////////////////
    // Edge Case Tests - Component Data Integrity
    //////////////////////////////////////////////////////////////////////////

    TEST_F(GroupSystemTest, ComponentDataIntegrityAfterMultipleOperations) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Store original values
        std::map<Entity, Position> originalPositions;
        for (auto entity : entities) {
            originalPositions[entity] = coordinator->getComponent<Position>(entity);
        }

        // Perform multiple updates
        system->updatePositions();
        system->updatePositions();
        system->updatePositions();

        // Verify cumulative changes
        for (size_t i = 0; i < entities.size(); ++i) {
            Position& pos = coordinator->getComponent<Position>(entities[i]);
            EXPECT_FLOAT_EQ(pos.x, i * 1.0f + i * 0.5f * 3);
            EXPECT_FLOAT_EQ(pos.y, i * 2.0f + i * 1.0f * 3);
            EXPECT_FLOAT_EQ(pos.z, i * 3.0f + i * 1.5f * 3);
        }
    }

    TEST_F(GroupSystemTest, ComponentAccessConsistencyAcrossSystems) {
        auto positionSystem = coordinator->registerGroupSystem<PositionSystem>();

        // Modify through position system
        positionSystem->updatePositions();

        // Verify changes are visible through the coordinator
        auto positions = positionSystem->get<Position>();
        auto entities_vec = positionSystem->getEntities();

        for (size_t i = 0; i < positions.size(); ++i) {
            Entity e = entities_vec[i];
            Position& coordPos = coordinator->getComponent<Position>(e);

            // System view should match what coordinator has
            EXPECT_FLOAT_EQ(positions[i].x, coordPos.x);
            EXPECT_FLOAT_EQ(positions[i].y, coordPos.y);
            EXPECT_FLOAT_EQ(positions[i].z, coordPos.z);
        }

        // Modify through coordinator
        for (auto entity : entities) {
            Position& pos = coordinator->getComponent<Position>(entity);
            pos.x += 100.0f;
        }

        // Changes should be visible in system
        positions = positionSystem->get<Position>();
        for (size_t i = 0; i < positions.size(); ++i) {
            Entity e = entities_vec[i];
            Position& coordPos = coordinator->getComponent<Position>(e);

            // System view should reflect coordinator changes
            EXPECT_FLOAT_EQ(positions[i].x, coordPos.x);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Edge Case Tests - Boundary Conditions
    //////////////////////////////////////////////////////////////////////////

    TEST_F(GroupSystemTest, EmptyGroupAfterAllEntitiesDestroyed) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Destroy all entities
        for (auto entity : entities) {
            coordinator->destroyEntity(entity);
        }
        entities.clear();

        // Verify empty group
        EXPECT_EQ(system->getEntities().size(), 0);

        // Operations on empty group should not crash
        auto positions = system->get<Position>();
        EXPECT_EQ(positions.size(), 0);

        system->updatePositions();
    }

    TEST_F(GroupSystemTest, LargeNumberOfEntities) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Create many entities
        std::vector<Entity> largeEntitySet;
        for (int i = 0; i < 100; ++i) {
            Entity e = coordinator->createEntity();
            coordinator->addComponent(e, Position(i * 1.0f, i * 2.0f, i * 3.0f));
            coordinator->addComponent(e, Velocity(i * 0.1f, i * 0.2f, i * 0.3f));
            largeEntitySet.push_back(e);
        }

        // Verify all entities are in the group
        EXPECT_EQ(system->getEntities().size(), entities.size() + 100);

        // Process all entities
        system->updatePositions();

        // Clean up
        for (auto e : largeEntitySet) {
            coordinator->destroyEntity(e);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Edge Case Tests - Access Permission Validation
    //////////////////////////////////////////////////////////////////////////

    TEST_F(GroupSystemTest, WriteAccessModifiesComponents) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        auto positions = system->get<Position>();

        // Write access should allow modification
        for (size_t i = 0; i < positions.size(); ++i) {
            positions[i].x = 500.0f;
        }

        // Verify modifications persisted
        for (auto entity : entities) {
            Position& pos = coordinator->getComponent<Position>(entity);
            EXPECT_FLOAT_EQ(pos.x, 500.0f);
        }
    }

    TEST_F(GroupSystemTest, MixedReadWriteAccess) {
        auto system = coordinator->registerGroupSystem<MixedAccessSystem>();

        // Get components
        auto positions = system->get<Position>();  // Write access
        auto tags = system->get<Tag>();            // Read access

        // Verify we can read tags
        for (size_t i = 0; i < tags.size(); ++i) {
            EXPECT_EQ(tags[i].category, i % 3);
        }

        // Verify we can write positions
        for (size_t i = 0; i < positions.size(); ++i) {
            positions[i].x = 1000.0f;
        }

        // Verify changes persisted
        for (auto entity : entities) {
            Position& pos = coordinator->getComponent<Position>(entity);
            EXPECT_FLOAT_EQ(pos.x, 1000.0f);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // Edge Case Tests - Entity Retrieval Consistency
    //////////////////////////////////////////////////////////////////////////

    TEST_F(GroupSystemTest, EntityOrderConsistency) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Get entities multiple times
        auto entities1 = system->getEntities();
        auto entities2 = system->getEntities();

        // Order should be consistent
        EXPECT_EQ(entities1.size(), entities2.size());
        for (size_t i = 0; i < entities1.size(); ++i) {
            EXPECT_EQ(entities1[i], entities2[i]);
        }
    }

    TEST_F(GroupSystemTest, EntityRetrievalAfterModifications) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        auto entitiesBefore = system->getEntities();
        std::vector<Entity> beforeVec(entitiesBefore.begin(), entitiesBefore.end());

        // Modify components (but don't change group membership)
        auto positions = system->get<Position>();
        for (size_t i = 0; i < positions.size(); ++i) {
            positions[i].x += 100.0f;
        }

        auto entitiesAfter = system->getEntities();
        std::vector<Entity> afterVec(entitiesAfter.begin(), entitiesAfter.end());

        // Entity list should be unchanged
        EXPECT_EQ(beforeVec, afterVec);
    }

    //////////////////////////////////////////////////////////////////////////
    // Edge Case Tests - Component Combinations
    //////////////////////////////////////////////////////////////////////////

    TEST_F(GroupSystemTest, AllComponentTypesInSystem) {
        class AllTypesSystem : public GroupSystem<
            Owned<Write<Position>, Read<Tag>>,
            NonOwned<Read<Velocity>>,
            ReadSingleton<GameSettings>> {
        public:
            void process() {
                auto positions = get<Position>();
                auto tags = get<Tag>();
                auto velocities = get<Velocity>();
                auto entities = getEntities();
                const auto& settings = getSingleton<GameSettings>();

                for (size_t i = 0; i < positions.size(); ++i) {
                    positions[i].x += velocities->get(entities[i]).vx * settings.gameSpeed;
                }
            }
        };

        auto system = coordinator->registerGroupSystem<AllTypesSystem>();
        ASSERT_NE(system, nullptr);

        system->process();

        // Verify changes
        for (size_t i = 0; i < entities.size(); ++i) {
            Position& pos = coordinator->getComponent<Position>(entities[i]);
            EXPECT_FLOAT_EQ(pos.x, i * 1.0f + i * 0.5f * 2.0f);
        }
    }

    TEST_F(GroupSystemTest, ComponentAccessAfterEntityRecreation) {
        auto system = coordinator->registerGroupSystem<PositionSystem>();

        // Store first entity ID
        Entity firstEntity = entities[0];

        // Destroy and recreate entity with same components
        coordinator->destroyEntity(firstEntity);
        Entity newEntity = coordinator->createEntity();
        coordinator->addComponent(newEntity, Position(99.0f, 99.0f, 99.0f));
        coordinator->addComponent(newEntity, Velocity(9.0f, 9.0f, 9.0f));

        // Replace in test entities
        entities[0] = newEntity;

        // System should now have the new entity
        auto groupEntities = system->getEntities();
        bool found = false;
        for (auto entity : groupEntities) {
            if (entity == newEntity) {
                found = true;
                break;
            }
        }
        EXPECT_TRUE(found);

        // Verify component access works
        auto positions = system->get<Position>();
        EXPECT_GT(positions.size(), 0);
    }
}
