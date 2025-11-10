//// CameraFactory.cpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Source file for the camera factory static class
//
///////////////////////////////////////////////////////////////////////////////

#include "CameraFactory.hpp"

#include <utility>
#include <array>
#include "Application.hpp"
#include "RenderPipeline.hpp"
#include "ShaderStorageBuffer.hpp"
#include "ShaderUniformBuffer.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "renderPasses/ForwardPass.hpp"
#include "renderPasses/GPUResources.hpp"

namespace nexo {
    ecs::Entity CameraFactory::createPerspectiveCamera(glm::vec3 pos, unsigned int width, unsigned int height,
                                                       std::shared_ptr<renderer::NxFramebuffer> renderTarget,
                                                       const glm::vec4 &clearColor, float fov, float nearPlane,
                                                       float farPlane)
    {
        components::TransformComponent transform{};
        transform.pos = pos;

        components::CameraComponent camera{};
        camera.width     = width;
        camera.height    = height;
        camera.fov       = fov;
        camera.nearPlane = nearPlane;
        camera.farPlane  = farPlane;
        camera.type      = components::CameraType::PERSPECTIVE;

        auto forwardPass               = std::make_shared<renderer::ForwardPass>();
        renderer::PassId forwardPassId = camera.pipeline.addRenderPass(forwardPass);
        camera.pipeline.setFinalOutputPass(forwardPassId);
        camera.pipeline.setCameraClearColor(clearColor);
        if (renderTarget) {
            camera.m_renderTarget = std::move(renderTarget);
            camera.pipeline.setRenderTarget(camera.m_renderTarget);
        }
        camera.clearColor = clearColor;

        auto instanceBuffer = renderer::NxShaderStorageBuffer::create(sizeof(renderer::GpuInstanceData) * 1000);
        camera.pipeline.addStorageBuffer(INSTANCE_BUFFER, renderer::RESERVED_BINDING_POINTS.at(INSTANCE_BUFFER), instanceBuffer);
        auto perViewUBO = renderer::NxShaderUniformBuffer::create(sizeof(renderer::GpuPerView));
        camera.pipeline.addUniformBuffer(PER_VIEW_UBO, renderer::RESERVED_BINDING_POINTS.at(PER_VIEW_UBO), perViewUBO);
        auto lightUBO = renderer::NxShaderUniformBuffer::create(sizeof(renderer::GpuLightBlock));
        camera.pipeline.addUniformBuffer(LIGHT_UBO, renderer::RESERVED_BINDING_POINTS.at(LIGHT_UBO), lightUBO);
        auto materialSSBO = renderer::NxShaderStorageBuffer::create(sizeof(renderer::GpuMaterial) * 1000);
        camera.pipeline.addStorageBuffer(MATERIAL_BUFFER, renderer::RESERVED_BINDING_POINTS.at(MATERIAL_BUFFER), materialSSBO);
        camera.pipeline.setupShadowMaps();

        ecs::Entity newCamera = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newCamera, transform);
        Application::m_coordinator->addComponent<components::CameraComponent>(newCamera, camera);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newCamera, uuid);
        return newCamera;
    }
} // namespace nexo
