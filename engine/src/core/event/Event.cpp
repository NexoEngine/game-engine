//// Event.cpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the event and event manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "Event.hpp"
#include "Listener.hpp"

namespace nexo::event {

    void EventManager::dispatchEvents() {
        while (!m_eventQueue.empty()) {
            auto event = m_eventQueue.front();
            m_eventQueue.pop();

            std::type_index typeIndex(typeid(*event));
            if (m_listeners.contains(typeIndex)) {
                for (auto *listener : m_listeners[typeIndex]) {
                    event->trigger(*listener);
                    if (event->consumed)
                        break;
                }
            }
        }
    }
}
