//// ActionGroup.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        02/12/2025
//  Description: Test file for ActionGroup class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "context/ActionGroup.hpp"
#include "../mocks/MockAction.hpp"

namespace nexo::editor {

    class ActionGroupTest : public ::testing::Test {
    protected:
        ActionGroup group;

        std::unique_ptr<testing::CountingAction> createCountingAction() {
            return std::make_unique<testing::CountingAction>();
        }
    };

    TEST_F(ActionGroupTest, HasActionsReturnsFalseWhenEmpty) {
        EXPECT_FALSE(group.hasActions());
    }

    TEST_F(ActionGroupTest, AddActionMakesHasActionsReturnTrue) {
        group.addAction(createCountingAction());
        EXPECT_TRUE(group.hasActions());
    }

    TEST_F(ActionGroupTest, RedoExecutesAllActionsInOrder) {
        std::vector<int> executionOrder;

        class OrderedAction : public Action {
        public:
            explicit OrderedAction(int id, std::vector<int>& order) : m_id(id), m_order(order) {}
            void redo() override { m_order.push_back(m_id); }
            void undo() override { m_order.push_back(-m_id); }
        private:
            int m_id;
            std::vector<int>& m_order;
        };

        group.addAction(std::make_unique<OrderedAction>(1, executionOrder));
        group.addAction(std::make_unique<OrderedAction>(2, executionOrder));
        group.addAction(std::make_unique<OrderedAction>(3, executionOrder));

        group.redo();

        ASSERT_EQ(executionOrder.size(), 3u);
        EXPECT_EQ(executionOrder[0], 1);
        EXPECT_EQ(executionOrder[1], 2);
        EXPECT_EQ(executionOrder[2], 3);
    }

    TEST_F(ActionGroupTest, UndoExecutesActionsInReverseOrder) {
        std::vector<int> executionOrder;

        class OrderedAction : public Action {
        public:
            explicit OrderedAction(int id, std::vector<int>& order) : m_id(id), m_order(order) {}
            void redo() override { m_order.push_back(m_id); }
            void undo() override { m_order.push_back(-m_id); }
        private:
            int m_id;
            std::vector<int>& m_order;
        };

        group.addAction(std::make_unique<OrderedAction>(1, executionOrder));
        group.addAction(std::make_unique<OrderedAction>(2, executionOrder));
        group.addAction(std::make_unique<OrderedAction>(3, executionOrder));

        group.undo();

        ASSERT_EQ(executionOrder.size(), 3u);
        EXPECT_EQ(executionOrder[0], -3);  // Undo IDs are negated
        EXPECT_EQ(executionOrder[1], -2);
        EXPECT_EQ(executionOrder[2], -1);
    }

    TEST_F(ActionGroupTest, RedoOnEmptyGroupDoesNothing) {
        EXPECT_NO_THROW(group.redo());
    }

    TEST_F(ActionGroupTest, UndoOnEmptyGroupDoesNothing) {
        EXPECT_NO_THROW(group.undo());
    }

    TEST_F(ActionGroupTest, MultipleActionsAllGetExecuted) {
        auto action1 = createCountingAction();
        auto action2 = createCountingAction();
        auto action3 = createCountingAction();

        auto* ptr1 = action1.get();
        auto* ptr2 = action2.get();
        auto* ptr3 = action3.get();

        group.addAction(std::move(action1));
        group.addAction(std::move(action2));
        group.addAction(std::move(action3));

        group.redo();

        EXPECT_EQ(ptr1->redoCount, 1);
        EXPECT_EQ(ptr2->redoCount, 1);
        EXPECT_EQ(ptr3->redoCount, 1);

        group.undo();

        EXPECT_EQ(ptr1->undoCount, 1);
        EXPECT_EQ(ptr2->undoCount, 1);
        EXPECT_EQ(ptr3->undoCount, 1);
    }

    TEST_F(ActionGroupTest, SingleActionRedo) {
        auto action = createCountingAction();
        auto* actionPtr = action.get();

        group.addAction(std::move(action));
        group.redo();

        EXPECT_EQ(actionPtr->redoCount, 1);
        EXPECT_EQ(actionPtr->undoCount, 0);
    }

    TEST_F(ActionGroupTest, SingleActionUndo) {
        auto action = createCountingAction();
        auto* actionPtr = action.get();

        group.addAction(std::move(action));
        group.undo();

        EXPECT_EQ(actionPtr->redoCount, 0);
        EXPECT_EQ(actionPtr->undoCount, 1);
    }

}
