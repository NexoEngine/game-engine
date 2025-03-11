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
#include "core/scene/Scene.hpp"

#include <format>

namespace nexo::layer {
    //TODO: This should be done per shader material rather than globally, but for now we only have one shader
    void Layer::setupLights(const std::shared_ptr<renderer::RendererContext> &rendererContext,
                            const scene::SceneContext &sceneContext) const
    {

    }

    void Layer::onRender(std::shared_ptr<renderer::RendererContext> &rendererContext,
                         const scene::SceneContext &sceneContext)
    {
        if (!m_camera)
        {
            LOG(NEXO_WARN, "Layer::onRender(): no camera is found, disabling render");
            isRendered = false;
            return;
        }
        setupLights(rendererContext, sceneContext);
        for (const auto entity: m_entities)
        {
            const auto &transform = getComponent<components::TransformComponent>(entity);
            const auto &renderComponent = getComponent<components::RenderComponent>(entity);
            if (renderComponent.isRendered)
            {
             	rendererContext->renderer3D.beginScene(m_camera->getViewProjectionMatrix(), m_camera->getPosition());
                renderComponent.draw(rendererContext, transform, entity);
                rendererContext->renderer3D.endScene();
            }
        }
    }

    void Layer::onUpdate(Timestep ts)
    {
        if (m_camera)
            m_camera->onUpdate(ts);
    }

    void Layer::handleEvent(const event::IEvent &event)
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
        if (m_entities.erase(entity) != 0)
            LOG(NEXO_DEV, "Entity removed from layer");
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
