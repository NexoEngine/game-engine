//// Group.test.cpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      iMeaNz
//  Date:        03/04/2025
//  Description: Test file for the ECS Group class
//
///////////////////////////////////////////////////////////////////////////////

#include <climits>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ecs/Definitions.hpp"
#include "ecs/ECSExceptions.hpp"
#include "ecs/ComponentArray.hpp"
#include "ecs/Components.hpp"
#include "ecs/Group.hpp"

namespace nexo::ecs {

    // Define test components
    struct Position {
        float x;
        float y;

        bool operator==(const Position& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct Velocity {
        float x;
        float y;

        bool operator==(const Velocity& other) const {
            return x == other.x && y == other.y;
        }
    };

    struct Health {
        int value;

        bool operator==(const Health& other) const {
            return value == other.value;
        }
    };

    struct Tag {
        std::string name;

        bool operator==(const Tag& other) const {
            return name == other.name;
        }
    };

    class GroupTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // Create component arrays
            positionArray = std::make_shared<ComponentArray<Position>>();
            velocityArray = std::make_shared<ComponentArray<Velocity>>();
            healthArray = std::make_shared<ComponentArray<Health>>();
            tagArray = std::make_shared<ComponentArray<Tag>>();

            // Insert test data
            for (Entity i = 0; i < 10; ++i) {
                positionArray->insertData(i, Position{static_cast<float>(i), static_cast<float>(i * 2)});
                velocityArray->insertData(i, Velocity{static_cast<float>(i + 0.5f), static_cast<float>((i + 1) * 2)});
                healthArray->insertData(i, Health{100 - static_cast<int>(i * 10)});
                tagArray->insertData(i, Tag{"Entity" + std::to_string(i)});

                // Add entities 0-4 to the group section
                if (i < 5) {
                    positionArray->addToGroup(i);
                    velocityArray->addToGroup(i);
                    healthArray->addToGroup(i);
                    tagArray->addToGroup(i);
                }
            }
        }

        // Helper method to set up health values for partition tests
        void groupHealthData() {
            // Modify health values to have distinct groups
            // Entity 0: Health 90 (High)
            // Entity 1: Health 60 (Medium)
            // Entity 2: Health 30 (Low)
            // Entity 3: Health 80 (High)
            // Entity 4: Health 20 (Low)
            healthArray->getData(0).value = 90;
            healthArray->getData(1).value = 60;
            healthArray->getData(2).value = 30;
            healthArray->getData(3).value = 80;
            healthArray->getData(4).value = 20;
        }

        // Helper method to create groups with different configurations
        template<typename... Owned>
        auto createGroup(const get_t<>& nonOwned = get<>()) {
            using OwnedTuple = std::tuple<std::shared_ptr<ComponentArray<Owned>>...>;
            using NonOwnedTuple = std::tuple<>;
            auto ownedArrays = std::make_tuple(getComponentArray<Owned>()...);
            auto nonOwnedArrays = std::make_tuple();
            return std::make_shared<Group<OwnedTuple, NonOwnedTuple>>(ownedArrays, nonOwnedArrays);
        }

        template<typename... Owned, typename... NonOwned>
        auto createGroup(const get_t<NonOwned...>& nonOwned) {
            using OwnedTuple = std::tuple<std::shared_ptr<ComponentArray<Owned>>...>;
            using NonOwnedTuple = std::tuple<std::shared_ptr<ComponentArray<NonOwned>>...>;
            auto ownedArrays = std::make_tuple(getComponentArray<Owned>()...);
            auto nonOwnedArrays = std::make_tuple(getComponentArray<NonOwned>()...);
            return std::make_shared<Group<OwnedTuple, NonOwnedTuple>>(ownedArrays, nonOwnedArrays);
        }



        template<typename T>
        std::shared_ptr<ComponentArray<T>> getComponentArray() {
            if constexpr (std::is_same_v<T, Position>) {
                return positionArray;
            } else if constexpr (std::is_same_v<T, Velocity>) {
                return velocityArray;
            } else if constexpr (std::is_same_v<T, Health>) {
                return healthArray;
            } else if constexpr (std::is_same_v<T, Tag>) {
                return tagArray;
            }
        }

        std::shared_ptr<ComponentArray<Position>> positionArray;
        std::shared_ptr<ComponentArray<Velocity>> velocityArray;
        std::shared_ptr<ComponentArray<Health>> healthArray;
        std::shared_ptr<ComponentArray<Tag>> tagArray;
    };

    // ========================================================================
    // Basic Functionality Tests
    // ========================================================================

    TEST_F(GroupTest, Construction) {
        auto group = createGroup<Position, Velocity>(get<Health, Tag>());

        // Check that signatures are correctly set
        EXPECT_TRUE(group->ownedSignature().test(getComponentTypeID<Position>()));
        EXPECT_TRUE(group->ownedSignature().test(getComponentTypeID<Velocity>()));
        EXPECT_FALSE(group->ownedSignature().test(getComponentTypeID<Health>()));
        EXPECT_FALSE(group->ownedSignature().test(getComponentTypeID<Tag>()));

        // Check all signature
        EXPECT_TRUE(group->allSignature().test(getComponentTypeID<Position>()));
        EXPECT_TRUE(group->allSignature().test(getComponentTypeID<Velocity>()));
        EXPECT_TRUE(group->allSignature().test(getComponentTypeID<Health>()));
        EXPECT_TRUE(group->allSignature().test(getComponentTypeID<Tag>()));

        // Verify size
        EXPECT_EQ(group->size(), 5);
    }

    TEST_F(GroupTest, AddToGroup) {
        auto group = createGroup<Position, Velocity>();

        // Initially the group has entities 0-4
        EXPECT_EQ(group->size(), 5);

        // Add entity 5 to the group
        group->addToGroup(5);
        EXPECT_EQ(group->size(), 6);

        // Check that entity 5 is in the group using the entities() method
        auto entities = group->entities();
        EXPECT_EQ(entities.size(), 6);
        EXPECT_NE(std::find(entities.begin(), entities.end(), 5), entities.end());
    }

    TEST_F(GroupTest, RemoveFromGroup) {
        auto group = createGroup<Position, Velocity>();

        // Initially the group has entities 0-4
        EXPECT_EQ(group->size(), 5);

        // Remove entity 2 from the group
        group->removeFromGroup(2);
        EXPECT_EQ(group->size(), 4);

        // Check that entity 2 is no longer in the group
        auto entities = group->entities();
        EXPECT_EQ(entities.size(), 4);
        EXPECT_EQ(std::find(entities.begin(), entities.end(), 2), entities.end());
    }

    TEST_F(GroupTest, EntitiesMethod) {
        auto group = createGroup<Position, Velocity>();

        // Check that entities() returns the correct entities
        auto entities = group->entities();
        EXPECT_EQ(entities.size(), 5);

        // Entities should be 0-4
        for (Entity i = 0; i < 5; ++i) {
            EXPECT_NE(std::find(entities.begin(), entities.end(), i), entities.end());
        }

        // Entity 5 should not be in the group
        EXPECT_EQ(std::find(entities.begin(), entities.end(), 5), entities.end());
    }

    TEST_F(GroupTest, SortingInvalidatedFlag) {
        auto group = createGroup<Position, Velocity>();

        // Initially sorting should be invalidated
        EXPECT_TRUE(group->sortingInvalidated());

        // Sort the group
        group->sortBy<Position, float>([](const Position& p) { return p.x; });

        // Sorting should no longer be invalidated
        EXPECT_FALSE(group->sortingInvalidated());

        // Adding a new entity should invalidate sorting
        group->addToGroup(5);
        EXPECT_TRUE(group->sortingInvalidated());

        // Sort again
        group->sortBy<Position, float>([](const Position& p) { return p.x; });
        EXPECT_FALSE(group->sortingInvalidated());

        // Removing an entity should invalidate sorting
        group->removeFromGroup(0);
        EXPECT_TRUE(group->sortingInvalidated());
    }

    // ========================================================================
    // Iterator Tests
    // ========================================================================

    TEST_F(GroupTest, IteratorBasics) {
        auto group = createGroup<Position, Velocity>();

        // Check begin() and end()
        auto it = group->begin();
        auto end = group->end();

        EXPECT_NE(it, end);

        // Advance iterator manually and check it eventually reaches end
        std::size_t count = 0;
        while (it != end) {
            ++it;
            ++count;
        }

        EXPECT_EQ(count, 5);
    }

    TEST_F(GroupTest, IteratorDereference) {
        auto group = createGroup<Position>();

        // Dereference the first iterator and check entity and position
        auto it = group->begin();
        auto [entity, position] = *it;

        EXPECT_EQ(entity, 0);
        EXPECT_FLOAT_EQ(position.x, 0.0f);
        EXPECT_FLOAT_EQ(position.y, 0.0f);

        // Move to the next entity
        ++it;
        auto [entity2, position2] = *it;

        EXPECT_EQ(entity2, 1);
        EXPECT_FLOAT_EQ(position2.x, 1.0f);
        EXPECT_FLOAT_EQ(position2.y, 2.0f);
    }

    TEST_F(GroupTest, RangeBasedFor) {
        auto group = createGroup<Position, Velocity>();

        std::vector<Entity> entities;
        std::vector<Position> positions;
        std::vector<Velocity> velocities;

        // Use range-based for loop to collect all entities and components
        for (auto [entity, position, velocity] : *group) {
            entities.push_back(entity);
            positions.push_back(position);
            velocities.push_back(velocity);
        }

        // Check that we got all 5 entities
        EXPECT_EQ(entities.size(), 5);

        // Check that the entities and components match
        for (std::size_t i = 0; i < entities.size(); ++i) {
            EXPECT_EQ(entities[i], i);
            EXPECT_FLOAT_EQ(positions[i].x, i);
            EXPECT_FLOAT_EQ(positions[i].y, i * 2);
            EXPECT_FLOAT_EQ(velocities[i].x, i + 0.5f);
            EXPECT_FLOAT_EQ(velocities[i].y, (i + 1) * 2);
        }
    }

    TEST_F(GroupTest, EachMethod) {
        auto group = createGroup<Position, Velocity>(get<Health>());

        std::vector<Entity> entities;
        std::vector<Position> positions;
        std::vector<Velocity> velocities;
        std::vector<Health> healths;

        // Use each method to collect all entities and components
        group->each([&](Entity entity, Position& position, Velocity& velocity, Health& health) {
            entities.push_back(entity);
            positions.push_back(position);
            velocities.push_back(velocity);
            healths.push_back(health);
        });

        // Check that we got all 5 entities
        EXPECT_EQ(entities.size(), 5);

        // Check that the entities and components match
        for (std::size_t i = 0; i < entities.size(); ++i) {
            EXPECT_EQ(entities[i], i);
            EXPECT_FLOAT_EQ(positions[i].x, i);
            EXPECT_FLOAT_EQ(positions[i].y, i * 2);
            EXPECT_FLOAT_EQ(velocities[i].x, i + 0.5f);
            EXPECT_FLOAT_EQ(velocities[i].y, (i + 1) * 2);
            EXPECT_EQ(healths[i], Health{100 - static_cast<int>(i * 10)});
        }
    }

    TEST_F(GroupTest, EachInRangeMethod) {
        auto group = createGroup<Position, Velocity>();

        std::vector<Entity> entities;

        // Use eachInRange to collect entities from index 1 to 3
        group->eachInRange(1, 3, [&](Entity entity, Position& position, Velocity& velocity) {
            entities.push_back(entity);
        });

        // Check that we got entities 1, 2, and 3
        EXPECT_EQ(entities.size(), 3);
        EXPECT_EQ(entities[0], 1);
        EXPECT_EQ(entities[1], 2);
        EXPECT_EQ(entities[2], 3);
    }

    TEST_F(GroupTest, EachInRangeOutOfBounds) {
        auto group = createGroup<Position, Velocity>();

        std::vector<Entity> entities;

        // Start at a valid index but request too many elements
        group->eachInRange(3, 10, [&](Entity entity, Position& position, Velocity& velocity) {
            entities.push_back(entity);
        });

        // We should only get entities 3 and 4 (not out of bounds)
        EXPECT_EQ(entities.size(), 2);
        EXPECT_EQ(entities[0], 3);
        EXPECT_EQ(entities[1], 4);
    }

    // ========================================================================
    // Component Access Tests
    // ========================================================================

    TEST_F(GroupTest, GetMethod) {
        auto group = createGroup<Position, Velocity>();

        // Get all positions
        auto positions = group->get<Position>();

        // Check that the positions are correct
        EXPECT_EQ(positions.size(), 5);
        for (std::size_t i = 0; i < positions.size(); ++i) {
	        EXPECT_FLOAT_EQ(positions[i].x, i);
	        EXPECT_FLOAT_EQ(positions[i].y, i * 2);
        }

        // Get all velocities
        auto velocities = group->get<Velocity>();

        // Check that the velocities are correct
        EXPECT_EQ(velocities.size(), 5);
        for (std::size_t i = 0; i < velocities.size(); ++i) {
	        EXPECT_FLOAT_EQ(velocities[i].x, i + 0.5f);
	        EXPECT_FLOAT_EQ(velocities[i].y, (i + 1) * 2);
        }
    }

    TEST_F(GroupTest, GetMethodWithNonOwnedComponent) {
        auto group = createGroup<Position>(get<Velocity>());

        // Get all positions (owned)
        auto positions = group->get<Position>();

        // Check that the positions are correct
        EXPECT_EQ(positions.size(), 5);
        for (std::size_t i = 0; i < positions.size(); ++i) {
	        EXPECT_FLOAT_EQ(positions[i].x, i);
	        EXPECT_FLOAT_EQ(positions[i].y, i * 2);
        }

        // Try to get velocities (non-owned) - this should compile but might have different behavior
        // For our implementation, we still want to access the velocity components
        auto velocities = group->get<Velocity>();

        // The non-owned components should be accessible through the get method
        EXPECT_GT(velocities->size(), 0);
    }

    // ========================================================================
    // Sorting Tests
    // ========================================================================

    TEST_F(GroupTest, SortByPositionXAscending) {
        auto group = createGroup<Position, Velocity>();

        // Add entities in reverse order to ensure sorting will change the order
        for (Entity i = 9; i >= 5; --i) {
            positionArray->addToGroup(i);
            velocityArray->addToGroup(i);
        }

        // Sort by position.x in ascending order
        group->sortBy<Position, float>([](const Position& p) { return p.x; });

        // Check that entities are sorted correctly
        auto entities = group->entities();

        // Entities should be in order 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
        for (std::size_t i = 0; i < entities.size(); ++i) {
            EXPECT_EQ(entities[i], i);
        }
    }

    TEST_F(GroupTest, SortByPositionXDescending) {
        auto group = createGroup<Position, Velocity>();

        // Add more entities to ensure we have enough for a good test
        for (Entity i = 5; i < 10; ++i) {
            positionArray->addToGroup(i);
            velocityArray->addToGroup(i);
        }

        // Sort by position.x in descending order
        group->sortBy<Position, float>([](const Position& p) { return p.x; }, false);

        // Check that entities are sorted correctly
        auto entities = group->entities();

        // Entities should be in reverse order 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
        for (std::size_t i = 0; i < entities.size(); ++i) {
            EXPECT_EQ(entities[i], 9 - i);
        }
    }

    TEST_F(GroupTest, SortByNonOwnedComponent) {
        auto group = createGroup<Position>(get<Health>());

        // Add more entities to ensure we have enough for a good test
        for (Entity i = 5; i < 10; ++i) {
            positionArray->addToGroup(i);
        }

        // Sort by health value (non-owned) in ascending order (lower health first)
        group->sortBy<Health, int>([](const Health& h) { return h.value; });

        // Check that entities are sorted correctly
        auto entities = group->entities();

        // Health values are 100, 90, 80, 70, 60, 50, 40, 30, 20, 10
        // So entities should be in reverse order 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
        for (std::size_t i = 0; i < entities.size(); ++i) {
            EXPECT_EQ(entities[i], 9 - i);
        }
    }

    TEST_F(GroupTest, SortSaveWithNoInvalidation) {
        auto group = createGroup<Position, Velocity>();

        // Sort the group
        group->sortBy<Position, float>([](const Position& p) { return p.x; });

        // Get the current order
        auto initialEntities = group->entities();

        // Count function calls to check if sorting is skipped
        int callCount = 0;
        auto countingExtractor = [&callCount](const Position& p) {
            callCount++;
            return p.x;
        };

        // Call sort again - it should be skipped since sorting is not invalidated
        group->sortBy<Position, float>(countingExtractor);

        // No extraction function calls should happen since sorting was skipped
        EXPECT_EQ(callCount, 0);

        // Order should remain the same
        auto newEntities = group->entities();
        EXPECT_TRUE(std::equal(initialEntities.begin(), initialEntities.end(), newEntities.begin()));
    }

    // ========================================================================
    // Partitioning Tests
    // ========================================================================

    TEST_F(GroupTest, GetPartitionViewBasic) {
        auto group = createGroup<Position, Tag>();

        // Create a partition view based on tag name first character
        auto partitionView = group->getPartitionView<Tag, char>([](const Tag& tag) {
            return tag.name.empty() ? ' ' : tag.name[0];
        });

        // All entities have tags starting with 'E', so there should be one partition
        EXPECT_EQ(partitionView.partitionCount(), 1);

        // Get the keys (should be 'E')
        auto keys = partitionView.getPartitionKeys();
        EXPECT_EQ(keys.size(), 1);
        EXPECT_EQ(keys[0], 'E');

        // Get the partition for 'E'
        const auto* partition = partitionView.getPartition('E');
        ASSERT_NE(partition, nullptr);
        EXPECT_EQ(partition->key, 'E');
        EXPECT_EQ(partition->count, 5);
        EXPECT_EQ(partition->startIndex, 0);
    }

    TEST_F(GroupTest, MultiplePartitions) {
        auto group = createGroup<Position, Health>();

        // Modify health values to create multiple partitions
        // Group health values into three categories: Low (0-30), Medium (31-70), High (71-100)
        groupHealthData();

        // Create a partition view based on health category
        auto partitionView = group->getPartitionView<Health, std::string>([](const Health& health) {
            if (health.value <= 30) return "Low";
            else if (health.value <= 70) return "Medium";
            else return "High";
        });

        // There should be three partitions
        EXPECT_EQ(partitionView.partitionCount(), 3);

        // Get the keys
        auto keys = partitionView.getPartitionKeys();
        EXPECT_EQ(keys.size(), 3);
        EXPECT_NE(std::find(keys.begin(), keys.end(), "Low"), keys.end());
        EXPECT_NE(std::find(keys.begin(), keys.end(), "Medium"), keys.end());
        EXPECT_NE(std::find(keys.begin(), keys.end(), "High"), keys.end());

        // Check each partition
        const auto* lowPartition = partitionView.getPartition("Low");
        ASSERT_NE(lowPartition, nullptr);

        const auto* mediumPartition = partitionView.getPartition("Medium");
        ASSERT_NE(mediumPartition, nullptr);

        const auto* highPartition = partitionView.getPartition("High");
        ASSERT_NE(highPartition, nullptr);
    }

    TEST_F(GroupTest, PartitionEachMethod) {
        auto group = createGroup<Position, Health>();

        // Modify health values to create multiple partitions
        groupHealthData();

        // Create a partition view
        auto partitionView = group->getPartitionView<Health, std::string>([](const Health& health) {
            if (health.value <= 30) return "Low";
            else if (health.value <= 70) return "Medium";
            else return "High";
        });

        // Collect entities in the "Low" partition
        std::vector<Entity> lowEntities;
        partitionView.each("Low", [&](Entity entity, Position& position, Health& health) {
            lowEntities.push_back(entity);
            // Verify the health value is in the low range
            EXPECT_LE(health.value, 30);
        });

        // Collect entities in the "High" partition
        std::vector<Entity> highEntities;
        partitionView.each("High", [&](Entity entity, Position& position, Health& health) {
            highEntities.push_back(entity);
            // Verify the health value is in the high range
            EXPECT_GE(health.value, 71);
        });

        // Check that we collected the right number of entities
        const auto* lowPartition = partitionView.getPartition("Low");
        ASSERT_NE(lowPartition, nullptr);
        EXPECT_EQ(lowEntities.size(), lowPartition->count);

        const auto* highPartition = partitionView.getPartition("High");
        ASSERT_NE(highPartition, nullptr);
        EXPECT_EQ(highEntities.size(), highPartition->count);
    }

    TEST_F(GroupTest, EntityPartitionView) {
        auto group = createGroup<Position, Velocity>();

        // Create a partition view based directly on entity ID parity
        auto partitionView = group->getEntityPartitionView<std::string>(
            "parity",
            [](Entity e) { return e % 2 == 0 ? "Even" : "Odd"; }
        );

        // There should be two partitions: Even and Odd
        EXPECT_EQ(partitionView.partitionCount(), 2);

        // Get the keys
        auto keys = partitionView.getPartitionKeys();
        EXPECT_EQ(keys.size(), 2);
        EXPECT_NE(std::find(keys.begin(), keys.end(), "Even"), keys.end());
        EXPECT_NE(std::find(keys.begin(), keys.end(), "Odd"), keys.end());

        // Check each partition
        std::vector<Entity> evenEntities;
        partitionView.each("Even", [&](Entity entity, Position& position, Velocity& velocity) {
            evenEntities.push_back(entity);
            // Verify the entity ID is even
            EXPECT_EQ(entity % 2, 0);
        });

        std::vector<Entity> oddEntities;
        partitionView.each("Odd", [&](Entity entity, Position& position, Velocity& velocity) {
            oddEntities.push_back(entity);
            // Verify the entity ID is odd
            EXPECT_EQ(entity % 2, 1);
        });

        // In our initial setup, we have entities 0, 1, 2, 3, 4 in the group
        EXPECT_EQ(evenEntities.size(), 3); // 0, 2, 4
        EXPECT_EQ(oddEntities.size(), 2);  // 1, 3
    }

    TEST_F(GroupTest, PartitionInvalidation) {
        auto group = createGroup<Position, Velocity>();

        // Create a partition view
        auto partitionView = group->getEntityPartitionView<std::string>(
            "parity",
            [](Entity e) { return e % 2 == 0 ? "Even" : "Odd"; }
        );

        // Initial counts
        auto* evenPartition = partitionView.getPartition("Even");
        auto* oddPartition = partitionView.getPartition("Odd");
        ASSERT_NE(evenPartition, nullptr);
        ASSERT_NE(oddPartition, nullptr);

        size_t initialEvenCount = evenPartition->count;
        size_t initialOddCount = oddPartition->count;

        // Add a new entity to the group
        group->addToGroup(5); // Odd entity

        // Get the partition view again - should rebuild
        auto updatedPartitionView = group->getEntityPartitionView<std::string>(
            "parity",
            [](Entity e) { return e % 2 == 0 ? "Even" : "Odd"; }
        );

        // Check the new counts
        evenPartition = updatedPartitionView.getPartition("Even");
        oddPartition = updatedPartitionView.getPartition("Odd");
        ASSERT_NE(evenPartition, nullptr);
        ASSERT_NE(oddPartition, nullptr);

        // Even count should be the same, odd count should increase by 1
        EXPECT_EQ(evenPartition->count, initialEvenCount);
        EXPECT_EQ(oddPartition->count, initialOddCount + 1);
    }

    // ========================================================================
    // Edge Cases and Error Handling
    // ========================================================================

    TEST_F(GroupTest, EmptyGroup) {
        // Clear all entities from component arrays
        positionArray = std::make_shared<ComponentArray<Position>>();
        velocityArray = std::make_shared<ComponentArray<Velocity>>();

        // Create an empty group
        auto group = createGroup<Position, Velocity>();

        // Check size and entities
        EXPECT_EQ(group->size(), 0);
        EXPECT_EQ(group->entities().size(), 0);

        // Check iterator
        EXPECT_EQ(group->begin(), group->end());

        // Ensure each method doesn't error on empty group
        int callCount = 0;
        group->each([&](Entity, Position&, Velocity&) { callCount++; });
        EXPECT_EQ(callCount, 0);

        // Ensure get method returns empty span
        auto positions = group->get<Position>();
        EXPECT_EQ(positions.size(), 0);

        // Ensure sorting doesn't error
        group->sortBy<Position, float>([](const Position& p) { return p.x; });

        // Create partition view
        auto partitionView = group->getPartitionView<Position, float>([](const Position& p) { return p.x; });

        // Should have no partitions
        EXPECT_EQ(partitionView.partitionCount(), 0);
    }

    TEST_F(GroupTest, RemoveNonExistingEntity) {
        auto group = createGroup<Position, Velocity>();

        // Initially the group has entities 0-4
        EXPECT_EQ(group->size(), 5);

        // Try to remove an entity that's not in the group
        EXPECT_THROW(group->removeFromGroup(100), ComponentNotFound);

        // Size should remain the same
        EXPECT_EQ(group->size(), 5);
    }

    TEST_F(GroupTest, FindNonExistingPartition) {
        auto group = createGroup<Position, Tag>();

        // Create a partition view
        auto partitionView = group->getPartitionView<Tag, char>([](const Tag& tag) {
            return tag.name.empty() ? ' ' : tag.name[0];
        });

        // Try to get a partition that doesn't exist
        const auto* nonExistingPartition = partitionView.getPartition('Z');
        EXPECT_EQ(nonExistingPartition, nullptr);

        // Try to iterate over a non-existing partition
        int callCount = 0;
        partitionView.each('Z', [&](Entity, Position&, Tag&) { callCount++; });
        EXPECT_EQ(callCount, 0);
    }
};
