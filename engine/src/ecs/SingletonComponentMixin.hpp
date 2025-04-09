//// SingletonComponentMixin.hpp //////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        06/04/2025
//  Description: Base class for common singleton components operations in systems
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Access.hpp"
#include "SingletonComponent.hpp"
#include <unordered_map>
#include <typeindex>
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

        protected:
			/**
			* @brief Cache of singleton components for faster access
			*
			* Maps component type IDs to their singleton component instances
			*/
            std::unordered_map<std::type_index, std::shared_ptr<ISingletonComponent>> m_singletonComponents;

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
                try {
                    auto* derived = static_cast<Derived*>(this);
                    std::shared_ptr<ISingletonComponent> instance = derived->coord->template getRawSingletonComponent<T>();
                    m_singletonComponents[getTypeIndex<T>()] = instance;
                } catch (const nexo::ecs::SingletonComponentNotRegistered&) {
                    // Singleton not registered yet, we'll try again when getSingleton is called
                }
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
                const std::type_index typeIndex = getTypeIndex<T>();

                if (!m_singletonComponents.contains(typeIndex)) {
                    // Late binding in case the singleton was registered after system creation
                    cacheSingletonComponent<T>();
                }

                // Get the stored singleton component wrapper
                auto& singletonComponentPtr = m_singletonComponents[typeIndex];
                auto* componentWrapper = dynamic_cast<SingletonComponent<T>*>(singletonComponentPtr.get());

                if (!componentWrapper)
                    THROW_EXCEPTION(SingletonComponentNotRegistered);

                // Return the reference with appropriate constness
                if constexpr (hasReadSingletonAccess<T>()) {
                    // For read-only access, return const reference
                    return const_cast<const T&>(componentWrapper->getInstance());
                } else {
                    // For read-write access, return non-const reference
                    return componentWrapper->getInstance();
                }
            }
    };
}
