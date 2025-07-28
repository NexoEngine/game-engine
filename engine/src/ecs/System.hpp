//// System.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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

	/**
	* @class SparseSet
	*
	* @brief A sparse set implementation for efficient entity storage and lookup
	*
	* This class provides O(1) insertion, removal, and lookup operations for entities.
	* It uses a sparse-dense pattern where entities are stored contiguously in a dense array,
	* while maintaining a sparse lookup map to quickly find entity positions.
	*/
    class SparseSet {
        public:
	        /**
	         * @brief Insert an entity into the set
	         *
	         * @param entity The entity to insert
	         */
	        void insert(Entity entity);

	        /**
	         * @brief Remove an entity from the set
	         *
	         * @param entity The entity to remove
	         */
	        void erase(Entity entity);

	        /**
	         * @brief Check if the set is empty
	         *
	         * @return true if the set contains no entities, false otherwise
	         */
	        bool empty() const { return dense.empty(); }

	        /**
	         * @brief Check if an entity exists in the set
	         *
	         * @param entity The entity to check
	         * @return true if the entity exists in the set, false otherwise
	         */
	        bool contains(const Entity entity) const { return sparse.contains(entity); }

	        /**
	         * @brief Get the number of entities in the set
	         *
	         * @return The number of entities
	         */
	        size_t size() const { return dense.size(); }

	        /**
	         * @brief Get the dense array of entities
	         *
	         * @return Const reference to the vector of entities
	         */
	        const std::vector<Entity>& getDense() const { return dense; }

	        /**
	         * @brief Get an iterator to the beginning of the entity collection
	         *
	         * @return Iterator to the first entity
	         */
	        auto begin() const { return dense.begin(); }

	        /**
	         * @brief Get an iterator to the end of the entity collection
	         *
	         * @return Iterator to the position after the last entity
	         */
	        auto end() const { return dense.end(); }

        private:
            /**
             * @brief Dense array of entities in insertion order
             */
            std::vector<Entity> dense;

            /**
             * @brief Sparse lookup map from entity ID to position in dense array
             */
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
            /**
             * @brief Global coordinator instance shared by all systems
             */
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

        /**
         * @brief Entities that match this system's signature
         */
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

            /**
             * @brief Registers a new group-based system of type T in the ECS framework
             *
             * @tparam T The type of the system to be registered (must derive from AGroupSystem)
             * @tparam Args Constructor argument types for the system
             * @param args Constructor arguments for the system
             * @return std::shared_ptr<T> Shared pointer to the newly registered system
             */
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
	        /**
	         * @brief Map of system type to component signature
	         */
	        std::unordered_map<std::type_index, Signature> m_signatures{};

	        /**
	         * @brief Map of query system type to system instance
	         */
	        std::unordered_map<std::type_index, std::shared_ptr<AQuerySystem>> m_querySystems{};

	        /**
	         * @brief Map of group system type to system instance
	         */
	        std::unordered_map<std::type_index, std::shared_ptr<AGroupSystem>> m_groupSystems{};
    };
}
