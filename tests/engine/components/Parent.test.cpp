///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date: Today lol
//  Description: Test file for the Parent and Root components
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "components/Parent.hpp"
#include "utils/TestHelpers.hpp"
#include "assets/AssetRef.hpp"
#include "assets/Assets/Model/Model.hpp"
#include <limits>

using namespace nexo::components;
using namespace nexo::test;
using namespace nexo::ecs;
using namespace nexo::assets;

class ParentComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        parentComp = ParentComponent{};
        rootComp = RootComponent{};
    }
    
    ParentComponent parentComp;
    RootComponent rootComp;
};

// ParentComponent Tests

TEST_F(ParentComponentTest, DefaultConstruction) {
    ParentComponent defaultParent;
    // Entity should be uninitialized (typically 0 or max value)
    // The actual default value depends on ecs::Entity implementation
    EXPECT_GE(defaultParent.parent, 0);
}

TEST_F(ParentComponentTest, ValueConstruction) {
    nexo::ecs::Entity parentEntity = 42;
    ParentComponent parent{parentEntity};
    EXPECT_EQ(parent.parent, 42);
}

TEST_F(ParentComponentTest, CopyConstruction) {
    parentComp.parent = 123;
    ParentComponent copy(parentComp);
    
    EXPECT_EQ(copy.parent, 123);
    EXPECT_EQ(copy.parent, parentComp.parent);
}

TEST_F(ParentComponentTest, MoveConstruction) {
    parentComp.parent = 456;
    ParentComponent moved(std::move(parentComp));
    
    EXPECT_EQ(moved.parent, 456);
}

TEST_F(ParentComponentTest, CopyAssignment) {
    ParentComponent first{100};
    ParentComponent second{200};
    
    second = first;
    EXPECT_EQ(second.parent, 100);
    EXPECT_EQ(first.parent, 100);
}

TEST_F(ParentComponentTest, MoveAssignment) {
    ParentComponent first{300};
    ParentComponent second{400};
    
    second = std::move(first);
    EXPECT_EQ(second.parent, 300);
}

TEST_F(ParentComponentTest, EntityValueRange) {
    // Test with various entity values
    std::vector<nexo::ecs::Entity> testValues = {
        0,
        1,
        100,
        1000,
        std::numeric_limits<nexo::ecs::Entity>::max() / 2,
        std::numeric_limits<nexo::ecs::Entity>::max() - 1,
        std::numeric_limits<nexo::ecs::Entity>::max()
    };
    
    for (auto value : testValues) {
        parentComp.parent = value;
        EXPECT_EQ(parentComp.parent, value);
    }
}

TEST_F(ParentComponentTest, MultipleParentComponents) {
    std::vector<ParentComponent> parents;
    
    // Create multiple parent components with different values
    for (nexo::ecs::Entity i = 0; i < 100; ++i) {
        parents.push_back(ParentComponent{i * 10});
    }
    
    // Verify all have correct values
    for (size_t i = 0; i < parents.size(); ++i) {
        EXPECT_EQ(parents[i].parent, i * 10);
    }
}

TEST_F(ParentComponentTest, AggregateInitialization) {
    ParentComponent parent1{};
    ParentComponent parent2{42};
    
    EXPECT_GE(parent1.parent, 0);
    EXPECT_EQ(parent2.parent, 42);
}

// RootComponent Tests
class RootComponentTest : public ::testing::Test {
protected:
    RootComponent rootComp;
    
    void SetUp() override {
        rootComp = RootComponent{};
    }
};

TEST_F(RootComponentTest, DefaultConstruction) {
    RootComponent defaultRoot;
    
    EXPECT_EQ(defaultRoot.name, "Root");
    EXPECT_FALSE(defaultRoot.modelRef.isValid());
    EXPECT_EQ(defaultRoot.childCount, 0);
}

TEST_F(RootComponentTest, NameModification) {
    rootComp.name = "CustomRoot";
    EXPECT_EQ(rootComp.name, "CustomRoot");
    
    rootComp.name = "";
    EXPECT_EQ(rootComp.name, "");
    
    rootComp.name = "Very Long Root Name That Should Still Work Fine";
    EXPECT_EQ(rootComp.name, "Very Long Root Name That Should Still Work Fine");
}

TEST_F(RootComponentTest, ChildCountManagement) {
    EXPECT_EQ(rootComp.childCount, 0);
    
    // Add children
    rootComp.childCount++;
    EXPECT_EQ(rootComp.childCount, 1);
    
    rootComp.childCount += 5;
    EXPECT_EQ(rootComp.childCount, 6);
    
    // Remove children
    rootComp.childCount--;
    EXPECT_EQ(rootComp.childCount, 5);
    
    // Edge case: negative (should be prevented in actual usage)
    rootComp.childCount = -1;
    EXPECT_EQ(rootComp.childCount, -1);
    
    // Large numbers
    rootComp.childCount = 10000;
    EXPECT_EQ(rootComp.childCount, 10000);
}

TEST_F(RootComponentTest, CopyConstruction) {
    rootComp.name = "TestRoot";
    rootComp.childCount = 3;
    // Note: modelRef copy behavior depends on AssetRef implementation
    
    RootComponent copy(rootComp);
    
    EXPECT_EQ(copy.name, "TestRoot");
    EXPECT_EQ(copy.childCount, 3);
    
    // Verify independence
    copy.name = "ModifiedRoot";
    copy.childCount = 5;
    
    EXPECT_EQ(rootComp.name, "TestRoot");
    EXPECT_EQ(rootComp.childCount, 3);
}

TEST_F(RootComponentTest, MoveConstruction) {
    rootComp.name = "MoveTestRoot";
    rootComp.childCount = 7;
    
    RootComponent moved(std::move(rootComp));
    
    EXPECT_EQ(moved.name, "MoveTestRoot");
    EXPECT_EQ(moved.childCount, 7);
}

TEST_F(RootComponentTest, CopyAssignment) {
    RootComponent first;
    first.name = "First";
    first.childCount = 2;
    
    RootComponent second;
    second.name = "Second";
    second.childCount = 4;
    
    second = first;
    
    EXPECT_EQ(second.name, "First");
    EXPECT_EQ(second.childCount, 2);
}

TEST_F(RootComponentTest, MoveAssignment) {
    RootComponent first;
    first.name = "First";
    first.childCount = 10;
    
    RootComponent second;
    second = std::move(first);
    
    EXPECT_EQ(second.name, "First");
    EXPECT_EQ(second.childCount, 10);
}

TEST_F(RootComponentTest, SpecialCharactersInName) {
    std::vector<std::string> specialNames = {
        "Root-01",
        "Root_02",
        "Root.03",
        "Root@04",
        "Root#05",
        "Root$06",
        "Root%07",
        "Root&08",
        "Root*09",
        "Root(10)",
        "Root[11]",
        "Root{12}",
        "Root<13>",
        "Root\\14",
        "Root/15",
        "Root|16",
        "Root'17",
        "Root\"18\"",
        "Root`19`",
        "Root~20",
        "Root!21",
        "Root?22",
        "Root:23",
        "Root;24",
        "Root,25",
        "Root 26",
        "Root\t27",
        "Root\n28"
    };
    
    for (const auto& specialName : specialNames) {
        rootComp.name = specialName;
        EXPECT_EQ(rootComp.name, specialName);
    }
}

TEST_F(RootComponentTest, UnicodeInName) {
    std::vector<std::string> unicodeNames = {
        "根", // Chinese
        "ルート", // Japanese
        "루트", // Korean  
        "Корень", // Russian
        "جذر", // Arabic
        "ρίζα", // Greek
        "שורש", // Hebrew
        "ราก", // Thai
        "🌳🌲🌴", // Emojis
        "Root™",
        "Root©",
        "Root®",
        "Root°",
        "Root±",
        "Root²",
        "Root½",
        "Root€",
        "Root£",
        "Root¥",
        "Root§"
    };
    
    for (const auto& unicodeName : unicodeNames) {
        rootComp.name = unicodeName;
        EXPECT_EQ(rootComp.name, unicodeName);
    }
}

TEST_F(RootComponentTest, MultipleRootComponents) {
    std::vector<RootComponent> roots;
    
    for (int i = 0; i < 50; ++i) {
        RootComponent root;
        root.name = "Root_" + std::to_string(i);
        root.childCount = i * 2;
        roots.push_back(root);
    }
    
    // Verify all roots maintain their values
    for (size_t i = 0; i < roots.size(); ++i) {
        EXPECT_EQ(roots[i].name, "Root_" + std::to_string(i));
        EXPECT_EQ(roots[i].childCount, i * 2);
    }
}

TEST_F(RootComponentTest, ExtremeChildCounts) {
    // Zero children
    rootComp.childCount = 0;
    EXPECT_EQ(rootComp.childCount, 0);
    
    // Maximum int
    rootComp.childCount = std::numeric_limits<int>::max();
    EXPECT_EQ(rootComp.childCount, std::numeric_limits<int>::max());
    
    // Minimum int (edge case, shouldn't happen in practice)
    rootComp.childCount = std::numeric_limits<int>::min();
    EXPECT_EQ(rootComp.childCount, std::numeric_limits<int>::min());
}

// Integration test with mock ECS
class ParentRootIntegrationTest : public EcsTestFixture {
protected:
    void SetUp() override {
        EcsTestFixture::SetUp();
    }
    void TearDown() override {
        EcsTestFixture::TearDown();
    }
};

TEST_F(ParentRootIntegrationTest, HierarchyCreation) {
    auto* coordinator = this->coordinator.get();
    
    // Register additional component
    coordinator->registerComponent<RootComponent>();
    
    // Create root entity
    auto root = coordinator->createEntity();
    RootComponent rootComp;
    rootComp.name = "SceneRoot";
    coordinator->addComponent(root, rootComp);
    
    // Create children
    std::vector<nexo::ecs::Entity> children;
    for (int i = 0; i < 5; ++i) {
        auto child = coordinator->createEntity();
        coordinator->addComponent(child, ParentComponent{root});
        children.push_back(child);
    }
    
    // Update root's child count
    auto& rootRef = coordinator->getComponent<RootComponent>(root);
    rootRef.childCount = 5;
    
    // Verify hierarchy
    EXPECT_EQ(rootRef.childCount, 5);
    for (auto child : children) {
        auto& parentComp = coordinator->getComponent<ParentComponent>(child);
        EXPECT_EQ(parentComp.parent, root);
    }
}

// Test circular reference detection (conceptual test)
TEST(ParentComponentTest, CircularReferenceScenario) {
    // This test documents how circular references should be detected
    // Actual detection would be implemented in the system using these components
    
    nexo::ecs::Entity entity1 = 1;
    nexo::ecs::Entity entity2 = 2;
    nexo::ecs::Entity entity3 = 3;
    
    ParentComponent parent1{entity2}; // 1's parent is 2
    ParentComponent parent2{entity3}; // 2's parent is 3
    ParentComponent parent3{entity1}; // 3's parent is 1 (circular!)
    
    // In a real system, we would traverse parents and detect the cycle
    std::set<nexo::ecs::Entity> visited;
    nexo::ecs::Entity current = entity1;
    
    // Simulated parent lookup
    auto getParent = [&](nexo::ecs::Entity e) -> nexo::ecs::Entity {
        if (e == entity1) return parent1.parent;
        if (e == entity2) return parent2.parent;
        if (e == entity3) return parent3.parent;
        return std::numeric_limits<nexo::ecs::Entity>::max();
    };
    
    // Detect cycle
    bool hasCycle = false;
    while (current != std::numeric_limits<nexo::ecs::Entity>::max()) {
        if (visited.count(current) > 0) {
            hasCycle = true;
            break;
        }
        visited.insert(current);
        current = getParent(current);
    }
    
    EXPECT_TRUE(hasCycle);
}

// Tests moved out of nexo::components namespace to fix gtest inheritance issues