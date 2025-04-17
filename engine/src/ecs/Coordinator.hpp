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
                m_hasComponentFunctions[typeid(T)] = [this](Entity entity) -> bool {
                    return this->entityHasComponent<T>(entity);
                };

                m_getComponentFunctions[typeid(T)] = [this](Entity entity) -> std::any {
                    return std::any(this->getComponent<T>(entity));
                };
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
                m_componentManager->addComponent<T>(entity, component, signature);

                m_entityManager->setSignature(entity, signature);

                m_systemManager->entitySignatureChanged(entity, oldSignature, signature);
            }

            /**
            * @brief Removes a component from an entity, updates its signature, and notifies systems.
            *
            * @param entity - The ID of the entity.
            */
            template<typename T>
            void removeComponent(const Entity entity) const
            {
                Signature signature = m_entityManager->getSignature(entity);
                const Signature oldSignature = signature;
                signature.set(m_componentManager->getComponentType<T>(), false);
                m_componentManager->removeComponent<T>(entity, oldSignature);


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
                if (m_componentManager->tryRemoveComponent<T>(entity, signature))
                {
                    Signature oldSignature = signature;
                    signature.set(m_componentManager->getComponentType<T>(), false);
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
            std::vector<std::type_index> getAllComponentTypes(Entity entity) const;

            /**
             * @brief Retrieves all components associated with an entity.
             *
             * @param entity The target entity identifier.
             * @return std::vector<std::pair<std::type_index, std::any>> A vector of pairs, each containing a component type and its instance.
             */
            std::vector<std::pair<std::type_index, std::any>> getAllComponents(Entity entity);

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
                    bool hasAllRequired = (entitySignature & requiredSignature) == requiredSignature;

                    // Entity must not have any excluded components
                    bool hasAnyExcluded = (entitySignature & excludeSignature).any();

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
            bool entityHasComponent(const Entity entity) const
            {
                const Signature signature = m_entityManager->getSignature(entity);
                const ComponentType componentType = m_componentManager->getComponentType<T>();
                return signature.test(componentType);
            }
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

            std::unordered_map<std::type_index, std::function<bool(Entity)>> m_hasComponentFunctions;
            std::unordered_map<std::type_index, std::function<std::any(Entity)>> m_getComponentFunctions;
    };
}
