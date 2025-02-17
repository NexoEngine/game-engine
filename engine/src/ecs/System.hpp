//// System.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the ecs system class and system manager
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <set>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "Signature.hpp"

namespace nexo::ecs {
    class Coordinator;
}

namespace nexo::ecs {
    /**
    * @class System
    *
    * @brief Base class for systems in an ECS (Entity-Component-System) architecture.
    *
    * Systems are responsible for processing entities that have a specific set of components.
    * This class provides the basic structure for a system, mainly a set of entities that the system will process.
    */
    class System {
        public:
            std::set<Entity> entities;
            static std::shared_ptr<Coordinator> coord;
    };

    /**
    * @class SystemManager
    *
    * @brief Manages systems in the ECS architecture.
    *
    * This class is responsible for registering systems, setting their signatures,
    * and updating systems with relevant entities based on entity signature changes.
    */
    class SystemManager {
        public:
            /**
            * @brief Registers a new system of type T in the ECS framework.
            *
            * @tparam T - The type of the system to be registered.
            * @return std::shared_ptr<T> - Shared pointer to the newly registered system.
            */
            template<typename T>
            std::shared_ptr<T> registerSystem() {
                std::type_index typeName(typeid(T));

                if (m_systems.contains(typeName))
                {
                    LOG(NEXO_WARN, "ECS::SystemManager::registerSystem: trying to register a system more than once");
                    return nullptr;
                }

                auto system = std::make_shared<T>();
                m_systems.insert({typeName, system});
                return system;
            }

            /**
            * @brief Sets the signature for a system.
            *
            * The signature determines which entities the system will process based on their component makeup.
            * @tparam T - The type of the system for which the signature is being set.
            * @param signature - The signature to associate with the system.
            */
            template<typename T>
            void setSignature(Signature signature) {
                std::type_index typeName(typeid(T));

                if (!m_systems.contains(typeName))
                    THROW_EXCEPTION(SystemNotRegistered);

                m_signatures.insert({typeName, signature});
            }

            /**
            * @brief Handles the destruction of an entity by removing it from all systems.
            *
            * @param entity - The ID of the destroyed entity.
            */
            void entityDestroyed(Entity entity) const;

            /**
            * @brief Updates the systems with an entity when its signature changes.
            *
            * This ensures that systems process only relevant entities based on their current components.
            * @param entity - The ID of the entity whose signature has changed.
            * @param entitySignature - The new signature of the entity.
            */
            void entitySignatureChanged(Entity entity, Signature entitySignature);
        private:
            std::unordered_map<std::type_index, Signature> m_signatures{};
            std::unordered_map<std::type_index, std::shared_ptr<System>> m_systems{};
    };
}
