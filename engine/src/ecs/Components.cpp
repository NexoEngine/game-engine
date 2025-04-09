//// Components.cpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source file for the component manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "Components.hpp"

namespace nexo::ecs {

    void ComponentManager::entityDestroyed(const Entity entity, const Signature &entitySignature)
    {
        for (const auto &group: m_groupRegistry | std::views::values) {
            if ((entitySignature & group->allSignature()) == group->allSignature())
                group->removeFromGroup(entity);
        }
        for (const auto& componentArray : m_componentArrays) {
            if (componentArray)
                componentArray->entityDestroyed(entity);
        }
    }

}
