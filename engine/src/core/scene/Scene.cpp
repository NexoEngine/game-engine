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
#include "core/layer/Layer.hpp"

namespace nexo::scene {

    void Scene::addLayer(const std::string &layerName)
    {
        auto newLayer = std::make_shared<layer::Layer>(layerName);
        m_layerStack.pushLayer(newLayer);
        LOG(NEXO_DEV, "Scene::{}::addLayer: layer {} added", name, layerName);
    }

    void Scene::addOverlay(const std::string &overlayName)
    {
        auto newOverlay = std::make_shared<layer::Layer>(overlayName);
        m_layerStack.pushOverlay(newOverlay);
        LOG(NEXO_DEV, "Scene::{}::addOverlay: overlay {} added", name, overlayName);
    }

    void Scene::removeLayer(const std::string &layerName)
    {
        auto layer = getLayer(layerName);
        m_layerStack.popLayer(layer);
        LOG(NEXO_DEV, "Scene::{}::removeLayer: layer {} removed", name, layerName);
    }

    void Scene::removeOverlay(const std::string &overlayName)
    {
        auto overlay = getLayer(overlayName);
        m_layerStack.popOverlay(overlay);
        LOG(NEXO_DEV, "Scene::{}::removeOverlay: overlay {} removed", name, overlayName);
    }

    std::shared_ptr<layer::Layer> Scene::getLayer(const std::string &layerName) const
    {
        return m_layerStack[layerName];
    }

    void Scene::addEntityToLayer(const ecs::Entity entity, const std::string &layerName)
    {
        if (const auto layer = m_layerStack[layerName]; layer != nullptr)
        {
            layer->addEntity(entity);
            LOG(NEXO_DEV, "Scene::{}::addEntityToLayerByName: entity {} added to layer {}", name, entity, layer->name);
        }
    }

    void Scene::addGlobalEntity(const ecs::Entity entity)
    {
        m_globalEntities.insert(entity);
        LOG(NEXO_DEV, "Scene::{}::addGlobalEntityToScene: entity {} added", name, entity);
    }

    void Scene::removeEntityFromLayer(const ecs::Entity entity, const std::string &layerName)
    {
        if (const auto layer = m_layerStack[layerName]; layer != nullptr)
        {
            layer->removeEntity(entity);
            LOG(NEXO_DEV, "Scene::{}::removeEntityFromLayerByName: entity {} removed from layer {}", name, entity, layer->name);
        }
    }

    void Scene::removeGlobalEntity(ecs::Entity entity)
    {
        if (m_globalEntities.erase(entity) == 0)
        {
            LOG(NEXO_WARN, "Scene::{}::removeGlobalEntity: entity {} not found", name, entity);
            return;
        }
        LOG(NEXO_DEV, "Scene::{}::removeGlobalEntity: entity {} removed", name, entity);
    }

    void Scene::entityDestroyed(const ecs::Entity entity)
    {
        m_globalEntities.erase(entity);
        for (const auto& layer : m_layerStack)
            layer->entityDestroyed(entity);
    }


    std::set<ecs::Entity> Scene::getEntities() const
    {
        std::set<ecs::Entity> entities;
        entities.insert(m_globalEntities.begin(), m_globalEntities.end());
        for (auto const &layer: m_layerStack)
            entities.insert(layer->getEntities().begin(), layer->getEntities().end());
        return entities;
    }

    void Scene::attachCameraToLayer(const std::shared_ptr<camera::Camera> &camera, const std::string &layerName)
    {
        if (const auto layer = m_layerStack[layerName]; layer != nullptr)
        {
            layer->attachCamera(camera);
            LOG(NEXO_DEV, "Scene::{}::attachCameraToLayerByName: camera added to layer {}", name, layer->name);
        }
    }

    void Scene::detachCameraFromLayer(const std::string &layerName)
    {
        if (const auto layer = m_layerStack[layerName]; layer != nullptr)
        {
            layer->detachCamera();
            LOG(NEXO_DEV, "Scene::{}::detachCameraFromLayerByName: camera removed from layer {}", name, layer->name);
        }
    }

    std::shared_ptr<camera::Camera> Scene::getCameraLayer(const std::string &layerName)
    {
        if (const auto layer = m_layerStack[layerName]; layer != nullptr)
        {
            return layer->getCamera();
        }
        return nullptr;
    }

    void Scene::setLayerRenderStatus(const bool status, const std::string &layerName)
    {
        if (const auto layer = m_layerStack[layerName]; layer != nullptr)
        {
            layer->isRendered = status;
            std::string strStatus = status ? "rendered" : "not rendered";
            LOG(NEXO_DEV, "Scene::{}::setLayerRenderStatusByName: layer {} is now {}", name, layer->name, strStatus);
        }
    }

    void Scene::setLayerActiveStatus(bool status, const std::string &layerName)
    {
        if (const auto layer = m_layerStack[layerName]; layer != nullptr)
        {
            layer->isRendered = status;
            std::string strStatus = status ? "active" : "unactive";
            LOG(NEXO_DEV, "Scene::{}::setLayerActiveStatusByName: layer {} is now {}", name, layer->name, strStatus);
        }
    }

    bool Scene::getLayerRenderStatus(const std::string &layerName) const
    {
        if (const auto layer = m_layerStack[layerName]; layer != nullptr)
            return layer->isRendered;
        return false;
    }

    bool Scene::getLayerActiveStatus(const std::string &layerName) const
    {
        if (const auto layer = m_layerStack[layerName]; layer != nullptr)
            return layer->isActive;
        return false;
    }

    void Scene::onUpdate(const core::Timestep timestep)
    {
        for (const auto &layer: m_layerStack)
        {
            if (layer->isActive)
                layer->onUpdate(timestep);
        }
    }

    void Scene::onRender()
    {
        for (const auto &layer: m_layerStack)
        {
            if (layer->isRendered)
                layer->onRender();
        }
    }

    void Scene::dispatchEventToLayers(event::Event &event)
    {
        const std::type_index eventType(typeid(event));
        for (const auto &layer: m_layerStack)
        {
            if (const auto &listenedEventTypes = layer->getListenedEventTypes();
                layer->isActive && listenedEventTypes.find(eventType) != listenedEventTypes.end())
            {
                event.trigger(*layer);
                if (event.consumed)
                    return;
            }
        }
    }
}
