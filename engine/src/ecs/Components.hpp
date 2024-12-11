//// Components.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the components and component manager class for the ecs
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <array>
#include <cassert>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <optional>

#include "Logger.hpp"
#include "ECSExceptions.hpp"

namespace nexo::ecs
{
    using Entity = std::uint32_t;

    // Maximum entity count, given that 13 bits are used for ID
    constexpr Entity MAX_ENTITIES = 8191;
    using ComponentType = std::uint8_t;

    constexpr ComponentType MAX_COMPONENT_TYPE = 32;

    /**
        * @class IComponentArray
        *
        * @brief Interface for a component array in the ECS framework.
        *
        * This class defines the interface for component arrays, which are used to store
        * components of entities in the ECS system. It provides the foundation for managing
        * the lifecycle of components associated with entities.
        */
    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;

        virtual void entityDestroyed(Entity entity) = 0;
    };

    /**
        * @class ComponentArray<T>
        *
        * @brief Templated class that manages a specific type of component for all entities.
        *
        * This class manages the storage, retrieval, and deletion of components of a specific type.
        * It ensures efficient access and modification of components associated with entities.
        *
        * @tparam T - The type of the component this array will manage.
        */
    template <typename T>
    class ComponentArray final : public IComponentArray
    {
    public:
        /**
            * @brief Inserts a component for a specific entity.
            *
            * @param entity - The entity to which the component will be added.
            * @param component - The component to add.
            */
        void insertData(const Entity entity, T component)
        {
            if (m_size == MAX_ENTITIES)
                THROW_EXCEPTION(OutOfRange, MAX_ENTITIES);
            if (m_entityToIndexMap.contains(entity))
            {
                LOG(NEXO_WARN, "ECS::ComponentArray::insertData: Component already added to entity {}", entity);
                return;
            }

            size_t newIndex = m_size;
            m_entityToIndexMap[entity] = newIndex;
            m_indexToEntityMap[newIndex] = entity;
            m_componentArray[newIndex] = component;
            ++m_size;
        }

        /**
            * @brief Removes a component from a specific entity.
            *
            * @param entity - The entity from which the component will be removed.
            */
        void removeData(const Entity entity)
        {
            if (!m_entityToIndexMap.contains(entity))
                THROW_EXCEPTION(ComponentNotFound, entity);

            size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
            size_t indexOfLastElement = m_size - 1;
            m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastElement];

            const Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
            m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            m_entityToIndexMap.erase(entity);
            m_indexToEntityMap.erase(indexOfLastElement);

            --m_size;
        }

        /**
            * @brief Retrieves a reference to a component associated with a specific entity.
            *
            * @param entity - The entity whose component is to be retrieved.
            * @return T& - A reference to the requested component.
            */
        T& getData(const Entity entity)
        {
            if (!m_entityToIndexMap.contains(entity))
                THROW_EXCEPTION(ComponentNotFound, entity);

            return m_componentArray[m_entityToIndexMap[entity]];
        }

        /**
            * @brief Cleans up components associated with a destroyed entity.
            *
            * @param entity - The destroyed entity.
            */
        void entityDestroyed(const Entity entity) override
        {
            if (m_entityToIndexMap.contains(entity))
                removeData(entity);
        }

        bool hasComponent(const Entity entity) const
        {
            return m_entityToIndexMap.contains(entity);
        }

    private:
        std::array<T, MAX_ENTITIES> m_componentArray;

        std::unordered_map<Entity, size_t> m_entityToIndexMap;

        std::unordered_map<size_t, Entity> m_indexToEntityMap;

        size_t m_size = 0;
    };

    /**
        * @class ComponentManager
        *
        * @brief Manages the registration and handling of components in an ECS architecture.
        *
        * The ComponentManager is responsible for managing different types of components in the ECS framework.
        * It allows the registration of component types, adding and removing components to entities, and
        * accessing components of entities.
        */
    class ComponentManager
    {
    public:
        /**
            * @brief Registers a new component type in the system.
            *
            * Each component type is associated with a unique ComponentType ID and a ComponentArray
            * to manage instances of the component.
            */
        template <typename T>
        void registerComponent()
        {
            std::type_index typeName(typeid(T));
            if (m_componentTypes.contains(typeName))
            {
                LOG(NEXO_WARN, "ECS::ComponentManager::registerComponent: Component already registered");
                return;
            }

            m_componentTypes.insert({typeName, _nextComponentType});

            m_componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

            ++_nextComponentType;
        }

        /**
            * @brief Retrieves the ComponentType ID for a specific component type.
            *
            * @return ComponentType - The unique ID associated with the component type.
            */
        template <typename T>
        ComponentType getComponentType()
        {
            const std::type_index typeName(typeid(T));
            if (!m_componentTypes.contains(typeName))
                THROW_EXCEPTION(ComponentNotRegistered);

            return m_componentTypes[typeName];
        }

        /**
            * @brief Adds a component of a specific type to an entity.
            *
            * @param entity - The entity to which the component will be added.
            * @param component - The component to add to the entity.
            */
        template <typename T>
        void addComponent(Entity entity, T component)
        {
            getComponentArray<T>()->insertData(entity, component);
        }

        /**
            * @brief Removes a component of a specific type from an entity.
            *
            * @param entity - The entity from which the component will be removed.
            */
        template <typename T>
        void removeComponent(Entity entity)
        {
            getComponentArray<T>()->removeData(entity);
        }

        template <typename T>
        bool tryRemoveComponent(Entity entity)
        {
            if (!getComponentArray<T>()->hasComponent(entity))
                return false;
            getComponentArray<T>()->removeData(entity);
            return true;
        }

        /**
            * @brief Retrieves a reference to a component of a specific type from an entity.
            *
            * @param entity - The entity whose component is to be retrieved.
            * @return T& - A reference to the requested component.
            */
        template <typename T>
        T& getComponent(Entity entity)
        {
            return getComponentArray<T>()->getData(entity);
        }

        template <typename T>
        std::optional<std::reference_wrapper<T>> tryGetComponent(Entity entity)
        {
            if (!getComponentArray<T>()->hasComponent(entity))
                return std::nullopt;
            return getComponent<T>(entity);
        }

        /**
            * @brief Retrieves a reference to all components of a specific type from an entity.
            *
            * @param entity - The entity whose component is to be retrieved.
            */
        std::vector<std::shared_ptr<IComponentArray>> getAllComponents(Entity entity)
        {
            std::vector<std::shared_ptr<IComponentArray>> components;

            for (const auto& [type, array] : m_componentArrays)
            {
                auto componentArray = std::dynamic_pointer_cast<ComponentArray<std::shared_ptr<IComponentArray>>>(array);
                if (componentArray && componentArray->hasComponent(entity))
                {
                    components.push_back(componentArray->getData(entity));
                }
            }
            return components;
        }

        std::optional<std::vector<std::shared_ptr<IComponentArray>>> tryGetAllComponents(const Entity entity)
        {
            std::vector<std::shared_ptr<IComponentArray>> components = getAllComponents(entity);
            if (components.empty())
            {
                return std::nullopt;
            }
            return components;
        }

        /**
            * @brief Handles the destruction of an entity by ensuring all associated components are removed.
            *
            * @param entity - The entity that has been destroyed.
            */
        void entityDestroyed(Entity entity) const;

    private:
        std::unordered_map<std::type_index, ComponentType> m_componentTypes{};
        std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_componentArrays;

        ComponentType _nextComponentType{};

        /**
            * @brief Retrieves the ComponentArray associated with a specific component type.
            *
            * @return std::shared_ptr<ComponentArray<T>> - Shared pointer to the component array of the specified type.
            */
        template <typename T>
        std::shared_ptr<ComponentArray<T>> getComponentArray()
        {
            const std::type_index typeName(typeid(T));

            if (!m_componentArrays.contains(typeName))
                THROW_EXCEPTION(ComponentNotRegistered);

            return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
        }
    };
}
