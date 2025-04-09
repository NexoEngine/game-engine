//// GroupSystem.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        06/04/2025
//  Description: Header file for the Group system class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "System.hpp"
#include "Access.hpp"
#include "Group.hpp"
#include "ComponentArray.hpp"
#include "Coordinator.hpp"
#include "SingletonComponentMixin.hpp"
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
     * @tparam SingletonAccessTypes Singleton component access types (ReadSingleton<T> or WriteSingleton<T>)
     */
    template<typename OwnedAccess, typename NonOwnedAccess = NonOwned<>, typename... SingletonAccessTypes>
    class GroupSystem : public AGroupSystem, public SingletonComponentMixin<
	    													GroupSystem<OwnedAccess,
	                 													NonOwnedAccess,
	                              										SingletonAccessTypes...>,
	                                        				SingletonAccessTypes...> {
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
	        auto tupleToTypeList(std::index_sequence<I...>)
			{
	            return std::tuple<std::tuple_element_t<I, Tuple>...>{};
	        }

	        // Function to create a type list from a tuple type
	        template<typename Tuple>
	        auto tupleToTypeList()
			{
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
		            >
					{
		                if constexpr (GetComponentAccess<std::remove_const_t<U>>::accessType == AccessType::Write) {
		                    return const_cast<std::remove_const_t<U>&>(m_span[index]);
		                } else {
		                    return m_span[index];
		                }
		            }

		            template<typename U = T>
		            auto operator[](size_t index) const -> const std::remove_const_t<U>&
					{
		                return m_span[index];
		            }

		            // Iterator support
		            auto begin() { return m_span.begin(); }
		            auto end() { return m_span.end(); }
		            auto begin() const { return m_span.begin(); }
		            auto end() const { return m_span.end(); }
	        };

	    public:
	        // Make the base class a friend to access protected members
	        friend class SingletonComponentMixin<
							GroupSystem<OwnedAccess,
										NonOwnedAccess,
  										SingletonAccessTypes...>,
            				SingletonAccessTypes...>;

	        GroupSystem()
			{
    			static_assert(std::tuple_size_v<OwnedTypes> > 0,
                 	"GroupSystem must have at least one owned component type");
				if (!coord) {
			        THROW_EXCEPTION(InternalError, "Coordinator is null in GroupSystem constructor");
			        return;
			    }

	            // Create the group
	            m_group = createGroupImpl(
	                tupleToTypeList<OwnedTypes>(),
	                tupleToTypeList<NonOwnedTypes>()
	            );

	            // Initialize singleton components
	            this->initializeSingletonComponents();
	        }

	        /**
	         * @brief Get component array with correct access permissions
	         *
	         * @tparam T The component type
	         * @return ComponentSpan with enforced access control
	         */
	        template<typename T>
	        auto get()
			{
			    if (!m_group) {
			        THROW_EXCEPTION(InternalError, "Group is null in GroupSystem");
			    }
				constexpr bool isOwned = isOwnedComponent<T>();
				if constexpr (isOwned) {
					// Get the span from the group
					auto baseSpan = m_group->template get<T>();

					// Wrap it in our access-controlled span
					return ComponentSpan<std::conditional_t<
					    GetComponentAccess<T>::accessType == AccessType::Read,
					    const T,
					    T
					>>(baseSpan);
				} else {
					// For non-owned components, return the component array itself
					auto componentArray = m_group->template get<T>();

					// Apply access control by wrapping in a special pointer wrapper if needed
					if constexpr (GetComponentAccess<T>::accessType == AccessType::Read) {
						return std::shared_ptr<const ComponentArray<T>>(m_group->template get<T>());
					} else {
					    return componentArray;
					}
				}
	        }

			/**
			* @brief Check if a component type is owned by this system
			*
			* @tparam T The component type to check
			* @return true if owned, false if non-owned
			*/
			template<typename T>
			static constexpr bool isOwnedComponent()
			{
			    using OwnedTraitResult = ComponentAccessTrait<T, OwnedAccessTypes>;
			    return OwnedTraitResult::found;
			}

	        /**
	         * @brief Get all entities in this group
	         *
	         * @return Span of entities in the group
	         */
	        auto getEntities() const
			{
			    if (!m_group) {
			        THROW_EXCEPTION(InternalError, "Group is null in GroupSystem");
			    }
	            return m_group->entities();
	        }

	    protected:
	        std::shared_ptr<ActualGroupType> m_group = nullptr;

	    private:
	        /**
	         * @brief Implementation to create a group with the extracted component types
	         */
	        template<typename... OT, typename... NOT>
	        std::shared_ptr<ActualGroupType> createGroupImpl(std::tuple<OT...>, std::tuple<NOT...>)
			{
	            if constexpr (sizeof...(OT) > 0) {
	                if constexpr (sizeof...(NOT) > 0) {
	                    auto group = coord->template registerGroup<OT...>(nexo::ecs::get<NOT...>());
	                    return std::static_pointer_cast<ActualGroupType>(group);
	                } else {
	                    auto group = coord->template registerGroup<OT...>(nexo::ecs::get<>());
	                    return std::static_pointer_cast<ActualGroupType>(group);
	                }
	            }
	            return nullptr;
	        }
    };
}
