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

#include <cassert>
#include <unordered_map>
#include <typeindex>
#include <memory>

namespace nexo::ecs {
    class ISingletonComponent {
        public:
            virtual ~ISingletonComponent() = default;
    };

    template <typename T>
    class SingletonComponent final : public ISingletonComponent {
        public:
            explicit SingletonComponent(T instance) : _instance(instance) {};

            T &getInstance() {
                return _instance;
            }
        private:
            T _instance;
    };

    class SingletonComponentManager {
        public:
            template <typename T>
            void registerSingletonComponent(T component) {
                std::type_index typeName(typeid(T));
                assert(m_singletonComponents.find(typeName) == m_singletonComponents.end() && "Registering singleton component more than once.");
                m_singletonComponents.insert({typeName, std::make_shared<SingletonComponent<T>>(std::move(component))});
            }

            template <typename T>
            T &getSingletonComponent() {
                const std::type_index typeName(typeid(T));
                assert(m_singletonComponents.find(typeName) != m_singletonComponents.end() && "Singleton component not registered before use.");

                auto componentPtr = dynamic_cast<SingletonComponent<T>*>(m_singletonComponents[typeName].get());
                assert(componentPtr && "Dynamic cast failed to convert to the required SingletonComponent type.");

                return componentPtr->getInstance();
            }

            template <typename T>
            void unregisterSingletonComponent() {
                const std::type_index typeName(typeid(T));
                assert(m_singletonComponents.find(typeName) != m_singletonComponents.end() && "Singleton component not registered.");
                m_singletonComponents.erase(typeName);
            }
        private:
            std::unordered_map<std::type_index, std::shared_ptr<ISingletonComponent>> m_singletonComponents{};
    };
}
