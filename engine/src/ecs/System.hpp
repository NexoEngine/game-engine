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

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "Definitions.hpp"
#include "Logger.hpp"
#include "ECSExceptions.hpp"

namespace nexo::ecs {
    class Coordinator;
}

namespace nexo::ecs {

    class SparseSet {
        public:
            void insert(Entity entity);

            void erase(Entity entity);

            bool empty() const { return dense.empty(); }

            bool contains(const Entity entity) const { return sparse.contains(entity); }

            size_t size() const { return dense.size(); }

            const std::vector<Entity>& getDense() const { return dense; }
            auto begin() const { return dense.begin(); }
            auto end() const { return dense.end(); }

        private:
            std::vector<Entity> dense;
            std::unordered_map<Entity, size_t> sparse;
    };

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
            virtual ~System() = default;
            static std::shared_ptr<Coordinator> coord;
    };

    /**
     * @class AQuerySystem
     * @brief Base abstract for all query-based systems
     */
    class AQuerySystem : public System {
    public:
        ~AQuerySystem() override = default;
        virtual const Signature &getSignature() const = 0;

        SparseSet entities;

    };

    /**
     * @class AGroupSystem
     * @brief Abstract base class for all group-based systems
     */
    class AGroupSystem : public System {
    public:
        ~AGroupSystem() override = default;
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
            template<typename T, typename... Args>
            std::shared_ptr<T> registerQuerySystem(Args&&... args)
            {
                static_assert(std::is_base_of_v<AQuerySystem, T>, "T must derive from AQuerySystem");
                std::type_index typeName(typeid(T));

                if (m_querySystems.contains(typeName))
                {
                    LOG(NEXO_WARN, "ECS::SystemManager::registerSystem: trying to register a system more than once");
                    return nullptr;
                }

                auto system = std::make_shared<T>(std::forward<Args>(args)...);
                m_querySystems.insert({typeName, system});
                return system;
            }

            template<typename T, typename... Args>
            std::shared_ptr<T> registerGroupSystem(Args&&... args)
            {
            	static_assert(std::is_base_of_v<AGroupSystem, T>, "T must derive from AGroupSystem");
                std::type_index typeName(typeid(T));

                if (m_groupSystems.contains(typeName))
                {
                    LOG(NEXO_WARN, "ECS::SystemManager::registerSystem: trying to register a system more than once");
                    return nullptr;
                }

                auto system = std::make_shared<T>(std::forward<Args>(args)...);
                m_groupSystems.insert({typeName, system});
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
            void setSignature(Signature signature)
            {
                std::type_index typeName(typeid(T));

                m_signatures.insert({typeName, signature});
            }

            /**
            * @brief Handles the destruction of an entity by removing it from all systems.
            *
            * @param entity - The ID of the destroyed entity.
            * @param signature - The signature of the entity.
            */
            void entityDestroyed(Entity entity, Signature signature) const;

            /**
            * @brief Updates the systems with an entity when its signature changes.
            *
            * This ensures that systems process only relevant entities based on their current components.
            * @param entity - The ID of the entity whose signature has changed.
            * @param oldSignature - The old signature of the entity.
            * @param newSignature - The new signature of the entity.
            */
            void entitySignatureChanged(Entity entity, Signature oldSignature, Signature newSignature);
        private:
            std::unordered_map<std::type_index, Signature> m_signatures{};
            std::unordered_map<std::type_index, std::shared_ptr<AQuerySystem>> m_querySystems{};
            std::unordered_map<std::type_index, std::shared_ptr<AGroupSystem>> m_groupSystems{};
    };
}
