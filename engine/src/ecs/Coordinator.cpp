//// Coordinator.cpp //////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
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

    std::vector<ComponentType> Coordinator::getAllComponentTypes(const Entity entity) const
    {
        std::vector<ComponentType> types;

        Signature signature = m_entityManager->getSignature(entity);

        // We have a mapping from component type IDs to type_index
        for (ComponentType type = 0; type < MAX_COMPONENT_TYPE; ++type) {
            if (signature.test(type)) {
                types.emplace_back(type);
            }
        }

        return types;
    }

    std::vector<std::type_index> Coordinator::getAllComponentTypeIndices(const Entity entity) const
    {
        const std::vector<ComponentType>& types = getAllComponentTypes(entity);
        std::vector<std::type_index> typeIndices;
        typeIndices.reserve(types.size());

        for (const auto& type : types)
        {
            typeIndices.push_back(m_typeIDtoTypeIndex.at(type));
        }

        return typeIndices;
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

    Entity Coordinator::duplicateEntity(const Entity sourceEntity) const
    {
        const Entity newEntity = createEntity();
        const Signature signature = m_entityManager->getSignature(sourceEntity);
        Signature destSignature = m_entityManager->getSignature(newEntity);
        for (ComponentType type = 0; type < MAX_COMPONENT_TYPE; ++type) {
            if (signature.test(type) && m_typeIDtoTypeIndex.contains(type)) {
                const Signature previousSignature = destSignature;
                destSignature.set(type, true);
                m_componentManager->duplicateComponent(type, sourceEntity, newEntity, previousSignature, destSignature);
            }
        }
        m_entityManager->setSignature(newEntity, destSignature);
        m_systemManager->entitySignatureChanged(newEntity, Signature{}, destSignature);
        return newEntity;
    }

    bool Coordinator::supportsMementoPattern(const std::any& component) const
    {
        const auto typeId = std::type_index(component.type());
        const auto it = m_supportsMementoPattern.find(typeId);
        return (it != m_supportsMementoPattern.end()) && it->second;
    }

    std::any Coordinator::saveComponent(const std::any& component) const
    {
        const auto typeId = std::type_index(component.type());
        const auto it = m_saveComponentFunctions.find(typeId);
        if (it != m_saveComponentFunctions.end())
            return it->second(component);
        return {};
    }

    std::any Coordinator::restoreComponent(const std::any& memento, const std::type_index& componentType) const
    {
        const auto it = m_restoreComponentFunctions.find(componentType);
        if (it != m_restoreComponentFunctions.end())
            return it->second(memento);
        return {};
    }

    void Coordinator::addComponentAny(const Entity entity, const std::type_index& typeIndex, const std::any& component)
    {
        const auto it = m_addComponentFunctions.find(typeIndex);
        if (it != m_addComponentFunctions.end())
            it->second(entity, component);
    }
}
