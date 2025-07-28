//// System.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source file for the system manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "System.hpp"

#include <ranges>

namespace nexo::ecs {

    void SparseSet::insert(Entity entity)
    {
        if (contains(entity))
        {
            LOG(NEXO_WARN, "Entity {} already added to the sparse set", entity);
            return;
        }

        sparse[entity] = dense.size();
        dense.push_back(entity);
    }

    void SparseSet::erase(Entity entity)
    {
        if (!contains(entity))
        {
            LOG(NEXO_WARN, "Entity {} does not exist in the sparse set", entity);
            return;
        }

        const size_t index = sparse[entity];
        const size_t lastIndex = dense.size() - 1;
        const Entity lastEntity = dense[lastIndex];

        dense[index] = lastEntity;
        sparse[lastEntity] = index;
        dense.pop_back();
        sparse.erase(entity);
    }

    void SystemManager::entityDestroyed(const Entity entity, const Signature signature) const
    {
        for (const auto& system : std::ranges::views::values(m_querySystems)) {
            if (const Signature &systemSignature = system->getSignature(); (signature & systemSignature) == systemSignature)
                system->entities.erase(entity);
        }
    }

    void SystemManager::entitySignatureChanged(const Entity entity,
                                                 const Signature oldSignature,
                                                 const Signature newSignature)
    {
        for (const auto& system : std::ranges::views::values(m_querySystems)) {
            const Signature systemSignature = system->getSignature();
            // Check if entity qualifies now but did not qualify before.
            if (((oldSignature & systemSignature) != systemSignature) &&
                ((newSignature & systemSignature) == systemSignature)) {
                system->entities.insert(entity);
            }
            // Otherwise, if the entity no longer qualifies but did before, remove it.
            else if (((oldSignature & systemSignature) == systemSignature) &&
                     ((newSignature & systemSignature) != systemSignature)) {
                system->entities.erase(entity);
            }
        }
    }
}
