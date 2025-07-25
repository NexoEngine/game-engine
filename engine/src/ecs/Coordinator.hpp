//// Coordinator.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the coordinator class of the ecs
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <any>

#include "Components.hpp"
#include "System.hpp"
#include "SingletonComponent.hpp"
#include "Entity.hpp"
#include "Logger.hpp"
#include "TypeErasedComponent/ComponentDescription.hpp"

namespace nexo::ecs {

    template <typename T>
    struct Exclude {
        using type = T;
    };

    // Check if type is an Exclude specialization
    template <typename T>
    struct is_exclude : std::false_type {};

    template <typename T>
    struct is_exclude<Exclude<T>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_exclude_v = is_exclude<T>::value;

    // Extract the actual type from Exclude<T>
    template <typename T>
    struct extract_type {
        using type = T;
    };

    template <typename T>
    struct extract_type<Exclude<T>> {
        using type = T;
    };

    template <typename T>
    using extract_type_t = typename extract_type<T>::type;

    // Check if T has a nested Memento type
    template<typename T, typename = void>
    struct has_memento_type : std::false_type {};

    template<typename T>
    struct has_memento_type<T, std::void_t<typename T::Memento>> : std::true_type {};

    // Check if T has a save() method that returns Memento
    template<typename T, typename = void>
    struct has_save_method : std::false_type {};

    template<typename T>
    struct has_save_method<T,
        std::void_t<decltype(std::declval<const T&>().save())>>
        : std::is_same<decltype(std::declval<const T&>().save()), typename T::Memento> {};

    // Check if T::Memento has a restore() method that returns T
    template<typename T, typename = void>
    struct has_restore_method : std::false_type {};

    template<typename T>
    struct has_restore_method<T,
        std::void_t<decltype(std::declval<T&>().restore(std::declval<const typename T::Memento&>()))>>
        : std::is_same<decltype(std::declval<T&>().restore(std::declval<const typename T::Memento&>())), void> {};

    // Combined check for full memento pattern support
    template<typename T>
    struct supports_memento_pattern :
        std::conjunction<
            has_memento_type<T>,
            has_save_method<T>,
            has_restore_method<T>
        > {};

    template<typename T>
    inline constexpr bool has_memento_type_v = has_memento_type<T>::value;

    template<typename T>
    inline constexpr bool has_save_method_v = has_save_method<T>::value;

    template<typename T>
    inline constexpr bool has_restore_method_v = has_restore_method<T>::value;

    template<typename T>
    inline constexpr bool supports_memento_pattern_v = supports_memento_pattern<T>::value;


    /**
     * @class Coordinator
     *
     * @brief Central orchestrator for the Entity-Component-System (ECS) architecture.
     *
     * The Coordinator class ties together the functionalities of the EntityManager,
     * ComponentManager, and SystemManager to facilitate the creation, management,
     * and interaction of entities, components, and systems within the ECS framework.
     */
    class Coordinator {
        public:
            /**
            * @brief Initializes the Coordinator, creating instances of EntityManager,
            * ComponentManager, SystemManager, EventManager, SingletonComponentManager
            * and SceneManager.
            */
            void init();

            /**
            * @brief Creates a new entity.
            *
            * @return Entity - The newly created entity's ID.
            */
            Entity createEntity() const;

            /**
            * @brief Destroys an entity and cleans up its components and system references.
            *
            * @param entity - The ID of the entity to destroy.
            */
            void destroyEntity(Entity entity) const;

            /**
            * @brief Registers a new component type within the ComponentManager.
            */
            template <typename T>
            void registerComponent()
            {
                m_componentManager->registerComponent<T>();

                m_getComponentFunctions[typeid(T)] = [this](const Entity entity) -> std::any {
                    return this->getComponent<T>(entity);
                };

                m_getComponentPointers[typeid(T)] = [this](const Entity entity) -> std::any {
                    auto opt = this->tryGetComponent<T>(entity);
                    if (!opt.has_value())
                        return {};
                    T* ptr = &opt.value().get();
                    return std::any(static_cast<void*>(ptr));
                };
                m_typeIDtoTypeIndex.emplace(getComponentType<T>(), typeid(T));

                m_addComponentFunctions[typeid(T)] = [this](const Entity entity, const std::any& componentAny) {
                    T component = std::any_cast<T>(componentAny);
                    this->addComponent<T>(entity, component);
                };

                if constexpr (supports_memento_pattern_v<T>) {
                    m_supportsMementoPattern.emplace(typeid(T), true);

                    m_saveComponentFunctions[typeid(T)] = [](const std::any& componentAny) -> std::any {
                        const T& component = std::any_cast<const T&>(componentAny);
                        return std::any(component.save());
                    };

                    m_restoreComponentFunctions[typeid(T)] = [](const std::any& mementoAny) -> std::any {
                        const typename T::Memento& memento = std::any_cast<const typename T::Memento&>(mementoAny);
                        T component{};
                        component.restore(memento);
                        return std::any(std::move(component));
                    };
                } else {
                    m_supportsMementoPattern.emplace(typeid(T), false);
                }
            }

            void addComponentDescription(const ComponentType componentType, const ComponentDescription& description)
            {
                m_componentDescriptions[componentType] = std::make_shared<ComponentDescription>(description);
            }

            ComponentType registerComponent(const size_t componentSize, const size_t initialCapacity = 1024)
            {
                const auto typeID = m_componentManager->registerComponent(componentSize, initialCapacity);
                return typeID;
            }

            /**
             * @brief Registers a new singleton component
             *
             * @tparam T Class that should inherit from SingletonComponent class
             * @param args Optional argument to forward to the singleton component constructor
             */
            template <typename T, typename... Args>
            void registerSingletonComponent(Args&&... args)
            {
                m_singletonComponentManager->registerSingletonComponent<T>(std::forward<Args>(args)...);
            }

            /**
            * @brief Adds a component to an entity, updates its signature, and notifies systems.
            *
            * @param entity - The ID of the entity.
            * @param component - The component to add to the entity.
            */
            template <typename T>
            void addComponent(const Entity entity, T component)
            {
                Signature signature = m_entityManager->getSignature(entity);
                const Signature oldSignature = signature;
                signature.set(m_componentManager->getComponentType<T>(), true);
                m_componentManager->addComponent<T>(entity, component, oldSignature, signature);

                m_entityManager->setSignature(entity, signature);

                m_systemManager->entitySignatureChanged(entity, oldSignature, signature);
            }

            /**
             * @brief Adds a component to an entity, updates its signature, and notifies systems.
             *
             * This function allows adding a component by its type ID and raw data pointer.
             *
             * @param entity The ID of the entity to which the component will be added.
             * @param componentType The type ID of the component to be added.
             * @param componentData Pointer to the raw component data.
             *
             * @pre componentType must be a valid registered component type.
             * @pre componentData must point to valid memory of the component's size.
             */
            void addComponent(const Entity entity, const ComponentType componentType, const void *componentData) const
            {
                Signature signature = m_entityManager->getSignature(entity);
                const Signature oldSignature = signature;
                signature.set(componentType, true);
                m_componentManager->addComponent(entity, componentType, componentData, oldSignature, signature);

                m_entityManager->setSignature(entity, signature);

                m_systemManager->entitySignatureChanged(entity, oldSignature, signature);
            }

            /**
             * @brief Removes a component from an entity using ComponentType, updates its signature, and notifies systems.
             *
             * @param entity - The ID of the entity.
             * @param componentType - The ID of the component type to remove.
             */
            void removeComponent(const Entity entity, const ComponentType componentType)
            {
                Signature signature = m_entityManager->getSignature(entity);
                const Signature oldSignature = signature;

                signature.set(componentType, false);

                m_componentManager->removeComponent(entity, componentType, oldSignature, signature);

                m_entityManager->setSignature(entity, signature);
                m_systemManager->entitySignatureChanged(entity, oldSignature, signature);
            }


            /**
            * @brief Removes a component from an entity, updates its signature, and notifies systems.
            *
            * This is the runtime equivalent of removeComponent<T>(), useful when the component type
            * is only known at runtime (e.g., from scripting APIs).
            *
            * @param entity - The ID of the entity.
            */
            template<typename T>
            void removeComponent(const Entity entity) const
            {
                Signature signature = m_entityManager->getSignature(entity);
                const Signature oldSignature = signature;
                signature.set(m_componentManager->getComponentType<T>(), false);
                m_componentManager->removeComponent<T>(entity, oldSignature, signature);


                m_entityManager->setSignature(entity, signature);

                m_systemManager->entitySignatureChanged(entity, oldSignature, signature);
            }

            /**
             * @brief Attempts to remove a component from an entity.
             *
             * If the component exists, it is removed and the entity's signature is updated.
             *
             * @tparam T The component type.
             * @param entity The target entity identifier.
             */
            template<typename T>
            void tryRemoveComponent(const Entity entity) const
            {
                Signature signature = m_entityManager->getSignature(entity);
                Signature oldSignature = signature;
                signature.set(m_componentManager->getComponentType<T>(), false);
                if (m_componentManager->tryRemoveComponent<T>(entity, oldSignature, signature))
                {
                    m_entityManager->setSignature(entity, signature);

                    m_systemManager->entitySignatureChanged(entity, oldSignature, signature);
                }
            }

            /**
             * @brief Remove a singleton component
             *
             * @tparam T Class that should inherit from the SingletonComponent class
             */
            template <typename T>
            void removeSingletonComponent() const
            {
                m_singletonComponentManager->unregisterSingletonComponent<T>();
            }

            /**
            * @brief Retrieves a reference to a component of an entity.
            *
            * @param entity - The ID of the entity.
            * @return T& - Reference to the requested component.
            */
            template <typename T>
            T &getComponent(const Entity entity)
            {
                return m_componentManager->getComponent<T>(entity);
            }

            /**
             * @brief Retrieves the component array for a specific component type
             *
             * @tparam T The component type
             * @return std::shared_ptr<ComponentArray<T>> Shared pointer to the component array
             */
            template <typename T>
            std::shared_ptr<ComponentArray<T>> getComponentArray()
            {
                return m_componentManager->getComponentArray<T>();
            }

            /**
             * @brief Attempts to retrieve a component from an entity.
             *
             * @tparam T The component type.
             * @param entity The target entity identifier.
             * @return std::optional<std::reference_wrapper<T>> A reference to the component if it exists.
             */
            template<typename T>
            std::optional<std::reference_wrapper<T>> tryGetComponent(const Entity entity)
            {
                return m_componentManager->tryGetComponent<T>(entity);
            }

            void *tryGetComponentById(const ComponentType componentType, const Entity entity)const
            {
                return m_componentManager->tryGetComponent(entity, componentType);
            }

            const std::unordered_map<ComponentType, std::type_index>& getTypeIdToTypeIndex() const {
                return m_typeIDtoTypeIndex;
            }

            const std::unordered_map<std::type_index, std::function<void(Entity, const std::any&)>>& getAddComponentFunctions() const {
                return m_addComponentFunctions;
            }

            Signature getSignature(const Entity entity) const {
                return m_entityManager->getSignature(entity);
            }

            /**
             * @brief Get the Singleton Component object
             *
             * @tparam T Class that should inherit from the SingletonComponent class
             * @return T& The instance of the desired singleton component
             */
            template <typename T>
            T &getSingletonComponent()
            {
                return m_singletonComponentManager->getSingletonComponent<T>();
            }

            /**
             * @brief Get the Raw Singleton Component object
             *
             * @tparam T Class that should inherit from the SingletonComponent class
             * @return std::shared_ptr<ISingletonComponent> The pointer to the desired singleton component
             */
            template <typename T>
            std::shared_ptr<ISingletonComponent> getRawSingletonComponent() const
            {
                return m_singletonComponentManager->getRawSingletonComponent<T>();
            }

            /**
             * @brief Retrieves all component types associated with an entity.
             *
             * @param entity The target entity identifier.
             * @return std::vector<std::type_index> A list of type indices for each component the entity has.
             */
            std::vector<ComponentType> getAllComponentTypes(Entity entity) const;

            std::vector<std::type_index> getAllComponentTypeIndices(Entity entity) const;

            /**
             * @brief Retrieves all components associated with an entity.
             *
             * @param entity The target entity identifier.
             * @return std::vector<std::pair<std::type_index, std::any>> A vector of pairs, each containing a component type and its instance.
             */
            std::vector<std::any> getAllComponents(Entity entity);

            /**
            * @brief Retrieves all entities that have the specified components.
            *
            * @tparam Components The component types to filter by.
            * @return std::set<Entity> A set of entities that contain all the specified components.
            */
            template<typename... Components>
            std::vector<Entity> getAllEntitiesWith() const
            {
                // Prepare signatures
                Signature requiredSignature;
                Signature excludeSignature;

                // Process each component type
                (processComponentSignature<Components>(requiredSignature, excludeSignature), ...);

                // Query entities
                std::span<const Entity> livingEntities = m_entityManager->getLivingEntities();
                std::vector<Entity> result;
                result.reserve(livingEntities.size());

                for (Entity entity : livingEntities)
                {
                    const Signature entitySignature = m_entityManager->getSignature(entity);

                    // Entity must have all required components
                    const bool hasAllRequired = (entitySignature & requiredSignature) == requiredSignature;

                    // Entity must not have any excluded components
                    const bool hasAnyExcluded = (entitySignature & excludeSignature).any();

                    if (hasAllRequired && !hasAnyExcluded)
                        result.push_back(entity);
                }

                return result;
            }

            /**
            * @brief Gets the component type ID for a specific component type.
            *
            * @return components::ComponentType - The ID of the component type.
            */
            template<typename T>
            ComponentType getComponentType() const
            {
                return m_componentManager->getComponentType<T>();
            }

            /**
             * @brief Retrieves all registered component descriptions.
             *
             * @return const std::unordered_map<ComponentType, std::shared_ptr<ComponentDescription>>&
             *         A const reference to the map of component types to their descriptions.
             */
            const std::unordered_map<ComponentType, std::shared_ptr<ComponentDescription>>& getComponentDescriptions() const
            {
                return m_componentDescriptions;
            }

            /**
            * @brief Registers a new query system
            *
            * @tparam T The system type to register
            * @tparam Args Additional constructor arguments
            * @return std::shared_ptr<T> Shared pointer to the registered system
            */
            template <typename T, typename... Args>
            std::shared_ptr<T> registerQuerySystem(Args&&... args) {
                auto newQuerySystem =  m_systemManager->registerQuerySystem<T>(std::forward<Args>(args)...);
                std::span<const Entity> livingEntities = m_entityManager->getLivingEntities();
                const Signature querySystemSignature = newQuerySystem->getSignature();
                for (Entity entity : livingEntities) {
                    const Signature entitySignature = m_entityManager->getSignature(entity);
                    if ((entitySignature & querySystemSignature) == querySystemSignature) {
                        newQuerySystem->entities.insert(entity);
                    }
                }
                return newQuerySystem;
            }

            /**
            * @brief Registers a new group system
            *
            * @tparam T The system type to register
            * @tparam Args Additional constructor arguments
            * @return std::shared_ptr<T> Shared pointer to the registered system
            */
            template <typename T, typename... Args>
            std::shared_ptr<T> registerGroupSystem(Args&&... args) {
                return m_systemManager->registerGroupSystem<T>(std::forward<Args>(args)...);
            }

            /**
             * @brief Creates or retrieves a group for specific component combinations
             *
             * @tparam Owned Component types that are owned by the group
             * @param nonOwned A get_t<...> tag specifying non-owned component types
             * @return A shared pointer to the group (either existing or newly created)
             */
            template<typename... Owned>
		    auto registerGroup(const auto & nonOwned)
			{
				return m_componentManager->registerGroup<Owned...>(nonOwned);
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
				return m_componentManager->getGroup<Owned...>(nonOwned);
			}

            /**
            * @brief Sets the signature for a system, defining which entities it will process.
            *
            * @param signature - The signature to associate with the system.
            */
            template <typename T>
            void setSystemSignature(const Signature signature) const
            {
                m_systemManager->setSignature<T>(signature);
            }

            /**
             * @brief Checks whether an entity has a specific component.
             *
             * @tparam T The component type.
             * @param entity The target entity.
             * @return true If the entity has the component.
             * @return false Otherwise.
             */
            template<typename T>
            [[nodiscard]] bool entityHasComponent(const Entity entity) const
            {
                const ComponentType componentType = m_componentManager->getComponentType<T>();
                return entityHasComponent(entity, componentType);
            }

            /**
             * @brief Checks whether an entity has a specific component by its type ID.
             *
             * @param entity The target entity.
             * @param componentType The type ID of the component.
             * @return true If the entity has the component.
             * @return false Otherwise.
             */
            [[nodiscard]] bool entityHasComponent(const Entity entity, const ComponentType componentType) const
            {
                const Signature signature = m_entityManager->getSignature(entity);
                return signature.test(componentType);
            }

            template<typename T>
            void setRestoreComponent() {
                m_restoreComponentFunctions[typeid(T)] = [](const std::any&) -> std::any {
                    return std::any(T{});
                };
            }


            bool supportsMementoPattern(const std::any& component) const;
            std::any saveComponent(const std::any& component) const;
            std::any restoreComponent(const std::any& memento, const std::type_index& componentType) const;
            void addComponentAny(Entity entity, const std::type_index& typeIndex, const std::any& component);

            Entity duplicateEntity(Entity sourceEntity) const;

            /**
            * @brief Retrieves all entities that have all the specified components.
            *
            * This method iterates over all entities and returns a list of those that possess
            * each of the given component types. It uses the current signature of each entity
            * to determine component ownership.
            *
            * @tparam ComponentTypes - A variadic list of component types to filter by.
            * @return std::vector<Entity> - A list of entities matching all specified component types.
            */
            template<typename... ComponentTypes>
            std::vector<Entity> getEntitiesWithComponents() const
            {
                std::vector<Entity> result;
                std::span<const Entity> livingEntities = m_entityManager->getLivingEntities();
                result.reserve(livingEntities.size());
                for (Entity entity : livingEntities)
                {
                    const bool hasAll = (entityHasComponent<ComponentTypes>(entity) && ...);
                    if (hasAll)
                    {
                        result.push_back(entity);
                    }
                }
                return result;
            }

        void updateSystemEntities() const;

        private:
            template<typename Component>
            void processComponentSignature(Signature& required, Signature& excluded) const {
                if constexpr (is_exclude_v<Component>) {
                    // This is an excluded component
                    using ActualType = typename Component::type;
                    excluded.set(m_componentManager->getComponentType<ActualType>(), true);
                } else {
                    // This is a required component
                    required.set(m_componentManager->getComponentType<Component>(), true);
                }
            }

            std::shared_ptr<ComponentManager> m_componentManager;
            std::shared_ptr<EntityManager> m_entityManager;
            std::shared_ptr<SystemManager> m_systemManager;
            std::shared_ptr<SingletonComponentManager> m_singletonComponentManager;

            std::unordered_map<ComponentType, std::type_index> m_typeIDtoTypeIndex;
            std::unordered_map<std::type_index, bool> m_supportsMementoPattern;
            std::unordered_map<std::type_index, std::function<std::any(const std::any&)>> m_saveComponentFunctions;
            std::unordered_map<std::type_index, std::function<std::any(const std::any&)>> m_restoreComponentFunctions;
            std::unordered_map<std::type_index, std::function<void(Entity, const std::any&)>> m_addComponentFunctions;
            std::unordered_map<std::type_index, std::function<std::any(Entity)>> m_getComponentFunctions;
            std::unordered_map<std::type_index, std::function<std::any(Entity)>> m_getComponentPointers;

            std::unordered_map<ComponentType, std::shared_ptr<ComponentDescription>> m_componentDescriptions;
    };
}
