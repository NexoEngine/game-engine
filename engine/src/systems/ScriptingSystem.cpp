//// ScriptingSystem.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        21/06/2025
//  Description: Source file for the scripting system
//
///////////////////////////////////////////////////////////////////////////////

#include "ScriptingSystem.hpp"

#include "scripting/native/Scripting.hpp"
#include <tracy/Tracy.hpp>

namespace nexo::system {

    ScriptingSystem::ScriptingSystem()
    {
        ZoneScoped;
        ZoneName("Scripting System Constructor", 26);

        scripting::HostHandler::Parameters params;
        params.errorCallback = [this](const scripting::HostString& message) {
            LOG(NEXO_ERROR, "Scripting host error: {}", message.to_utf8());
            m_latestScriptingError = message.to_utf8();
        };

        scripting::HostHandler& host = scripting::HostHandler::getInstance();

        // Initialize the host
        if (host.initialize(params) != scripting::HostHandler::SUCCESS) {
            LOG(NEXO_ERROR, "Failed to initialize host");
            THROW_EXCEPTION(scripting::ScriptingBackendInitFailed, m_latestScriptingError);
        }
    }

    int ScriptingSystem::init()
    {
        ZoneScoped;
        ZoneName("Scripting System Init", 20);

        const auto &scriptHost = scripting::HostHandler::getInstance();

        {
            ZoneScopedN("Update World State");
            updateWorldState();
        }

        {
            ZoneScopedN("Initialize Components");
            if (auto ret = scriptHost.getManagedApi().SystemBase.InitializeComponents(); ret != 0) {
                LOG(NEXO_ERROR, "Failed to initialize scripting components, returned: {}", ret);
                return ret;
            }
            LOG(NEXO_INFO, "Scripting components initialized successfully");
        }

        {
            ZoneScopedN("Initialize Systems");
            if (auto ret = scriptHost.getManagedApi().SystemBase.InitializeSystems(&m_worldState, sizeof(m_worldState)); ret != 0) {
                LOG(NEXO_ERROR, "Failed to initialize scripting systems, returned: {}", ret);
                return ret;
            }
            LOG(NEXO_INFO, "Scripting systems initialized successfully");
        }

        if (scriptHost.runScriptExample() == EXIT_FAILURE) {
            LOG(NEXO_ERROR, "Error in runScriptExample");
            return 1;
        }

        LOG(NEXO_INFO, "Successfully ran runScriptExample");
        return 0;
    }

    int ScriptingSystem::update()
    {
        ZoneScoped;
        ZoneName("Scripting System Update", 22);

        const auto &scriptHost = scripting::HostHandler::getInstance();
        const auto &api = scriptHost.getManagedApi();

        {
            ZoneScopedN("Update World State");
            updateWorldState();
        }

        {
            ZoneScopedN("Update Managed Systems");
            if (const auto ret = api.SystemBase.UpdateSystems(&m_worldState, sizeof(m_worldState)); ret != 0) {
                LOG_ONCE(NEXO_ERROR, "Failed to update scripting systems");
                return ret;
            }
            Logger::resetOnce(NEXO_LOG_ONCE_KEY("Failed to update scripting systems"));
        }

        return 0;
    }

    int ScriptingSystem::shutdown()
    {
        ZoneScoped;
        ZoneName("Scripting System Shutdown", 24);

        const auto &scriptHost = scripting::HostHandler::getInstance();
        const auto &api = scriptHost.getManagedApi();

        {
            ZoneScopedN("Update World State");
            updateWorldState();
        }

        {
            ZoneScopedN("Shutdown Managed Systems");
            if (auto ret = api.SystemBase.ShutdownSystems(&m_worldState, sizeof(m_worldState)); ret != 0) {
                LOG(NEXO_ERROR, "Failed to shutdown scripting systems: {}", ret);
                return ret;
            }
            LOG(NEXO_INFO, "Scripting systems shutdown successfully");
        }

        return 0;
    }

    void ScriptingSystem::updateWorldState()
    {
        ZoneScoped;
        ZoneName("Update World State", 18);

        Application& app = Application::getInstance();
        m_worldState.update(app.getWorldState());
    }

} // namespace nexo::system
