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
                if (m_singletonComponents.find(typeName) != m_singletonComponents.end())
                {
                    LOG(NEXO_WARN, "ECS::SingletonComponentManager::registerSingletonComponent: trying to register a singleton component more than once");
                    return;
                }
                m_singletonComponents.insert({typeName, std::make_shared<SingletonComponent<T>>(std::move(component))});
            }

            template <typename T>
            T &getSingletonComponent() {
                const std::type_index typeName(typeid(T));
                if (m_singletonComponents.find(typeName) == m_singletonComponents.end())
                    THROW_EXCEPTION(SingletonComponentNotRegistered);

                auto componentPtr = dynamic_cast<SingletonComponent<T>*>(m_singletonComponents[typeName].get());

                return componentPtr->getInstance();
            }

            template <typename T>
            void unregisterSingletonComponent() {
                const std::type_index typeName(typeid(T));
                if (m_singletonComponents.find(typeName) == m_singletonComponents.end())
                    THROW_EXCEPTION(SingletonComponentNotRegistered);
                m_singletonComponents.erase(typeName);
            }
        private:
            std::unordered_map<std::type_index, std::shared_ptr<ISingletonComponent>> m_singletonComponents{};
    };
}
