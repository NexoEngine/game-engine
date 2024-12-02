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

    void Scene::addLayer(LayerId layerId, const std::string &layerName)
    {
        const auto newLayer = std::make_shared<layer::Layer>(layerId, layerName);
        m_layerStack.pushLayer(newLayer);
        LOG(NEXO_DEV, "Scene::{}::addLayer: layer {} added", name, layerName);
    }

    void Scene::addOverlay(LayerId layerId, const std::string &overlayName)
    {
        const auto newOverlay = std::make_shared<layer::Layer>(layerId, overlayName);
        m_layerStack.pushOverlay(newOverlay);
        LOG(NEXO_DEV, "Scene::{}::addOverlay: overlay {} added", name, overlayName);
    }

    void Scene::removeLayer(LayerId layerId)
    {
        const auto layer = getLayer(layerId);
        m_layerStack.popLayer(layer);
        LOG(NEXO_DEV, "Scene::{}::removeLayer: layer {} removed", name, layerId);
    }

    void Scene::removeOverlay(LayerId layerId)
    {
        const auto overlay = getLayer(layerId);
        m_layerStack.popOverlay(overlay);
        LOG(NEXO_DEV, "Scene::{}::removeOverlay: overlay {} removed", name, layerId);
    }

    std::shared_ptr<layer::Layer> Scene::getLayer(const LayerId layerId) const
    {
        return m_layerStack.byId(layerId);
    }

    void Scene::addEntityToLayer(const ecs::Entity entity, const LayerId layerId)
    {
        if (const auto layer = m_layerStack.byId(layerId); layer != nullptr)
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

    void Scene::removeEntityFromLayer(const ecs::Entity entity, const LayerId layerId)
    {
        if (const auto layer = m_layerStack.byId(layerId); layer != nullptr)
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
        {
            layer->entityDestroyed(entity);
        }
    }

    std::set<ecs::Entity> Scene::getEntities() const
    {
        std::set<ecs::Entity> entities;
        entities.insert(m_globalEntities.begin(), m_globalEntities.end());
        for (auto const &layer: m_layerStack)
            entities.insert(layer->getEntities().begin(), layer->getEntities().end());
        return entities;
    }

    std::set<ecs::Entity> Scene::getRenderedEntities() const
    {
        std::set<ecs::Entity> entities;
        entities.insert(m_globalEntities.begin(), m_globalEntities.end());
        for (auto const &layer: m_layerStack)
        {
            if (layer->isRendered)
                entities.insert(layer->getEntities().begin(), layer->getEntities().end());
        }
        return entities;
    }

    void Scene::attachCameraToLayer(const std::shared_ptr<camera::Camera> &camera, const LayerId layerId)
    {
        if (const auto layer = m_layerStack.byId(layerId); layer != nullptr)
        {
            layer->attachCamera(camera);
            LOG(NEXO_DEV, "Scene::{}::attachCameraToLayerByName: camera added to layer {}", name, layer->name);
        }
    }

    void Scene::detachCameraFromLayer(const LayerId layerId)
    {
        if (const auto layer = m_layerStack.byId(layerId); layer != nullptr)
        {
            layer->detachCamera();
            LOG(NEXO_DEV, "Scene::{}::detachCameraFromLayerByName: camera removed from layer {}", name, layer->name);
        }
    }

    std::shared_ptr<camera::Camera> Scene::getCameraLayer(const LayerId layerId)
    {
        if (const auto layer = m_layerStack.byId(layerId); layer != nullptr)
        {
            return layer->getCamera();
        }
        return nullptr;
    }

    void Scene::setLayerRenderStatus(const bool status, const LayerId layerId)
    {
        if (const auto layer = m_layerStack.byId(layerId); layer != nullptr)
        {
            layer->isRendered = status;
            std::string strStatus = status ? "rendered" : "not rendered";
            LOG(NEXO_DEV, "Scene::{}::setLayerRenderStatusByName: layer {} is now {}", name, layer->name, strStatus);
        }
    }

    void Scene::setLayerActiveStatus(const bool status, const LayerId layerId)
    {
        if (const auto layer = m_layerStack.byId(layerId); layer != nullptr)
        {
            layer->isActive = status;
            std::string strStatus = status ? "active" : "unactive";
            LOG(NEXO_DEV, "Scene::{}::setLayerActiveStatusByName: layer {} is now {}", name, layer->name, strStatus);
        }
    }

    bool Scene::getLayerRenderStatus(const LayerId layerId) const
    {
        if (const auto layer = m_layerStack.byId(layerId); layer != nullptr)
            return layer->isRendered;
        return false;
    }

    bool Scene::getLayerActiveStatus(const LayerId layerId) const
    {
        if (const auto layer = m_layerStack.byId(layerId); layer != nullptr)
            return layer->isActive;
        return false;
    }

    unsigned int Scene::addLight(const std::shared_ptr<components::Light> &light)
    {
        if (nbLights >= m_lights.size())
        {
            LOG(NEXO_ERROR, "Scene::{}::addLight: Max number of lights reached", name);
            return 0;
        }
        if (light->type == components::LightType::DIRECTIONAL && nbDirectionalLights >= MAX_DIRECTIONAL_LIGHTS)
        {
            LOG(NEXO_ERROR, "Scene::{}::addLight: Max number of directional lights reached", name);
            return 0;
        }
        if (light->type == components::LightType::POINT && nbPointLights >= MAX_POINT_LIGHTS)
        {
            LOG(NEXO_ERROR, "Scene::{}::addLight: Max number of point lights reached", name);
            return 0;
        }
        if (light->type == components::LightType::DIRECTIONAL)
            nbDirectionalLights++;
        else
            nbPointLights++;
        m_lights[nbLights] = light;
        LOG(NEXO_INFO, "Scene::{}::addLight: New light added", name);
        return nbLights++;
    }

    void Scene::removeLight(const unsigned int index)
    {
        if (index >= nbLights)
        {
            LOG(NEXO_ERROR, "Scene::{}::removeLight: index out of range", name);
            return;
        }
        if (m_lights[index]->type == components::LightType::DIRECTIONAL)
            nbDirectionalLights--;
        else
            nbPointLights--;
        for (unsigned int i = index; i < nbLights - 1; ++i)
            m_lights[i] = m_lights[i + 1];
        m_lights[--nbLights] = nullptr;
        LOG(NEXO_INFO, "Scene::{}::removeLight: Light removed", name);
    }

    void Scene::onUpdate(const Timestep timestep) const
    {
        for (const auto &layer: m_layerStack)
        {
            if (layer->isActive)
                layer->onUpdate(timestep);
        }
    }

    void Scene::onRender()
    {
        SceneContext context;
        context.lightContext.m_lights = m_lights;
        context.lightContext.ambientLight = ambientLight;
        context.lightContext.nbLights = nbLights;
        context.lightContext.nbDirectionalLights = nbDirectionalLights;
        context.lightContext.nbPointLights = nbPointLights;
        for (const auto &layer: m_layerStack)
        {
            if (layer->isRendered)
                layer->onRender(m_rendererContext, context);
        }
    }

    void Scene::dispatchEventToLayers(event::IEvent &event)
    {
        const std::type_index eventType(typeid(event));
        for (const auto &layer: m_layerStack)
        {
            if (const auto &listenedEventTypes = layer->getListenedEventTypes();
                layer->isActive && listenedEventTypes.contains(eventType))
            {
                event.trigger(*layer);
                if (event.consumed)
                    return;
            }
        }
    }
}
