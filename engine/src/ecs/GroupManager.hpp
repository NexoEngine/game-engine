//// GroupManager.hpp //////////////////////////////////////////////////////////
//
//  Author:      Mehdy MORVAN
//  Date:        10/02/2026
//  Description: Header file for the group manager class — manages component
//               group registration, lookup, and entity membership tracking.
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <ranges>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "ComponentArray.hpp"
#include "Definitions.hpp"
#include "ECSExceptions.hpp"
#include "Exception.hpp"
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
    struct get_t {};

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
        Signature ownedSignature;    ///< Bits set for components owned by the group
        Signature nonOwnedSignature; ///< Bits set for components used but not owned by the group

        /**
         * @brief Equality comparison operator
         */
        bool operator==(const GroupKey& other) const
        {
            return ownedSignature == other.ownedSignature && nonOwnedSignature == other.nonOwnedSignature;
        }

        /**
         * @brief Returns a string representation of the component types in this key
         * Used for error messages and debugging
         *
         * @return String describing the components
         */
        [[nodiscard]] std::string toString() const
        {
            std::stringstream ss;
            ss << "Owned: {";
            bool first = true;

            // Add owned component IDs
            for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; ++i) {
                if (ownedSignature.test(i)) {
                    if (!first) ss << ", ";
                    ss << "Component#" << i;
                    first = false;
                }
            }

            ss << "}, Non-owned: {";
            first = true;

            // Add non-owned component IDs
            for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; ++i) {
                if (nonOwnedSignature.test(i)) {
                    if (!first) ss << ", ";
                    ss << "Component#" << i;
                    first = false;
                }
            }

            ss << "}";
            return ss.str();
        }
    };
} // namespace nexo::ecs

/**
 * @brief Hash function for GroupKey
 *
 * Allows GroupKey to be used as a key in unordered_map
 */
template<>
struct std::hash<nexo::ecs::GroupKey> {
    size_t operator()(const nexo::ecs::GroupKey& key) const noexcept
    {
        const size_t h1 = std::hash<nexo::ecs::Signature>()(key.ownedSignature);
        const size_t h2 = std::hash<nexo::ecs::Signature>()(key.nonOwnedSignature);
        return h1 ^ (h2 << 1);
    }
}; // namespace std

namespace nexo::ecs {

    /**
     * @class GroupManager
     *
     * @brief Manages component group registration, lookup, and entity membership.
     *
     * The GroupManager is responsible for:
     * - Registering and retrieving component groups
     * - Detecting overlapping group ownership
     * - Maintaining the component-to-groups index
     * - Updating group membership when components are added/removed or entities destroyed
     *
     * This class is owned by ComponentManager and should not be used directly by
     * client code. Use Coordinator or ComponentManager's group API instead.
     */
    class GroupManager {
       public:
        using ComponentArrayStore = std::array<std::shared_ptr<IComponentArray>, MAX_COMPONENT_TYPE>;

        /**
         * @brief Constructs a GroupManager with a reference to the component array storage.
         *
         * @param componentArrays Reference to the component arrays owned by ComponentManager.
         *        Must outlive this GroupManager instance.
         */
        explicit GroupManager(const ComponentArrayStore& componentArrays) : m_componentArrays(componentArrays) {}

        GroupManager(const GroupManager&)            = delete;
        GroupManager& operator=(const GroupManager&) = delete;
        GroupManager(GroupManager&&)                  = delete;
        GroupManager& operator=(GroupManager&&)       = delete;

        // =========================================================
        // Public API
        // =========================================================

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
                using OwnedTuple    = std::tuple<std::shared_ptr<ComponentArray<Owned>>...>;
                using NonOwnedTuple = decltype(getNonOwnedTuple(nonOwned));
                return std::static_pointer_cast<Group<OwnedTuple, NonOwnedTuple>>(it->second);
            }

            // Check for conflicts with existing groups
            for (const auto& existingKey : std::ranges::views::keys(m_groupRegistry)) {
                if (hasCommonOwnedComponents(existingKey, newGroupKey)) {
                    for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; i++) {
                        if (existingKey.ownedSignature.test(i) && newGroupKey.ownedSignature.test(i)) {
                            THROW_EXCEPTION(OverlappingGroupsException, existingKey.toString(), newGroupKey.toString(),
                                            i);
                        }
                    }
                }
            }

            auto group                   = createNewGroup<Owned...>(nonOwned);
            m_groupRegistry[newGroupKey] = group;

            // Populate the component-to-groups index
            const Signature& allSig = group->allSignature();
            for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; ++i) {
                if (allSig.test(i)) {
                    m_componentToGroups[i].push_back(group);
                }
            }

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
         * @throws GroupNotFound if the group doesn't exist
         */
        template<typename... Owned>
        auto getGroup(const auto& nonOwned)
        {
            const GroupKey groupKey = generateGroupKey<Owned...>(nonOwned);

            const auto it = m_groupRegistry.find(groupKey);
            if (it == m_groupRegistry.end()) THROW_EXCEPTION(GroupNotFound, "Group not found");

            using OwnedTuple    = std::tuple<std::shared_ptr<ComponentArray<Owned>>...>;
            using NonOwnedTuple = decltype(getNonOwnedTuple(nonOwned));
            return std::static_pointer_cast<Group<OwnedTuple, NonOwnedTuple>>(it->second);
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
            return (key1.ownedSignature & key2.ownedSignature).any();
        }

        // =========================================================
        // Notification methods — called by ComponentManager
        // =========================================================

        /**
         * @brief Updates group membership after a component was added to an entity.
         *
         * @param componentType The type of the component that was added
         * @param oldSignature The entity's signature before the addition
         * @param newSignature The entity's signature after the addition
         * @param entity The entity that received the component
         */
        void onComponentAdded(const ComponentType componentType, const Signature& oldSignature,
                              const Signature& newSignature, const Entity entity)
        {
            for (const auto& group : m_componentToGroups[componentType]) {
                if (((oldSignature & group->allSignature()) != group->allSignature()) &&
                    ((newSignature & group->allSignature()) == group->allSignature())) {
                    group->addToGroup(entity);
                }
            }
        }

        /**
         * @brief Updates group membership after a component was removed from an entity.
         *
         * @param componentType The type of the component that was removed
         * @param previousSignature The entity's signature before the removal
         * @param newSignature The entity's signature after the removal
         * @param entity The entity that lost the component
         */
        void onComponentRemoved(const ComponentType componentType, const Signature& previousSignature,
                                const Signature& newSignature, const Entity entity)
        {
            for (const auto& group : m_componentToGroups[componentType]) {
                if (((previousSignature & group->allSignature()) == group->allSignature()) &&
                    ((newSignature & group->allSignature()) != group->allSignature())) {
                    group->removeFromGroup(entity);
                }
            }
        }

        /**
         * @brief Removes an entity from all groups it belongs to.
         *
         * @param entity The entity that was destroyed
         * @param entitySignature The entity's signature at destruction time
         */
        void onEntityDestroyed(Entity entity, const Signature& entitySignature);

       private:
        // =========================================================
        // Internal helpers
        // =========================================================

        /**
         * @brief Gets a typed component array from the store
         */
        template<typename T>
        [[nodiscard]] std::shared_ptr<ComponentArray<T>> getComponentArray()
        {
            const ComponentType typeID     = getComponentTypeID<T>();
            const auto& componentArray = m_componentArrays[typeID];
            if (componentArray == nullptr) THROW_EXCEPTION(ComponentNotRegistered);

            return std::static_pointer_cast<ComponentArray<T>>(componentArray);
        }

        /**
         * @brief Helper function to get the tuple of non-owned component arrays
         */
        template<typename... NonOwning>
        auto getNonOwnedTuple(const get_t<NonOwning...>&)
        {
            return std::make_tuple(getComponentArray<NonOwning>()...);
        }

        /**
         * @brief Creates a new group for the specified component types
         */
        template<typename... Owned>
        auto createNewGroup(const auto& nonOwned)
        {
            auto nonOwnedArrays = getNonOwnedTuple(nonOwned);

            auto ownedArrays    = std::make_tuple(getComponentArray<Owned>()...);
            using OwnedTuple    = std::tuple<std::shared_ptr<ComponentArray<Owned>>...>;
            using NonOwnedTuple = decltype(nonOwnedArrays);

            // Find entities that should be in this group
            auto driver               = std::get<0>(ownedArrays);
            const std::size_t minSize = std::apply(
                [](auto&&... arrays) -> std::size_t { return std::min({static_cast<std::size_t>(arrays->size())...}); },
                ownedArrays);

            for (std::size_t i = 0; i < minSize; ++i) {
                [[maybe_unused]] Entity e = driver->getEntityAtIndex(i);
                bool valid                = true;

                // Check in owned arrays
                std::apply([&](auto&&... arrays) { valid = (valid && ... && arrays->hasComponent(e)); }, ownedArrays);

                // Check in non-owned arrays
                std::apply([&](auto&&... arrays) { valid = (valid && ... && arrays->hasComponent(e)); },
                           nonOwnedArrays);

                if (valid) {
                    std::apply([&](auto&&... arrays) { ((arrays->addToGroup(e)), ...); }, ownedArrays);
                }
            }

            return std::make_shared<Group<OwnedTuple, NonOwnedTuple>>(ownedArrays, nonOwnedArrays);
        }

        /**
         * @brief Generates a unique key for a group based on its component types
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
         */
        template<typename... NonOwning>
        static void setNonOwnedBits(Signature& signature, const get_t<NonOwning...>&)
        {
            ((signature.set(getComponentTypeID<NonOwning>())), ...);
        }

        // =========================================================
        // Data members
        // =========================================================

        /// Reference to ComponentManager's component array storage
        const ComponentArrayStore& m_componentArrays;

        /// Registry of groups indexed by their component signatures
        std::unordered_map<GroupKey, std::shared_ptr<IGroup>> m_groupRegistry;

        /// Index from component type to groups that include that component
        std::array<std::vector<std::shared_ptr<IGroup>>, MAX_COMPONENT_TYPE> m_componentToGroups{};
    };

} // namespace nexo::ecs
