//// MockAction.hpp ///////////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        02/12/2025
//  Description: Mock Action class for testing ActionHistory and ActionGroup
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <gmock/gmock.h>
#include "context/actions/Action.hpp"

namespace nexo::editor::testing {

    class MockAction : public Action {
    public:
        MOCK_METHOD(void, redo, (), (override));
        MOCK_METHOD(void, undo, (), (override));
    };

    // Simple counter action for testing without GMock
    class CountingAction : public Action {
    public:
        int redoCount = 0;
        int undoCount = 0;

        void redo() override { ++redoCount; }
        void undo() override { ++undoCount; }
    };

}
