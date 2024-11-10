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

namespace nexo::ecs {

    using Entity = std::uint32_t;

    constexpr Entity ENTITY_ID_MASK = 0x1FFF; // Lower 13 bits for ID (0b00000000000000000001111111111111)
    constexpr Entity ACTIVE_FLAG_MASK = 1 << 13; // 1 bit at position 13 for active status
    constexpr Entity RENDERED_FLAG_MASK = 1 << 14; // 1 bit at position 14 for rendered status

    // Maximum entity count, given that 13 bits are used for ID
    constexpr Entity MAX_ENTITIES = 8191;
    using ComponentType = std::uint8_t;

    const ComponentType MAX_COMPONENT_TYPE = 32;

    /**
        * @class IComponentArray
        *
        * @brief Interface for a component array in the ECS framework.
        *
        * This class defines the interface for component arrays, which are used to store
        * components of entities in the ECS system. It provides the foundation for managing
        * the lifecycle of components associated with entities.
        */
    class IComponentArray {
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
    template<typename T>
    class ComponentArray final : public IComponentArray {
        public:
            /**
                * @brief Inserts a component for a specific entity.
                *
                * @param entity - The entity to which the component will be added.
                * @param component - The component to add.
                */
            void insertData(const Entity entity, T component)
            {
                assert(
                    m_entityToIndexMap.find(entity) == m_entityToIndexMap.end() &&
                    "Component added to same entity more than once.");

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
                assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Removing non-existent component.");

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
            T &getData(const Entity entity)
            {
                assert(
                    m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Retrieving non-existent component.");

                return m_componentArray[m_entityToIndexMap[entity]];
            }

            /**
                * @brief Cleans up components associated with a destroyed entity.
                *
                * @param entity - The destroyed entity.
                */
            void entityDestroyed(const Entity entity) override
            {
                if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
                    removeData(entity);
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
    class ComponentManager {
        public:
            /**
                * @brief Registers a new component type in the system.
                *
                * Each component type is associated with a unique ComponentType ID and a ComponentArray
                * to manage instances of the component.
                */
            template<typename T>
            void registerComponent()
            {
                std::type_index typeName(typeid(T));

                assert(
                    m_componentTypes.find(typeName) == m_componentTypes.end() &&
                    "Registering component type more than once.");

                m_componentTypes.insert({typeName, _nextComponentType});

                m_componentArrays.insert({typeName, std::make_shared<ComponentArray<T> >()});

                ++_nextComponentType;
            }

            /**
                * @brief Retrieves the ComponentType ID for a specific component type.
                *
                * @return ComponentType - The unique ID associated with the component type.
                */
            template<typename T>
            ComponentType getComponentType()
            {
                const std::type_index typeName(typeid(T));

                assert(
                    m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");

                return m_componentTypes[typeName];
            }

            /**
                * @brief Adds a component of a specific type to an entity.
                *
                * @param entity - The entity to which the component will be added.
                * @param component - The component to add to the entity.
                */
            template<typename T>
            void addComponent(Entity entity, T component)
            {
                getComponentArray<T>()->insertData(entity, component);
            }

            /**
                * @brief Removes a component of a specific type from an entity.
                *
                * @param entity - The entity from which the component will be removed.
                */
            template<typename T>
            void removeComponent(Entity entity)
            {
                getComponentArray<T>()->removeData(entity);
            }

            /**
                * @brief Retrieves a reference to a component of a specific type from an entity.
                *
                * @param entity - The entity whose component is to be retrieved.
                * @return T& - A reference to the requested component.
                */
            template<typename T>
            T &getComponent(Entity entity)
            {
                return getComponentArray<T>()->getData(entity);
            }

            /**
                * @brief Handles the destruction of an entity by ensuring all associated components are removed.
                *
                * @param entity - The entity that has been destroyed.
                */
            void entityDestroyed(Entity entity) const;

        private:
            std::unordered_map<std::type_index, ComponentType> m_componentTypes{};
            std::unordered_map<std::type_index, std::shared_ptr<IComponentArray> > m_componentArrays;

            ComponentType _nextComponentType{};

            /**
                * @brief Retrieves the ComponentArray associated with a specific component type.
                *
                * @return std::shared_ptr<ComponentArray<T>> - Shared pointer to the component array of the specified type.
                */
            template<typename T>
            std::shared_ptr<ComponentArray<T> > getComponentArray()
            {
                const std::type_index typeName(typeid(T));

                assert(
                    m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");

                return std::static_pointer_cast<ComponentArray<T> >(m_componentArrays[typeName]);
            }
    };
}
