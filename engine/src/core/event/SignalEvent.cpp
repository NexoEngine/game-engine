//// SignalEvent.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        29/11/2024
//  Description: Source file to handle signals via Events
//
///////////////////////////////////////////////////////////////////////////////

#include "SignalEvent.hpp"

namespace nexo::event {

    void SignalHandler::signalHandler(const int signal)
    {
        emitEventToAll<EventAnySignal>(signal);
        switch (signal)
        {
            case SIGTERM:
                emitEventToAll<EventSignalTerminate>();
                break;
            case SIGINT:
                emitEventToAll<EventSignalInterrupt>();
                break;
            default:
                LOG(NEXO_WARN, "SignalHandler: cannot emit event: unknown signal {}", signal);
                break;
        }
    }

    void SignalHandler::defaultSignalHandler(int signal)
    {
        emitEventToAll<EventAnySignal>(signal);
    }

    template<typename EventType, typename... Args>
    void SignalHandler::emitEventToAll(Args &&... args)
    {
        for (const auto &eventManager: getInstance()->m_eventManagers)
        {
            eventManager->emitEvent<EventType>(std::forward<Args>(args)...);
        }
    }

    SignalHandler::SignalHandler()
    {
        initSignals();
    }

    void SignalHandler::registerEventManager(std::shared_ptr<EventManager> eventManager)
    {
        m_eventManagers.push_back(std::move(eventManager));
    }

    std::shared_ptr<SignalHandler> SignalHandler::getInstance()
    {
        if (!s_instance)
            s_instance = std::make_shared<SignalHandler>();
        return s_instance;
    }

    void SignalHandler::initSignals() const
    {
        signal(SIGTERM, signalHandler);
        signal(SIGINT, signalHandler);
    }
} // namespace nexo::event
