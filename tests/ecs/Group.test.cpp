//// Group.test.cpp /////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/04/2025
//  Description: Test file for the group class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "Group.hpp"
#include "ComponentArray.hpp"
#include "ECSExceptions.hpp"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

namespace nexo::ecs {

	// Test component types
	struct PositionComponent {
	    float x, y, z;

	    PositionComponent(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	        : x(x), y(y), z(z) {}

	    bool operator==(const PositionComponent& other) const {
	        return x == other.x && y == other.y && z == other.z;
	    }
	};

	struct VelocityComponent {
	    float vx, vy, vz;

	    VelocityComponent(float vx = 0.0f, float vy = 0.0f, float vz = 0.0f)
	        : vx(vx), vy(vy), vz(vz) {}

	    bool operator==(const VelocityComponent& other) const {
	        return vx == other.vx && vy == other.vy && vz == other.vz;
	    }
	};

	struct TagComponent {
	    std::string tag;
	    int category;

	    TagComponent(const std::string& tag = "", int category = 0)
	        : tag(tag), category(category) {}

	    bool operator==(const TagComponent& other) const {
	        return tag == other.tag && category == other.category;
	    }
	};

	struct HealthComponent {
	    int health;
	    int maxHealth;

	    HealthComponent(int health = 100, int maxHealth = 100)
	        : health(health), maxHealth(maxHealth) {}

	    bool operator==(const HealthComponent& other) const {
	        return health == other.health && maxHealth == other.maxHealth;
	    }
	};

	// Helper functions to setup component arrays and test entities
	class GroupTest : public ::testing::Test {
	protected:
	    // Component arrays
	    std::shared_ptr<ComponentArray<PositionComponent>> positionArray;
	    std::shared_ptr<ComponentArray<VelocityComponent>> velocityArray;
	    std::shared_ptr<ComponentArray<TagComponent>> tagArray;
	    std::shared_ptr<ComponentArray<HealthComponent>> healthArray;

	    // Test entities
	    std::vector<Entity> entities;

	    void SetUp() override {
	        // Create component arrays
	        positionArray = std::make_shared<ComponentArray<PositionComponent>>();
	        velocityArray = std::make_shared<ComponentArray<VelocityComponent>>();
	        tagArray = std::make_shared<ComponentArray<TagComponent>>();
	        healthArray = std::make_shared<ComponentArray<HealthComponent>>();

	        // Initialize with test entities
	        for (Entity i = 0; i < 5; ++i) {
	            entities.push_back(i);

	            // Add components with test data
	            positionArray->insert(i, PositionComponent(i * 1.0f, i * 2.0f, i * 3.0f));
	            velocityArray->insert(i, VelocityComponent(i * 0.5f, i * 1.0f, i * 1.5f));
	            tagArray->insert(i, TagComponent("Entity_" + std::to_string(i), i % 3));
	            healthArray->insert(i, HealthComponent(100 - i * 10, 100));
	        }
	    }

	    // Helper to create a group with owned and non-owned components
	    template<typename... Owned>
	    auto createGroup(auto nonOwned) {
	        using OwnedTuple = std::tuple<std::shared_ptr<ComponentArray<Owned>>...>;

	        auto ownedArrays = std::make_tuple(std::static_pointer_cast<ComponentArray<Owned>>(getNthArray<Owned>())...);
	        auto nonOwnedArrays = nonOwned;

	        return std::make_shared<Group<OwnedTuple, decltype(nonOwnedArrays)>>(ownedArrays, nonOwnedArrays);
	    }

	    // Helper to get component array by type
	    template<typename T>
	    std::shared_ptr<IComponentArray> getNthArray() {
	        if constexpr (std::is_same_v<T, PositionComponent>) {
	            return positionArray;
	        } else if constexpr (std::is_same_v<T, VelocityComponent>) {
	            return velocityArray;
	        } else if constexpr (std::is_same_v<T, TagComponent>) {
	            return tagArray;
	        } else if constexpr (std::is_same_v<T, HealthComponent>) {
	            return healthArray;
	        } else {
	            static_assert(dependent_false<T>::value, "Unknown component type");
	            return nullptr;
	        }
	    }
	};

	TEST_F(GroupTest, ConstructorInitializesCorrectly) {
	    // Create a group with position as owned, velocity as non-owned
	    auto group = createGroup<PositionComponent>(std::make_tuple(velocityArray));

	    // Check signatures
	    auto ownedSignature = group->ownedSignature();
	    auto allSignature = group->allSignature();

	    // Owned signature should only have position bit set
	    EXPECT_TRUE(ownedSignature.test(getComponentTypeID<PositionComponent>()));
	    EXPECT_FALSE(ownedSignature.test(getComponentTypeID<VelocityComponent>()));

	    // All signature should have both position and velocity bits set
	    EXPECT_TRUE(allSignature.test(getComponentTypeID<PositionComponent>()));
	    EXPECT_TRUE(allSignature.test(getComponentTypeID<VelocityComponent>()));
	}

	TEST_F(GroupTest, AddToGroupAddsEntities) {
	    auto group = createGroup<PositionComponent, VelocityComponent>(std::make_tuple(tagArray));

	    // Add a few entities to the group
	    for (Entity i = 0; i < 3; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Check size
	    EXPECT_EQ(group->size(), 3);

	    // Check entities
	    auto groupEntities = group->entities();
	    EXPECT_EQ(groupEntities.size(), 3);

	    // Verify the entities are the ones we added
	    std::set<Entity> expectedEntities = {0, 1, 2};
	    std::set<Entity> actualEntities(groupEntities.begin(), groupEntities.end());
	    EXPECT_EQ(actualEntities, expectedEntities);
	}

	TEST_F(GroupTest, RemoveFromGroupRemovesEntities) {
	    auto group = createGroup<PositionComponent, VelocityComponent>(std::make_tuple(tagArray));

	    // Add all entities to the group
	    for (Entity i = 0; i < 5; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Check initial size
	    EXPECT_EQ(group->size(), 5);

	    // Remove some entities
	    group->removeFromGroup(entities[1]);
	    group->removeFromGroup(entities[3]);

	    // Check updated size
	    EXPECT_EQ(group->size(), 3);

	    // Check remaining entities
	    auto groupEntities = group->entities();
	    EXPECT_EQ(groupEntities.size(), 3);

	    // Verify the remaining entities
	    std::set<Entity> expectedEntities = {0, 2, 4};
	    std::set<Entity> actualEntities(groupEntities.begin(), groupEntities.end());
	    EXPECT_EQ(actualEntities, expectedEntities);
	}

	TEST_F(GroupTest, GetReturnsSpanOfComponents) {
	    auto group = createGroup<PositionComponent>(std::make_tuple(velocityArray));

	    // Add some entities to the group
	    for (Entity i = 0; i < 3; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Get owned components
	    auto positions = group->get<PositionComponent>();
	    // Compile-time check: positions should be a span type since it is owned.
		static_assert(std::is_same_v<decltype(positions), std::span<PositionComponent>>,
                  "positions should be a span of PositionComponent.");
	    EXPECT_EQ(positions.size(), 3);

	    // Get non-owned components
	    auto velocities = group->get<VelocityComponent>();
	    // Compile-time check: velocities should be a component array type since it is not owned.
		static_assert(std::is_same_v<decltype(velocities), std::shared_ptr<ComponentArray<VelocityComponent>>>,
                  "velocities should be a component array of VelocityComponent.");

	    // Check access to component data
	    for (size_t i = 0; i < 3; ++i) {
	        EXPECT_FLOAT_EQ(positions[i].x, i * 1.0f);
	        EXPECT_FLOAT_EQ(positions[i].y, i * 2.0f);
	        EXPECT_FLOAT_EQ(positions[i].z, i * 3.0f);
	    }
	}

	TEST_F(GroupTest, IteratorBasicFunctionality) {
	    auto group = createGroup<PositionComponent, VelocityComponent>(std::make_tuple(tagArray));

	    // Add entities to the group
	    for (Entity i = 0; i < 3; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Use iterators to access entities and components
	    size_t count = 0;
	    for (auto [entity, position, velocity] : *group) {
	        EXPECT_TRUE(entity < 3); // Should be one of our first 3 entities
	        EXPECT_FLOAT_EQ(position.x, entity * 1.0f);
	        EXPECT_FLOAT_EQ(velocity.vx, entity * 0.5f);
	        count++;
	    }

	    EXPECT_EQ(count, 3);
	}

	TEST_F(GroupTest, IteratorEmptyGroup) {
	    auto group = createGroup<PositionComponent>(std::make_tuple(velocityArray));

		size_t count = std::distance(group->begin(), group->end());

	    EXPECT_EQ(count, 0);
	}

	TEST_F(GroupTest, EachMethodCallsFunction) {
	    auto group = createGroup<PositionComponent, VelocityComponent>(std::make_tuple(tagArray));

	    // Add entities to the group
	    for (Entity i = 0; i < 3; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Use each method to process entities
	    int callCount = 0;
	    group->each([&callCount](Entity e, PositionComponent& pos, VelocityComponent& vel, TagComponent& tag) {
	        EXPECT_EQ(pos.x, e * 1.0f);
	        EXPECT_EQ(vel.vx, e * 0.5f);
	        EXPECT_EQ(tag.tag, "Entity_" + std::to_string(e));
	        callCount++;
	    });

	    EXPECT_EQ(callCount, 3);
	}

	TEST_F(GroupTest, EachInRangeMethodCallsFunction) {
	    auto group = createGroup<PositionComponent, VelocityComponent>(std::make_tuple(tagArray));

	    // Add entities to the group
	    for (Entity i = 0; i < 5; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Use eachInRange to process a subset of entities
	    int callCount = 0;
	    group->eachInRange(1, 2, [&callCount](Entity e, PositionComponent&, VelocityComponent&, TagComponent&) {
	        EXPECT_GE(e, 1);
	        EXPECT_LE(e, 3);
	        callCount++;
	    });

	    EXPECT_EQ(callCount, 2);
	}

	TEST_F(GroupTest, SortByOwnedComponent) {
	    auto group = createGroup<PositionComponent, HealthComponent>(std::make_tuple(tagArray));

	    // Add entities to the group in a specific order
	    group->addToGroup(entities[0]); // health = 100
	    group->addToGroup(entities[2]); // health = 80
	    group->addToGroup(entities[1]); // health = 90
	    group->addToGroup(entities[4]); // health = 60
	    group->addToGroup(entities[3]); // health = 70

	    // Sort by health (ascending)
	    group->sortBy<HealthComponent, int>([](const HealthComponent& h) { return h.health; });

	    // Check the new order
	    auto healthComponents = group->get<HealthComponent>();
	    EXPECT_EQ(healthComponents.size(), 5);

	    // Should be in ascending order of health: 60, 70, 80, 90, 100
	    EXPECT_EQ(healthComponents[0].health, 60);
	    EXPECT_EQ(healthComponents[1].health, 70);
	    EXPECT_EQ(healthComponents[2].health, 80);
	    EXPECT_EQ(healthComponents[3].health, 90);
	    EXPECT_EQ(healthComponents[4].health, 100);

	    // Sort by health (descending)
	    group->sortBy<HealthComponent, int>(
	        [](const HealthComponent& h) { return h.health; },
	        false // descending
	    );

	    // Check the new order
	    healthComponents = group->get<HealthComponent>();

	    // Should be in descending order of health: 100, 90, 80, 70, 60
	    EXPECT_EQ(healthComponents[0].health, 100);
	    EXPECT_EQ(healthComponents[1].health, 90);
	    EXPECT_EQ(healthComponents[2].health, 80);
	    EXPECT_EQ(healthComponents[3].health, 70);
	    EXPECT_EQ(healthComponents[4].health, 60);

	    // Verify sorting invalidated flag is managed correctly
	    EXPECT_FALSE(group->sortingInvalidated());

	    // Add a new entity to invalidate sorting
	    group->addToGroup(entities[0]); // This is already in the group, but will still mark as invalidated
	    EXPECT_TRUE(group->sortingInvalidated());
	}

	TEST_F(GroupTest, SortByNonOwnedComponent) {
	    auto group = createGroup<PositionComponent>(std::make_tuple(tagArray, healthArray));

	    // Add entities to the group
	    for (Entity i = 0; i < 5; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Sort by a non-owned component (health)
	    group->sortBy<HealthComponent, int>([](const HealthComponent& h) { return h.health; });

	    // Check that the sort was successful
	    auto groupEntities = group->entities();
	    EXPECT_EQ(groupEntities[0], 4); // Entity 4 has lowest health (60)
	    EXPECT_EQ(groupEntities[4], 0); // Entity 0 has highest health (100)
	}

	TEST_F(GroupTest, InvalidateSorting) {
	    auto group = createGroup<PositionComponent>(std::make_tuple(healthArray));

	    // Add entities to the group
	    for (Entity i = 0; i < 5; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Sort by health (ascending)
	    group->sortBy<HealthComponent, int>([](const HealthComponent& h) { return h.health; });

	    // Now sort is not invalidated
	    EXPECT_FALSE(group->sortingInvalidated());

	    // Modify health values directly (this won't invalidate sorting in the Group)
	    for (int i = 4; i >= 0; --i) {
	        healthArray->get(i).health = 100 + i * 10; // Reverse the order
	    }

		group->invalidateSorting();

	    // Sort
	    group->sortBy<HealthComponent, int>([](const HealthComponent& h) { return h.health; });

	    // The original order should still be there, not the new values
	    auto groupEntities = group->entities();
	    EXPECT_EQ(groupEntities[0], 0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Partition Tests
	//////////////////////////////////////////////////////////////////////////

	TEST_F(GroupTest, PartitionByComponentField) {
	    auto group = createGroup<PositionComponent, TagComponent>(std::make_tuple(healthArray));

	    // Add entities to the group
	    for (Entity i = 0; i < 5; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Create a partition by category (0, 1, 2)
	    auto partitionView = group->getPartitionView<TagComponent, int>(
	        [](const TagComponent& tag) { return tag.category; }
	    );

	    // Check the number of partitions
	    EXPECT_EQ(partitionView.partitionCount(), 3);

	    // Get partition keys
	    auto keys = partitionView.getPartitionKeys();
	    std::sort(keys.begin(), keys.end());
	    EXPECT_EQ(keys[0], 0);
	    EXPECT_EQ(keys[1], 1);
	    EXPECT_EQ(keys[2], 2);

	    // Check entities in each partition
	    int countCategory0 = 0;
	    partitionView.each(0, [&countCategory0](Entity, PositionComponent&, TagComponent& tag, HealthComponent&) {
	        EXPECT_EQ(tag.category, 0);
	        countCategory0++;
	    });
	    EXPECT_EQ(countCategory0, 2); // Entities 0 and 3 have category 0

	    int countCategory1 = 0;
	    partitionView.each(1, [&countCategory1](Entity, PositionComponent&, TagComponent& tag, HealthComponent&) {
	        EXPECT_EQ(tag.category, 1);
	        countCategory1++;
	    });
	    EXPECT_EQ(countCategory1, 2); // Entities 1 and 4 have category 1

	    int countCategory2 = 0;
	    partitionView.each(2, [&countCategory2](Entity, PositionComponent&, TagComponent& tag, HealthComponent&) {
	        EXPECT_EQ(tag.category, 2);
	        countCategory2++;
	    });
	    EXPECT_EQ(countCategory2, 1); // Only entity 2 has category 2
	}

	TEST_F(GroupTest, PartitionInvalidation) {
	    auto group = createGroup<PositionComponent, TagComponent>(std::make_tuple(healthArray));

	    // Add entities to the group
	    for (Entity i = 0; i < 5; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Create a partition
	    auto partitionView = group->getPartitionView<TagComponent, int>(
	        [](const TagComponent& tag) { return tag.category; }
	    );

	    // Initial check
	    EXPECT_EQ(partitionView.partitionCount(), 3);

	    // Modify tag category for entity 0 (from 0 to 3)
	    tagArray->get(0).category = 3;

		group->invalidatePartitions();

	    // Get the view again
	    auto newView = group->getPartitionView<TagComponent, int>(
	        [](const TagComponent& tag) { return tag.category; }
	    );

	    // Should now have 4 partitions (0,1,2,3)
	    EXPECT_EQ(newView.partitionCount(), 4);

	    // Check new partition
	    auto keys = newView.getPartitionKeys();
	    EXPECT_TRUE(std::find(keys.begin(), keys.end(), 3) != keys.end());
	}

	TEST_F(GroupTest, PartitionWithNonExistentKey) {
	    auto group = createGroup<PositionComponent, TagComponent>(std::make_tuple(healthArray));

	    // Add entities to the group
	    for (Entity i = 0; i < 5; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Create a partition
	    auto partitionView = group->getPartitionView<TagComponent, int>(
	        [](const TagComponent& tag) { return tag.category; }
	    );

	    // Try to get a partition with a non-existent key
	    const auto* partition = partitionView.getPartition(99);
	    EXPECT_EQ(partition, nullptr);

	    // Try to iterate through a non-existent partition
	    int callCount = 0;
	    partitionView.each(99, [&callCount](Entity, PositionComponent&, TagComponent&, HealthComponent&) {
	        callCount++;
	    });

	    // Should not call the function
	    EXPECT_EQ(callCount, 0);
	}

	TEST_F(GroupTest, EntityPartitionView) {
	    auto group = createGroup<PositionComponent, TagComponent>(std::make_tuple(healthArray));

	    // Add entities to the group
	    for (Entity i = 0; i < 5; ++i) {
	        group->addToGroup(entities[i]);
	    }

	    // Create a partition based directly on entity IDs
	    auto partitionView = group->getEntityPartitionView<int>(
	        "test_partition",
	        [](Entity e) { return e % 2; } // Partition by even/odd
	    );

	    // Should have 2 partitions (0 for even, 1 for odd)
	    EXPECT_EQ(partitionView.partitionCount(), 2);

	    // Check each partition
	    int evenCount = 0;
	    partitionView.each(0, [&evenCount](Entity e, PositionComponent&, TagComponent&, HealthComponent&) {
	        EXPECT_EQ(e % 2, 0); // Should be even
	        evenCount++;
	    });
	    EXPECT_EQ(evenCount, 3); // Entities 0, 2, 4

	    int oddCount = 0;
	    partitionView.each(1, [&oddCount](Entity e, PositionComponent&, TagComponent&, HealthComponent&) {
	        EXPECT_EQ(e % 2, 1); // Should be odd
	        oddCount++;
	    });
	    EXPECT_EQ(oddCount, 2); // Entities 1, 3
	}

	TEST_F(GroupTest, EmptyGroup) {
	    auto group = createGroup<PositionComponent>(std::make_tuple(velocityArray));

	    // Check size
	    EXPECT_EQ(group->size(), 0);

	    // Check entities
	    auto groupEntities = group->entities();
	    EXPECT_EQ(groupEntities.size(), 0);

	    // Try using each method
	    int callCount = 0;
	    group->each([&callCount](Entity, PositionComponent&, VelocityComponent&) {
	        callCount++;
	    });
	    EXPECT_EQ(callCount, 0);

	    // Try sorting - shouldn't crash
	    group->sortBy<PositionComponent, float>([](const PositionComponent& p) { return p.x; });

	    // Try partitioning
	    auto partitionView = group->getPartitionView<PositionComponent, int>(
	        [](const PositionComponent& p) { return static_cast<int>(p.x); }
	    );
	    EXPECT_EQ(partitionView.partitionCount(), 0);
	}

	TEST_F(GroupTest, RemoveNonExistentEntity) {
	    auto group = createGroup<PositionComponent>(std::make_tuple(velocityArray));

	    // Add some entities
	    group->addToGroup(entities[0]);
	    group->addToGroup(entities[1]);

	    // Try removing an entity that's not in the group
	    group->removeFromGroup(entities[3]);

	    // Size should remain unchanged
	    EXPECT_EQ(group->size(), 2);
	}

	TEST_F(GroupTest, AddEntityTwice) {
	    auto group = createGroup<PositionComponent>(std::make_tuple(velocityArray));

	    // Add an entity
	    group->addToGroup(entities[0]);
	    EXPECT_EQ(group->size(), 1);

	    // Add the same entity again
	    group->addToGroup(entities[0]);

	    // Size should remain the same
	    EXPECT_EQ(group->size(), 1);
	}

	TEST_F(GroupTest, ModifyComponentsViaSpan) {
	    auto group = createGroup<PositionComponent>(std::make_tuple(velocityArray));

	    // Add entities
	    group->addToGroup(entities[0]);
	    group->addToGroup(entities[1]);

	    // Get span of position components
	    auto positions = group->get<PositionComponent>();

	    // Modify positions through the span
	    positions[0].x = 100.0f;
	    positions[1].x = 200.0f;

	    // Verify changes were applied to the original component array
	    EXPECT_FLOAT_EQ(positionArray->get(entities[0]).x, 100.0f);
	    EXPECT_FLOAT_EQ(positionArray->get(entities[1]).x, 200.0f);
	}

	TEST_F(GroupTest, GroupIteratorOutOfBounds) {
		auto group = createGroup<PositionComponent>(std::make_tuple(velocityArray));
		group->addToGroup(entities[0]);

		auto it = group->begin();
		ASSERT_NO_THROW(*it); // First element is valid

		it = group->end();
		ASSERT_THROW(*it, OutOfRange); // Dereferencing end iterator should throw
	}
}
