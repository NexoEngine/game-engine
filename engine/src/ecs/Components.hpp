//// Components.hpp ///////////////////////////////////////////////////////////
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
//  Description: Header file for the component manager class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <array>
#include <cassert>
#include <memory>
#include <optional>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "ComponentArray.hpp"
#include "Definitions.hpp"
#include "ECSExceptions.hpp"
#include "Exception.hpp"
#include "GroupManager.hpp"
#include "Logger.hpp"

namespace nexo::ecs {

    /**
     * @class ComponentManager
     *
     * @brief Central manager for all component types and their arrays.
     *
     * The ComponentManager is responsible for:
     * - Registering component types in the ECS
     * - Creating and maintaining component arrays
     * - Adding/removing components from entities
     * - Managing component group registrations
     * - Handling entity destruction with respect to components
     *
     * ## Component Access API Tiers
     *
     * Component access is provided through three consistent tiers:
     *
     * | Tier          | Method                          | Returns                           | On failure              |
     * |---------------|---------------------------------|-----------------------------------|-------------------------|
     * | Safe typed    | `getComponent<T>(entity)`       | `T&`                              | throws ComponentNotFound|
     * | Try typed     | `tryGetComponent<T>(entity)`    | `optional<reference_wrapper<T>>`  | `nullopt`               |
     * | Raw/runtime   | `tryGetComponent(entity, typeID)` | `void*`                         | `nullptr`               |
     *
     * Use the **safe** tier when you know the entity has the component (e.g. inside a system
     * that filters on the component's signature). Use the **try** tier for optional access
     * when the component may or may not be present. Use the **raw** tier for runtime-typed
     * access (e.g. scripting, serialization).
     */
    class ComponentManager {
       public:
        ComponentManager() : m_groupManager(m_componentArrays) {}

        ComponentManager(const ComponentManager&)            = delete;
        ComponentManager& operator=(const ComponentManager&) = delete;
        ComponentManager(ComponentManager&&)                  = delete;
        ComponentManager& operator=(ComponentManager&&)       = delete;

        /**
         * @brief Registers a component type in the ECS
         *
         * Creates a new component array for the specified component type.
         * If the component type is already registered, a warning is logged.
         *
         * @tparam T The component type to register
         */
        template<typename T>
        void registerComponent()
        {
            const ComponentType typeID = getComponentTypeID<T>();

            assert(typeID < m_componentArrays.size() && "Component type ID exceeds component array size");
            if (m_componentArrays[typeID] != nullptr) {
                LOG(NEXO_WARN, "Component already registered");
                return;
            }

            m_componentArrays[typeID] = std::make_shared<ComponentArray<T>>();
        }

        /**
         * @brief Retrieves the component array for a specific component type
         *
         * @tparam T The component type
         * @return std::shared_ptr<ComponentArray<T>> Shared pointer to the component array
         * @throws ComponentNotRegistered if the component type is not registered
         */
        ComponentType registerComponent(const size_t componentSize, const size_t initialCapacity = 1024)
        {
            const ComponentType typeID = generateComponentTypeID();
            assert(typeID < m_componentArrays.size() && "Component type ID exceeds component array size");

            assert(m_componentArrays[typeID] == nullptr &&
                   "TypeErasedComponent already registered, should really not happen");
            m_componentArrays[typeID] = std::make_shared<TypeErasedComponentArray>(componentSize, initialCapacity);
            return typeID;
        }

        /**
         * @brief Gets the unique identifier for a component type
         *
         * @tparam T The component type
         * @return The component type ID
         * @throws ComponentNotRegistered if the component type is not registered
         */
        template<typename T>
        [[nodiscard]] ComponentType getComponentType() const
        {
            const ComponentType typeID = getComponentTypeID<T>();

            if (m_componentArrays[typeID] == nullptr) THROW_EXCEPTION(ComponentNotRegistered);

            return typeID;
        }

        /**
         * @brief Adds a component to an entity
         *
         * Adds the component to the appropriate component array and
         * updates any groups that match the entity's new signature.
         *
         * @tparam T The component type
         * @param entity The entity to add the component to
         * @param component The component instance to add
         * @param oldSignature The entity's current component signature
         * @param newSignature The entity's new component signature
         */
        template<typename T>
        void addComponent(Entity entity, T component, const Signature oldSignature, const Signature newSignature)
        {
            getComponentArray<T>()->insert(entity, std::move(component));
            m_groupManager.onComponentAdded(getComponentTypeID<T>(), oldSignature, newSignature, entity);
        }

        /**
         * @brief Adds a component to an entity using type ID and raw data
         *
         * Adds the component using the component type ID and raw data pointer,
         * useful for runtime component type handling. Updates any groups that
         * match the entity's new signature.
         *
         * @param entity The entity to add the component to
         * @param componentType The type ID of the component to add
         * @param componentData Pointer to the raw component data
         * @param oldSignature The entity's signature before adding the component
         * @param newSignature The entity's signature after adding the component
         *
         * @pre componentType must be a valid registered component type
         * @pre componentData must point to valid memory of the component's size
         */
        void addComponent(const Entity entity, const ComponentType componentType, const void* componentData,
                          const Signature oldSignature, const Signature newSignature)
        {
            getComponentArray(componentType)->insertRaw(entity, componentData);
            m_groupManager.onComponentAdded(componentType, oldSignature, newSignature, entity);
        }

        /**
         * @brief Adds a component to an entity using type ID and constructor
         *
         * Adds the component using the component type ID and a constructor function,
         * useful for runtime component type handling. Updates any groups that
         * match the entity's new signature.
         *
         * @param entity The entity to add the component to
         * @param componentType The type ID of the component to add
         * @param constructor Pointer to the constructor function
         * @param oldSignature The entity's signature before adding the component
         * @param newSignature The entity's signature after adding the component
         *
         * @pre componentType must be a valid-registered component type
         */
        void addComponentWithConstructor(const Entity entity, const ComponentType componentType,
                                         void (*constructor)(void* memoryDst), const Signature oldSignature,
                                         const Signature newSignature)
        {
            getComponentArray(componentType)->insertRawWithConstructor(entity, constructor);
            m_groupManager.onComponentAdded(componentType, oldSignature, newSignature, entity);
        }

        /**
         * @brief Removes a component from an entity using type ID
         *
         * Removes the component using the component type ID and updates any groups that
         * depend on the component.
         *
         * @param entity The entity to remove the component from
         * @param componentType The type ID of the component to remove
         * @param previousSignature The entity's signature before removal
         * @param newSignature The entity's signature after removal
         *
         * @pre componentType must be a valid registered component type
         */
        void removeComponent(const Entity entity, const ComponentType componentType, const Signature previousSignature,
                             const Signature newSignature)
        {
            m_groupManager.onComponentRemoved(componentType, previousSignature, newSignature, entity);
            getComponentArray(componentType)->remove(entity);
        }

        /**
         * @brief Removes a component from an entity
         *
         * Removes the entity from any groups that required the component
         * and then removes the component from its array.
         *
         * @tparam T The component type
         * @param entity The entity to remove the component from
         * @param previousSignature The entity's signature before removal
         * @param newSignature The entity's signature after removal
         */
        template<typename T>
        void removeComponent(Entity entity, const Signature previousSignature, const Signature newSignature)
        {
            m_groupManager.onComponentRemoved(getComponentTypeID<T>(), previousSignature, newSignature, entity);
            getComponentArray<T>()->remove(entity);
        }

        /**
         * @brief Attempts to remove a component from an entity
         *
         * Checks if the entity has the component first, then removes it
         * if it exists. Updates groups as needed.
         *
         * @tparam T The component type
         * @param entity The entity to remove the component from
         * @param previousSignature The entity's signature before the attempted removal
         * @param newSignature The entity's signature after the attempted removal
         * @return true if the component was removed, false if it didn't exist
         */
        template<typename T>
        bool tryRemoveComponent(Entity entity, const Signature previousSignature, const Signature newSignature)
        {
            auto componentArray = getComponentArray<T>();
            if (!componentArray->hasComponent(entity)) return false;

            m_groupManager.onComponentRemoved(getComponentTypeID<T>(), previousSignature, newSignature, entity);
            componentArray->remove(entity);
            return true;
        }

        /**
         * @brief Gets a component from an entity
         *
         * @tparam T The component type
         * @param entity The entity to get the component from
         * @return Reference to the component
         * @throws ComponentNotFound if the entity doesn't have the component
         */
        template<typename T>
        [[nodiscard]] T& getComponent(Entity entity)
        {
            return getComponentArray<T>()->get(entity);
        }

        /**
         * @brief Duplicates a component from one entity to another.
         *
         * This template function retrieves the component of type `T` from the source entity
         * and adds it to the destination entity. It also updates the entity's signature
         * to reflect the addition of the component.
         *
         * @tparam T The type of the component to duplicate.
         * @param sourceEntity The entity from which the component is duplicated.
         * @param destEntity The entity to which the component is added.
         * @param oldSignature The signature of the destination entity before the duplication.
         * @param newSignature The signature of the destination entity after the duplication.
         */
        template<typename T>
        void duplicateComponent(Entity sourceEntity, Entity destEntity, const Signature oldSignature,
                                const Signature newSignature)
        {
            const auto& componentArray = getComponentArray<T>();
            const auto sourceComponent = componentArray->get(sourceEntity);
            addComponent(destEntity, sourceComponent, oldSignature, newSignature);
        }

        /**
         * @brief Duplicates a component from one entity to another using a type ID.
         *
         * This function duplicates a component identified by its `ComponentType` from the
         * source entity to the destination entity. It also updates the destination entity's
         * signature and ensures that the entity is added to any relevant groups.
         *
         * @param componentType The type ID of the component to duplicate.
         * @param sourceEntity The entity from which the component is duplicated.
         * @param destEntity The entity to which the component is added.
         * @param oldSignature The signature of the destination entity before the duplication.
         * @param newSignature The signature of the destination entity after the duplication.
         */
        void duplicateComponent(const ComponentType componentType, const Entity sourceEntity, const Entity destEntity,
                                const Signature oldSignature, const Signature newSignature)
        {
            const auto& componentArray = m_componentArrays[componentType];
            componentArray->duplicateComponent(sourceEntity, destEntity);
            m_groupManager.onComponentAdded(componentType, oldSignature, newSignature, destEntity);
        }

        /**
         * @brief Gets the component array for a specific component type with ComponentType (const version)
         *
         * @param typeID The component type ID
         * @return Const shared pointer to the component array
         * @throws ComponentNotRegistered if the component type is not registered
         */
        [[nodiscard]] std::shared_ptr<IComponentArray> getComponentArray(const ComponentType typeID) const
        {
            const auto& componentArray = m_componentArrays[typeID];
            if (componentArray == nullptr) THROW_EXCEPTION(ComponentNotRegistered);

            return componentArray;
        }

        /**
         * @brief Gets the component array for a specific component type
         *
         * @tparam T The component type
         * @return Shared pointer to the component array
         * @throws ComponentNotRegistered if the component type is not registered
         */
        template<typename T>
        [[nodiscard]] std::shared_ptr<ComponentArray<T>> getComponentArray()
        {
            const ComponentType typeID = getComponentTypeID<T>();
            return std::static_pointer_cast<ComponentArray<T>>(getComponentArray(typeID));
        }

        /**
         * @brief Gets the component array for a specific component type (const version)
         *
         * @tparam T The component type
         * @return Const shared pointer to the component array
         * @throws ComponentNotRegistered if the component type is not registered
         */
        template<typename T>
        [[nodiscard]] std::shared_ptr<const ComponentArray<T>> getComponentArray() const
        {
            const ComponentType typeID = getComponentTypeID<T>();
            return std::static_pointer_cast<const ComponentArray<T>>(getComponentArray(typeID));
        }

        /**
         * @brief Safely attempts to get a component from an entity
         *
         * @tparam T The component type
         * @param entity The entity to get the component from
         * @return Optional reference to the component, or nullopt if not found
         */
        template<typename T>
        [[nodiscard]] std::optional<std::reference_wrapper<T>> tryGetComponent(Entity entity)
        {
            auto componentArray = getComponentArray<T>();
            if (!componentArray->hasComponent(entity)) return std::nullopt;

            return componentArray->get(entity);
        }

        /**
         * @brief Safely attempts to get a component from an entity
         *
         * @param entity The entity to get the component from
         * @param typeID The component type ID
         * @return Pointer to the component if it exists, or nullptr if not found
         */
        [[nodiscard]] void* tryGetComponent(const Entity entity, const ComponentType typeID) const
        {
            const auto componentArray = getComponentArray(typeID);
            if (!componentArray->hasComponent(entity)) return nullptr;

            return componentArray->getRawComponent(entity);
        }

        /**
         * @brief Notifies all component arrays that an entity has been destroyed
         *
         * Removes the entity from all component arrays it exists in.
         *
         * @param entity The destroyed entity
         * @param entitySignature Signature of the entity to be destroyed
         */
        void entityDestroyed(Entity entity, const Signature& entitySignature);

        /**
         * @brief Creates or retrieves a group for specific component combinations
         *
         * @tparam Owned Component types that are owned by the group
         * @param nonOwned A get_t<...> tag specifying non-owned component types
         * @return A shared pointer to the group (either existing or newly created)
         */
        template<typename... Owned>
        auto registerGroup(const auto& nonOwned)
        {
            return m_groupManager.registerGroup<Owned...>(nonOwned);
        }

        /**
         * @brief Retrieves an existing group for specific component combinations
         *
         * @tparam Owned Component types that are owned by the group
         * @param nonOwned A get_t<...> tag specifying non-owned component types
         * @return A shared pointer to the existing group
         */
        template<typename... Owned>
        auto getGroup(const auto& nonOwned)
        {
            return m_groupManager.getGroup<Owned...>(nonOwned);
        }

        /**
         * @brief Checks if two groups share any common owned components
         *
         * @param key1 First group key to compare
         * @param key2 Second group key to compare
         * @return true if the keys share at least one owned component, false otherwise
         */
        [[nodiscard]] static bool hasCommonOwnedComponents(const GroupKey& key1, const GroupKey& key2)
        {
            return GroupManager::hasCommonOwnedComponents(key1, key2);
        }

       private:
        /**
         * @brief Array of component arrays indexed by component type ID
         *
         * Provides O(1) lookup of component arrays by their type ID.
         * Declared before m_groupManager because GroupManager holds a reference to it.
         */
        std::array<std::shared_ptr<IComponentArray>, MAX_COMPONENT_TYPE> m_componentArrays{};

        /**
         * @brief Manages group registration, lookup, and entity membership tracking.
         *
         * Must be declared after m_componentArrays (initialization order dependency).
         */
        GroupManager m_groupManager;
    };
} // namespace nexo::ecs
