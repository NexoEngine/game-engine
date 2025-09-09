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

	void EventManager::dispatchEvents()
	{
	    size_t size = m_eventQueue.size();
	    while (size--) {
	        auto event = m_eventQueue.front();
	        m_eventQueue.pop_front();

	        // Store a reference to the event to avoid evaluating *event with side effects.
	        const IEvent &ev = *event;
	        if (const std::type_index typeIndex(typeid(ev)); m_listeners.contains(typeIndex)) {
	            for (const auto listener : m_listeners[typeIndex]) {
	                event->trigger(*listener);
	                if (event->consumed)
	                    break;
	            }
	        }
	        m_eventQueue.push_back(event);
	    }
	}

    void EventManager::clearEvents()
    {
    	m_eventQueue.clear();
    }
}
