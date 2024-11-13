//// Event.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the event and event manager class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <unordered_map>
#include <typeindex>
#include <functional>
#include <queue>
#include <memory>

#include "Listener.hpp"
#include "Logger.hpp"

namespace nexo::scene {
    class Scene;
}

#define LISTENS_TO(...) public event::Listens<__VA_ARGS__>

namespace nexo::event {

    class Event {
        public:

            virtual ~Event() = default;
            virtual void trigger(BaseListener &) {}
            bool consumed = false;
        protected:
            template<class T>
            static void triggerListener(T &event, BaseListener &listener)
            {
                if (auto *p = dynamic_cast<Listener<T>*>(&listener))
                    return p->handleEvent(event);
                LOG(NEXO_WARN, "Event(triggerListener): Listener {} is missing a handler", listener.getListenerName());
            }

            #define LISTENABLE() \
                virtual void trigger(BaseListener &l) \
                    { return triggerListener(*this, l); }
    };

    /**
     * @class EventManager
     * @brief Manages event handling and dispatching in the ECS system.
     *
     * This class is responsible for registering event listeners, emitting events, and
     * managing the event dispatch process. It allows for loose coupling between
     * different components of the ECS system by using an event-driven approach.
     */
    class EventManager {
    public:
        template <typename EventType>
        void registerListener(BaseListener* listener) {
            const std::type_index typeIndex(typeid(EventType));
            if (m_listeners.find(typeIndex) == m_listeners.end()) {
                m_listeners[typeIndex] = std::vector<BaseListener*>();
            }

            m_listeners[typeIndex].push_back(listener);
            LOG(NEXO_DEV, "EventManager(registerListeners): Registered listener {}", listener->getListenerName());
        }

        template <typename EventType>
        void unregisterListener(BaseListener* listener) {
            const std::type_index typeIndex(typeid(EventType));

            if (const auto it = m_listeners.find(typeIndex); it != m_listeners.end()) {
                auto& listeners = it->second;

                if (const auto listenerIt = std::find(listeners.begin(), listeners.end(), listener); listenerIt != listeners.end()) {
                    listeners.erase(listenerIt);
                    LOG(NEXO_DEV, "EventManager(unregisterListener): Unregistered listener {}", listener->getListenerName());

                    if (listeners.empty()) {
                        m_listeners.erase(it);
                    }
                } else {
                    LOG(NEXO_WARN, "EventManager(unregisterListener): Listener not found for this event type");
                }
            } else {
                LOG(NEXO_WARN, "EventManager(unregisterListener): No listeners found for this event type");
            }
        }

        template <typename EventType>
        void emitEvent(std::shared_ptr<EventType> event) {
            m_eventQueue.push(event);
        }

        void dispatchEvents(scene::Scene &scene);

    private:
        std::unordered_map<std::type_index, std::vector<BaseListener *>> m_listeners;
        std::queue<std::shared_ptr<Event>> m_eventQueue;
    };
}

