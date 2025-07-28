//// Event.cpp ////////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
