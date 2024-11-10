//// Scene.cpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the scene class
//
///////////////////////////////////////////////////////////////////////////////
#include "Scene.hpp"

namespace nexo::scene {

    void Scene::addLayer(const std::shared_ptr<layer::Layer> &layer)
    {
        m_layerStack.pushLayer(layer);
        LOG(NEXO_DEV, "Scene(addLayer): layer {} added to scene {}", layer->getName(), m_debugSceneName);
    }

    void Scene::addOverlay(const std::shared_ptr<layer::Layer> &overlay)
    {
        m_layerStack.pushOverlay(overlay);
        LOG(NEXO_DEV, "Scene(addOverlay): overlay {} added to scene {}", overlay->getName(), m_debugSceneName);
    }

    void Scene::removeLayer(const std::shared_ptr<layer::Layer> &layer)
    {
        m_layerStack.popLayer(layer);
        LOG(NEXO_DEV, "Scene(removeLayer): layer {} removed from scene {}", layer->getName(), m_debugSceneName);
    }

    void Scene::removeOverlay(const std::shared_ptr<layer::Layer> &overlay)
    {
        m_layerStack.popOverlay(overlay);
        LOG(NEXO_DEV, "Scene(removeOverlay): overlay {} removed from scene {}", overlay->getName(), m_debugSceneName);
    }

    std::shared_ptr<layer::Layer> Scene::getLayerAt(const unsigned int index) const
    {
        if (index < m_layerStack.size())
            return m_layerStack[index];
        return nullptr;
    }

    void Scene::addEntity(const ecs::Entity entity, const int layerIndex)
    {
        if (layerIndex == -1)
        {
            m_globalEntities.insert(entity);
            LOG(NEXO_DEV, "Scene(addEntity): entity {} added to scene {} global entities", entity, m_debugSceneName);
            return;
        }
        if (layerIndex >= 0 && static_cast<unsigned int>(layerIndex) < m_layerStack.size())
        {
            m_layerStack[layerIndex]->addEntity(entity);
            LOG(NEXO_DEV, "Scene(addEntity): entity {} added to scene({})'s layer {}", entity, m_debugSceneName, m_layerStack[layerIndex]->getName());
        } else
            LOG(NEXO_WARN, "Scene(addEntity): Scene {}: index {} out of bounds", m_debugSceneName, layerIndex);
    }

    void Scene::removeEntity(const ecs::Entity entity, const int layerIndex)
    {
        if (layerIndex == -1)
        {
            m_globalEntities.erase(entity);
            LOG(NEXO_DEV, "Scene(removeEntity): entity {} removed from scene {} global entities", entity, m_debugSceneName);
            return;
        }
        if (layerIndex >= 0 && static_cast<unsigned int>(layerIndex) < m_layerStack.size())
        {
            m_layerStack[layerIndex]->removeEntity(entity);
            LOG(NEXO_DEV, "Scene(removeEntity): entity {} removed from scene({})'s layer {}", entity, m_debugSceneName, m_layerStack[layerIndex]->getName());
        } else
            LOG(NEXO_WARN, "Scene(addEntity): Scene {}: index {} out of bounds", m_debugSceneName, layerIndex);
    }

    void Scene::entityDestroyed(const ecs::Entity entity)
    {
        for (const auto &layer : m_layerStack)
            layer->removeEntity(entity);
    }

    std::set<ecs::Entity> Scene::getEntities() const
    {
        std::set<ecs::Entity> entities;
        entities.insert(m_globalEntities.begin(), m_globalEntities.end());
        for (auto const &layer: m_layerStack)
            entities.insert(layer->getEntities().begin(), layer->getEntities().end());
        return entities;
    }

    void Scene::attachCamera(const std::shared_ptr<camera::Camera> &camera, const unsigned int layerIndex)
    {
        if (layerIndex < m_layerStack.size())
        {
            m_layerStack[layerIndex]->attachCamera(camera);
            LOG(NEXO_DEV, "Scene(attachCamera): camera added to scene({})'s layer {}", m_debugSceneName, m_layerStack[layerIndex]->getName());
            return;
        }
        LOG(NEXO_WARN, "Scene(addEntity): Scene {}: index {} out of bounds", m_debugSceneName, layerIndex);
    }

    void Scene::detachCamera(const unsigned int layerIndex)
    {
        if (layerIndex < m_layerStack.size())
        {
            m_layerStack[layerIndex]->detachCamera();
            LOG(NEXO_DEV, "Scene(detachCamera): camera removed from scene({})'s layer {}", m_debugSceneName, m_layerStack[layerIndex]->getName());
            return;
        }
        LOG(NEXO_WARN, "Scene(detachCamera): Scene {}: index {} out of bounds", m_debugSceneName, layerIndex);
    }

    void Scene::setLayerRenderStatus(const bool status, const unsigned int layerIndex)
    {
        if (layerIndex < m_layerStack.size())
        {
            m_layerStack[layerIndex]->setRenderStatus(status);
            std::string strStatus = status ? "rendered" : "not rendered";
            LOG(NEXO_DEV, "Scene(setLayerRenderStatus): scene({})'s layer {} is now {}", m_debugSceneName, m_layerStack[layerIndex]->getName(), strStatus);
            return;
        }
        LOG(NEXO_WARN, "Scene(setLayerRenderStatus): Scene {}: index {} out of bounds", m_debugSceneName, layerIndex);
    }

    void Scene::setLayerActiveStatus(const bool status, const unsigned int layerIndex)
    {
        if (layerIndex < m_layerStack.size())
        {
            m_layerStack[layerIndex]->setActiveStatus(status);
            std::string strStatus = status ? "active" : "unactive";
            LOG(NEXO_DEV, "Scene(setLayerActiveStatus): scene({})'s layer {} is now {}", m_debugSceneName, m_layerStack[layerIndex]->getName(), strStatus);
            return;
        }
        LOG(NEXO_WARN, "Scene(setLayerActiveStatus): Scene {}: index {} out of bounds", m_debugSceneName, layerIndex);
    }

    bool Scene::getLayerRenderStatus(const unsigned int layerIndex) const
    {
        if (layerIndex < m_layerStack.size())
        {
            return m_layerStack[layerIndex]->isRendered();
        }
        LOG(NEXO_WARN, "Scene(getLayerRenderStatus): Scene {}: index {} out of bounds", m_debugSceneName, layerIndex);
        return false;
    }

    bool Scene::getLayerActiveStatus(const unsigned int layerIndex) const
    {
        if (layerIndex < m_layerStack.size())
            return m_layerStack[layerIndex]->isActive();
        LOG(NEXO_WARN, "Scene(getLayerActiveStatus): Scene {}: index {} out of bounds", m_debugSceneName, layerIndex);
        return false;
    }

    std::shared_ptr<nexo::camera::Camera> &Scene::getCamera(const unsigned int layerIndex)
    {
        static std::shared_ptr<nexo::camera::Camera> nullCamera = nullptr;
        if (layerIndex < m_layerStack.size())
            return m_layerStack[layerIndex]->getCamera();
        LOG(NEXO_WARN, "Scene(getCamera): Scene {}: index {} out of bounds", m_debugSceneName, layerIndex);
        return nullCamera;
    }

    void Scene::onUpdate(const core::Timestep timestep)
    {
        for (const auto &layer: m_layerStack)
        {
            if (layer->isActive())
                layer->onUpdate(timestep);
        }
    }

    void Scene::onRender()
    {
        for (const auto &layer: m_layerStack)
        {
            if (layer->isRendered())
                layer->onRender();
        }
    }

    void Scene::dispatchEventToLayers(event::Event &event)
    {
        const std::type_index eventType(typeid(event));
        for (const auto &layer: m_layerStack)
        {
            if (const auto &listenedEventTypes = layer->getListenedEventTypes();
                layer->isActive() && listenedEventTypes.find(eventType) != listenedEventTypes.end())
            {
                event.trigger(*layer);
                if (event.consumed)
                    return;
            }
        }
    }

    void Scene::setActiveStatus(const bool status)
    {
        m_isActive = status;
        for (ecs::Entity entity: m_globalEntities)
            ecs::setActive(entity, status);
        for (auto const &layer: m_layerStack)
            layer->setActiveStatus(status);
    }

    void Scene::setRenderStatus(const bool status)
    {
        m_isRendered = status;
        for (ecs::Entity entity: m_globalEntities)
            ecs::setRendered(entity, status);
        for (auto const &layer: m_layerStack)
            layer->setRenderStatus(status);
    }
}
