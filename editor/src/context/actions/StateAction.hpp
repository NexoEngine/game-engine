//// StateAction.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/04/2025
//  Description: Header file for the state action class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Action.hpp"

#include <memory>
#include <functional>

namespace nexo::editor {

    /**
    * Generic action for objects that can save and restore their state
    * Object must implement save() and restore() methods
    */
    template<typename T>
    class StateAction : public Action {
        public:
            StateAction(
                T& target,
                const typename T::Memento& before,
                const typename T::Memento& after
            ) : m_target(target), m_beforeState(before), m_afterState(after){}

            static std::unique_ptr<StateAction<T>> createFromOperation(
                T& target,
                std::function<void()> operation
            ) {

                // Capture before state
                auto beforeState = target.save();

                // Execute operation
                operation();

                // Capture after state
                auto afterState = target.save();

                // Create action
                return std::make_unique<StateAction<T>>(target, beforeState, afterState);
            }

            void redo() override
            {
                m_target = m_afterState.restore();
            }

            void undo() override
            {
                m_target = m_beforeState.restore();
            }

        private:
            T& m_target;
            typename T::Memento m_beforeState;
            typename T::Memento m_afterState;
    };

}
