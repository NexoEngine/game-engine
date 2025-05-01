//// ActionHistory.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/04/2025
//  Description: Header file for the action history class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "actions/Action.hpp"
#include <memory>
#include <deque>
#include <string>

namespace nexo::editor {

    /**
    * Maintains the undo and redo stacks
    */
    class ActionHistory {
        public:
            // Add a action to history after it was already executed
            void addAction(std::unique_ptr<Action> action);

            bool canUndo() const;
            bool canRedo() const;
            void undo();
            void redo();

            void setMaxUndoLevels(size_t levels);

            void clear(unsigned int count = 0);
            unsigned int getUndoStackSize() const;

        private:
            std::deque<std::unique_ptr<Action>> undoStack;
            std::deque<std::unique_ptr<Action>> redoStack;
            size_t maxUndoLevels = 50;
    };

}
