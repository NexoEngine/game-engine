//// ActionHistory.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/04/2025
//  Description: Source file for the action history class
//
///////////////////////////////////////////////////////////////////////////////

#include "ActionHistory.hpp"

namespace nexo::editor {
    void ActionHistory::addAction(std::unique_ptr<Action> action)
    {
        undoStack.push_back(std::move(action));
        redoStack.clear();

        while (undoStack.size() > maxUndoLevels)
            undoStack.pop_front();
    }

    bool ActionHistory::canUndo() const
    {
        return !undoStack.empty();
    }

    bool ActionHistory::canRedo() const
    {
        return !redoStack.empty();
    }

    void ActionHistory::undo()
    {
        if (!canUndo())
            return;
        auto action = std::move(undoStack.back());
        undoStack.pop_back();
        action->undo();
        redoStack.push_back(std::move(action));
    }

    void ActionHistory::redo()
    {
        if (!canRedo())
            return;
        auto action = std::move(redoStack.back());
        redoStack.pop_back();
        action->redo();
        undoStack.push_back(std::move(action));
    }

    void ActionHistory::setMaxUndoLevels(size_t levels)
    {
        maxUndoLevels = levels;
        while (undoStack.size() > maxUndoLevels)
            undoStack.pop_front();
    }

    void ActionHistory::clear(unsigned int count)
    {
        if (!count) {
            undoStack.clear();
            redoStack.clear();
            return;
        }
        unsigned int elementsToRemove = std::min(static_cast<unsigned int>(undoStack.size()), count);
        for (unsigned int i = 0; i < elementsToRemove; ++i)
            undoStack.pop_back();
    }
}
