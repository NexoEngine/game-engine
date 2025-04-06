#pragma once

#include "System.hpp"
#include "Access.hpp"
#include "Group.hpp"
#include "ComponentArray.hpp"
#include "Coordinator.hpp"
#include "Access.hpp"
#include <tuple>
#include <memory>
#include <type_traits>
#include <span>

namespace nexo::ecs {

    /**
     * @class GroupSystem
     * @brief System that uses component groups for optimized access with enforced permissions
     *
     * @tparam OwnedAccess Owned<> wrapper with component access types
     * @tparam NonOwnedAccess NonOwned<> wrapper with component access types
     */
    template<typename OwnedAccess, typename NonOwnedAccess = NonOwned<>>
    class GroupSystem : public AGroupSystem {
    private:
        // Extract component access types
        using OwnedAccessTypes = typename OwnedAccess::ComponentTypes;
        using NonOwnedAccessTypes = typename NonOwnedAccess::ComponentTypes;

        // Extract raw component types for group creation
        template<typename... T>
        struct GetComponentTypes;

        template<typename... T>
        struct GetComponentTypes<std::tuple<T...>> {
            using Types = std::tuple<typename T::ComponentType...>;
        };

        using OwnedTypes = typename GetComponentTypes<OwnedAccessTypes>::Types;
        using NonOwnedTypes = typename GetComponentTypes<NonOwnedAccessTypes>::Types;

        // Helper to unpack tuple types to parameter pack
        template<typename Tuple, std::size_t... I>
        auto tupleToTypeList(std::index_sequence<I...>) {
            return std::tuple<std::tuple_element_t<I, Tuple>...>{};
        }

        // Function to create a type list from a tuple type
        template<typename Tuple>
        auto tupleToTypeList() {
            return tupleToTypeList<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{});
        }

        // Type aliases for the actual group
        template<typename... T>
        using ComponentArraysTuple = std::tuple<std::shared_ptr<ComponentArray<T>>...>;

        // Group type is determined by the owned and non-owned component arrays
        template<typename OT, typename NOT>
        struct GroupTypeFromTuples;

        template<typename... OT, typename... NOT>
        struct GroupTypeFromTuples<std::tuple<OT...>, std::tuple<NOT...>> {
            using Type = Group<ComponentArraysTuple<OT...>, ComponentArraysTuple<NOT...>>;
        };

        // The actual group type for this system
        using ActualGroupType = typename GroupTypeFromTuples<OwnedTypes, NonOwnedTypes>::Type;

        // Component access trait to find the access type for a component
        template<typename T, typename AccessTuple, typename = void>
        struct ComponentAccessTrait {
            static constexpr bool found = false;
            static constexpr AccessType accessType = AccessType::Read;
        };

        template<typename T, typename... AccessTypes>
        struct ComponentAccessTrait<T, std::tuple<AccessTypes...>,
            std::void_t<std::enable_if_t<(std::is_same_v<T, typename AccessTypes::ComponentType> || ...)>>> {
            static constexpr bool found = true;

            template<typename... As>
            static constexpr AccessType GetAccessTypeFromPack() {
                AccessType result = AccessType::Read;
                ((std::is_same_v<T, typename As::ComponentType> ? result = As::accessType : result), ...);
                return result;
            }

            static constexpr AccessType accessType = GetAccessTypeFromPack<AccessTypes...>();
        };

        template<typename T>
        struct GetComponentAccess {
            using OwnedTrait = ComponentAccessTrait<T, OwnedAccessTypes>;
            using NonOwnedTrait = ComponentAccessTrait<T, NonOwnedAccessTypes>;

            static constexpr bool found = OwnedTrait::found || NonOwnedTrait::found;
            static constexpr AccessType accessType =
                OwnedTrait::found ? OwnedTrait::accessType :
                NonOwnedTrait::found ? NonOwnedTrait::accessType :
                AccessType::Read;
        };

        /**
         * @brief Access-controlled span wrapper for component arrays
         */
        template<typename T>
        class ComponentSpan {
        private:
            std::span<T> m_span;

        public:
            explicit ComponentSpan(std::span<T> span) : m_span(span) {}

            size_t size() const { return m_span.size(); }

            // Conditionally define operator[] based on access type
            template<typename U = T>
            auto operator[](size_t index) -> std::conditional_t<
                GetComponentAccess<std::remove_const_t<U>>::accessType == AccessType::Write,
                std::remove_const_t<U>&,
                const std::remove_const_t<U>&
            > {
                if constexpr (GetComponentAccess<std::remove_const_t<U>>::accessType == AccessType::Write) {
                    return const_cast<std::remove_const_t<U>&>(m_span[index]);
                } else {
                    return m_span[index];
                }
            }

            template<typename U = T>
            auto operator[](size_t index) const -> const std::remove_const_t<U>& {
                return m_span[index];
            }

            // Iterator support
            auto begin() { return m_span.begin(); }
            auto end() { return m_span.end(); }
            auto begin() const { return m_span.begin(); }
            auto end() const { return m_span.end(); }
        };

    public:
        GroupSystem() {
            if (!coord) return;

            // Create the group
            m_group = createGroupImpl(
                tupleToTypeList<OwnedTypes>(),
                tupleToTypeList<NonOwnedTypes>()
            );
        }

        /**
         * @brief Get component array with correct access permissions
         *
         * @tparam T The component type
         * @return ComponentSpan with enforced access control
         */
        template<typename T>
        auto get() {
            // Get the span from the group
            auto baseSpan = m_group->template get<T>();

            // Wrap it in our access-controlled span
            return ComponentSpan<std::conditional_t<
                GetComponentAccess<T>::accessType == AccessType::Read,
                const T,
                T
            >>(baseSpan);
        }

        /**
         * @brief Get a component for an entity with appropriate access control
         *
         * @tparam AccessType The component access type (Read<T> or Write<T>)
         * @param entity The entity to get the component for
         * @return Component reference with appropriate const-ness
         */
        template<typename AccessType>
        auto getComponent(Entity entity) {
            using T = typename AccessType::ComponentType;

            if constexpr (AccessType::accessType == AccessType::Read) {
                return std::cref(m_group->template get<T>(entity));
            } else {
                return std::ref(m_group->template get<T>(entity));
            }
        }

        /**
         * @brief Get all entities in this group
         *
         * @return Span of entities in the group
         */
        auto getEntities() const {
            return m_group->entities();
        }

        /**
         * @brief Get direct access to the underlying group
         *
         * @return The group object
         */
        auto& group() const {
            return *m_group;
        }

    protected:
        std::shared_ptr<ActualGroupType> m_group;

    private:
        /**
         * @brief Implementation to create a group with the extracted component types
         */
        template<typename... OT, typename... NOT>
        std::shared_ptr<ActualGroupType> createGroupImpl(std::tuple<OT...>, std::tuple<NOT...>) {
            if constexpr (sizeof...(OT) > 0) {
                if constexpr (sizeof...(NOT) > 0) {
                    auto group = coord->registerGroup<OT...>(nexo::ecs::get<NOT...>());
                    return std::static_pointer_cast<ActualGroupType>(group);
                } else {
                    auto group = coord->registerGroup<OT...>(nexo::ecs::get<>());
                    return std::static_pointer_cast<ActualGroupType>(group);
                }
            }
            return nullptr;
        }
    };
}
