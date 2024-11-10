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

namespace nexo::ecs {

    EntityManager::EntityManager()
    {
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
            m_availableEntities.push(entity);
    }

    Entity EntityManager::createEntity()
    {
        assert(m_livingEntityCount < MAX_ENTITIES && "Too many living entities");

        const Entity id = m_availableEntities.front();
        m_availableEntities.pop();
        ++m_livingEntityCount;

        return id;
    }

    void EntityManager::destroyEntity(const Entity entity)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range");

        m_signatures[entity].reset();

        m_availableEntities.push(entity);
        if (m_livingEntityCount > 0)
            --m_livingEntityCount;
    }

    void EntityManager::setSignature(const Entity entity, const Signature signature)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range");

        m_signatures[entity] = signature;
    }

    Signature EntityManager::getSignature(const Entity entity) const
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        return m_signatures[entity];
    }
}
