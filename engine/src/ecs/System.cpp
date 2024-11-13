//// System.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source file for the system manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "System.hpp"

namespace nexo::ecs {

    void SystemManager::entityDestroyed(const Entity entity) const
    {
        for (const auto &[fst, snd] : m_systems) {
            auto const &system = snd;
            system->entities.erase(entity);
        }
    }

    void SystemManager::entitySignatureChanged(const Entity entity, const Signature entitySignature)
    {
        for (const auto &[fst, snd] : m_systems) {
            auto const &type = fst;
            auto const &system = snd;

            if (auto const &systemSignature = m_signatures[type]; (entitySignature & systemSignature) == systemSignature)
                system->entities.insert(entity);
            else
                system->entities.erase(entity);
        }
    }
}