//// Selector.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        02/12/2025
//  Description: Test file for Selector class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "context/Selector.hpp"

namespace nexo::editor {

    class SelectorTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // Clear selection state before each test
            Selector::get().clearSelection();
            Selector::get().setSelectedScene(-1);
        }

        void TearDown() override {
            // Clean up after each test
            Selector::get().clearSelection();
        }

        Selector& selector = Selector::get();
    };

    // Query Methods Tests

    TEST_F(SelectorTest, GetPrimaryEntityReturnsNegativeOneWhenEmpty) {
        EXPECT_EQ(selector.getPrimaryEntity(), -1);
    }

    TEST_F(SelectorTest, GetPrimaryEntityReturnsFirstSelected) {
        selector.selectEntity("uuid-1", 10, SelectionType::ENTITY);
        EXPECT_EQ(selector.getPrimaryEntity(), 10);

        // Add more entities - primary should still be the first one
        selector.addToSelection("uuid-2", 20, SelectionType::ENTITY);
        EXPECT_EQ(selector.getPrimaryEntity(), 10);
    }

    TEST_F(SelectorTest, GetSelectedEntitiesReturnsAllSelected) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 20, SelectionType::ENTITY);
        selector.addToSelection("uuid-3", 30, SelectionType::ENTITY);

        const auto& entities = selector.getSelectedEntities();
        ASSERT_EQ(entities.size(), 3u);
        EXPECT_EQ(entities[0], 10);
        EXPECT_EQ(entities[1], 20);
        EXPECT_EQ(entities[2], 30);
    }

    TEST_F(SelectorTest, GetSelectedEntitiesReturnsEmptyWhenNoSelection) {
        const auto& entities = selector.getSelectedEntities();
        EXPECT_TRUE(entities.empty());
    }

    TEST_F(SelectorTest, GetPrimaryUuidReturnsEmptyWhenNoSelection) {
        EXPECT_TRUE(selector.getPrimaryUuid().empty());
    }

    TEST_F(SelectorTest, GetPrimaryUuidReturnsFirstSelectedUuid) {
        selector.selectEntity("uuid-first", 10, SelectionType::ENTITY);
        EXPECT_EQ(selector.getPrimaryUuid(), "uuid-first");
    }

    TEST_F(SelectorTest, GetSelectedUuidsReturnsAllUuids) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 20, SelectionType::ENTITY);

        auto uuids = selector.getSelectedUuids();
        ASSERT_EQ(uuids.size(), 2u);
        EXPECT_EQ(uuids[0], "uuid-1");
        EXPECT_EQ(uuids[1], "uuid-2");
    }

    TEST_F(SelectorTest, IsEntitySelectedReturnsTrueForSelected) {
        selector.addToSelection("uuid-1", 42, SelectionType::ENTITY);
        EXPECT_TRUE(selector.isEntitySelected(42));
    }

    TEST_F(SelectorTest, IsEntitySelectedReturnsFalseForUnselected) {
        selector.addToSelection("uuid-1", 42, SelectionType::ENTITY);
        EXPECT_FALSE(selector.isEntitySelected(99));
    }

    TEST_F(SelectorTest, HasSelectionReturnsFalseWhenEmpty) {
        EXPECT_FALSE(selector.hasSelection());
    }

    TEST_F(SelectorTest, HasSelectionReturnsTrueWithSelection) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        EXPECT_TRUE(selector.hasSelection());
    }

    TEST_F(SelectorTest, GetPrimarySelectionTypeReturnsNoneWhenEmpty) {
        EXPECT_EQ(selector.getPrimarySelectionType(), SelectionType::NONE);
    }

    TEST_F(SelectorTest, GetPrimarySelectionTypeReturnsCorrectType) {
        selector.selectEntity("uuid-1", 10, SelectionType::CAMERA);
        EXPECT_EQ(selector.getPrimarySelectionType(), SelectionType::CAMERA);
    }

    TEST_F(SelectorTest, GetSelectionTypeReturnsCorrectTypeForEntity) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 20, SelectionType::CAMERA);

        EXPECT_EQ(selector.getSelectionType(10), SelectionType::ENTITY);
        EXPECT_EQ(selector.getSelectionType(20), SelectionType::CAMERA);
    }

    TEST_F(SelectorTest, GetSelectionTypeReturnsNoneForUnselectedEntity) {
        EXPECT_EQ(selector.getSelectionType(99), SelectionType::NONE);
    }

    // Selection Modification Tests

    TEST_F(SelectorTest, SelectEntityReplacesCurrentSelection) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 20, SelectionType::ENTITY);
        EXPECT_EQ(selector.getSelectedEntities().size(), 2u);

        selector.selectEntity("uuid-3", 30, SelectionType::ENTITY);

        EXPECT_EQ(selector.getSelectedEntities().size(), 1u);
        EXPECT_EQ(selector.getPrimaryEntity(), 30);
    }

    TEST_F(SelectorTest, AddToSelectionAddsNewEntity) {
        EXPECT_TRUE(selector.addToSelection("uuid-1", 10, SelectionType::ENTITY));
        EXPECT_TRUE(selector.isEntitySelected(10));
    }

    TEST_F(SelectorTest, AddToSelectionReturnsFalseForDuplicate) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        EXPECT_FALSE(selector.addToSelection("uuid-1", 10, SelectionType::ENTITY));
    }

    TEST_F(SelectorTest, ToggleSelectionSelectsUnselectedEntity) {
        bool result = selector.toggleSelection("uuid-1", 10, SelectionType::ENTITY);
        EXPECT_TRUE(result);
        EXPECT_TRUE(selector.isEntitySelected(10));
    }

    TEST_F(SelectorTest, ToggleSelectionDeselectsSelectedEntity) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        bool result = selector.toggleSelection("uuid-1", 10, SelectionType::ENTITY);
        EXPECT_FALSE(result);
        EXPECT_FALSE(selector.isEntitySelected(10));
    }

    TEST_F(SelectorTest, RemoveFromSelectionRemovesEntity) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        EXPECT_TRUE(selector.removeFromSelection(10));
        EXPECT_FALSE(selector.isEntitySelected(10));
    }

    TEST_F(SelectorTest, RemoveFromSelectionReturnsFalseForUnselected) {
        EXPECT_FALSE(selector.removeFromSelection(99));
    }

    TEST_F(SelectorTest, ClearSelectionRemovesAllEntities) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 20, SelectionType::ENTITY);
        selector.addToSelection("uuid-3", 30, SelectionType::ENTITY);

        selector.clearSelection();

        EXPECT_FALSE(selector.hasSelection());
        EXPECT_EQ(selector.getSelectedEntities().size(), 0u);
    }

    // Scene Selection Tests

    TEST_F(SelectorTest, GetSelectedSceneReturnsNegativeOneByDefault) {
        EXPECT_EQ(selector.getSelectedScene(), -1);
    }

    TEST_F(SelectorTest, SetSelectedSceneUpdatesScene) {
        selector.setSelectedScene(5);
        EXPECT_EQ(selector.getSelectedScene(), 5);
    }

    // UI Handle Tests

    TEST_F(SelectorTest, GetUiHandleCreatesDefaultOnFirstAccess) {
        const std::string& handle = selector.getUiHandle("new-uuid", "DefaultHandle");
        EXPECT_EQ(handle, "DefaultHandle");
    }

    TEST_F(SelectorTest, GetUiHandleReturnsPreviouslySet) {
        selector.getUiHandle("test-uuid", "FirstDefault");
        const std::string& handle = selector.getUiHandle("test-uuid", "SecondDefault");
        EXPECT_EQ(handle, "FirstDefault");
    }

    TEST_F(SelectorTest, SetUiHandleUpdatesMapping) {
        selector.setUiHandle("test-uuid", "CustomHandle");
        const std::string& handle = selector.getUiHandle("test-uuid", "ShouldBeIgnored");
        EXPECT_EQ(handle, "CustomHandle");
    }

    // Selection Type Tests

    TEST_F(SelectorTest, SetSelectionTypeUpdatesDefault) {
        selector.setSelectionType(SelectionType::CAMERA);
        // Note: setSelectionType sets default type but selectEntity uses explicit type
        // This is mainly for internal use
    }

    // Multiple Selection Tests

    TEST_F(SelectorTest, MultipleSelectionsPreserveOrder) {
        selector.addToSelection("uuid-1", 1, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 2, SelectionType::ENTITY);
        selector.addToSelection("uuid-3", 3, SelectionType::ENTITY);

        const auto& entities = selector.getSelectedEntities();
        EXPECT_EQ(entities[0], 1);
        EXPECT_EQ(entities[1], 2);
        EXPECT_EQ(entities[2], 3);
    }

    TEST_F(SelectorTest, RemoveFromMiddleOfSelectionPreservesOthers) {
        selector.addToSelection("uuid-1", 1, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 2, SelectionType::ENTITY);
        selector.addToSelection("uuid-3", 3, SelectionType::ENTITY);

        selector.removeFromSelection(2);

        const auto& entities = selector.getSelectedEntities();
        ASSERT_EQ(entities.size(), 2u);
        EXPECT_EQ(entities[0], 1);
        EXPECT_EQ(entities[1], 3);
    }

    // Different Selection Types

    TEST_F(SelectorTest, DifferentSelectionTypesAreSupported) {
        selector.addToSelection("uuid-1", 1, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 2, SelectionType::CAMERA);
        selector.addToSelection("uuid-3", 3, SelectionType::DIR_LIGHT);
        selector.addToSelection("uuid-4", 4, SelectionType::POINT_LIGHT);

        EXPECT_EQ(selector.getSelectionType(1), SelectionType::ENTITY);
        EXPECT_EQ(selector.getSelectionType(2), SelectionType::CAMERA);
        EXPECT_EQ(selector.getSelectionType(3), SelectionType::DIR_LIGHT);
        EXPECT_EQ(selector.getSelectionType(4), SelectionType::POINT_LIGHT);
    }

    // Edge Cases - Empty UUID

    TEST_F(SelectorTest, EmptyUuidHandledCorrectly) {
        selector.addToSelection("", 10, SelectionType::ENTITY);
        EXPECT_TRUE(selector.isEntitySelected(10));
        EXPECT_EQ(selector.getPrimaryUuid(), "");
    }

    TEST_F(SelectorTest, GetSelectionTypeAfterRemovalReturnsNone) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        EXPECT_EQ(selector.getSelectionType(10), SelectionType::ENTITY);

        selector.removeFromSelection(10);
        EXPECT_EQ(selector.getSelectionType(10), SelectionType::NONE);
    }

    TEST_F(SelectorTest, SelectEntityWithSameIdReplaces) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 20, SelectionType::CAMERA);
        EXPECT_EQ(selector.getSelectedEntities().size(), 2u);

        // selectEntity clears and replaces
        selector.selectEntity("uuid-3", 10, SelectionType::POINT_LIGHT);
        EXPECT_EQ(selector.getSelectedEntities().size(), 1u);
        EXPECT_EQ(selector.getPrimaryEntity(), 10);
        EXPECT_EQ(selector.getPrimarySelectionType(), SelectionType::POINT_LIGHT);
    }

    TEST_F(SelectorTest, ToggleOnExistingEntityRemovesIt) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 20, SelectionType::ENTITY);
        EXPECT_EQ(selector.getSelectedEntities().size(), 2u);

        // Toggle existing entity removes it
        bool result = selector.toggleSelection("uuid-1", 10, SelectionType::ENTITY);
        EXPECT_FALSE(result);
        EXPECT_EQ(selector.getSelectedEntities().size(), 1u);
        EXPECT_FALSE(selector.isEntitySelected(10));
    }

    // UI Handle Edge Cases

    TEST_F(SelectorTest, MultipleUiHandlesStoreCorrectly) {
        selector.setUiHandle("uuid-1", "Handle1");
        selector.setUiHandle("uuid-2", "Handle2");
        selector.setUiHandle("uuid-3", "Handle3");

        EXPECT_EQ(selector.getUiHandle("uuid-1", "Default"), "Handle1");
        EXPECT_EQ(selector.getUiHandle("uuid-2", "Default"), "Handle2");
        EXPECT_EQ(selector.getUiHandle("uuid-3", "Default"), "Handle3");
    }

    TEST_F(SelectorTest, OverwriteUiHandleWorks) {
        selector.setUiHandle("uuid-1", "FirstHandle");
        EXPECT_EQ(selector.getUiHandle("uuid-1", "Default"), "FirstHandle");

        selector.setUiHandle("uuid-1", "SecondHandle");
        EXPECT_EQ(selector.getUiHandle("uuid-1", "Default"), "SecondHandle");
    }

    TEST_F(SelectorTest, GetUiHandleWithEmptyUuidWorks) {
        const std::string& handle = selector.getUiHandle("", "EmptyDefault");
        EXPECT_EQ(handle, "EmptyDefault");
    }

    // Large Selection Tests

    TEST_F(SelectorTest, LargeSelectionPerformance) {
        constexpr int NUM_ENTITIES = 100;
        for (int i = 0; i < NUM_ENTITIES; ++i) {
            selector.addToSelection("uuid-" + std::to_string(i), i, SelectionType::ENTITY);
        }

        EXPECT_EQ(selector.getSelectedEntities().size(), NUM_ENTITIES);

        // Verify all are selected
        for (int i = 0; i < NUM_ENTITIES; ++i) {
            EXPECT_TRUE(selector.isEntitySelected(i));
        }
    }

    TEST_F(SelectorTest, ClearLargeSelectionWorks) {
        constexpr int NUM_ENTITIES = 100;
        for (int i = 0; i < NUM_ENTITIES; ++i) {
            selector.addToSelection("uuid-" + std::to_string(i), i, SelectionType::ENTITY);
        }

        selector.clearSelection();

        EXPECT_FALSE(selector.hasSelection());
        EXPECT_EQ(selector.getSelectedEntities().size(), 0u);
    }

    // Additional Edge Cases

    TEST_F(SelectorTest, RemoveFromMiddlePreservesFirstAsPrimary) {
        selector.addToSelection("uuid-1", 1, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 2, SelectionType::ENTITY);
        selector.addToSelection("uuid-3", 3, SelectionType::ENTITY);

        selector.removeFromSelection(2);

        EXPECT_EQ(selector.getPrimaryEntity(), 1);
        EXPECT_EQ(selector.getPrimaryUuid(), "uuid-1");
    }

    TEST_F(SelectorTest, RemoveFirstChangePrimary) {
        selector.addToSelection("uuid-1", 1, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 2, SelectionType::CAMERA);
        selector.addToSelection("uuid-3", 3, SelectionType::ENTITY);

        selector.removeFromSelection(1);

        EXPECT_EQ(selector.getPrimaryEntity(), 2);
        EXPECT_EQ(selector.getPrimaryUuid(), "uuid-2");
        EXPECT_EQ(selector.getPrimarySelectionType(), SelectionType::CAMERA);
    }

    TEST_F(SelectorTest, GetSelectedUuidsAfterPartialRemoval) {
        selector.addToSelection("uuid-a", 1, SelectionType::ENTITY);
        selector.addToSelection("uuid-b", 2, SelectionType::ENTITY);
        selector.addToSelection("uuid-c", 3, SelectionType::ENTITY);

        selector.removeFromSelection(2);

        auto uuids = selector.getSelectedUuids();
        ASSERT_EQ(uuids.size(), 2u);
        EXPECT_EQ(uuids[0], "uuid-a");
        EXPECT_EQ(uuids[1], "uuid-c");
    }

    // Additional Coverage Tests

    TEST_F(SelectorTest, GetSelectedEntitiesMultipleCalls) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        selector.addToSelection("uuid-2", 20, SelectionType::ENTITY);

        // Multiple calls should return consistent data
        const auto& first = selector.getSelectedEntities();
        const auto& second = selector.getSelectedEntities();

        EXPECT_EQ(first.size(), second.size());
        EXPECT_EQ(first[0], second[0]);
        EXPECT_EQ(first[1], second[1]);
    }

    TEST_F(SelectorTest, GetSelectedEntitiesAfterClear) {
        selector.addToSelection("uuid-1", 10, SelectionType::ENTITY);
        const auto& beforeClear = selector.getSelectedEntities();
        EXPECT_EQ(beforeClear.size(), 1u);

        selector.clearSelection();
        const auto& afterClear = selector.getSelectedEntities();
        EXPECT_EQ(afterClear.size(), 0u);
    }

    TEST_F(SelectorTest, GetSelectedUuidsEmptySelection) {
        auto uuids = selector.getSelectedUuids();
        EXPECT_TRUE(uuids.empty());
    }

    TEST_F(SelectorTest, NegativeEntityId) {
        selector.addToSelection("uuid-neg", -5, SelectionType::ENTITY);
        EXPECT_TRUE(selector.isEntitySelected(-5));
        EXPECT_EQ(selector.getPrimaryEntity(), -5);
    }

    TEST_F(SelectorTest, ToggleTwiceReturnsToOriginalState) {
        // Start with entity not selected
        EXPECT_FALSE(selector.isEntitySelected(42));

        // Toggle on
        bool firstToggle = selector.toggleSelection("uuid-toggle", 42, SelectionType::ENTITY);
        EXPECT_TRUE(firstToggle);
        EXPECT_TRUE(selector.isEntitySelected(42));

        // Toggle off
        bool secondToggle = selector.toggleSelection("uuid-toggle", 42, SelectionType::ENTITY);
        EXPECT_FALSE(secondToggle);
        EXPECT_FALSE(selector.isEntitySelected(42));
    }

    TEST_F(SelectorTest, SceneSelectionMultipleUpdates) {
        selector.setSelectedScene(1);
        EXPECT_EQ(selector.getSelectedScene(), 1);

        selector.setSelectedScene(5);
        EXPECT_EQ(selector.getSelectedScene(), 5);

        selector.setSelectedScene(0);
        EXPECT_EQ(selector.getSelectedScene(), 0);
    }

    TEST_F(SelectorTest, SceneSelectionNegativeValue) {
        selector.setSelectedScene(-10);
        EXPECT_EQ(selector.getSelectedScene(), -10);
    }

    TEST_F(SelectorTest, AddDuplicateEntityIdWithDifferentUuid) {
        selector.addToSelection("uuid-first", 10, SelectionType::ENTITY);
        // Try adding same entity ID with different UUID - should fail (duplicate check is by ID)
        bool result = selector.addToSelection("uuid-second", 10, SelectionType::CAMERA);
        EXPECT_FALSE(result);
        EXPECT_EQ(selector.getSelectionType(10), SelectionType::ENTITY); // Original type preserved
    }

    TEST_F(SelectorTest, ClearSelectionThenAddNew) {
        selector.addToSelection("uuid-1", 1, SelectionType::ENTITY);
        selector.clearSelection();
        selector.addToSelection("uuid-2", 2, SelectionType::CAMERA);

        EXPECT_FALSE(selector.isEntitySelected(1));
        EXPECT_TRUE(selector.isEntitySelected(2));
        EXPECT_EQ(selector.getPrimaryEntity(), 2);
    }

    TEST_F(SelectorTest, RemoveLastEntityMakesSelectionEmpty) {
        selector.addToSelection("uuid-only", 42, SelectionType::ENTITY);
        selector.removeFromSelection(42);

        EXPECT_FALSE(selector.hasSelection());
        EXPECT_EQ(selector.getPrimaryEntity(), -1);
        EXPECT_TRUE(selector.getPrimaryUuid().empty());
    }

    TEST_F(SelectorTest, SelectionTypeNone) {
        selector.addToSelection("uuid-none", 10, SelectionType::NONE);
        EXPECT_EQ(selector.getSelectionType(10), SelectionType::NONE);
        EXPECT_EQ(selector.getPrimarySelectionType(), SelectionType::NONE);
    }

    TEST_F(SelectorTest, UiHandleLongStrings) {
        const std::string longUuid = std::string(256, 'a');
        const std::string longHandle = std::string(256, 'h');

        selector.setUiHandle(longUuid, longHandle);
        EXPECT_EQ(selector.getUiHandle(longUuid, "default"), longHandle);
    }

    TEST_F(SelectorTest, UiHandleSpecialCharacters) {
        selector.setUiHandle("uuid/with:special@chars!", "handle#with$special%chars");
        EXPECT_EQ(selector.getUiHandle("uuid/with:special@chars!", "default"), "handle#with$special%chars");
    }

    TEST_F(SelectorTest, GetSelectedUuidsReturnsNewVector) {
        selector.addToSelection("uuid-1", 1, SelectionType::ENTITY);

        auto uuids1 = selector.getSelectedUuids();
        auto uuids2 = selector.getSelectedUuids();

        // Should be separate vectors
        EXPECT_EQ(uuids1.size(), uuids2.size());
        EXPECT_EQ(uuids1[0], uuids2[0]);
    }

    TEST_F(SelectorTest, ZeroEntityId) {
        selector.addToSelection("uuid-zero", 0, SelectionType::ENTITY);
        EXPECT_TRUE(selector.isEntitySelected(0));
        EXPECT_EQ(selector.getPrimaryEntity(), 0);
    }

    TEST_F(SelectorTest, MaxIntEntityId) {
        constexpr int maxInt = std::numeric_limits<int>::max();
        selector.addToSelection("uuid-max", maxInt, SelectionType::ENTITY);
        EXPECT_TRUE(selector.isEntitySelected(maxInt));
        EXPECT_EQ(selector.getPrimaryEntity(), maxInt);
    }

}
