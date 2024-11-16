//// Layer.cpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        12/11/2024
//  Description: Source file for the layer class
//
///////////////////////////////////////////////////////////////////////////////

#include "Layer.hpp"
#include "components/Components.hpp"
#include "renderer/Renderer2D.hpp"

namespace nexo::layer {

    void Layer::onRender()
    {
        if (!m_camera)
        {
            LOG(NEXO_WARN, "Layer::onRender(): no camera is found, disabling render");
            isRendered = false;
            return;
        }
        renderer::Renderer2D::beginScene(m_camera->getViewProjectionMatrix());
        for (const auto entity : m_entities)
        {
            auto transform = getComponent<components::TransformComponent>(entity);
            auto renderComponent = getComponent<components::RenderComponent>(entity);
            if (renderComponent.isRendered)
                renderComponent.draw(transform);
        }
        renderer::Renderer2D::endScene();
    }

    void Layer::onUpdate(core::Timestep ts)
    {
        if (m_camera)
            m_camera->onUpdate(ts);
    }

    void Layer::handleEvent(const event::Event &event)
    {
        if (const auto handler = m_callbackEventFunctions.at(typeid(event)))
            handler(event);
    }

    void Layer::addEntity(const ecs::Entity entity)
    {
        if (!hasComponent<components::TransformComponent>(entity) || !hasComponent<components::RenderComponent>(entity))
        {
            LOG(NEXO_ERROR, "Layer::{}::addEntity: entity {} does not have necessary components", name, entity);
            return;
        }
        m_entities.insert(entity);
    }

    void Layer::removeEntity(const ecs::Entity entity)
    {
        if (!m_entities.contains(entity))
        {
            LOG(NEXO_WARN, "Layer::{}::removeEntity: Entity {} not found", name, entity);
            return;
        }
        m_entities.erase(entity);
    }

    void Layer::entityDestroyed(const ecs::Entity entity)
    {
        m_entities.erase(entity);
    }

    void Layer::attachCamera(const std::shared_ptr<camera::Camera> &camera)
    {
        if (camera == nullptr)
        {
            LOG(NEXO_ERROR, "Layer::{}::attachCamera: Camera cannot be null", name);
            return;
        }
        m_camera = camera;
        isRendered = true;
    }

    void Layer::detachCamera()
    {
        m_camera.reset();
        isRendered = false;
    }

}