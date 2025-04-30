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
        const Signature signature = m_entityManager->getSignature(entity);
        m_entityManager->destroyEntity(entity);
        m_componentManager->entityDestroyed(entity, signature);
        m_systemManager->entityDestroyed(entity, signature);
    }

    std::vector<std::type_index> Coordinator::getAllComponentTypes(const Entity entity) const
    {
        std::vector<std::type_index> types;

        Signature signature = m_entityManager->getSignature(entity);

        // We have a mapping from component type IDs to type_index
        for (ComponentType type = 0; type < MAX_COMPONENT_TYPE; ++type) {
            if (signature.test(type) && m_typeIDtoTypeIndex.contains(type)) {
                types.emplace_back(m_typeIDtoTypeIndex.at(type));
            }
        }

        return types;
    }

    std::vector<std::pair<std::type_index, std::any>> Coordinator::getAllComponents(const Entity entity)
    {
        std::vector<std::pair<std::type_index, std::any>> components;

        // Get the entity's signature which already tells us which components it has
        Signature signature = m_entityManager->getSignature(entity);

        // Iterate only through components that the entity actually has
        for (ComponentType type = 0; type < MAX_COMPONENT_TYPE; ++type) {
            if (signature.test(type) && m_typeIDtoTypeIndex.contains(type)) {
                const auto& typeIndex = m_typeIDtoTypeIndex.at(type);
                components.emplace_back(typeIndex, m_getComponentFunctions[typeIndex](entity));
            }
        }

        return components;
    }

    bool Coordinator::supportsMementoPattern(const std::type_index typeIndex) const
    {
        auto it = m_supportsMementoPattern.find(typeIndex);
        return (it != m_supportsMementoPattern.end()) && it->second;
    }
}
