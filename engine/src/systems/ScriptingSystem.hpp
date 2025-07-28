//// ScriptingSystem.hpp //////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        21/06/2025
//  Description: Header file for the scripting system
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>

#include "scripting/native/systems/ManagedWorldState.hpp"

namespace nexo::system {

    class ScriptingSystem {
        public:
            ScriptingSystem();
            ~ScriptingSystem() = default;

            int  init();

            int  update();

            int  shutdown();

        private:
            void updateWorldState();

            std::string m_latestScriptingError;
            scripting::ManagedWorldState m_worldState = {};
    };

}