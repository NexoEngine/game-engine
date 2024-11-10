//// Coordinator.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the coordinator class of the ecs
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <any>

#include "System.hpp"
#include "SingletonComponent.hpp"
#include "Entity.hpp"
#include "core/scene/SceneManager.hpp"
#include "core/Logger.hpp"


namespace nexo::ecs {
    /**
     * @class Coordinator
     *
     * @brief Central orchestrator for the Entity-Component-System (ECS) architecture.
     *
     * The Coordinator class ties together the functionalities of the EntityManager,
     * ComponentManager, and SystemManager to facilitate the creation, management,
     * and interaction of entities, components, and systems within the ECS framework.
     */
    class Coordinator {
        public:
            /**
            * @brief Initializes the Coordinator, creating instances of EntityManager,
            * ComponentManager, SystemManager, EventManager, SingletonComponentManager
            * and SceneManager.
            */
            void init();

            /**
            * @brief Creates a new entity.
            *
            * @return Entity - The newly created entity's ID.
            */
            Entity createEntity() const;

            /**
            * @brief Destroys an entity and cleans up its components and system references.
            *
            * @param entity - The ID of the entity to destroy.
            */
            void destroyEntity(Entity entity) const;

            /**
            * @brief Registers a new component type within the ComponentManager.
            */
            template <typename T>
            void registerComponent() {
                m_componentManager->registerComponent<T>();
                m_hasComponentFunctions[typeid(T)] = [this](Entity entity) -> bool {
                    return this->entityHasComponent<T>(entity);
                };

                m_getComponentFunctions[typeid(T)] = [this](Entity entity) -> std::any {
                    return std::any(this->getComponent<T>(entity));
                };
            }

            /**
             * @brief Registers a new singleton component
             *
             * @tparam T Class that should inherit from SingletonComponent class
             * @param component
             */
            template <typename T>
            void registerSingletonComponent(T component) {
                m_singletonComponentManager->registerSingletonComponent<T>(component);
            }

            /**
            * @brief Adds a component to an entity, updates its signature, and notifies systems.
            *
            * @param entity - The ID of the entity.
            * @param component - The component to add to the entity.
            */
            template <typename T>
            void addComponent(const Entity entity, T component) {
                m_componentManager->addComponent<T>(entity, component);

                auto signature = m_entityManager->getSignature(entity);
                signature.set(m_componentManager->getComponentType<T>(), true);
                m_entityManager->setSignature(entity, signature);

                m_systemManager->entitySignatureChanged(entity, signature);
                updateSystemEntities();
            }

            /**
            * @brief Removes a component from an entity, updates its signature, and notifies systems.
            *
            * @param entity - The ID of the entity.
            */
            template<typename T>
            void removeComponent(const Entity entity) {
                m_componentManager->removeComponent<T>(entity);

                auto signature = m_entityManager->getSignature(entity);
                signature.set(m_componentManager->getComponentType<T>(), false);
                m_entityManager->setSignature(entity, signature);

                m_systemManager->entitySignatureChanged(entity, signature);
                updateSystemEntities();
            }

            /**
             * @brief Remove a singleton component
             *
             * @tparam T Class that should inherit from the SingletonComponent class
             */
            template <typename T>
            void removeSingletonComponent() const
            {
                m_singletonComponentManager->unregisterSingletonComponent<T>();
            }

            /**
            * @brief Retrieves a reference to a component of an entity.
            *
            * @param entity - The ID of the entity.
            * @return T& - Reference to the requested component.
            */
            template <typename T>
            T &getComponent(const Entity entity) {
                return m_componentManager->getComponent<T>(entity);
            }

            /**
             * @brief Get the Singleton Component object
             *
             * @tparam T Class that should inherit from the SingletonComponent class
             * @return T& The instance of the desired singleton component
             */
            template <typename T>
            T &getSingletonComponent() {
                return m_singletonComponentManager->getSingletonComponent<T>();
            }

            std::vector<std::pair<std::type_index, std::any>> getAllComponents(const Entity entity);

            /**
            * @brief Gets the component type ID for a specific component type.
            *
            * @return components::ComponentType - The ID of the component type.
            */
            template<typename T>
            ComponentType getComponentType() const
            {
                return m_componentManager->getComponentType<T>();
            }

            /**
            * @brief Registers a new system within the SystemManager.
            *
            * @return std::shared_ptr<T> - Shared pointer to the newly registered system.
            */
            template <typename T>
            std::shared_ptr<T> registerSystem() {
                return m_systemManager->registerSystem<T>();
            }

            /**
            * @brief Sets the signature for a system, defining which entities it will process.
            *
            * @param signature - The signature to associate with the system.
            */
            template <typename T>
            void setSystemSignature(const Signature signature) const
            {
                m_systemManager->setSignature<T>(signature);
            }

            /**
             * @brief Create a Scene object
             *
             * @param id The id of the scene to be created
             */
            void createScene(scene::SceneId id) const;

            /**
             * @brief Delete a scene object
             *
             * @param id The id of the scene to be deleted
             */
            void deleteScene(scene::SceneId id) const;

            /**
             * @brief Activate a scene
             *
             * @param id The id of the scene
             */
            void activateScene(scene::SceneId id) const;

            /**
             * @brief Deactivate a scene
             *
             * @param id The id of the scene
             */
            void deactivateScene(scene::SceneId id) const;

            bool isSceneActive(scene::SceneId id) const;

            bool isSceneRendered(scene::SceneId sceneID) const;

            void setSceneRenderStatus(scene::SceneId sceneID, bool status) const;

            void setSceneActiveStatus(scene::SceneId sceneID, bool status) const;

            void addEntityToScene(Entity entity, scene::SceneId sceneID, int layerIndex = -1, bool active = true, bool rendered = true) const;

            template<typename DerivedLayer>
            void addEntityToScene(const Entity entity, const scene::SceneId sceneID, const bool active = true, const bool rendered = true) const
            {
                m_sceneManager->addEntityToScene<DerivedLayer>(entity, sceneID, active, rendered);
                const auto signature = m_entityManager->getSignature(entity);
                m_systemManager->entitySignatureChanged(entity, signature);
                updateSystemEntities();
            }

            void removeEntityFromScene(Entity entity, scene::SceneId sceneID, int layerIndex = -1) const;

            template<typename DerivedLayer>
            void removeEntityFromScene(const Entity entity, const scene::SceneId sceneID) const
            {
                m_sceneManager->removeEntityFromScene<DerivedLayer>(entity, sceneID);
                updateSystemEntities();
            }

            void attachCamera(scene::SceneId id, std::shared_ptr<camera::Camera> &camera, unsigned int layerIndex) const;

            template<typename DerivedLayer>
            void attachCamera(const scene::SceneId id, std::shared_ptr<camera::Camera> &camera) const
            {
                m_sceneManager->attachCamera<DerivedLayer>(id, camera);
            }

            void detachCamera(scene::SceneId id, unsigned int layerIndex) const;

            template<typename DerivedLayer>
            void detachCamera(const scene::SceneId id) const
            {
                m_sceneManager->detachCamera<DerivedLayer>(id);
            }

            std::shared_ptr<camera::Camera> getCamera(scene::SceneId id, unsigned int layerIndex) const;

            template<typename DerivedLayer>
            std::shared_ptr<camera::Camera> getCamera(const scene::SceneId id) const
            {
                return m_sceneManager->getCamera<DerivedLayer>(id);
            }

            [[nodiscard]] scene::SceneManager& getSceneManager() const { return *m_sceneManager; }

            void setSceneWindowOffset(const scene::SceneId id, const glm::vec2 offset) const { m_sceneManager->setWindowOffset(id, offset); }

            [[nodiscard]] glm::vec2 getSceneWindowOffset(const scene::SceneId id) const { return m_sceneManager->getWindowOffset(id); }

        private:
            void updateSystemEntities() const;

            template<typename T>
            bool entityHasComponent(const Entity entity) const
            {
                const auto signature = m_entityManager->getSignature(entity);
                const ComponentType componentType = m_componentManager->getComponentType<T>();
                return signature.test(componentType);
            }

            std::shared_ptr<ComponentManager> m_componentManager;
            std::shared_ptr<EntityManager> m_entityManager;
            std::shared_ptr<SystemManager> m_systemManager;
            std::shared_ptr<SingletonComponentManager> m_singletonComponentManager;
            std::shared_ptr<scene::SceneManager> m_sceneManager;

            std::unordered_map<std::type_index, std::function<bool(Entity)>> m_hasComponentFunctions;
            std::unordered_map<std::type_index, std::function<std::any(Entity)>> m_getComponentFunctions;
    };
}




