//// SingletonComponent.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the singleton component class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "Logger.hpp"
#include "ECSExceptions.hpp"

namespace nexo::ecs {

	/**
	* @brief Base interface for singleton components.
	*
	* All singleton components must derive from this interface. It ensures proper polymorphic destruction.
	*/
    class ISingletonComponent {
        public:
            virtual ~ISingletonComponent() = default;
    };


    /**
     * @brief Template class representing a singleton component.
     *
     * This class wraps an instance of type T as a singleton component.
     *
     * @tparam T The type of the singleton component.
     */
     template <typename T>
     class SingletonComponent final : public ISingletonComponent {
     public:
         /**
          * @brief Templated constructor for perfect forwarding.
          *
          * This constructor is enabled only when the argument is an rvalue,
          * preventing accidental copying of objects of type T.
          *
          * @tparam U The type of the provided instance.
          * @param instance The instance to be moved into the singleton.
          */
         template<typename U, typename = std::enable_if_t<std::is_rvalue_reference_v<U&&>>>
         explicit SingletonComponent(U&& instance) : _instance(std::forward<U>(instance)) {}

         T &getInstance() {
             return _instance;
         }
     private:
         T _instance;
     };


    /**
     * @brief Manager for singleton components in the ECS.
     *
     * The SingletonComponentManager is responsible for registering, retrieving, and unregistering
     * singleton components. Singleton components are globally unique and accessed via their type.
     */
    class SingletonComponentManager {
        public:

			/**
			* @brief Registers a singleton component.
			*
			* If a singleton component of the same type is already registered, a warning is logged.
			*
			* @tparam T The type of the singleton component.
			* @param component The instance of the component to register.
			*/
            template <typename T>
            void registerSingletonComponent(T&& component) {
                using Decayed = std::decay_t<T>;
                std::type_index typeName(typeid(Decayed));
                if (m_singletonComponents.contains(typeName))
                {
                    LOG(NEXO_WARN, "ECS::SingletonComponentManager::registerSingletonComponent: trying to register a singleton component more than once");
                    return;
                }
                m_singletonComponents.insert({typeName, std::make_shared<SingletonComponent<Decayed>>(std::forward<T>(component))});
            }

            /**
             * @brief Retrieves a singleton component instance.
             *
             * @tparam T The type of the singleton component.
             * @return T& A reference to the registered singleton component.
             * @throws SingletonComponentNotRegistered if the component is not registered.
             */
            template <typename T>
            T &getSingletonComponent() {
                const std::type_index typeName(typeid(T));
                if (!m_singletonComponents.contains(typeName))
                    THROW_EXCEPTION(SingletonComponentNotRegistered);

                auto componentPtr = dynamic_cast<SingletonComponent<T>*>(m_singletonComponents[typeName].get());

                return componentPtr->getInstance();
            }

            /**
             * @brief Unregisters a singleton component.
             *
             * Removes the singleton component of type T from the manager.
             *
             * @tparam T The type of the singleton component.
             * @throws SingletonComponentNotRegistered if the component is not registered.
             */
            template <typename T>
            void unregisterSingletonComponent() {
                const std::type_index typeName(typeid(T));
                if (!m_singletonComponents.contains(typeName))
                    THROW_EXCEPTION(SingletonComponentNotRegistered);
                m_singletonComponents.erase(typeName);
            }
        private:
            std::unordered_map<std::type_index, std::shared_ptr<ISingletonComponent>> m_singletonComponents{};
    };
}
