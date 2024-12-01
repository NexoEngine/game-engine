//// SignalEvent.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        29/11/2024
//  Description: Header file to handle signals via Events
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <csignal>
#include <cstring>
#include <iostream>

#include "Event.hpp"
#include "Signals.hpp"

namespace nexo::event {

    class EventAnySignal final : public Event<EventAnySignal> {
        public:
            explicit EventAnySignal(int signal) : signal(signal) {};
            int signal;
    };
    std::ostream& operator<<(std::ostream& os, const EventAnySignal& event);

    /**
     * @brief Template class to handle a specific signal
     * @tparam TSignal The number of the signal to handle
     */
    template <int TSignal>
    class EventSignal final : public Event<EventSignal<TSignal>> {};
    template <int TSignal>
    std::ostream& operator<<(std::ostream& os, const EventSignal<TSignal>& event);

    using EventSignalTerminate = EventSignal<SIGTERM>;
    using EventSignalInterrupt = EventSignal<SIGINT>;

    // Init signal handling
    class SignalHandler {
        public:
            SignalHandler();
            ~SignalHandler() = default;

            // Singleton
            SignalHandler(SignalHandler const&) = delete;
            void operator=(SignalHandler const&) = delete;

            void registerEventManager(std::shared_ptr<EventManager> eventManager);

            /**
            * @brief Register a signal to be handled
            * @note This method is in public to allow custom signal handling, in this case, the signal will be emitted as an EventSignal
            */
            void registerSignal(int signal);

            static std::shared_ptr<SignalHandler> getInstance();

        private:
            static void signalHandler(int signal);
            static void defaultSignalHandler(int signal);

            template <typename EventType, typename... Args>
            static void emitEventToAll(Args&&... args);

            void initSignals();

            std::vector<std::shared_ptr<EventManager>> m_eventManagers;

            // Singleton
            inline static std::shared_ptr<SignalHandler> s_instance = nullptr;
    };

} // namespace nexo::event
