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
#include <concepts>

#include "Listener.hpp"
#include "Logger.hpp"

namespace nexo::scene {
    class Scene;
}

#define LISTENS_TO(...) public event::Listens<__VA_ARGS__>

namespace nexo::event {

    template<typename L, typename E>
    concept HandlesEvent =
        std::derived_from<L, BaseListener> &&
        requires(L* l, E& e) {
            { l->handleEvent(e) } -> std::same_as<void>;
        };

	/**
	* @brief Base interface for all events.
	*
	* All events must inherit from this interface and implement the trigger() method
	* to notify listeners when the event is emitted.
	*/
    class IEvent {
        public:
            virtual ~IEvent() = default;

            /**
             * @brief Triggers the event for the provided listener.
             *
             * Derived event classes must override this method to dispatch the event to the listener.
             *
             * @param listener The listener to trigger the event on.
             */
            virtual void trigger(BaseListener& listener) = 0;

            bool consumed = false;
    };

    /**
     * @brief Templated event class.
     *
     * Implements the trigger() method to dispatch the event to a listener that can handle the specific DerivedEvent type.
     *
     * @tparam DerivedEvent The derived event type.
     */
    template<typename DerivedEvent>
    class Event : public IEvent {
        public:
			/**
			* @brief Triggers the event by calling the appropriate listener handler.
			*
			* Casts the event to the DerivedEvent type and dispatches it using the helper triggerListener().
			*
			* @param listener The listener to trigger the event on.
			*/
            void trigger(BaseListener &listener) override
            {
                triggerListener(static_cast<DerivedEvent&>(*this), listener);
            }
        protected:
			/**
			* @brief Helper function to dispatch the event to a listener.
			*
			* If the listener can handle the event type, its handleEvent() method is called.
			* Otherwise, a warning is logged.
			*
			* @tparam T The event type.
			* @param event The event instance.
			* @param listener The listener to dispatch the event to.
			*/
            template<class T>
            static void triggerListener(T &event, BaseListener &listener)
            {
                if (auto *p = dynamic_cast<Listener<T>*>(&listener))
                    return p->handleEvent(event);
                LOG(NEXO_WARN, "Event(triggerListener): Listener {} is missing a handler", listener.getListenerName());
            }
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

		/**
		* @brief Registers a listener for a specific event type.
		*
		* Adds the provided listener to the internal list for the given EventType.
		*
		* @tparam EventType The event type to listen for.
		* @param listener Pointer to the listener.
		*/
		template<typename EventType, typename L>
        requires HandlesEvent<L, EventType>
        void registerListener(L* listener) {
            const std::type_index typeIndex(typeid(EventType));
            if (!m_listeners.contains(typeIndex)) {
                m_listeners[typeIndex] = std::vector<BaseListener*>();
            }

            m_listeners[typeIndex].push_back(listener);
            LOG(NEXO_DEV, "EventManager(registerListeners): Registered listener {}", listener->getListenerName());
        }

        /**
         * @brief Unregisters a listener for a specific event type.
         *
         * Removes the provided listener from the internal list for the given EventType.
         *
         * @tparam EventType The event type.
         * @param listener Pointer to the listener.
         */
        template <typename EventType>
        void unregisterListener(BaseListener* listener) {
            const std::type_index typeIndex(typeid(EventType));

            if (const auto it = m_listeners.find(typeIndex); it != m_listeners.end()) {
                auto& listeners = it->second;

                if (const auto listenerIt = std::ranges::find(listeners, listener); listenerIt != listeners.end()) {
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

        /**
         * @brief Queues an event for dispatch.
         *
         * Adds the provided event to the internal event queue.
         *
         * @tparam EventType The event type.
         * @param event Shared pointer to the event.
         */
        template <typename EventType>
        void emitEvent(std::shared_ptr<EventType> event) {
            m_eventQueue.push_back(event);
        }

        /**
         * @brief Constructs and emits an event with the given arguments.
         *
         * Forwards arguments to the event's constructor and queues the event.
         *
         * @tparam EventType The event type.
         * @tparam Args The types of arguments to forward.
         * @param args Arguments to construct the event.
         */
        template <typename EventType, typename... Args>
        void emitEvent(Args&&... args) {
            emitEvent(std::make_shared<EventType>(std::forward<Args>(args)...));
        }

        /**
         * @brief Dispatches all queued events.
         *
         * Iterates through the event queue, triggers each event for its registered listeners,
         * and re-queues the event if it was not consumed.
         */
        void dispatchEvents();

        /**
         * @brief Clears all events from the queue.
         */
        void clearEvents();

    private:
        std::unordered_map<std::type_index, std::vector<BaseListener *>> m_listeners;
        std::deque<std::shared_ptr<IEvent>> m_eventQueue;
    };
}
