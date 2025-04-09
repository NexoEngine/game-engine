//// Entity.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the entity class
//
///////////////////////////////////////////////////////////////////////////////
#include "Entity.hpp"
#include "ECSExceptions.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>

namespace nexo::ecs {

    EntityManager::EntityManager()
    {
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
            m_availableEntities.push_back(entity);
    }

    Entity EntityManager::createEntity()
    {
        if (m_livingEntities.size() >= MAX_ENTITIES)
            THROW_EXCEPTION(TooManyEntities);

        const Entity id = m_availableEntities.front();
        m_availableEntities.pop_front();
        m_livingEntities.push_back(id);

        return id;
    }

    void EntityManager::destroyEntity(const Entity entity)
    {
        if (entity >= MAX_ENTITIES)
            THROW_EXCEPTION(OutOfRange, entity);

        const auto it = std::ranges::find(m_livingEntities, entity);
        if (it != m_livingEntities.end())
            m_livingEntities.erase(it);
        else
            return;
        m_signatures[entity].reset();

        m_availableEntities.push_front(entity);

    }

    void EntityManager::setSignature(const Entity entity, const Signature signature)
    {
        if (entity >= MAX_ENTITIES)
            THROW_EXCEPTION(OutOfRange, entity);

        m_signatures[entity] = signature;
    }

    Signature EntityManager::getSignature(const Entity entity) const
    {
        if (entity >= MAX_ENTITIES)
            THROW_EXCEPTION(OutOfRange, entity);

        return m_signatures[entity];
    }

    std::uint32_t EntityManager::getLivingEntityCount() const
    {
        return m_livingEntities.size();
    }

    std::span<const Entity> EntityManager::getLivingEntities() const
    {
        return {m_livingEntities};
    }

}
