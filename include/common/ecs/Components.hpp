/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Components.hpp
*/

#pragma once

#include <cstdint>
#include <array>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "Entity.hpp"

namespace ecs {
    using Entity = std::uint32_t;

    const Entity MAX_ENTITIES = 5000;
    namespace components {
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
        class ComponentArray : public IComponentArray {
            public:
                /**
                * @brief Inserts a component for a specific entity.
                *
                * @param entity - The entity to which the component will be added.
                * @param component - The component to add.
                */
                void insertData(Entity entity, T component) {
                    assert(_entityToIndexMap.find(entity) == _entityToIndexMap.end() && "Component added to same entity more than once.");

                    size_t newIndex = _size;
                    _entityToIndexMap[entity] = newIndex;
                    _indexToEntityMap[newIndex] = entity;
                    _componentArray[newIndex] = component;
                    ++_size;
                }

                /**
                * @brief Removes a component from a specific entity.
                *
                * @param entity - The entity from which the component will be removed.
                */
                void removeData(Entity entity) {
                    assert(_entityToIndexMap.find(entity) != _entityToIndexMap.end() && "Removing non-existent component.");

                    size_t indexOfRemovedEntity = _entityToIndexMap[entity];
                    size_t indexOfLastElement = _size - 1;
                    _componentArray[indexOfRemovedEntity] = _componentArray[indexOfLastElement];

                    Entity entityOfLastElement = _indexToEntityMap[indexOfLastElement];
                    _entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
                    _indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

                    _entityToIndexMap.erase(entity);
                    _indexToEntityMap.erase(indexOfLastElement);

                    --_size;
                }

                /**
                * @brief Retrieves a reference to a component associated with a specific entity.
                *
                * @param entity - The entity whose component is to be retrieved.
                * @return T& - A reference to the requested component.
                */
                T &getData(Entity entity) {
                    assert(_entityToIndexMap.find(entity) != _entityToIndexMap.end() && "Retrieving non-existent component.");

                    return _componentArray[_entityToIndexMap[entity]];
                }
                /**
                * @brief Cleans up components associated with a destroyed entity.
                *
                * @param entity - The destroyed entity.
                */
                void entityDestroyed(Entity entity) override {
                    if (_entityToIndexMap.find(entity) != _entityToIndexMap.end())
                        removeData(entity);
                }
            private:
                std::array<T, MAX_ENTITIES> _componentArray;

                std::unordered_map<Entity, size_t> _entityToIndexMap;

                std::unordered_map<size_t, Entity> _indexToEntityMap;

                size_t _size;
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
                template <typename T>
                void registerComponent() {
                    std::type_index typeName(typeid(T));

                    assert(_componentTypes.find(typeName) == _componentTypes.end() && "Registering component type more than once.");

                    _componentTypes.insert({typeName, _nextComponentType});

                    _componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

                    ++_nextComponentType;
                }

                /**
                * @brief Retrieves the ComponentType ID for a specific component type.
                *
                * @return ComponentType - The unique ID associated with the component type.
                */
                template<typename T>
                ComponentType getComponentType() {
                    std::type_index typeName(typeid(T));

                    assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered before use.");

                    return _componentTypes[typeName];
                }

                /**
                * @brief Adds a component of a specific type to an entity.
                *
                * @param entity - The entity to which the component will be added.
                * @param component - The component to add to the entity.
                */
                template<typename T>
                void addComponent(Entity entity, T component) {
                    getComponentArray<T>()->insertData(entity, component);
                }

                /**
                * @brief Removes a component of a specific type from an entity.
                *
                * @param entity - The entity from which the component will be removed.
                */
                template<typename T>
                void removeComponent(Entity entity) {
                    getComponentArray<T>()->removeData(entity);
                }

                /**
                * @brief Retrieves a reference to a component of a specific type from an entity.
                *
                * @param entity - The entity whose component is to be retrieved.
                * @return T& - A reference to the requested component.
                */
                template<typename T>
                T &getComponent(Entity entity) {
                    return getComponentArray<T>()->getData(entity);
                }

                /**
                * @brief Handles the destruction of an entity by ensuring all associated components are removed.
                *
                * @param entity - The entity that has been destroyed.
                */
                void entityDestroyed(Entity entity) {
                    for (auto const &pair : _componentArrays) {
                        auto const &component = pair.second;
                        component->entityDestroyed(entity);
                    }
                }
            private:
                std::unordered_map<std::type_index, ComponentType> _componentTypes{};
                std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> _componentArrays;

                ComponentType _nextComponentType{};

                /**
                * @brief Retrieves the ComponentArray associated with a specific component type.
                *
                * @return std::shared_ptr<ComponentArray<T>> - Shared pointer to the component array of the specified type.
                */
                template<typename T>
                std::shared_ptr<ComponentArray<T>> getComponentArray() {
                    std::type_index typeName(typeid(T));

                    assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered before use.");

                    return std::static_pointer_cast<ComponentArray<T>>(_componentArrays[typeName]);

                }
        };
    }
}