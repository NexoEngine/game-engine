//// StateAction.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for StateAction template class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "context/actions/StateAction.hpp"
#include <string>
#include <memory>

namespace nexo::editor {

    // Simple integer wrapper with Memento pattern
    class IntState {
    public:
        struct Memento {
            int value;

            IntState restore() const {
                IntState state;
                state.m_value = value;
                return state;
            }
        };

        IntState() : m_value(0) {}
        explicit IntState(int value) : m_value(value) {}

        Memento save() const {
            return Memento{m_value};
        }

        int getValue() const { return m_value; }

    private:
        int m_value;
    };

    // Float wrapper with Memento pattern
    class FloatState {
    public:
        struct Memento {
            float value;

            FloatState restore() const {
                FloatState state;
                state.m_value = value;
                return state;
            }
        };

        FloatState() : m_value(0.0f) {}
        explicit FloatState(float value) : m_value(value) {}

        Memento save() const {
            return Memento{m_value};
        }

        float getValue() const { return m_value; }

    private:
        float m_value;
    };

    // String wrapper with Memento pattern
    class StringState {
    public:
        struct Memento {
            std::string value;

            StringState restore() const {
                StringState state;
                state.m_text = value;
                return state;
            }
        };

        StringState() : m_text("") {}
        explicit StringState(const std::string& text) : m_text(text) {}

        Memento save() const {
            return Memento{m_text};
        }

        const std::string& getText() const { return m_text; }

    private:
        std::string m_text;
    };

    // Complex struct with multiple fields and Memento pattern
    struct Point3D {
        struct Memento {
            float x, y, z;

            Point3D restore() const {
                Point3D point;
                point.m_x = x;
                point.m_y = y;
                point.m_z = z;
                return point;
            }
        };

        Point3D() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}
        Point3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

        Memento save() const {
            return Memento{m_x, m_y, m_z};
        }

        float getX() const { return m_x; }
        float getY() const { return m_y; }
        float getZ() const { return m_z; }

    private:
        float m_x, m_y, m_z;
    };

    // Test fixture for StateAction tests
    class StateActionTest : public ::testing::Test {
    protected:
        void SetUp() override {}
        void TearDown() override {}
    };

    // Tests for IntState
    TEST_F(StateActionTest, IntStateActionConstruction) {
        IntState target(5);
        auto beforeState = IntState(5).save();
        auto afterState = IntState(10).save();

        StateAction<IntState> action(target, beforeState, afterState);

        // Action should not modify target until redo/undo is called
        EXPECT_EQ(target.getValue(), 5);
    }

    TEST_F(StateActionTest, IntStateActionRedo) {
        IntState target(5);
        auto beforeState = IntState(5).save();
        auto afterState = IntState(10).save();

        StateAction<IntState> action(target, beforeState, afterState);
        action.redo();

        EXPECT_EQ(target.getValue(), 10);
    }

    TEST_F(StateActionTest, IntStateActionUndo) {
        IntState target(10);
        auto beforeState = IntState(5).save();
        auto afterState = IntState(10).save();

        StateAction<IntState> action(target, beforeState, afterState);
        action.undo();

        EXPECT_EQ(target.getValue(), 5);
    }

    TEST_F(StateActionTest, IntStateActionMultipleRedoUndoCycles) {
        IntState target(5);
        auto beforeState = IntState(5).save();
        auto afterState = IntState(10).save();

        StateAction<IntState> action(target, beforeState, afterState);

        // First cycle
        action.redo();
        EXPECT_EQ(target.getValue(), 10);
        action.undo();
        EXPECT_EQ(target.getValue(), 5);

        // Second cycle
        action.redo();
        EXPECT_EQ(target.getValue(), 10);
        action.undo();
        EXPECT_EQ(target.getValue(), 5);

        // Third cycle
        action.redo();
        EXPECT_EQ(target.getValue(), 10);
        action.undo();
        EXPECT_EQ(target.getValue(), 5);
    }

    TEST_F(StateActionTest, IntStateActionAlternatingRedoUndo) {
        IntState target(0);
        auto beforeState = IntState(0).save();
        auto afterState = IntState(100).save();

        StateAction<IntState> action(target, beforeState, afterState);

        action.redo();
        EXPECT_EQ(target.getValue(), 100);

        action.redo();
        EXPECT_EQ(target.getValue(), 100);

        action.undo();
        EXPECT_EQ(target.getValue(), 0);

        action.undo();
        EXPECT_EQ(target.getValue(), 0);

        action.redo();
        EXPECT_EQ(target.getValue(), 100);
    }

    // Tests for FloatState
    TEST_F(StateActionTest, FloatStateActionRedo) {
        FloatState target(3.14f);
        auto beforeState = FloatState(3.14f).save();
        auto afterState = FloatState(2.71f).save();

        StateAction<FloatState> action(target, beforeState, afterState);
        action.redo();

        EXPECT_FLOAT_EQ(target.getValue(), 2.71f);
    }

    TEST_F(StateActionTest, FloatStateActionUndo) {
        FloatState target(2.71f);
        auto beforeState = FloatState(3.14f).save();
        auto afterState = FloatState(2.71f).save();

        StateAction<FloatState> action(target, beforeState, afterState);
        action.undo();

        EXPECT_FLOAT_EQ(target.getValue(), 3.14f);
    }

    TEST_F(StateActionTest, FloatStateActionMultipleCycles) {
        FloatState target(1.0f);
        auto beforeState = FloatState(1.0f).save();
        auto afterState = FloatState(2.0f).save();

        StateAction<FloatState> action(target, beforeState, afterState);

        for (int i = 0; i < 5; ++i) {
            action.redo();
            EXPECT_FLOAT_EQ(target.getValue(), 2.0f);
            action.undo();
            EXPECT_FLOAT_EQ(target.getValue(), 1.0f);
        }
    }

    // Tests for StringState
    TEST_F(StateActionTest, StringStateActionRedo) {
        StringState target("Hello");
        auto beforeState = StringState("Hello").save();
        auto afterState = StringState("World").save();

        StateAction<StringState> action(target, beforeState, afterState);
        action.redo();

        EXPECT_EQ(target.getText(), "World");
    }

    TEST_F(StateActionTest, StringStateActionUndo) {
        StringState target("World");
        auto beforeState = StringState("Hello").save();
        auto afterState = StringState("World").save();

        StateAction<StringState> action(target, beforeState, afterState);
        action.undo();

        EXPECT_EQ(target.getText(), "Hello");
    }

    TEST_F(StateActionTest, StringStateActionEmptyStrings) {
        StringState target("");
        auto beforeState = StringState("").save();
        auto afterState = StringState("Content").save();

        StateAction<StringState> action(target, beforeState, afterState);

        action.redo();
        EXPECT_EQ(target.getText(), "Content");

        action.undo();
        EXPECT_EQ(target.getText(), "");
    }

    TEST_F(StateActionTest, StringStateActionLongStrings) {
        std::string longStr1(1000, 'A');
        std::string longStr2(1000, 'B');

        StringState target(longStr1);
        auto beforeState = StringState(longStr1).save();
        auto afterState = StringState(longStr2).save();

        StateAction<StringState> action(target, beforeState, afterState);

        action.redo();
        EXPECT_EQ(target.getText(), longStr2);

        action.undo();
        EXPECT_EQ(target.getText(), longStr1);
    }

    TEST_F(StateActionTest, StringStateActionMultipleCycles) {
        StringState target("Initial");
        auto beforeState = StringState("Initial").save();
        auto afterState = StringState("Modified").save();

        StateAction<StringState> action(target, beforeState, afterState);

        for (int i = 0; i < 3; ++i) {
            action.redo();
            EXPECT_EQ(target.getText(), "Modified");
            action.undo();
            EXPECT_EQ(target.getText(), "Initial");
        }
    }

    // Tests for Point3D (complex struct)
    TEST_F(StateActionTest, Point3DActionRedo) {
        Point3D target(1.0f, 2.0f, 3.0f);
        auto beforeState = Point3D(1.0f, 2.0f, 3.0f).save();
        auto afterState = Point3D(4.0f, 5.0f, 6.0f).save();

        StateAction<Point3D> action(target, beforeState, afterState);
        action.redo();

        EXPECT_FLOAT_EQ(target.getX(), 4.0f);
        EXPECT_FLOAT_EQ(target.getY(), 5.0f);
        EXPECT_FLOAT_EQ(target.getZ(), 6.0f);
    }

    TEST_F(StateActionTest, Point3DActionUndo) {
        Point3D target(4.0f, 5.0f, 6.0f);
        auto beforeState = Point3D(1.0f, 2.0f, 3.0f).save();
        auto afterState = Point3D(4.0f, 5.0f, 6.0f).save();

        StateAction<Point3D> action(target, beforeState, afterState);
        action.undo();

        EXPECT_FLOAT_EQ(target.getX(), 1.0f);
        EXPECT_FLOAT_EQ(target.getY(), 2.0f);
        EXPECT_FLOAT_EQ(target.getZ(), 3.0f);
    }

    TEST_F(StateActionTest, Point3DActionZeroValues) {
        Point3D target(0.0f, 0.0f, 0.0f);
        auto beforeState = Point3D(0.0f, 0.0f, 0.0f).save();
        auto afterState = Point3D(10.0f, 20.0f, 30.0f).save();

        StateAction<Point3D> action(target, beforeState, afterState);

        action.redo();
        EXPECT_FLOAT_EQ(target.getX(), 10.0f);
        EXPECT_FLOAT_EQ(target.getY(), 20.0f);
        EXPECT_FLOAT_EQ(target.getZ(), 30.0f);

        action.undo();
        EXPECT_FLOAT_EQ(target.getX(), 0.0f);
        EXPECT_FLOAT_EQ(target.getY(), 0.0f);
        EXPECT_FLOAT_EQ(target.getZ(), 0.0f);
    }

    TEST_F(StateActionTest, Point3DActionNegativeValues) {
        Point3D target(-1.0f, -2.0f, -3.0f);
        auto beforeState = Point3D(-1.0f, -2.0f, -3.0f).save();
        auto afterState = Point3D(1.0f, 2.0f, 3.0f).save();

        StateAction<Point3D> action(target, beforeState, afterState);

        action.redo();
        EXPECT_FLOAT_EQ(target.getX(), 1.0f);
        EXPECT_FLOAT_EQ(target.getY(), 2.0f);
        EXPECT_FLOAT_EQ(target.getZ(), 3.0f);

        action.undo();
        EXPECT_FLOAT_EQ(target.getX(), -1.0f);
        EXPECT_FLOAT_EQ(target.getY(), -2.0f);
        EXPECT_FLOAT_EQ(target.getZ(), -3.0f);
    }

    TEST_F(StateActionTest, Point3DActionMultipleCycles) {
        Point3D target(1.0f, 1.0f, 1.0f);
        auto beforeState = Point3D(1.0f, 1.0f, 1.0f).save();
        auto afterState = Point3D(2.0f, 2.0f, 2.0f).save();

        StateAction<Point3D> action(target, beforeState, afterState);

        for (int i = 0; i < 10; ++i) {
            action.redo();
            EXPECT_FLOAT_EQ(target.getX(), 2.0f);
            EXPECT_FLOAT_EQ(target.getY(), 2.0f);
            EXPECT_FLOAT_EQ(target.getZ(), 2.0f);

            action.undo();
            EXPECT_FLOAT_EQ(target.getX(), 1.0f);
            EXPECT_FLOAT_EQ(target.getY(), 1.0f);
            EXPECT_FLOAT_EQ(target.getZ(), 1.0f);
        }
    }

    // Test that StateAction works with Action interface
    TEST_F(StateActionTest, StateActionAsActionInterface) {
        IntState target(42);
        auto beforeState = IntState(42).save();
        auto afterState = IntState(84).save();

        std::unique_ptr<Action> action = std::make_unique<StateAction<IntState>>(
            target, beforeState, afterState
        );

        action->redo();
        EXPECT_EQ(target.getValue(), 84);

        action->undo();
        EXPECT_EQ(target.getValue(), 42);
    }

    TEST_F(StateActionTest, MultipleStateActionsOnSameTarget) {
        IntState target(0);

        auto state0 = IntState(0).save();
        auto state1 = IntState(1).save();
        auto state2 = IntState(2).save();

        StateAction<IntState> action1(target, state0, state1);
        StateAction<IntState> action2(target, state1, state2);

        // Apply first action
        action1.redo();
        EXPECT_EQ(target.getValue(), 1);

        // Apply second action
        action2.redo();
        EXPECT_EQ(target.getValue(), 2);

        // Undo second action
        action2.undo();
        EXPECT_EQ(target.getValue(), 1);

        // Undo first action
        action1.undo();
        EXPECT_EQ(target.getValue(), 0);
    }

    TEST_F(StateActionTest, StateActionWithSameBeforeAndAfterState) {
        IntState target(5);
        auto sameState = IntState(5).save();

        StateAction<IntState> action(target, sameState, sameState);

        action.redo();
        EXPECT_EQ(target.getValue(), 5);

        action.undo();
        EXPECT_EQ(target.getValue(), 5);
    }

}
