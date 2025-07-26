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

#include "Definitions.hpp"
#include "ECSExceptions.hpp"
#include "System.hpp"
#include "Access.hpp"
#include "ComponentArray.hpp"
#include "Coordinator.hpp"
#include "SingletonComponentMixin.hpp"
#include <type_traits>
#include <unordered_map>

namespace nexo::ecs {
    /**
     * @brief Metaprogramming helper to extract singleton components from a parameter pack
     *
     * This template recursively filters a list of components, extracting only the
     * singleton components to pass to SingletonComponentMixin. This ensures that
     * regular components aren't processed by the mixin.
     *
     * @tparam Components Component access specifiers to filter for singleton types
     */
    template<typename... Components>
    struct ExtractSingletonComponents;

    /**
     * @brief Base case specialization for empty component list
     *
     * When no components remain to be processed, this provides a void-specialized
     * SingletonComponentMixin as the base type.
     */
    template<>
    struct ExtractSingletonComponents<> {
        using type = SingletonComponentMixin<void>;
    };

    /**
     * @brief Recursive case for ExtractSingletonComponents
     *
     * Checks if the current component is a singleton. If it is, includes it in
     * the mixin via RebindWithComponent. If not, skips it and continues with the rest.
     *
     * @tparam Component The current component to check
     * @tparam Rest Remaining components to process
     */
    template<typename Component, typename... Rest>
    struct ExtractSingletonComponents<Component, Rest...> {
        // If Component is a singleton access type, include it in the mixin
        using type = std::conditional_t<
            IsSingleton<Component>::value,
            // Add this component to the mixin along with other singleton components
            typename ExtractSingletonComponents<Rest...>::type::template RebindWithComponent<Component>,
            // Skip this component and continue with the rest
            typename ExtractSingletonComponents<Rest...>::type
        >;
    };
    /**
     * @class QuerySystem
     * @brief System that directly queries component arrays
     *
     * @tparam Components Component access specifiers (Read<T>, Write<T>, ReadSingleton<T>, WriteSingleton<T>)
     */
     template<typename... Components>
     class QuerySystem : public AQuerySystem,
                         public ExtractSingletonComponents<Components...>::type::template RebindWithDerived<QuerySystem<Components...>> {
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
				if (!coord)
					THROW_EXCEPTION(InternalError, "Coordinator is null in QuerySystem constructor");

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
				const ComponentType typeIndex = getUniqueComponentTypeID<T>();
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
					m_componentArrays[getUniqueComponentTypeID<T>()] = componentArray;
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
			std::unordered_map<ComponentType, std::shared_ptr<IComponentArray>> m_componentArrays;

			/// Component signature defining required components for this system
			Signature m_signature;
    };
}
