//// QuerySystem.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        06/04/2025
//  Description: Header file for the query system class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ECSExceptions.hpp"
#include "System.hpp"
#include "Access.hpp"
#include "ComponentArray.hpp"
#include "Coordinator.hpp"
#include "SingletonComponentMixin.hpp"
#include <tuple>
#include <type_traits>
#include <unordered_map>

namespace nexo::ecs {
    /**
     * @class QuerySystem
     * @brief System that directly queries component arrays
     *
     * @tparam Components Component access specifiers (Read<T>, Write<T>, ReadSingleton<T>, WriteSingleton<T>)
     */
    template<typename... Components>
    class QuerySystem : public AQuerySystem, public SingletonComponentMixin<
														QuerySystem<Components...>,
														Components...> {
		private:
			/**
			* @brief Helper template to check if a component type has Read access in a parameter pack
			*
			* @tparam T The component type to check for Read access
			* @tparam First The first component access type in the parameter pack
			* @tparam Rest The remaining component access types
			*/
			template<typename T, typename First, typename... Rest>
			struct HasReadAccess {
				static constexpr bool value =
					(std::is_same_v<First, Read<T>> ||
					HasReadAccess<T, Rest...>::value);
			};

			/**
			* @brief Base case for HasReadAccess template recursion
			*
			* @tparam T The component type to check for Read access
			* @tparam First The last component access type in the parameter pack
			*/
			template<typename T, typename First>
			struct HasReadAccess<T, First> {
				static constexpr bool value = std::is_same_v<First, Read<T>>;
			};

			/**
			* @brief Convenience function to check if a component has Read-only access
			*
			* @tparam T The component type to check
			* @return true if the component has Read-only access, false otherwise
			*/
			template<typename T>
			static constexpr bool hasReadAccess()
			{
				return HasReadAccess<T, Components...>::value;
			}

	    public:
	        // Make the base class a friend to access protected members
			friend class SingletonComponentMixin<QuerySystem<Components...>, Components...>;

			/**
			* @brief Constructs a new QuerySystem
			*
			* Sets up the system signature based on required components,
			* caches component arrays for faster access, and initializes
			* singleton components.
			*
			* @throws InternalError if the coordinator is null
			*/
			QuerySystem()
			{
				if (!coord) {
					THROW_EXCEPTION(InternalError, "Coordinator is null in QuerySystem constructor");
				}

				// Set system signature based on required components (ignore singleton components)
				(setComponentSignatureIfRegular<Components>(m_signature), ...);

				// Cache component arrays for faster access (ignore singleton components)
				(cacheComponentArrayIfRegular<Components>(), ...);

				// Initialize singleton components
				this->initializeSingletonComponents();
			}

	        /**
	         * @brief Get a component for an entity with access type determined at compile time
	         *
	         * @tparam T The component type
	         * @param entity The entity to get the component from
	         * @return Reference to the component with appropriate const-ness
	         */
			template<typename T>
			std::conditional_t<hasReadAccess<T>(), const T&, T&> getComponent(Entity entity)
			{
				const std::type_index typeIndex = getTypeIndex<T>();
				const auto it = m_componentArrays.find(typeIndex);

				if (it == m_componentArrays.end())
					THROW_EXCEPTION(InternalError, "Component array not found");

				auto componentArray = std::static_pointer_cast<ComponentArray<T>>(it->second);

				if (!componentArray)
					THROW_EXCEPTION(InternalError, "Failed to cast component array");

				if (!componentArray->hasComponent(entity))
					THROW_EXCEPTION(InternalError, "Entity doesn't have requested component");
				return componentArray->get(entity);
			}

			/**
			* @brief Gets the component signature for this system
			*
			* @return const Signature& Reference to the system's component signature
			*/
			const Signature& getSignature() const override { return m_signature; }

			/**
			* @brief Gets a mutable reference to the component signature for this system
			*
			* @return Signature& Reference to the system's component signature
			*/
			Signature& getSignature() { return m_signature; }

	    protected:
	        /**
	         * @brief Caches component arrays for faster access (only for regular components)
	         *
	         * @tparam ComponentAccessType The component access type to cache
	         */
			template<typename ComponentAccessType>
			void cacheComponentArrayIfRegular()
			{
				if constexpr (!IsSingleton<ComponentAccessType>::value) {
					using T = typename ComponentAccessType::ComponentType;
					auto componentArray = coord->getComponentArray<T>();
					m_componentArrays[getTypeIndex<T>()] = componentArray;
				}
			}

	        /**
	         * @brief Sets the component bit in the system signature (only for regular components)
	         *
	         * @tparam ComponentAccessType The component access type
	         * @param signature The signature to modify
	         */
			template<typename ComponentAccessType>
			void setComponentSignatureIfRegular(Signature& signature)
			{
				if constexpr (!IsSingleton<ComponentAccessType>::value) {
					using T = typename ComponentAccessType::ComponentType;
					signature.set(coord->getComponentType<T>(), true);
				}
			}

		private:
			// Cache of component arrays for faster access
			std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_componentArrays;

			/// Component signature defining required components for this system
			Signature m_signature;
    };
}
