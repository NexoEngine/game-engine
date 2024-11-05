/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Components.hpp
*/

#pragma once

#include <cstdint>
#include <array>
#include <iostream>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "Entity.hpp"
#include "game_engine/save/IOriginator.hpp"
#include "game_engine/save/ASerializableMemento.hpp"

#include "loguru/loguru.hpp"
#include <nlohmann/json.hpp>
#include <fstream>


namespace ecs {
    using Entity = std::uint32_t;

    const Entity MAX_ENTITIES = 5000;
    namespace components {
        using ComponentType = std::uint8_t;

        const ComponentType MAX_COMPONENT_TYPE = 32;

        template<typename T>
        struct ComponentArrayMemento : engine::save::ASerializableMemento {
            std::array<T, MAX_ENTITIES> componentArray;
            std::unordered_map<Entity, size_t> entityToIndexMap;
            std::unordered_map<size_t, Entity> indexToEntityMap;
            size_t size{};

			[[nodiscard]] engine::save::json serialize() const final {
				//static_assert(engine::save::JSONSerializable<T>, "T must be JSONSerializable");
                engine::save::json data = {
                    { "size", size }
                };

                for (const auto& [key, value] : entityToIndexMap) {
                    data["entityToIndexMap"][std::to_string(key)] = value;
                }

                for (const auto& [key, value] : indexToEntityMap) {
                    data["indexToEntityMap"][key] = value;
                }

                // TODO: remove this
                //for (size_t i = 0; i < size; i++) {
                //    data["componentArray"][i] = componentArray[i];
                //}
				return data;
			}

			void deserialize(const engine::save::json& data) final {
				data.at("size").get_to(size);

				for (const auto& [key, value] : data.at("entityToIndexMap").items()) {
					entityToIndexMap[std::stoul(key)] = value;
				}

				for (const auto& [key, value] : data.at("indexToEntityMap").items()) {
					indexToEntityMap[std::stoul(key)] = value;
				}

				/*for (size_t i = 0; i < size; i++) {
                    componentArray[i] = *data.at("componentArray").at(i).get<T>();
				}*/
			}
		};

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
        class ComponentArray : public IComponentArray, public engine::save::IOriginator<ComponentArrayMemento<T>> {
            public:
                /**
                * @brief Inserts a component for a specific entity.
                *
                * @param entity - The entity to which the component will be added.
                * @param component - The component to add.
                */
                void insertData(Entity entity, T component) {
                    // TODO: do we want to enable overriding?
                    //assert(_entityToIndexMap.find(entity) == _entityToIndexMap.end() && "Component added to same entity more than once.");
					if (_entityToIndexMap.find(entity) != _entityToIndexMap.end()) {
						_componentArray[_entityToIndexMap[entity]] = component;
						return;
					}
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

                void restoreMemento(const ComponentArrayMemento<T> &memento) override {
                    _componentArray = memento.componentArray;
                    _entityToIndexMap = memento.entityToIndexMap;
                    _indexToEntityMap = memento.indexToEntityMap;
                    _size = memento.size;
                }

                std::shared_ptr<ComponentArrayMemento<T>> saveMemento() const override {
                    auto memento = std::make_shared<ComponentArrayMemento<T>>();
                    memento->componentArray = _componentArray;
                    memento->entityToIndexMap = _entityToIndexMap;
                    memento->indexToEntityMap = _indexToEntityMap;
                    memento->size = _size;
                    return memento;
                }
            private:
                std::array<T, MAX_ENTITIES> _componentArray;

                std::unordered_map<Entity, size_t> _entityToIndexMap;

                std::unordered_map<size_t, Entity> _indexToEntityMap;

                size_t _size{};
        };

		struct ComponentManagerMemento : engine::save::ASerializableMemento {
			std::unordered_map<std::type_index, ComponentType> componentTypes;
			std::unordered_map<std::type_index, std::shared_ptr<ComponentArrayMemento<std::any>>> componentArrays;
			ComponentType nextComponentType{};

            [[nodiscard]] engine::save::json serialize() const final {
                engine::save::json data = {
                    { "nextComponentType", nextComponentType } 
                }; 

				for (const auto& [key, value] : componentTypes) {
					data["componentTypes"][key.name()] = value;
				}

				for (const auto& [key, value] : componentArrays) {
					data["componentArrays"][key.name()] = value->serialize();
				}
                return data;
            }
            void deserialize(const engine::save::json& data) final {


                //data.at("componentTypes").get_to(componentTypes); data.at("componentArrays").get_to(componentArrays); data.at("nextComponentType").get_to(nextComponentType);
            }
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
				* @brief Retrieves the ComponentType ID for a specific component type.
                * 
				* @param typeName - The type_index of the component type.
				* @return ComponentType - The unique ID associated with the component type.
                */
				ComponentType getComponentType(std::type_index typeName) {
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

				template <typename T>
                void restoreMemento(const T& memento) {
                    _componentTypes = memento.componentTypes;
                    _nextComponentType = memento.nextComponentType;
                }

				template <typename T>
				std::shared_ptr<T> saveMemento() const {
					auto memento = std::make_shared<ComponentManagerMemento>();
					memento->componentTypes = _componentTypes;
                    memento->nextComponentType = _nextComponentType;
                    for (const auto& [key, value] : _componentArrays) {
                        const auto castedValue = std::static_pointer_cast<ComponentArray<std::any>>(value);
						memento->componentArrays[key] = castedValue->saveMemento();
                    }
					return memento;
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