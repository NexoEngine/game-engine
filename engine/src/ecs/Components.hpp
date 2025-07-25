//// Components.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the component manager class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <memory>
#include <bitset>
#include <optional>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include <set>
#include <sstream>
#include <ranges>

#include "ECSExceptions.hpp"
#include "Definitions.hpp"
#include "Exception.hpp"
#include "Logger.hpp"
#include "ComponentArray.hpp"
#include "Group.hpp"

namespace nexo::ecs {
	/**
	 * @brief Helper template to tag non-owned component types
	 *
	 * This template is used for tag dispatching to distinguish between
	 * owned and non-owned components in group registration.
	 *
	 * @tparam NonOwning The non-owned component types
	 */
	template<typename... NonOwning>
	struct get_t { };

	/**
	 * @brief Creates a type tag for specifying non-owned components
	 *
	 * This helper function is used to create a tag type that identifies
	 * which components should be accessible but not owned by a group.
	 *
	 * @tparam NonOwning The non-owned component types
	 * @return A tag object of get_t<NonOwning...>
	 */
	template<typename... NonOwning>
	get_t<NonOwning...> get()
	{
		return {};
	}

	/**
	 * @brief Type alias for a tuple of owned component arrays
	 *
	 * Used internally by the Group class to store owned components.
	 *
	 * @tparam Owned The owned component types
	 */
	template<typename... Owned>
	using OwnedComponents = std::tuple<std::shared_ptr<ComponentArray<Owned>>...>;

	/**
	 * @brief Type alias for a tuple of non-owned component arrays
	 *
	 * Used internally by the Group class to store references to non-owned components.
	 *
	 * @tparam NonOwned The non-owned component types
	 */
	template<typename... NonOwned>
	using NonOwnedComponents = std::tuple<std::shared_ptr<ComponentArray<NonOwned>>...>;

	/**
	 * @brief Type alias for a shared pointer to a Group
	 *
	 * Simplifies the declaration of Group instances by hiding the complex template types.
	 *
	 * @tparam OwnedGroup Tuple type for owned component arrays
	 * @tparam NonOwnedGroup Tuple type for non-owned component arrays
	 */
	template<typename OwnedGroup, typename NonOwnedGroup>
	using GroupAlias = std::shared_ptr<Group<OwnedGroup, NonOwnedGroup>>;

	/**
	 * @brief Structure to represent a group key using numeric types
	 *
	 * Provides a more efficient way to identify groups compared to strings.
	 * Uses a combination of two separate signatures to represent owned and non-owned components.
	 */
	struct GroupKey {
		Signature ownedSignature;     ///< Bits set for components owned by the group
		Signature nonOwnedSignature;  ///< Bits set for components used but not owned by the group

		/**
		 * @brief Equality comparison operator
		 */
		bool operator==(const GroupKey& other) const {
			return ownedSignature == other.ownedSignature &&
				   nonOwnedSignature == other.nonOwnedSignature;
		}

		/**
		 * @brief Returns a string representation of the component types in this key
		 * Used for error messages and debugging
		 *
		 * @return String describing the components
		 */
		[[nodiscard]] std::string toString() const {
			std::stringstream ss;
			ss << "Owned: {";
			bool first = true;

			// Add owned component IDs
			for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; ++i) {
				if (ownedSignature.test(i)) {
					if (!first)
						ss << ", ";
					ss << "Component#" << i;
					first = false;
				}
			}

			ss << "}, Non-owned: {";
			first = true;

			// Add non-owned component IDs
			for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; ++i) {
				if (nonOwnedSignature.test(i)) {
					if (!first)
						ss << ", ";
					ss << "Component#" << i;
					first = false;
				}
			}

			ss << "}";
			return ss.str();
		}
	};
}

// Add hash function for GroupKey to use it in an unordered_map
namespace std {
	/**
	 * @brief Hash function for GroupKey
	 *
	 * Allows GroupKey to be used as a key in unordered_map
	 */
	template<>
	struct hash<nexo::ecs::GroupKey> {
		size_t operator()(const nexo::ecs::GroupKey& key) const noexcept
		{
			const size_t h1 = std::hash<nexo::ecs::Signature>()(key.ownedSignature);
			const size_t h2 = std::hash<nexo::ecs::Signature>()(key.nonOwnedSignature);
			return h1 ^ (h2 << 1);
		}
	};
}

namespace nexo::ecs {

	/**
	 * @class ComponentManager
	 *
	 * @brief Central manager for all component types and their arrays
	 *
	 * The ComponentManager is responsible for:
	 * - Registering component types in the ECS
	 * - Creating and maintaining component arrays
	 * - Adding/removing components from entities
	 * - Managing component group registrations
	 * - Handling entity destruction with respect to components
	 */
	class ComponentManager {
		public:
		    ComponentManager() = default;

		    /**
		     * @brief Copy constructor (deleted)
		     *
		     * ComponentManager is not copyable to prevent duplication of component data.
		     */
		    ComponentManager(const ComponentManager&) = delete;

		    /**
		     * @brief Copy assignment operator (deleted)
		     *
		     * ComponentManager is not copyable to prevent duplication of component data.
		     */
		    ComponentManager& operator=(const ComponentManager&) = delete;

		    /**
		     * @brief Move constructor
		     *
		     * Allows transferring ownership of component arrays to a new manager.
		     */
		    ComponentManager(ComponentManager&&) noexcept = default;

		    /**
		     * @brief Move assignment operator
		     *
		     * Allows transferring ownership of component arrays to a new manager.
		     */
		    ComponentManager& operator=(ComponentManager&&) noexcept = default;

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

	        ComponentType registerComponent(const size_t componentSize, const size_t initialCapacity = 1024)
		    {
		        const ComponentType typeID = generateComponentTypeID();
		        assert(typeID < m_componentArrays.size() && "Component type ID exceeds component array size");

		        assert(m_componentArrays[typeID] == nullptr && "TypeErasedComponent already registered, should really not happen");
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

		        if (m_componentArrays[typeID] == nullptr)
		            THROW_EXCEPTION(ComponentNotRegistered);

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

				for (const auto& group : std::ranges::views::values(m_groupRegistry)) {
				    // Check if entity qualifies now but did not qualify before.
                    if (((oldSignature & group->allSignature()) != group->allSignature()) &&
                            ((newSignature & group->allSignature()) == group->allSignature())) {
		    			group->addToGroup(entity);
					}
				}
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
	        void addComponent(const Entity entity, const ComponentType componentType, const void *componentData, const Signature oldSignature, const Signature newSignature)
		    {
		        getComponentArray(componentType)->insertRaw(entity, componentData);

		        for (const auto& group : std::ranges::views::values(m_groupRegistry)) {
		            // Check if entity qualifies now but did not qualify before.
		            if (((oldSignature & group->allSignature()) != group->allSignature()) &&
                            ((newSignature & group->allSignature()) == group->allSignature())) {
		                group->addToGroup(entity);
                    }
		        }
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
	        void removeComponent(const Entity entity, const ComponentType componentType, const Signature previousSignature, const Signature newSignature)
		    {
		        for (const auto& group : std::ranges::views::values(m_groupRegistry))
		        {
		            if (((previousSignature & group->allSignature()) == group->allSignature()) &&
                        ((newSignature & group->allSignature()) != group->allSignature()))
		            {
		                group->removeFromGroup(entity);
		            }
		        }
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
                for (const auto& group : std::ranges::views::values(m_groupRegistry))
                {
                    // If the entity no longer qualifies but did before, remove it.
                    if (((previousSignature & group->allSignature()) == group->allSignature()) &&
                    ((newSignature & group->allSignature()) != group->allSignature())) {
                        group->removeFromGroup(entity);
                    }
                }
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
		        if (!componentArray->hasComponent(entity))
		            return false;

				for (const auto& group : std::ranges::views::values(m_groupRegistry))
				{
				    // If the entity no longer qualifies but did before, remove it.
                    if (((previousSignature & group->allSignature()) == group->allSignature()) &&
                    ((newSignature & group->allSignature()) != group->allSignature())) {
                        group->removeFromGroup(entity);
                    }
				}
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

			template<typename T>
			void duplicateComponent(
			    Entity sourceEntity,
				Entity destEntity,
				const Signature oldSignature,
				const Signature newSignature
			) {
				const auto &componentArray = getComponentArray<T>();
				const auto sourceComponent = componentArray->get(sourceEntity);
				addComponent(destEntity, sourceComponent, oldSignature, newSignature);
			}

			void duplicateComponent(
			    ComponentType componentType,
			    Entity sourceEntity,
				Entity destEntity,
				const Signature oldSignature,
				const Signature newSignature
			) {
			    const auto& componentArray = m_componentArrays[componentType];
				componentArray->duplicateComponent(sourceEntity, destEntity);

				for (const auto& group : std::ranges::views::values(m_groupRegistry)) {
				    // Check if entity qualifies now but did not qualify before.
                    if (((oldSignature & group->allSignature()) != group->allSignature()) &&
                            ((newSignature & group->allSignature()) == group->allSignature())) {
		    			group->addToGroup(destEntity);
					}
				}
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
		        if (componentArray == nullptr)
		            THROW_EXCEPTION(ComponentNotRegistered);

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
		        if (!componentArray->hasComponent(entity))
		            return std::nullopt;

		        return componentArray->get(entity);
		    }

	        /**
             * @brief Safely attempts to get a component from an entity
             *
             * @param entity The entity to get the component from
             * @param typeID The component type ID
             * @return Pointer to the component if it exists, or nullptr if not found
             */
            [[nodiscard]] void *tryGetComponent(const Entity entity, const ComponentType typeID) const
		    {
		        const auto componentArray = getComponentArray(typeID);
		        if (!componentArray->hasComponent(entity))
		            return nullptr;

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
		    void entityDestroyed(Entity entity, const Signature &entitySignature);

			/**
			 * @brief Creates or retrieves a group for specific component combinations
			 *
			 * Creates a group that provides optimized access to entities having
			 * a specific combination of components, or returns an existing one.
			 * Components specified in the template parameter pack are "owned" (internal to the group),
			 * while those in the nonOwned parameter are "non-owned" (externally referenced).
			 *
			 * @tparam Owned Component types that are owned by the group
			 * @param nonOwned A get_t<...> tag specifying non-owned component types
			 * @return A shared pointer to the group (either existing or newly created)
			 * @throws ComponentNotRegistered if any component type is not registered
			 * @throws OverlappingGroupsException if the new group would have overlapping owned
			 *         components with an existing group
			 */
			template<typename... Owned>
			auto registerGroup(const auto& nonOwned)
			{
			    const GroupKey newGroupKey = generateGroupKey<Owned...>(nonOwned);

			    // Check if this exact group already exists
			    auto it = m_groupRegistry.find(newGroupKey);
			    if (it != m_groupRegistry.end()) {
			        using OwnedTuple = std::tuple<std::shared_ptr<ComponentArray<Owned>>...>;
			        using NonOwnedTuple = decltype(getNonOwnedTuple(nonOwned));
			        return std::static_pointer_cast<Group<OwnedTuple, NonOwnedTuple>>(it->second);
			    }

			    // Check for conflicts with existing groups
				for (const auto& existingKey : std::ranges::views::keys(m_groupRegistry)) {
					if (hasCommonOwnedComponents(existingKey, newGroupKey)) {
						for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; i++) {
							if (existingKey.ownedSignature.test(i) && newGroupKey.ownedSignature.test(i)) {
								THROW_EXCEPTION(OverlappingGroupsException,
												existingKey.toString(),
												newGroupKey.toString(),
												i);
							}
						}
					}
				}

			    auto group = createNewGroup<Owned...>(nonOwned);
			    m_groupRegistry[newGroupKey] = group;
			    return group;
			}

			/**
			 * @brief Retrieves an existing group for specific component combinations
			 *
			 * Gets a previously registered group that matches the specified
			 * owned and non-owned component types.
			 *
			 * @tparam Owned Component types that are owned by the group
			 * @param nonOwned A get_t<...> tag specifying non-owned component types
			 * @return A shared pointer to the existing group
			 * @throws std::runtime_error if the group doesn't exist
			 */
			template<typename... Owned>
			auto getGroup(const auto& nonOwned)
			{
				const GroupKey groupKey = generateGroupKey<Owned...>(nonOwned);

				const auto it = m_groupRegistry.find(groupKey);
			    if (it == m_groupRegistry.end())
        			THROW_EXCEPTION(GroupNotFound, "Group not found");

			    using OwnedTuple = std::tuple<std::shared_ptr<ComponentArray<Owned>>...>;
			    using NonOwnedTuple = decltype(getNonOwnedTuple(nonOwned));
			    return std::static_pointer_cast<Group<OwnedTuple, NonOwnedTuple>>(it->second);
			}

            /**
             * @brief Checks if two groups share any common owned components
             *
             * Determines if two groups have any overlap in their owned components.
             * This is useful for determining if two groups might affect each other's sorting
             * or partitioning when entities are updated.
             *
             * @param key1 First group key to compare
             * @param key2 Second group key to compare
             * @return true if the keys share at least one owned component, false otherwise
             */
            [[nodiscard]] static bool hasCommonOwnedComponents(const GroupKey& key1, const GroupKey& key2)
            {
                return (key1.ownedSignature & key2.ownedSignature).any();
            }

		private:
		    /**
		     * @brief Array of component arrays indexed by component type ID
		     *
		     * Provides O(1) lookup of component arrays by their type ID.
		     */
		    std::array<std::shared_ptr<IComponentArray>, MAX_COMPONENT_TYPE> m_componentArrays{};

			/**
			 * @brief Registry of groups indexed by their component signatures
			 *
			 * Allows retrieval of previously created groups by their component types.
			 */
			std::unordered_map<GroupKey, std::shared_ptr<IGroup>> m_groupRegistry;

			/**
			 * @brief Helper function to get the tuple of non-owned component arrays
			 *
			 * @param nonOwned A get_t<...> tag specifying non-owned component types
			 * @return Tuple of non-owned component arrays
			 */
			template<typename... NonOwning>
			auto getNonOwnedTuple(const get_t<NonOwning...>&)
			{
			    return std::make_tuple(getComponentArray<NonOwning>()...);
			}

			/**
			 * @brief Creates a new group for the specified component types
			 *
			 * @tparam Owned Component types owned by the group
			 * @param nonOwned Tag for non-owned component types
			 * @return Shared pointer to the new group
			 */
			template<typename... Owned>
			auto createNewGroup(const auto& nonOwned)
			{
			    auto nonOwnedArrays = getNonOwnedTuple(nonOwned);

			    auto ownedArrays = std::make_tuple(getComponentArray<Owned>()...);
			    using OwnedTuple = std::tuple<std::shared_ptr<ComponentArray<Owned>>...>;
			    using NonOwnedTuple = decltype(nonOwnedArrays);

			    // Find entities that should be in this group
			    auto driver = std::get<0>(ownedArrays);
			    const std::size_t minSize = std::apply([](auto&&... arrays) -> std::size_t {
			        return std::min({ static_cast<std::size_t>(arrays->size())... });
			    }, ownedArrays);

			    for (std::size_t i = 0; i < minSize; ++i) {
			        Entity e = driver->getEntityAtIndex(i);
			        bool valid = true;

			        // Check in owned arrays
					std::apply([&](auto&&... arrays) {
						valid = (valid && ... && arrays->hasComponent(e));
					}, ownedArrays);

			        // Check in non-owned arrays
			        std::apply([&](auto&&... arrays) {
            			valid = (valid && ... && arrays->hasComponent(e));
			        }, nonOwnedArrays);

			        if (valid) {
			            std::apply([&](auto&&... arrays) {
			                ((arrays->addToGroup(e)), ...);
			            }, ownedArrays);
			        }
			    }

			    return std::make_shared<Group<OwnedTuple, NonOwnedTuple>>(ownedArrays, nonOwnedArrays);
			}

			/**
			 * @brief Generates a unique key for a group based on its component types
			 *
			 * Creates a GroupKey with separate signatures for owned and non-owned components.
			 *
			 * @tparam Owned Component types owned by the group
			 * @param nonOwned Tag for non-owned component types
			 * @return GroupKey uniquely identifying this group type combination
			 */
			template<typename... Owned>
			GroupKey generateGroupKey(const auto& nonOwned)
			{
			    GroupKey key;

			    // Set bits for owned components
			    ((key.ownedSignature.set(getComponentTypeID<Owned>())), ...);

			    // Set bits for non-owned components
			    setNonOwnedBits(key.nonOwnedSignature, nonOwned);

			    return key;
			}

			/**
			 * @brief Sets bits in the non-owned signature for each non-owned component
			 *
			 * @tparam NonOwning Non-owned component types
			 * @param signature The signature to modify
			 * @param nonOwned The non-owned components tag
			 */
			template<typename... NonOwning>
			static void setNonOwnedBits(Signature& signature, const get_t<NonOwning...>&)
			{
			    ((signature.set(getComponentTypeID<NonOwning>())), ...);
			}
	};
}
