//// ActionManager.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        02/12/2025
//  Description: Test file for ActionManager class (delegate methods only)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "context/ActionManager.hpp"
#include "../mocks/MockAction.hpp"

namespace nexo::editor {

    class ActionManagerTest : public ::testing::Test {
    protected:
        void SetUp() override {
            // Clear any existing state from previous tests
            ActionManager::get().clearHistory();
        }

        void TearDown() override {
            ActionManager::get().clearHistory();
        }

        std::unique_ptr<testing::CountingAction> createCountingAction() {
            return std::make_unique<testing::CountingAction>();
        }

        ActionManager& manager = ActionManager::get();
    };

    // Singleton Tests

    TEST_F(ActionManagerTest, SingletonInstanceIsSame) {
        ActionManager& instance1 = ActionManager::get();
        ActionManager& instance2 = ActionManager::get();
        EXPECT_EQ(&instance1, &instance2);
    }

    // Initial State Tests

    TEST_F(ActionManagerTest, CanUndoReturnsFalseInitially) {
        EXPECT_FALSE(manager.canUndo());
    }

    TEST_F(ActionManagerTest, CanRedoReturnsFalseInitially) {
        EXPECT_FALSE(manager.canRedo());
    }

    TEST_F(ActionManagerTest, GetUndoStackSizeReturnsZeroInitially) {
        EXPECT_EQ(manager.getUndoStackSize(), 0u);
    }

    // Record Action Tests

    TEST_F(ActionManagerTest, RecordActionIncreasesStackSize) {
        manager.recordAction(createCountingAction());
        EXPECT_EQ(manager.getUndoStackSize(), 1u);
    }

    TEST_F(ActionManagerTest, RecordActionEnablesUndo) {
        manager.recordAction(createCountingAction());
        EXPECT_TRUE(manager.canUndo());
    }

    TEST_F(ActionManagerTest, RecordMultipleActionsIncreasesStackSize) {
        manager.recordAction(createCountingAction());
        manager.recordAction(createCountingAction());
        manager.recordAction(createCountingAction());
        EXPECT_EQ(manager.getUndoStackSize(), 3u);
    }

    // Undo Tests

    TEST_F(ActionManagerTest, UndoDecreasesStackSize) {
        manager.recordAction(createCountingAction());
        manager.recordAction(createCountingAction());
        EXPECT_EQ(manager.getUndoStackSize(), 2u);

        manager.undo();
        EXPECT_EQ(manager.getUndoStackSize(), 1u);
    }

    TEST_F(ActionManagerTest, UndoEnablesRedo) {
        manager.recordAction(createCountingAction());
        EXPECT_FALSE(manager.canRedo());

        manager.undo();
        EXPECT_TRUE(manager.canRedo());
    }

    TEST_F(ActionManagerTest, UndoCallsActionUndo) {
        auto action = createCountingAction();
        auto* actionPtr = action.get();

        manager.recordAction(std::move(action));
        manager.undo();

        EXPECT_EQ(actionPtr->undoCount, 1);
    }

    TEST_F(ActionManagerTest, UndoOnEmptyDoesNothing) {
        EXPECT_NO_THROW(manager.undo());
        EXPECT_FALSE(manager.canRedo());
    }

    // Redo Tests

    TEST_F(ActionManagerTest, RedoIncreasesStackSize) {
        manager.recordAction(createCountingAction());
        manager.undo();
        EXPECT_EQ(manager.getUndoStackSize(), 0u);

        manager.redo();
        EXPECT_EQ(manager.getUndoStackSize(), 1u);
    }

    TEST_F(ActionManagerTest, RedoDisablesRedo) {
        manager.recordAction(createCountingAction());
        manager.undo();
        EXPECT_TRUE(manager.canRedo());

        manager.redo();
        EXPECT_FALSE(manager.canRedo());
    }

    TEST_F(ActionManagerTest, RedoCallsActionRedo) {
        auto action = createCountingAction();
        auto* actionPtr = action.get();

        manager.recordAction(std::move(action));
        manager.undo();
        manager.redo();

        EXPECT_EQ(actionPtr->redoCount, 1);
    }

    TEST_F(ActionManagerTest, RedoOnEmptyDoesNothing) {
        EXPECT_NO_THROW(manager.redo());
        EXPECT_FALSE(manager.canUndo());
    }

    // Clear History Tests

    TEST_F(ActionManagerTest, ClearHistoryRemovesAllActions) {
        manager.recordAction(createCountingAction());
        manager.recordAction(createCountingAction());
        manager.recordAction(createCountingAction());
        manager.undo();

        manager.clearHistory();

        EXPECT_FALSE(manager.canUndo());
        EXPECT_FALSE(manager.canRedo());
        EXPECT_EQ(manager.getUndoStackSize(), 0u);
    }

    TEST_F(ActionManagerTest, ClearHistoryWithCountRemovesNActions) {
        for (int i = 0; i < 5; ++i) {
            manager.recordAction(createCountingAction());
        }
        EXPECT_EQ(manager.getUndoStackSize(), 5u);

        manager.clearHistory(2);
        EXPECT_EQ(manager.getUndoStackSize(), 3u);
    }

    // CreateActionGroup Tests

    TEST_F(ActionManagerTest, CreateActionGroupReturnsValidGroup) {
        auto group = ActionManager::createActionGroup();
        EXPECT_NE(group, nullptr);
    }

    TEST_F(ActionManagerTest, CreateActionGroupReturnsEmptyGroup) {
        auto group = ActionManager::createActionGroup();
        EXPECT_FALSE(group->hasActions());
    }

    TEST_F(ActionManagerTest, ActionGroupCanBeRecorded) {
        auto group = ActionManager::createActionGroup();
        group->addAction(createCountingAction());
        group->addAction(createCountingAction());

        manager.recordAction(std::move(group));
        EXPECT_EQ(manager.getUndoStackSize(), 1u);
    }

    // Undo/Redo Integration Tests

    TEST_F(ActionManagerTest, MultipleUndoRedoCycles) {
        auto action = createCountingAction();
        auto* actionPtr = action.get();

        manager.recordAction(std::move(action));

        manager.undo();
        EXPECT_EQ(actionPtr->undoCount, 1);

        manager.redo();
        EXPECT_EQ(actionPtr->redoCount, 1);

        manager.undo();
        EXPECT_EQ(actionPtr->undoCount, 2);

        manager.redo();
        EXPECT_EQ(actionPtr->redoCount, 2);
    }

    TEST_F(ActionManagerTest, RecordActionClearsRedoStack) {
        manager.recordAction(createCountingAction());
        manager.undo();
        EXPECT_TRUE(manager.canRedo());

        manager.recordAction(createCountingAction());
        EXPECT_FALSE(manager.canRedo());
    }

}
