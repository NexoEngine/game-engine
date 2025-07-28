//// SingletonComponentMixin.hpp //////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/04/2025
//  Description: Header file for the singleton component mixin class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Access.hpp"
#include "SingletonComponent.hpp"
#include <memory>

namespace nexo::ecs {

    /**
     * @brief Mixin class that provides singleton component functionality to systems
     *
     * @tparam Derived The derived system type
     * @tparam SingletonAccessTypes Singleton component access types (ReadSingleton<T> or WriteSingleton<T>)
     */
    template<typename Derived, typename... SingletonAccessTypes>
    class SingletonComponentMixin {
        private:

            /**
            * @brief Helper to check if a singleton component has read-only access in the parameter pack
            *
            * This checks all SingletonAccessTypes to see if any match the given type T with ReadSingleton access.
            *
            * @tparam T The singleton component type to check
            */
            template<typename T>
            struct HasReadSingletonAccessImpl {
                static constexpr bool value = (... || (IsReadSingleton<SingletonAccessTypes>::value &&
                                                        std::is_same_v<typename SingletonAccessTypes::ComponentType, T>));
            };

        public:
            /**
             * @brief Rebind this mixin with a different derived class
             *
             * @tparam NewDerived The new derived class to use
             */
            template<typename NewDerived>
            using RebindWithDerived = SingletonComponentMixin<NewDerived, SingletonAccessTypes...>;

            /**
             * @brief Rebind this mixin with an additional singleton component
             *
             * @tparam NewComponent The new singleton component access type to add
             */
            template<typename NewComponent>
            using RebindWithComponent = SingletonComponentMixin<Derived, SingletonAccessTypes..., NewComponent>;

        protected:
            /**
            * @brief Cache of strongly-typed singleton components for faster access
            *
            * Stores components with their specific types to avoid dynamic casting
            */
            template<typename T>
            using SingletonComponentPtr = std::shared_ptr<SingletonComponent<T>>;

            // Type-specific cache for fast access without dynamic_cast
            template<typename T>
            struct TypedComponentCache {
                static inline SingletonComponentPtr<T> instance = nullptr;
            };

            /**
            * @brief Initializes singleton components for this system
            */
            void initializeSingletonComponents()
            {
                // Cache singleton components for faster access
                (cacheSingletonComponent<typename SingletonAccessTypes::ComponentType>(), ...);
            }

            /**
            * @brief Caches a specific singleton component
            *
            * @tparam T The singleton component type
            */
            template<typename T>
            void cacheSingletonComponent()
            {
                auto* derived = static_cast<Derived*>(this);
                const std::shared_ptr<ISingletonComponent> instance = derived->coord->template getRawSingletonComponent<T>();

                // Store in the type-specific cache
                auto typedInstance = std::static_pointer_cast<SingletonComponent<T>>(instance);
                TypedComponentCache<T>::instance = typedInstance;
            }

        public:

            /**
            * @brief Checks if a singleton component has read-only access
            *
            * @tparam T The singleton component type to check
            * @return true if the component has read-only access, false if it has read-write access
            */
            template<typename T>
            static constexpr bool hasReadSingletonAccess()
            {
                return HasReadSingletonAccessImpl<T>::value;
            }

            /**
            * @brief Get a singleton component with access type determined at compile time
            *
            * @tparam T The singleton component type
            * @return Reference to the singleton component with appropriate const-ness
            *
            * @warning MUST be captured with auto& or const auto& to preserve access restrictions!
            */
            template<typename T>
            std::conditional_t<hasReadSingletonAccess<T>(), const T&, T&> getSingleton()
            {
                if (!TypedComponentCache<T>::instance) {
                    // Late binding in case the singleton was registered after system creation
                    cacheSingletonComponent<T>();
                }

                auto& typedComponent = TypedComponentCache<T>::instance;

                if (!typedComponent)
                    THROW_EXCEPTION(SingletonComponentNotRegistered);

                // Return the reference with appropriate constness
                if constexpr (hasReadSingletonAccess<T>())
                    return const_cast<const T&>(typedComponent->getInstance());
                else
                    return typedComponent->getInstance();
            }
    };

    /**
     * @brief Base case specialization for SingletonComponentMixin with no components
     *
     * This specialization serves as the termination point for template recursion
     * when filtering singleton components. It provides a minimal implementation
     * with placeholder rebinding methods that allow component accumulation during
     * the template recursion process.
     */
    template<>
    class SingletonComponentMixin<void> {
        public:
            /**
            * @brief Rebind this base mixin with a derived class type
            *
            * This allows transitioning from the void placeholder to a concrete
            * derived type when no singleton components were found.
            *
            * @tparam NewDerived The derived class type to bind to
            */
            template<typename NewDerived>
            using RebindWithDerived = SingletonComponentMixin<NewDerived>;

            /**
             * @brief Begin building a mixin with a singleton component
             *
             * This is called when the first singleton component is found during
             * template recursion, transitioning from the void base case to a
             * mixin with actual components.
             *
             * @tparam NewComponent The first singleton component access type
             */
            template<typename NewComponent>
            using RebindWithComponent = SingletonComponentMixin<void, NewComponent>;

        protected:
            /**
            * @brief No-op implementation of singleton component initialization
            *
            * Since this specialization represents a mixin with no singleton components,
            * there's nothing to initialize.
            */
            void initializeSingletonComponents()
            {
                // No-op method
            }
    };
}
