//// ActionManagerComprehensive.test.cpp /////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        13/12/2025
//  Description: Comprehensive unit tests for ActionManager and ActionHistory
//               covering edge cases, stress tests, and complex scenarios
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "context/ActionManager.hpp"
#include "context/ActionHistory.hpp"
#include "../mocks/MockAction.hpp"
#include <memory>
#include <vector>

namespace nexo::editor {

    // ============================================================================
    // COMPREHENSIVE ACTIONHISTORY TESTS
    // ============================================================================

    class ActionHistoryComprehensiveTest : public ::testing::Test {
    protected:
        ActionHistory history;

        std::unique_ptr<testing::CountingAction> createCountingAction() {
            return std::make_unique<testing::CountingAction>();
        }

        // Helper to create a stateful action
        class StatefulAction : public Action {
        public:
            explicit StatefulAction(int& state, int setValue, int undoValue)
                : state_ref(state), set_value(setValue), undo_value(undoValue) {}

            void redo() override { state_ref = set_value; }
            void undo() override { state_ref = undo_value; }

        private:
            int& state_ref;
            int set_value;
            int undo_value;
        };

        std::unique_ptr<StatefulAction> createStatefulAction(int& state, int setValue, int undoValue) {
            return std::make_unique<StatefulAction>(state, setValue, undoValue);
        }
    };

    // ============================================================================
    // Edge Cases: Empty State Operations
    // ============================================================================

    TEST_F(ActionHistoryComprehensiveTest, MultipleUndoOnEmptyDoesNotCrash) {
        for (int i = 0; i < 10; ++i) {
            EXPECT_NO_THROW(history.undo());
        }
        EXPECT_FALSE(history.canUndo());
        EXPECT_FALSE(history.canRedo());
    }

    TEST_F(ActionHistoryComprehensiveTest, MultipleRedoOnEmptyDoesNotCrash) {
        for (int i = 0; i < 10; ++i) {
            EXPECT_NO_THROW(history.redo());
        }
        EXPECT_FALSE(history.canUndo());
        EXPECT_FALSE(history.canRedo());
    }

    TEST_F(ActionHistoryComprehensiveTest, AlternatingUndoRedoOnEmptyDoesNotCrash) {
        for (int i = 0; i < 5; ++i) {
            EXPECT_NO_THROW(history.undo());
            EXPECT_NO_THROW(history.redo());
        }
        EXPECT_FALSE(history.canUndo());
        EXPECT_FALSE(history.canRedo());
    }

    // ============================================================================
    // State Preservation Tests
    // ============================================================================

    TEST_F(ActionHistoryComprehensiveTest, UndoRedoPreservesState) {
        int state = 0;
        history.addAction(createStatefulAction(state, 10, 0));
        history.addAction(createStatefulAction(state, 20, 10));
        history.addAction(createStatefulAction(state, 30, 20));

        EXPECT_EQ(state, 0);

        history.undo();
        EXPECT_EQ(state, 20);

        history.undo();
        EXPECT_EQ(state, 10);

        history.redo();
        EXPECT_EQ(state, 20);

        history.redo();
        EXPECT_EQ(state, 30);
    }

    TEST_F(ActionHistoryComprehensiveTest, CompleteUndoRedoCycleRestoresState) {
        int state = 0;
        for (int i = 1; i <= 5; ++i) {
            history.addAction(createStatefulAction(state, i * 10, (i - 1) * 10));
        }

        // Undo all
        for (int i = 0; i < 5; ++i) {
            history.undo();
        }
        EXPECT_EQ(state, 0);
        EXPECT_FALSE(history.canUndo());
        EXPECT_TRUE(history.canRedo());

        // Redo all
        for (int i = 0; i < 5; ++i) {
            history.redo();
        }
        EXPECT_EQ(state, 50);
        EXPECT_TRUE(history.canUndo());
        EXPECT_FALSE(history.canRedo());
    }

    // ============================================================================
    // History Limit Enforcement Tests
    // ============================================================================

    TEST_F(ActionHistoryComprehensiveTest, MaxUndoLevelsEnforcedOnAdd) {
        history.setMaxUndoLevels(5);

        for (int i = 0; i < 10; ++i) {
            history.addAction(createCountingAction());
        }

        EXPECT_EQ(history.getUndoStackSize(), 5u);
        EXPECT_TRUE(history.canUndo());
    }

    TEST_F(ActionHistoryComprehensiveTest, MaxUndoLevelsZeroDisablesHistory) {
        history.setMaxUndoLevels(0);

        history.addAction(createCountingAction());
        EXPECT_EQ(history.getUndoStackSize(), 0u);
        EXPECT_FALSE(history.canUndo());
    }

    TEST_F(ActionHistoryComprehensiveTest, MaxUndoLevelsOneKeepsOnlyLatest) {
        history.setMaxUndoLevels(1);

        auto action1 = createCountingAction();
        auto* ptr1 = action1.get();
        history.addAction(std::move(action1));

        auto action2 = createCountingAction();
        auto* ptr2 = action2.get();
        history.addAction(std::move(action2));

        EXPECT_EQ(history.getUndoStackSize(), 1u);

        history.undo();
        EXPECT_EQ(ptr2->undoCount, 1);
        // ptr1 should be destroyed (can't verify as it's destroyed)
    }

    TEST_F(ActionHistoryComprehensiveTest, IncreasingMaxUndoLevelsKeepsExisting) {
        history.setMaxUndoLevels(3);

        for (int i = 0; i < 3; ++i) {
            history.addAction(createCountingAction());
        }
        EXPECT_EQ(history.getUndoStackSize(), 3u);

        history.setMaxUndoLevels(10);
        EXPECT_EQ(history.getUndoStackSize(), 3u);

        for (int i = 0; i < 5; ++i) {
            history.addAction(createCountingAction());
        }
        EXPECT_EQ(history.getUndoStackSize(), 8u);
    }

    // ============================================================================
    // Clear History Tests
    // ============================================================================

    TEST_F(ActionHistoryComprehensiveTest, ClearZeroRemovesAll) {
        for (int i = 0; i < 5; ++i) {
            history.addAction(createCountingAction());
        }
        history.undo();
        history.undo();

        history.clear(0);

        EXPECT_FALSE(history.canUndo());
        EXPECT_FALSE(history.canRedo());
        EXPECT_EQ(history.getUndoStackSize(), 0u);
    }

    TEST_F(ActionHistoryComprehensiveTest, ClearOneRemovesLatest) {
        for (int i = 0; i < 5; ++i) {
            history.addAction(createCountingAction());
        }

        history.clear(1);
        EXPECT_EQ(history.getUndoStackSize(), 4u);
    }

    TEST_F(ActionHistoryComprehensiveTest, ClearOnlyAffectsUndoStack) {
        for (int i = 0; i < 5; ++i) {
            history.addAction(createCountingAction());
        }
        history.undo();
        history.undo();
        EXPECT_TRUE(history.canRedo());

        history.clear(1);

        // Redo stack should remain intact
        EXPECT_TRUE(history.canRedo());
        EXPECT_EQ(history.getUndoStackSize(), 2u);
    }

    TEST_F(ActionHistoryComprehensiveTest, ClearExactStackSizeRemovesAll) {
        for (int i = 0; i < 5; ++i) {
            history.addAction(createCountingAction());
        }

        history.clear(5);
        EXPECT_EQ(history.getUndoStackSize(), 0u);
        EXPECT_FALSE(history.canUndo());
    }

    // ============================================================================
    // Sequential Operations Tests
    // ============================================================================

    TEST_F(ActionHistoryComprehensiveTest, LongSequenceOfUndoRedoOperations) {
        for (int i = 0; i < 20; ++i) {
            history.addAction(createCountingAction());
        }

        // Undo half
        for (int i = 0; i < 10; ++i) {
            history.undo();
        }
        EXPECT_EQ(history.getUndoStackSize(), 10u);

        // Redo some
        for (int i = 0; i < 5; ++i) {
            history.redo();
        }
        EXPECT_EQ(history.getUndoStackSize(), 15u);

        // Undo some more
        for (int i = 0; i < 7; ++i) {
            history.undo();
        }
        EXPECT_EQ(history.getUndoStackSize(), 8u);

        // Redo all available
        for (int i = 0; i < 12; ++i) {
            history.redo();
        }
        EXPECT_EQ(history.getUndoStackSize(), 20u);
        EXPECT_FALSE(history.canRedo());
    }

    TEST_F(ActionHistoryComprehensiveTest, AddAfterPartialUndoClearsRedoStack) {
        for (int i = 0; i < 5; ++i) {
            history.addAction(createCountingAction());
        }

        history.undo();
        history.undo();
        EXPECT_TRUE(history.canRedo());

        history.addAction(createCountingAction());
        EXPECT_FALSE(history.canRedo());
        EXPECT_EQ(history.getUndoStackSize(), 4u);
    }

    TEST_F(ActionHistoryComprehensiveTest, BranchingHistoryScenario) {
        int state = 0;

        // Initial sequence
        history.addAction(createStatefulAction(state, 10, 0));
        history.addAction(createStatefulAction(state, 20, 10));
        history.addAction(createStatefulAction(state, 30, 20));

        // Undo to branch point
        history.undo();
        history.undo();
        EXPECT_EQ(state, 10);

        // Create new branch
        history.addAction(createStatefulAction(state, 100, 10));
        EXPECT_EQ(state, 10);

        // Old redo should be cleared
        EXPECT_FALSE(history.canRedo());

        // Undo new branch
        history.undo();
        EXPECT_EQ(state, 10);

        // Redo new branch
        history.redo();
        EXPECT_EQ(state, 100);
    }

    // ============================================================================
    // Action Execution Order Tests
    // ============================================================================

    TEST_F(ActionHistoryComprehensiveTest, MultipleActionsUndoInReverseOrder) {
        std::vector<int> executionOrder;

        class OrderTrackingAction : public Action {
        public:
            OrderTrackingAction(int id, std::vector<int>& order)
                : id_(id), order_(order) {}

            void redo() override { order_.push_back(id_); }
            void undo() override { order_.push_back(-id_); }

        private:
            int id_;
            std::vector<int>& order_;
        };

        history.addAction(std::make_unique<OrderTrackingAction>(1, executionOrder));
        history.addAction(std::make_unique<OrderTrackingAction>(2, executionOrder));
        history.addAction(std::make_unique<OrderTrackingAction>(3, executionOrder));

        executionOrder.clear();

        history.undo();
        history.undo();
        history.undo();

        ASSERT_EQ(executionOrder.size(), 3u);
        EXPECT_EQ(executionOrder[0], -3);
        EXPECT_EQ(executionOrder[1], -2);
        EXPECT_EQ(executionOrder[2], -1);
    }

    TEST_F(ActionHistoryComprehensiveTest, MultipleActionsRedoInForwardOrder) {
        std::vector<int> executionOrder;

        class OrderTrackingAction : public Action {
        public:
            OrderTrackingAction(int id, std::vector<int>& order)
                : id_(id), order_(order) {}

            void redo() override { order_.push_back(id_); }
            void undo() override { order_.push_back(-id_); }

        private:
            int id_;
            std::vector<int>& order_;
        };

        history.addAction(std::make_unique<OrderTrackingAction>(1, executionOrder));
        history.addAction(std::make_unique<OrderTrackingAction>(2, executionOrder));
        history.addAction(std::make_unique<OrderTrackingAction>(3, executionOrder));

        executionOrder.clear();

        history.undo();
        history.undo();
        history.undo();

        executionOrder.clear();

        history.redo();
        history.redo();
        history.redo();

        ASSERT_EQ(executionOrder.size(), 3u);
        EXPECT_EQ(executionOrder[0], 1);
        EXPECT_EQ(executionOrder[1], 2);
        EXPECT_EQ(executionOrder[2], 3);
    }

    // ============================================================================
    // COMPREHENSIVE ACTIONMANAGER TESTS
    // ============================================================================

    class ActionManagerComprehensiveTest : public ::testing::Test {
    protected:
        void SetUp() override {
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

    // ============================================================================
    // Singleton Thread Safety Tests (basic check)
    // ============================================================================

    TEST_F(ActionManagerComprehensiveTest, SingletonReturnsSameInstance) {
        ActionManager& instance1 = ActionManager::get();
        ActionManager& instance2 = ActionManager::get();
        ActionManager& instance3 = ActionManager::get();

        EXPECT_EQ(&instance1, &instance2);
        EXPECT_EQ(&instance2, &instance3);
    }

    // ============================================================================
    // ActionGroup Integration Tests
    // ============================================================================

    TEST_F(ActionManagerComprehensiveTest, EmptyActionGroupCanBeRecorded) {
        auto group = ActionManager::createActionGroup();
        EXPECT_FALSE(group->hasActions());

        manager.recordAction(std::move(group));
        EXPECT_EQ(manager.getUndoStackSize(), 1u);
    }

    TEST_F(ActionManagerComprehensiveTest, ActionGroupUndoRedoExecutesAllActions) {
        auto group = ActionManager::createActionGroup();

        auto action1 = createCountingAction();
        auto action2 = createCountingAction();
        auto action3 = createCountingAction();

        auto* ptr1 = action1.get();
        auto* ptr2 = action2.get();
        auto* ptr3 = action3.get();

        group->addAction(std::move(action1));
        group->addAction(std::move(action2));
        group->addAction(std::move(action3));

        manager.recordAction(std::move(group));

        manager.undo();
        EXPECT_EQ(ptr1->undoCount, 1);
        EXPECT_EQ(ptr2->undoCount, 1);
        EXPECT_EQ(ptr3->undoCount, 1);

        manager.redo();
        EXPECT_EQ(ptr1->redoCount, 1);
        EXPECT_EQ(ptr2->redoCount, 1);
        EXPECT_EQ(ptr3->redoCount, 1);
    }

    TEST_F(ActionManagerComprehensiveTest, NestedActionGroups) {
        auto outerGroup = ActionManager::createActionGroup();
        auto innerGroup1 = ActionManager::createActionGroup();
        auto innerGroup2 = ActionManager::createActionGroup();

        auto action1 = createCountingAction();
        auto action2 = createCountingAction();
        auto action3 = createCountingAction();

        auto* ptr1 = action1.get();
        auto* ptr2 = action2.get();
        auto* ptr3 = action3.get();

        innerGroup1->addAction(std::move(action1));
        innerGroup1->addAction(std::move(action2));
        innerGroup2->addAction(std::move(action3));

        outerGroup->addAction(std::move(innerGroup1));
        outerGroup->addAction(std::move(innerGroup2));

        manager.recordAction(std::move(outerGroup));
        EXPECT_EQ(manager.getUndoStackSize(), 1u);

        manager.undo();
        EXPECT_EQ(ptr1->undoCount, 1);
        EXPECT_EQ(ptr2->undoCount, 1);
        EXPECT_EQ(ptr3->undoCount, 1);

        manager.redo();
        EXPECT_EQ(ptr1->redoCount, 1);
        EXPECT_EQ(ptr2->redoCount, 1);
        EXPECT_EQ(ptr3->redoCount, 1);
    }

    // ============================================================================
    // Complex Undo/Redo Scenarios
    // ============================================================================

    TEST_F(ActionManagerComprehensiveTest, InterleavedActionsAndGroups) {
        auto single1 = createCountingAction();
        auto* singlePtr1 = single1.get();

        auto group = ActionManager::createActionGroup();
        auto grouped1 = createCountingAction();
        auto grouped2 = createCountingAction();
        auto* groupedPtr1 = grouped1.get();
        auto* groupedPtr2 = grouped2.get();
        group->addAction(std::move(grouped1));
        group->addAction(std::move(grouped2));

        auto single2 = createCountingAction();
        auto* singlePtr2 = single2.get();

        manager.recordAction(std::move(single1));
        manager.recordAction(std::move(group));
        manager.recordAction(std::move(single2));

        EXPECT_EQ(manager.getUndoStackSize(), 3u);

        manager.undo();
        EXPECT_EQ(singlePtr2->undoCount, 1);

        manager.undo();
        EXPECT_EQ(groupedPtr1->undoCount, 1);
        EXPECT_EQ(groupedPtr2->undoCount, 1);

        manager.undo();
        EXPECT_EQ(singlePtr1->undoCount, 1);

        EXPECT_FALSE(manager.canUndo());
    }

    TEST_F(ActionManagerComprehensiveTest, MixedUndoRedoWithGroups) {
        for (int i = 0; i < 3; ++i) {
            auto group = ActionManager::createActionGroup();
            group->addAction(createCountingAction());
            group->addAction(createCountingAction());
            manager.recordAction(std::move(group));
        }

        manager.undo();
        manager.undo();
        EXPECT_EQ(manager.getUndoStackSize(), 1u);

        manager.redo();
        EXPECT_EQ(manager.getUndoStackSize(), 2u);

        manager.recordAction(createCountingAction());
        EXPECT_EQ(manager.getUndoStackSize(), 3u);
        EXPECT_FALSE(manager.canRedo());
    }

    // ============================================================================
    // Stress Tests
    // ============================================================================

    TEST_F(ActionManagerComprehensiveTest, LargeNumberOfActions) {
        const int actionCount = 1000;

        for (int i = 0; i < actionCount; ++i) {
            manager.recordAction(createCountingAction());
        }

        // Should be limited by max undo levels (default 50)
        EXPECT_LE(manager.getUndoStackSize(), 50u);
        EXPECT_GT(manager.getUndoStackSize(), 0u);
    }

    TEST_F(ActionManagerComprehensiveTest, LargeNumberOfUndoRedoCycles) {
        auto action = createCountingAction();
        auto* actionPtr = action.get();

        manager.recordAction(std::move(action));

        for (int i = 0; i < 100; ++i) {
            manager.undo();
            manager.redo();
        }

        EXPECT_EQ(actionPtr->undoCount, 100);
        EXPECT_EQ(actionPtr->redoCount, 100);
    }

    TEST_F(ActionManagerComprehensiveTest, LargeActionGroup) {
        auto group = ActionManager::createActionGroup();
        std::vector<testing::CountingAction*> actionPtrs;

        for (int i = 0; i < 100; ++i) {
            auto action = createCountingAction();
            actionPtrs.push_back(action.get());
            group->addAction(std::move(action));
        }

        manager.recordAction(std::move(group));
        manager.undo();

        for (auto* ptr : actionPtrs) {
            EXPECT_EQ(ptr->undoCount, 1);
        }

        manager.redo();

        for (auto* ptr : actionPtrs) {
            EXPECT_EQ(ptr->redoCount, 1);
        }
    }

    // ============================================================================
    // History Limit with Manager
    // ============================================================================

    TEST_F(ActionManagerComprehensiveTest, ClearHistoryPreservesState) {
        for (int i = 0; i < 10; ++i) {
            manager.recordAction(createCountingAction());
        }

        manager.undo();
        manager.undo();

        manager.clearHistory();

        EXPECT_FALSE(manager.canUndo());
        EXPECT_FALSE(manager.canRedo());
        EXPECT_EQ(manager.getUndoStackSize(), 0u);
    }

    TEST_F(ActionManagerComprehensiveTest, ClearHistoryPartial) {
        auto action1 = createCountingAction();
        auto action2 = createCountingAction();
        auto action3 = createCountingAction();

        auto* ptr1 = action1.get();
        auto* ptr2 = action2.get();
        auto* ptr3 = action3.get();

        manager.recordAction(std::move(action1));
        manager.recordAction(std::move(action2));
        manager.recordAction(std::move(action3));

        manager.clearHistory(2);
        EXPECT_EQ(manager.getUndoStackSize(), 1u);

        manager.undo();
        EXPECT_EQ(ptr1->undoCount, 1);
        // ptr2 and ptr3 should be destroyed
    }

    // ============================================================================
    // Edge Cases with Manager
    // ============================================================================

    TEST_F(ActionManagerComprehensiveTest, RecordActionAfterFullUndo) {
        manager.recordAction(createCountingAction());
        manager.recordAction(createCountingAction());

        manager.undo();
        manager.undo();
        EXPECT_FALSE(manager.canUndo());

        manager.recordAction(createCountingAction());
        EXPECT_TRUE(manager.canUndo());
        EXPECT_FALSE(manager.canRedo());
    }

    TEST_F(ActionManagerComprehensiveTest, AlternatingRecordAndUndoRedo) {
        auto action1 = createCountingAction();
        auto* ptr1 = action1.get();
        manager.recordAction(std::move(action1));

        manager.undo();
        manager.redo();

        auto action2 = createCountingAction();
        auto* ptr2 = action2.get();
        manager.recordAction(std::move(action2));

        manager.undo();
        EXPECT_EQ(ptr2->undoCount, 1);

        manager.redo();
        EXPECT_EQ(ptr2->redoCount, 1);
    }

    TEST_F(ActionManagerComprehensiveTest, StateConsistencyAfterComplexOperations) {
        int state = 0;

        class StateAction : public Action {
        public:
            StateAction(int& s, int val, int prev) : state(s), value(val), prevValue(prev) {}
            void redo() override { state = value; }
            void undo() override { state = prevValue; }
        private:
            int& state;
            int value;
            int prevValue;
        };

        manager.recordAction(std::make_unique<StateAction>(state, 10, 0));
        manager.recordAction(std::make_unique<StateAction>(state, 20, 10));
        manager.recordAction(std::make_unique<StateAction>(state, 30, 20));

        manager.undo();
        EXPECT_EQ(state, 20);

        manager.recordAction(std::make_unique<StateAction>(state, 100, 20));
        EXPECT_EQ(state, 20);

        manager.undo();
        EXPECT_EQ(state, 20);

        manager.redo();
        EXPECT_EQ(state, 100);

        manager.undo();
        manager.undo();
        EXPECT_EQ(state, 10);
    }

    // ============================================================================
    // Boundary Condition Tests
    // ============================================================================

    TEST_F(ActionManagerComprehensiveTest, UndoAtBoundary) {
        manager.recordAction(createCountingAction());
        EXPECT_TRUE(manager.canUndo());

        manager.undo();
        EXPECT_FALSE(manager.canUndo());

        manager.undo();  // Should not crash
        EXPECT_FALSE(manager.canUndo());
    }

    TEST_F(ActionManagerComprehensiveTest, RedoAtBoundary) {
        manager.recordAction(createCountingAction());
        manager.undo();
        EXPECT_TRUE(manager.canRedo());

        manager.redo();
        EXPECT_FALSE(manager.canRedo());

        manager.redo();  // Should not crash
        EXPECT_FALSE(manager.canRedo());
    }

    TEST_F(ActionManagerComprehensiveTest, ClearHistoryBoundaries) {
        manager.clearHistory(0);
        EXPECT_EQ(manager.getUndoStackSize(), 0u);

        manager.recordAction(createCountingAction());
        manager.clearHistory(100);  // More than exists
        EXPECT_EQ(manager.getUndoStackSize(), 0u);
    }

}  // namespace nexo::editor
