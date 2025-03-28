//// Coordinator.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source file for the coordinator class
//
///////////////////////////////////////////////////////////////////////////////

#include "Coordinator.hpp"

std::shared_ptr<nexo::ecs::Coordinator> nexo::ecs::System::coord = nullptr;

namespace nexo::ecs {

    void Coordinator::init()
    {
        m_componentManager = std::make_shared<ComponentManager>();
        m_entityManager = std::make_shared<EntityManager>();
        m_systemManager = std::make_shared<SystemManager>();
        m_singletonComponentManager = std::make_shared<SingletonComponentManager>();

        System::coord = std::shared_ptr<Coordinator>(this, [](const Coordinator*) {});

        LOG(NEXO_DEV, "ecs: Coordinator initialized");
    }

    Entity Coordinator::createEntity() const
    {
        return m_entityManager->createEntity();
    }

    void Coordinator::destroyEntity(const Entity entity) const
    {
    	auto signature = m_entityManager->getSignature(entity);
        m_entityManager->destroyEntity(entity);
        m_componentManager->entityDestroyed(entity);
        m_systemManager->entityDestroyed(entity, signature);
    }

    std::vector<std::type_index> Coordinator::getAllComponentTypes(const Entity entity) const
    {
        std::vector<std::type_index> types;

        for (const auto& [type, func] : m_hasComponentFunctions) {
            if (func(entity)) {
                types.emplace_back(type);
            }
        }

        return types;
    }

    std::vector<std::pair<std::type_index, std::any>> Coordinator::getAllComponents(const Entity entity)
    {
        std::vector<std::pair<std::type_index, std::any>> components;

        for (const auto& [type, func] : m_hasComponentFunctions) {
            if (func(entity)) {
                components.emplace_back(type, m_getComponentFunctions[type](entity));
            }
        }

        return components;
    }
}
