//// ActionHistory.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        02/12/2025
//  Description: Test file for ActionHistory class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "context/ActionHistory.hpp"
#include "../mocks/MockAction.hpp"

namespace nexo::editor {

    class ActionHistoryTest : public ::testing::Test {
    protected:
        ActionHistory history;

        std::unique_ptr<testing::CountingAction> createCountingAction() {
            return std::make_unique<testing::CountingAction>();
        }
    };

    // Stack Management Tests

    TEST_F(ActionHistoryTest, CanUndoReturnsFalseWhenEmpty) {
        EXPECT_FALSE(history.canUndo());
    }

    TEST_F(ActionHistoryTest, CanRedoReturnsFalseWhenEmpty) {
        EXPECT_FALSE(history.canRedo());
    }

    TEST_F(ActionHistoryTest, AddActionIncreasesUndoStackSize) {
        EXPECT_EQ(history.getUndoStackSize(), 0u);

        history.addAction(createCountingAction());
        EXPECT_EQ(history.getUndoStackSize(), 1u);

        history.addAction(createCountingAction());
        EXPECT_EQ(history.getUndoStackSize(), 2u);
    }

    TEST_F(ActionHistoryTest, CanUndoReturnsTrueWithActions) {
        history.addAction(createCountingAction());
        EXPECT_TRUE(history.canUndo());
    }

    TEST_F(ActionHistoryTest, CanRedoReturnsTrueAfterUndo) {
        history.addAction(createCountingAction());
        EXPECT_FALSE(history.canRedo());

        history.undo();
        EXPECT_TRUE(history.canRedo());
    }

    TEST_F(ActionHistoryTest, AddActionClearsRedoStack) {
        history.addAction(createCountingAction());
        history.undo();
        EXPECT_TRUE(history.canRedo());

        history.addAction(createCountingAction());
        EXPECT_FALSE(history.canRedo());
    }

    // Undo/Redo Operations Tests

    TEST_F(ActionHistoryTest, UndoOnEmptyStackDoesNothing) {
        EXPECT_NO_THROW(history.undo());
        EXPECT_FALSE(history.canRedo());
    }

    TEST_F(ActionHistoryTest, RedoOnEmptyStackDoesNothing) {
        EXPECT_NO_THROW(history.redo());
        EXPECT_FALSE(history.canUndo());
    }

    TEST_F(ActionHistoryTest, UndoMovesActionToRedoStack) {
        history.addAction(createCountingAction());
        EXPECT_EQ(history.getUndoStackSize(), 1u);
        EXPECT_FALSE(history.canRedo());

        history.undo();
        EXPECT_EQ(history.getUndoStackSize(), 0u);
        EXPECT_TRUE(history.canRedo());
    }

    TEST_F(ActionHistoryTest, RedoMovesActionToUndoStack) {
        history.addAction(createCountingAction());
        history.undo();
        EXPECT_EQ(history.getUndoStackSize(), 0u);

        history.redo();
        EXPECT_EQ(history.getUndoStackSize(), 1u);
        EXPECT_FALSE(history.canRedo());
    }

    TEST_F(ActionHistoryTest, UndoCallsActionUndo) {
        auto action = createCountingAction();
        auto* actionPtr = action.get();

        history.addAction(std::move(action));
        EXPECT_EQ(actionPtr->undoCount, 0);

        history.undo();
        EXPECT_EQ(actionPtr->undoCount, 1);
    }

    TEST_F(ActionHistoryTest, RedoCallsActionRedo) {
        auto action = createCountingAction();
        auto* actionPtr = action.get();

        history.addAction(std::move(action));
        history.undo();
        EXPECT_EQ(actionPtr->redoCount, 0);

        history.redo();
        EXPECT_EQ(actionPtr->redoCount, 1);
    }

    TEST_F(ActionHistoryTest, MultipleUndoRedoCycles) {
        auto action = createCountingAction();
        auto* actionPtr = action.get();

        history.addAction(std::move(action));

        history.undo();
        EXPECT_EQ(actionPtr->undoCount, 1);

        history.redo();
        EXPECT_EQ(actionPtr->redoCount, 1);

        history.undo();
        EXPECT_EQ(actionPtr->undoCount, 2);

        history.redo();
        EXPECT_EQ(actionPtr->redoCount, 2);
    }

    // History Limits Tests

    TEST_F(ActionHistoryTest, ExceedingMaxUndoLevelsRemovesOldest) {
        history.setMaxUndoLevels(3);

        for (int i = 0; i < 5; ++i) {
            history.addAction(createCountingAction());
        }

        EXPECT_EQ(history.getUndoStackSize(), 3u);
    }

    TEST_F(ActionHistoryTest, SetMaxUndoLevelsTrimsExistingStack) {
        for (int i = 0; i < 10; ++i) {
            history.addAction(createCountingAction());
        }
        EXPECT_EQ(history.getUndoStackSize(), 10u);

        history.setMaxUndoLevels(5);
        EXPECT_EQ(history.getUndoStackSize(), 5u);
    }

    TEST_F(ActionHistoryTest, ClearRemovesAllActions) {
        for (int i = 0; i < 5; ++i) {
            history.addAction(createCountingAction());
        }
        history.undo();
        history.undo();

        EXPECT_TRUE(history.canUndo());
        EXPECT_TRUE(history.canRedo());

        history.clear();

        EXPECT_FALSE(history.canUndo());
        EXPECT_FALSE(history.canRedo());
        EXPECT_EQ(history.getUndoStackSize(), 0u);
    }

    TEST_F(ActionHistoryTest, ClearWithCountRemovesNActions) {
        for (int i = 0; i < 5; ++i) {
            history.addAction(createCountingAction());
        }
        EXPECT_EQ(history.getUndoStackSize(), 5u);

        history.clear(2);
        EXPECT_EQ(history.getUndoStackSize(), 3u);
    }

    TEST_F(ActionHistoryTest, ClearWithCountExceedingStackSizeRemovesAll) {
        for (int i = 0; i < 3; ++i) {
            history.addAction(createCountingAction());
        }

        history.clear(10);
        EXPECT_EQ(history.getUndoStackSize(), 0u);
    }

    // State Consistency Tests

    TEST_F(ActionHistoryTest, AlternateUndoRedoMaintainsConsistency) {
        history.addAction(createCountingAction());
        history.addAction(createCountingAction());
        history.addAction(createCountingAction());

        history.undo();
        EXPECT_EQ(history.getUndoStackSize(), 2u);

        history.redo();
        EXPECT_EQ(history.getUndoStackSize(), 3u);

        history.undo();
        history.undo();
        EXPECT_EQ(history.getUndoStackSize(), 1u);

        history.redo();
        history.redo();
        EXPECT_EQ(history.getUndoStackSize(), 3u);
    }

}
