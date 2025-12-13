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

#include <any>
#include <memory>

#include "Components.hpp"
#include "Entity.hpp"
#include "Json.hpp"
#include "Logger.hpp"
#include "SingletonComponent.hpp"
#include "String.hpp"
#include "System.hpp"
#include "TypeErasedComponent/ComponentDescription.hpp"
#include "save/Serialization.hpp"
#include "save/SerializationConcepts.hpp"
#include "save/SerializationContext.hpp"

namespace nexo::ecs {

    template<typename T>
    struct Exclude {
        using type = T;
    };

    // Check if type is an Exclude specialization
    template<typename T>
    struct is_exclude : std::false_type {};

    template<typename T>
    struct is_exclude<Exclude<T>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_exclude_v = is_exclude<T>::value;

    // Extract the actual type from Exclude<T>
    template<typename T>
    struct extract_type {
        using type = T;
    };

    template<typename T>
    struct extract_type<Exclude<T>> {
        using type = T;
    };

    template<typename T>
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
    struct has_save_method<T, std::void_t<decltype(std::declval<const T&>().save())>>
        : std::is_same<decltype(std::declval<const T&>().save()), typename T::Memento> {};

    // Check if T::Memento has a restore() method that returns T
    template<typename T, typename = void>
    struct has_restore_method : std::false_type {};

    template<typename T>
    struct has_restore_method<
        T, std::void_t<decltype(std::declval<T&>().restore(std::declval<const typename T::Memento&>()))>>
        : std::is_same<decltype(std::declval<T&>().restore(std::declval<const typename T::Memento&>())), void> {};

    // Combined check for full memento pattern support
    template<typename T>
    struct supports_memento_pattern : std::conjunction<has_memento_type<T>, has_save_method<T>, has_restore_method<T>> {
    };

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
        [[nodiscard]] Entity createEntity() const;

        /**
         * @brief Destroys an entity and cleans up its components and system references.
         *
         * @param entity - The ID of the entity to destroy.
         */
        void destroyEntity(Entity entity) const;

        /**
         * @brief Registers a new component type within the ComponentManager.
         */
        template<typename T>
        void registerComponent(const std::string& displayName = type_name<T>())
        {
            m_componentManager->registerComponent<T>();
            addComponentDescription(getComponentType<T>(),
                                    ComponentDescription{displayName, {}, true, [](void* memoryDst) { new (memoryDst) T{}; }});

            m_getComponentFunctions[typeid(T)] = [this](const Entity entity) -> std::any {
                return this->getComponent<T>(entity);
            };

            m_getComponentPointers[typeid(T)] = [this](const Entity entity) -> std::any {
                auto opt = this->tryGetComponent<T>(entity);
                if (!opt.has_value()) return {};
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

            if constexpr (JSONSerializable<T>) {
                m_serializeComponentArrayFunctions[typeid(T)] =
                    [this](json& j, const save::SerializationContext& ctx = save::SerializationContext{}) {
                        auto componentArray = this->getComponentArray<T>();
                        if (componentArray == nullptr) return;

                        save::serialize(j, *componentArray, ctx);
                };
            }
        }

        /**
         * @brief Adds a description for a component type.
         *
         * This description can include metadata such as the component's name,
         * size, default values, serialization info, etc.
         *
         * @param componentType The type ID of the component.
         * @param description The ComponentDescription object containing metadata.
         */
        void addComponentDescription(const ComponentType componentType, const ComponentDescription& description)
        {
            m_componentDescriptions[componentType] = std::make_shared<ComponentDescription>(description);
        }

        /**
         * @brief Registers a new component type with the specified size and initial capacity.
         *
         * This function allows the registration of a new component type in the ECS framework.
         * It delegates the registration process to the ComponentManager and returns the
         * unique ComponentType identifier for the registered component.
         *
         * @param componentSize The size of the component in bytes.
         * @param initialCapacity The initial capacity for the component storage (default is 1024).
         * @return ComponentType The unique identifier for the registered component type.
         */
        [[nodiscard]] ComponentType registerComponent(const size_t componentSize,
                                                      const size_t initialCapacity = 1024) const
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
        template<typename T, typename... Args>
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
        template<typename T>
        void addComponent(const Entity entity, T component)
        {
            Signature signature          = m_entityManager->getSignature(entity);
            const Signature oldSignature = signature;
            signature.set(m_componentManager->getComponentType<T>(), true);
            m_componentManager->addComponent<T>(entity, component, oldSignature, signature);

            m_entityManager->setSignature(entity, signature);

            m_systemManager->entitySignatureChanged(entity, oldSignature, signature);
        }

        template<typename T>
        void addComponent(const Entity entity) const
        {
            addComponent(entity, T{});
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
        void addComponent(const Entity entity, const ComponentType componentType, const void* componentData) const
        {
            Signature signature          = m_entityManager->getSignature(entity);
            const Signature oldSignature = signature;
            signature.set(componentType, true);
            m_componentManager->addComponent(entity, componentType, componentData, oldSignature, signature);

            m_entityManager->setSignature(entity, signature);

            m_systemManager->entitySignatureChanged(entity, oldSignature, signature);
        }

        /**
         * @brief Adds a component to an entity using ComponentType, default-constructs it,
         * updates its signature, and notifies systems.
         *
         * @param entity - The ID of the entity.
         * @param componentType - The ID of the component type to add.
         */
        void addComponentWithDefault(const Entity entity, const ComponentType componentType) const
        {
            Signature signature          = m_entityManager->getSignature(entity);
            const Signature oldSignature = signature;
            signature.set(componentType, true);
            const auto& componentDescription = m_componentDescriptions.at(componentType);
            if (componentDescription == nullptr) {
                THROW_EXCEPTION(ComponentNotRegistered);
            }
            m_componentManager->addComponentWithConstructor(entity, componentType, componentDescription->constructor, oldSignature,
                                                            signature);

            m_entityManager->setSignature(entity, signature);

            m_systemManager->entitySignatureChanged(entity, oldSignature, signature);
        }

        /**
         * @brief Removes a component from an entity using ComponentType, updates its signature, and notifies systems.
         *
         * @param entity - The ID of the entity.
         * @param componentType - The ID of the component type to remove.
         */
        void removeComponent(const Entity entity, const ComponentType componentType) const
        {
            Signature signature          = m_entityManager->getSignature(entity);
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
            Signature signature          = m_entityManager->getSignature(entity);
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
            Signature signature    = m_entityManager->getSignature(entity);
            Signature oldSignature = signature;
            signature.set(m_componentManager->getComponentType<T>(), false);
            if (m_componentManager->tryRemoveComponent<T>(entity, oldSignature, signature)) {
                m_entityManager->setSignature(entity, signature);

                m_systemManager->entitySignatureChanged(entity, oldSignature, signature);
            }
        }

        /**
         * @brief Remove a singleton component
         *
         * @tparam T Class that should inherit from the SingletonComponent class
         */
        template<typename T>
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
        template<typename T>
        T& getComponent(const Entity entity)
        {
            return m_componentManager->getComponent<T>(entity);
        }

        /**
         * @brief Retrieves the component array for a specific component type
         *
         * @tparam T The component type
         * @return std::shared_ptr<ComponentArray<T>> Shared pointer to the component array
         */
        template<typename T>
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
         * @brief Attempts to retrieve a component from an entity by its ComponentType.
         *
         * @param componentType The type ID of the component.
         * @param entity The target entity identifier.
         * @return void* Pointer to the component if it exists, or nullptr if not found.
         */
        [[nodiscard]] void* tryGetComponentById(const ComponentType componentType, const Entity entity) const
        {
            return m_componentManager->tryGetComponent(entity, componentType);
        }

        /**
         * @brief Retrieves the mapping between component types and their corresponding type indices.
         *
         * This function provides access to the internal mapping that associates each
         * registered component type with its corresponding `std::type_index`.
         *
         * @return const std::unordered_map<ComponentType, std::type_index>&
         *         A constant reference to the mapping of component types to type indices.
         */
        [[nodiscard]] const std::unordered_map<ComponentType, std::type_index>& getTypeIdToTypeIndex() const
        {
            return m_typeIDtoTypeIndex;
        }

        /**
         * @brief Retrieves the mapping of functions used to add components to entities.
         *
         * This function provides access to the internal mapping that associates each
         * component type with its corresponding function for adding components to entities.
         *
         * @return const std::unordered_map<std::type_index, std::function<void(Entity, const std::any&)>>&
         *         A constant reference to the mapping of component addition functions.
         */
        [[nodiscard]] const std::unordered_map<std::type_index, std::function<void(Entity, const std::any&)>>&
        getAddComponentFunctions() const
        {
            return m_addComponentFunctions;
        }

        /**
         * @brief Retrieves the signature of a specific entity.
         *
         * This function returns the signature associated with the given entity, which
         * represents the set of components currently assigned to the entity.
         *
         * @param entity The ID of the entity whose signature is to be retrieved.
         * @return Signature The signature of the specified entity.
         */
        [[nodiscard]] Signature getSignature(const Entity entity) const
        {
            return m_entityManager->getSignature(entity);
        }

        /**
         * @brief Get the Singleton Component object
         *
         * @tparam T Class that should inherit from the SingletonComponent class
         * @return T& The instance of the desired singleton component
         */
        template<typename T>
        T& getSingletonComponent()
        {
            return m_singletonComponentManager->getSingletonComponent<T>();
        }

        /**
         * @brief Get the Raw Singleton Component object
         *
         * @tparam T Class that should inherit from the SingletonComponent class
         * @return std::shared_ptr<ISingletonComponent> The pointer to the desired singleton component
         */
        template<typename T>
        [[nodiscard]] std::shared_ptr<ISingletonComponent> getRawSingletonComponent() const
        {
            return m_singletonComponentManager->getRawSingletonComponent<T>();
        }

        /**
         * @brief Retrieves all component types associated with an entity.
         *
         * @param entity The target entity identifier.
         * @return std::vector<std::type_index> A list of type indices for each component the entity has.
         */
        [[nodiscard]] std::vector<ComponentType> getAllComponentTypes(Entity entity) const;

        [[nodiscard]] std::vector<std::type_index> getAllComponentTypeIndices(Entity entity) const;

        /**
         * @brief Retrieves all components associated with an entity.
         *
         * @param entity The target entity identifier.
         * @return std::vector<std::pair<std::type_index, std::any>> A vector of pairs, each containing a component type
         * and its instance.
         */
        std::vector<std::any> getAllComponents(Entity entity);

        /**
         * @brief Retrieves all entities that have the specified components.
         *
         * @tparam Components The component types to filter by.
         * @return std::set<Entity> A set of entities that contain all the specified components.
         */
         template<typename... Components>
         [[nodiscard]] std::vector<Entity> getAllEntitiesWith() const
         {
             Signature requiredSignature;
             Signature excludeSignature;
             (processComponentSignature<Components>(requiredSignature, excludeSignature), ...);

             std::span<const Entity> baseSpan = m_entityManager->getLivingEntities();
             std::size_t minSize              = baseSpan.size();

             auto considerRequiredArray = [&](auto tag) {
                 using C = decltype(tag);

                 if constexpr (!is_exclude_v<C>) {
                     using Actual = C;

                     auto compArray = m_componentManager->getComponentArray<Actual>();
                     const auto span = compArray->entities();

                     if (span.size() < minSize) {
                         minSize  = span.size();
                         baseSpan = span;
                     }
                 }
             };

             (considerRequiredArray(Components{}), ...);

             std::vector<Entity> result;
             result.reserve(minSize);

             for (Entity entity : baseSpan) {
                 const Signature entitySignature = m_entityManager->getSignature(entity);

                 const bool hasAllRequired = (entitySignature & requiredSignature) == requiredSignature;
                 const bool hasAnyExcluded = (entitySignature & excludeSignature).any();

                 if (hasAllRequired && !hasAnyExcluded) {
                     result.push_back(entity);
                 }
             }

             return result;
         }

        /**
         * @brief Gets the component type ID for a specific component type.
         *
         * @return components::ComponentType - The ID of the component type.
         */
        template<typename T>
        [[nodiscard]] ComponentType getComponentType() const
        {
            return m_componentManager->getComponentType<T>();
        }

        /**
         * @brief Retrieves all registered component descriptions.
         *
         * @return const std::unordered_map<ComponentType, std::shared_ptr<ComponentDescription>>&
         * A const reference to the map of component types to their descriptions.
         */
        [[nodiscard]] const std::unordered_map<ComponentType, std::shared_ptr<ComponentDescription>>&
        getComponentDescriptions() const
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
        template<typename T, typename... Args>
        std::shared_ptr<T> registerQuerySystem(Args&&... args)
        {
            auto newQuerySystem = m_systemManager->registerQuerySystem<T>(std::forward<Args>(args)...);
            const std::span<const Entity> livingEntities = m_entityManager->getLivingEntities();
            const Signature querySystemSignature         = newQuerySystem->getSignature();
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
        template<typename T, typename... Args>
        std::shared_ptr<T> registerGroupSystem(Args&&... args)
        {
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
        auto registerGroup(const auto& nonOwned)
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
        template<typename T>
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

        /**
         * @brief Sets a default save function for a component type that does not support the Memento pattern.
         *
         * This function registers a default save function for the specified component type T.
         * The default function simply returns a default-constructed instance of T wrapped in a std::any.
         * This is useful for component types that do not implement the Memento pattern but still need
         * to be handled in a uniform way.
         *
         * @tparam T The component type for which to set the default save function.
         */
        template<typename T>
        void setRestoreComponent()
        {
            m_restoreComponentFunctions[typeid(T)] = [](const std::any&) -> std::any { return std::any(T{}); };
        }

        /**
         * @brief Checks if a component type supports the Memento pattern.
         *
         * This method checks if the given component type has the necessary methods
         * to support saving and restoring its state using the Memento design pattern.
         *
         * @tparam T The component type to check.
         * @return true if the component type supports the Memento pattern, false otherwise.
         */
        [[nodiscard]] bool supportsMementoPattern(const std::any& component) const;

        /**
         * @brief Saves the state of a component using the Memento pattern.
         *
         * This method invokes the save function of the component to capture its current state
         * and returns it as a std::any object. The component must support the Memento pattern.
         *
         * @param component The component instance to save.
         * @return std::any containing the saved state (memento) of the component.
         * @throws std::bad_any_cast if the component does not support the Memento pattern.
         */
        [[nodiscard]] std::any saveComponent(const std::any& component) const;

        /**
         * @brief Restores a component's state from a memento.
         *
         * This method takes a memento object and the type of the component to restore.
         * It reconstructs the component's state from the memento and returns the restored
         * component as a std::any object. The component type must support the Memento pattern.
         *
         * @param memento The memento object containing the saved state.
         * @param componentType The type index of the component to restore.
         * @return std::any containing the restored component instance.
         * @throws std::bad_any_cast if the component type does not support the Memento pattern
         *         or if the memento is of an incompatible type.
         */
        [[nodiscard]] std::any restoreComponent(const std::any& memento, const std::type_index& componentType) const;

        /**
         * @brief Adds a component to an entity using type erasure.
         *
         * This method allows adding a component to an entity when the component type
         * is only known at runtime. It uses type erasure to handle the component generically.
         *
         * @param entity The ID of the entity to which the component will be added.
         * @param typeIndex The type index of the component to add.
         * @param component The component instance wrapped in a std::any.
         *
         * @throws std::bad_any_cast if the component cannot be cast to the expected type.
         * @throws ComponentNotRegistered if the component type is not registered.
         */
        void addComponentAny(Entity entity, const std::type_index& typeIndex, const std::any& component);

        /**
         * @brief Retrieves a component from an entity using type erasure.
         *
         * This method allows retrieving a component from an entity when the component type
         * is only known at runtime. It returns the component wrapped in a std::any.
         *
         * @param entity The ID of the entity from which to retrieve the component.
         * @param typeIndex The type index of the component to retrieve.
         * @return std::any containing the component instance, or an empty std::any if not found.
         *
         * @throws ComponentNotRegistered if the component type is not registered.
         */
        [[nodiscard]] std::any getComponentAny(Entity entity, const std::type_index& typeIndex) const;

        /**
         * @brief Duplicates an entity along with all its components.
         *
         * This method creates a new entity and copies all components from the source entity
         * to the new entity. It uses the registered component addition functions to ensure
         * that each component is properly added to the new entity.
         *
         * @param sourceEntity The ID of the entity to duplicate.
         * @return Entity The ID of the newly created duplicate entity.
         *
         * @throws ComponentNotRegistered if any component type of the source entity is not registered.
         */
        [[nodiscard]] Entity duplicateEntity(Entity sourceEntity) const;

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
        [[nodiscard]] std::vector<Entity> getEntitiesWithComponents() const
        {
            std::vector<Entity> result;
            std::span<const Entity> livingEntities = m_entityManager->getLivingEntities();
            result.reserve(livingEntities.size());
            for (Entity entity : livingEntities) {
                const bool hasAll = (entityHasComponent<ComponentTypes>(entity) && ...);
                if (hasAll) {
                    result.push_back(entity);
                }
            }
            return result;
        }

        [[nodiscard]] std::vector<ecs::ComponentType> getRegisteredComponentTypes() const
        {
            std::vector<ecs::ComponentType> result;
            result.reserve(m_componentDescriptions.size());
            for (const auto &p : m_componentDescriptions) result.push_back(p.first);
            return result;
        }

        /**
         * @brief Updates all system entities based on current entity signatures.
         *
         * This method iterates through all registered systems and updates their
         * entity lists according to the current signatures of all entities. It ensures
         * that each system processes only the entities that match its signature criteria.
         */
        void updateSystemEntities() const;

        /**
         * @brief Serialize a component array at runtime using its std::type_index
         * @return true if serialization was performed (a serializer existed), false otherwise
         */
        [[nodiscard]] bool serializeComponentArray(const std::type_index& typeIndex, nexo::json& out,
                                                   const nexo::save::SerializationContext& ctx = {}) const;

       private:
        /**
         * @brief Processes a component type to update required and excluded signatures.
         *
         * This helper function checks if the given component type is wrapped in an Exclude<T>
         * template. If it is, the actual component type is added to the excluded signature.
         * Otherwise, it is added to the required signature. This allows for flexible querying
         * of entities based on both required and excluded components.
         *
         * @tparam Component The component type to process, which may be wrapped in Exclude<T>.
         * @param required Reference to the signature representing required components.
         * @param excluded Reference to the signature representing excluded components.
         */
        template<typename Component>
        void processComponentSignature(Signature& required, Signature& excluded) const
        {
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
        std::unordered_map<std::type_index, std::function<void(json&, const save::SerializationContext&)>> m_serializeComponentArrayFunctions;

        std::unordered_map<ComponentType, std::shared_ptr<ComponentDescription>> m_componentDescriptions;
    };
} // namespace nexo::ecs
