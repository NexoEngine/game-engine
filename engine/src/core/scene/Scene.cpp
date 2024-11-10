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
    }

    void Scene::addOverlay(const std::shared_ptr<layer::Layer> &overlay)
    {
        m_layerStack.pushOverlay(overlay);
    }

    void Scene::removeLayer(const std::shared_ptr<layer::Layer> &layer)
    {
        m_layerStack.popLayer(layer);
    }

    void Scene::removeOverlay(const std::shared_ptr<layer::Layer> &overlay)
    {
        m_layerStack.popOverlay(overlay);
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
            return;
        }
        if (layerIndex >= 0 && static_cast<unsigned int>(layerIndex) < m_layerStack.size())
        {
            m_layerStack[layerIndex]->addEntity(entity);
        } else
            std::cout << "ERROR::SCENE::addEntity: index out of bounds!" << std::endl;
    }

    void Scene::removeEntity(const ecs::Entity entity, const int layerIndex)
    {
        if (layerIndex == -1)
        {
            m_globalEntities.erase(entity);
            return;
        }
        if (layerIndex >= 0 && static_cast<unsigned int>(layerIndex) < m_layerStack.size())
        {
            m_layerStack[layerIndex]->removeEntity(entity);
        } else
            std::cout << "ERROR::SCENE::removeEntity: index out of bounds!" << std::endl;
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
            return;
        }
        std::cout << "ERROR::SCENE::attachCamera: layer index out of bounds" << std::endl;
    }

    void Scene::detachCamera(const unsigned int layerIndex)
    {
        if (layerIndex < m_layerStack.size())
        {
            m_layerStack[layerIndex]->detachCamera();
            return;
        }
        std::cout << "ERROR::SCENE::detachCamera: layer index out of bounds" << std::endl;
    }

    void Scene::setLayerRenderStatus(const bool status, const unsigned int layerIndex)
    {
        if (layerIndex < m_layerStack.size())
        {
            m_layerStack[layerIndex]->setRenderStatus(status);
            return;
        }
        std::cout << "ERROR:SCENE::setLayerRenderStatus: layer index out of bounds" << std::endl;
    }

    void Scene::setLayerActiveStatus(const bool status, const unsigned int layerIndex)
    {
        if (layerIndex < m_layerStack.size())
        {
            m_layerStack[layerIndex]->setActiveStatus(status);
            return;
        }
        std::cout << "ERROR:SCENE::setLayerActiveStatus: layer index out of bounds" << std::endl;
    }

    bool Scene::getLayerRenderStatus(const unsigned int layerIndex) const
    {
        if (layerIndex < m_layerStack.size())
        {
            return m_layerStack[layerIndex]->isRendered();
        }
        std::cout << "ERROR::SCENE::getLayerRenderStatus: layer index out of bounds" << std::endl;
        return false;
    }

    bool Scene::getLayerActiveStatus(const unsigned int layerIndex) const
    {
        if (layerIndex < m_layerStack.size())
            return m_layerStack[layerIndex]->isActive();
        std::cout << "ERROR::SCENE::getLayerActiveStatus: layer index out of bounds" << std::endl;
        return false;
    }

    std::shared_ptr<nexo::camera::Camera> &Scene::getCamera(const unsigned int layerIndex)
    {
        static std::shared_ptr<nexo::camera::Camera> nullCamera = nullptr;
        if (layerIndex < m_layerStack.size())
            return m_layerStack[layerIndex]->getCamera();
        std::cout << "ERROR::SCENE::getCamera: layer index out of bounds" << std::endl;
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
