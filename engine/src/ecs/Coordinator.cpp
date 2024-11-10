//// Coordinator.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source file for the coordinator class
//
///////////////////////////////////////////////////////////////////////////////

#include "Coordinator.hpp"

namespace nexo::ecs {

    void Coordinator::init()
    {
        m_componentManager = std::make_shared<ComponentManager>();
        m_entityManager = std::make_shared<EntityManager>();
        m_systemManager = std::make_shared<SystemManager>();
        m_singletonComponentManager = std::make_shared<ecs::SingletonComponentManager>();
        m_sceneManager = std::make_shared<scene::SceneManager>();
        LOG(NEXO_DEV, "[ECS]: Coordinator initialized");
    }

    Entity Coordinator::createEntity() const
    {
        return m_entityManager->createEntity();
    }

    void Coordinator::destroyEntity(const Entity entity) const
    {
        m_entityManager->destroyEntity(entity);
        m_componentManager->entityDestroyed(entity);
        m_systemManager->entityDestroyed(entity);
        m_sceneManager->entityDestroyed(entity);
        updateSystemEntities();
    }

    std::vector<std::pair<std::type_index, std::any>> Coordinator::getAllComponents(const Entity entity)
    {
        std::vector<std::pair<std::type_index, std::any>> components;

        for (auto& [type, func] : m_hasComponentFunctions) {
            if (func(entity)) {
                components.emplace_back(type, m_getComponentFunctions[type](entity));
            }
        }

        return components;
    }

    void Coordinator::createScene(const scene::SceneId id) const
    {
        m_sceneManager->createScene(id);
    }

    void Coordinator::deleteScene(const scene::SceneId id) const
    {
        m_sceneManager->deleteScene(id);
    }

    void Coordinator::activateScene(const scene::SceneId id) const
    {
        m_sceneManager->setSceneActiveStatus(id, true);
        updateSystemEntities();
        for (const auto& entity : m_sceneManager->getAllActiveEntities()) {
            const auto signature = m_entityManager->getSignature(entity);
            m_systemManager->entitySignatureChanged(entity, signature);
        }
    }

    void Coordinator::deactivateScene(const scene::SceneId id) const
    {
        m_sceneManager->setSceneActiveStatus(id, false);
        updateSystemEntities();
        for (const auto& entity : m_sceneManager->getAllActiveEntities()) {
            const auto signature = m_entityManager->getSignature(entity);
            m_systemManager->entitySignatureChanged(entity, signature);
        }
    }

    bool Coordinator::isSceneActive(const scene::SceneId id) const
    {
        return m_sceneManager->isSceneActive(id);
    }

    bool Coordinator::isSceneRendered(const scene::SceneId sceneID) const
    {
        return m_sceneManager->isSceneRendered(sceneID);
    }

    void Coordinator::setSceneRenderStatus(const scene::SceneId sceneID, const bool status) const
    {
        m_sceneManager->setSceneRenderStatus(sceneID, status);
    }

    void Coordinator::setSceneActiveStatus(const scene::SceneId sceneID, const bool status) const
    {
        m_sceneManager->setSceneActiveStatus(sceneID, status);
    }

    void Coordinator::addEntityToScene(const Entity entity, const scene::SceneId sceneID, const int layerIndex, const bool active, const bool rendered) const
    {
        m_sceneManager->addEntityToScene(entity, sceneID, layerIndex, active, rendered);
        const auto signature = m_entityManager->getSignature(entity);
        m_systemManager->entitySignatureChanged(entity, signature);
        updateSystemEntities();
    }

    void Coordinator::removeEntityFromScene(const Entity entity, const scene::SceneId sceneID, const int layerIndex) const
    {
        m_sceneManager->removeEntityFromScene(entity, sceneID, layerIndex);
        updateSystemEntities();
    }

    void Coordinator::attachCamera(const scene::SceneId id, std::shared_ptr<camera::Camera> &camera, const unsigned int layerIndex) const
    {
        m_sceneManager->attachCamera(id, camera, layerIndex);
    }

    void Coordinator::detachCamera(const scene::SceneId id, const unsigned int layerIndex) const
    {
        m_sceneManager->detachCamera(id, layerIndex);
    }

    std::shared_ptr<camera::Camera> Coordinator::getCamera(const scene::SceneId id, const unsigned int layerIndex) const
    {
        return m_sceneManager->getCamera(id, layerIndex);
    }

    void Coordinator::updateSystemEntities() const
    {
        m_systemManager->updateSystemEntities(*m_sceneManager);
    }
}