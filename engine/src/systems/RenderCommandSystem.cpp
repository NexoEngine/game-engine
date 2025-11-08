//// RenderCommandSystem.cpp //////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        29/10/2025
//  Description: Source file for the render system
//
///////////////////////////////////////////////////////////////////////////////

#include "RenderCommandSystem.hpp"
#include "Application.hpp"
#include "Renderer3D.hpp"
#include "components/Camera.hpp"
#include "components/Editor.hpp"
#include "components/Light.hpp"
#include "components/Render3D.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/StaticMesh.hpp"
#include "components/Transform.hpp"
#include "core/event/Input.hpp"
#include "math/Projection.hpp"
#include "math/Vector.hpp"
#include "renderPasses/Masks.hpp"
#include "renderer/DrawCommand.hpp"
#include "renderer/ShaderLibrary.hpp"
#include "SystemProfiler.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <unistd.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace nexo::system {

    static renderer::DrawCommand createOutlineDrawCommand(const components::CameraContext &camera)
    {
        PROFILE_SCOPE("RenderCommandSystem::createOutlineDrawCommand");

        renderer::DrawCommand cmd;
        cmd.type       = renderer::CommandType::FULL_SCREEN;
        cmd.filterMask = 0;
        cmd.filterMask |= renderer::F_OUTLINE_PASS;
        cmd.shader = renderer::ShaderLibrary::getInstance().get("Outline pulse flat");

        cmd.setUniform("uViewProjection", camera.viewProjectionMatrix);
        cmd.setUniform("uCamPos", camera.cameraPosition);

        cmd.setUniform("uMaskTexture", 0);
        cmd.setUniform("uDepthTexture", 1);
        cmd.setUniform("uDepthMaskTexture", 2);
        cmd.setUniform("uTime", static_cast<float>(glfwGetTime()));
        const glm::vec2 screenSize = {camera.renderTargetSize.x, camera.renderTargetSize.y};
        cmd.setUniform("uScreenSize", screenSize);
        cmd.setUniform("uOutlineWidth", 10.0f);
        return cmd;
    }

    static renderer::DrawCommand createGridDrawCommand(const components::CameraContext &camera,
                                                       const components::RenderContext &renderContext)
    {
        PROFILE_SCOPE("RenderCommandSystem::createGridDrawCommand");

        renderer::DrawCommand cmd;
        cmd.type       = renderer::CommandType::FULL_SCREEN;
        cmd.filterMask = 0;
        cmd.filterMask |= renderer::F_GRID_PASS;
        cmd.shader = renderer::ShaderLibrary::getInstance().get("Grid shader");

        cmd.setUniform("uViewProjection", camera.viewProjectionMatrix);
        cmd.setUniform("uCamPos", camera.cameraPosition);

        const components::RenderContext::GridParams &gridParams = renderContext.gridParams;
        cmd.setUniform("uGridSize", gridParams.gridSize);
        cmd.setUniform("uGridCellSize", gridParams.cellSize);
        cmd.setUniform("uGridMinPixelsBetweenCells", gridParams.minPixelsBetweenCells);
        constexpr glm::vec4 gridColorThin  = {0.5f, 0.55f, 0.7f, 0.6f};
        constexpr glm::vec4 gridColorThick = {0.7f, 0.75f, 0.9f, 0.8f};
        cmd.setUniform("uGridColorThin", gridColorThin);
        cmd.setUniform("uGridColorThick", gridColorThick);

        {
            PROFILE_SCOPE("RenderCommandSystem::createGridDrawCommand::MouseWorldPos");
            const glm::vec2 globalMousePos   = event::getMousePosition();
            glm::vec3 mouseWorldPos          = camera.cameraPosition; // Default position (camera position)
            const glm::vec2 renderTargetSize = camera.renderTargetSize;

            if (renderContext.isChildWindow) {
                // viewportBounds[0] is min (top-left), viewportBounds[1] is max (bottom-right)
                const glm::vec2 &viewportMin = renderContext.viewportBounds[0];
                const glm::vec2 &viewportMax = renderContext.viewportBounds[1];
                const glm::vec2 viewportSize(viewportMax.x - viewportMin.x, viewportMax.y - viewportMin.y);

                // Check if mouse is within the viewport bounds
                if (math::isPosInBounds(globalMousePos, viewportMin, viewportMax)) {
                    // Calculate relative mouse position within the viewport
                    glm::vec2 relativeMousePos(globalMousePos.x - viewportMin.x, globalMousePos.y - viewportMin.y);

                    // Convert to normalized coordinates [0,1]
                    glm::vec2 normalizedPos(relativeMousePos.x / viewportSize.x, relativeMousePos.y / viewportSize.y);

                    // Convert to framebuffer coordinates
                    glm::vec2 framebufferPos(normalizedPos.x * renderTargetSize.x, normalizedPos.y * renderTargetSize.y);

                    // Project ray
                    const glm::vec3 rayDir = math::projectRayToWorld(
                        framebufferPos.x, framebufferPos.y, camera.viewProjectionMatrix, camera.cameraPosition,
                        static_cast<unsigned int>(renderTargetSize.x), static_cast<unsigned int>(renderTargetSize.y));

                    // Calculate intersection with y=0 plane (grid plane)
                    if (rayDir.y != 0.0f) {
                        float t = -camera.cameraPosition.y / rayDir.y;
                        if (t > 0.0f) {
                            mouseWorldPos = camera.cameraPosition + rayDir * t;
                        }
                    }
                }
            } else {
                const glm::vec3 rayDir = math::projectRayToWorld(
                    globalMousePos.x, globalMousePos.y, camera.viewProjectionMatrix, camera.cameraPosition,
                    static_cast<unsigned int>(renderTargetSize.x), static_cast<unsigned int>(renderTargetSize.y));

                if (rayDir.y != 0.0f) {
                    float t = -camera.cameraPosition.y / rayDir.y;
                    if (t > 0.0f) {
                        mouseWorldPos = camera.cameraPosition + rayDir * t;
                    }
                }
            }

            cmd.setUniform("uMouseWorldPos", mouseWorldPos);
        }

        cmd.setUniform("uTime", static_cast<float>(glfwGetTime()));
        return cmd;
    }

    static renderer::DrawCommand createSelectedDrawCommand(const components::StaticMeshComponent &mesh,
                                                           const std::shared_ptr<assets::Material> &materialAsset,
                                                           const components::TransformComponent &transform)
    {
        PROFILE_SCOPE("RenderCommandSystem::createSelectedDrawCommand");

        renderer::DrawCommand cmd;
        cmd.vao             = mesh.vao;
        const bool isOpaque = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->isOpaque : true;
        if (isOpaque) {
            cmd.shader = renderer::ShaderLibrary::getInstance().get("Flat color");
        } else {
            cmd.shader = renderer::ShaderLibrary::getInstance().get("Albedo unshaded transparent");
            cmd.setUniform("uMaterial.albedoColor",
                materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoColor : glm::vec4(0.0f));
            const auto albedoTextureAsset =
                materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoTexture.lock() : nullptr;
            const auto albedoTexture =
                albedoTextureAsset && albedoTextureAsset->isLoaded() ? albedoTextureAsset->getData()->texture : nullptr;
            cmd.setUniform("uMaterial.albedoTexIndex", renderer::NxRenderer3D::get().getTextureIndex(albedoTexture));
        }
        cmd.setUniform("uMatModel", transform.worldMatrix);
        cmd.filterMask = 0;
        cmd.filterMask = renderer::F_OUTLINE_MASK;
        return cmd;
    }

    static renderer::DrawCommand createDrawCommand(const ecs::Entity entity,
                                                   const std::shared_ptr<renderer::NxShader> &shader,
                                                   const components::StaticMeshComponent &mesh,
                                                   const std::shared_ptr<assets::Material> &materialAsset,
                                                   const components::TransformComponent &transform)
    {
        PROFILE_SCOPE("RenderCommandSystem::createDrawCommand");

        renderer::DrawCommand cmd;
        cmd.vao    = mesh.vao;
        cmd.shader = shader;
        cmd.setUniform("uMatModel", transform.worldMatrix);
        cmd.setUniform("uEntityId", static_cast<int>(entity));

        {
            PROFILE_SCOPE("RenderCommandSystem::createDrawCommand::MaterialSetup");
            cmd.setUniform("uMaterial.albedoColor",
                materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoColor : glm::vec4(0.0f));
            const auto albedoTextureAsset =
                materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoTexture.lock() : nullptr;
            const auto albedoTexture =
                albedoTextureAsset && albedoTextureAsset->isLoaded() ? albedoTextureAsset->getData()->texture : nullptr;
            cmd.setUniform("uMaterial.albedoTexIndex", renderer::NxRenderer3D::get().getTextureIndex(albedoTexture));

            cmd.setUniform("uMaterial.specularColor",
                materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->specularColor : glm::vec4(0.0f));
            const auto specularTextureAsset =
                materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->metallicMap.lock() : nullptr;
            const auto specularTexture = specularTextureAsset && specularTextureAsset->isLoaded() ?
                                             specularTextureAsset->getData()->texture : nullptr;
            cmd.setUniform("uMaterial.specularTexIndex", renderer::NxRenderer3D::get().getTextureIndex(specularTexture));

            cmd.setUniform("uMaterial.emissiveColor",
                materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->emissiveColor : glm::vec3(0.0f));
            const auto emissiveTextureAsset =
                materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->emissiveMap.lock() : nullptr;
            const auto emissiveTexture = emissiveTextureAsset && emissiveTextureAsset->isLoaded() ?
                                             emissiveTextureAsset->getData()->texture : nullptr;
            cmd.setUniform("uMaterial.emissiveTexIndex", renderer::NxRenderer3D::get().getTextureIndex(emissiveTexture));

            cmd.setUniform("uMaterial.roughness",
                materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->roughness : 1.0f);
            const auto roughnessTextureAsset =
                materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->roughnessMap.lock() : nullptr;
            const auto roughnessTexture = roughnessTextureAsset && roughnessTextureAsset->isLoaded() ?
                                              roughnessTextureAsset->getData()->texture : nullptr;
            cmd.setUniform("uMaterial.roughnessTexIndex", renderer::NxRenderer3D::get().getTextureIndex(roughnessTexture));
        }

        cmd.filterMask = 0;
        cmd.filterMask |= renderer::F_FORWARD_PASS;
        return cmd;
    }

    void RenderCommandSystem::update()
    {
        const std::span<const ecs::Entity> entitySpan = m_group->entities();
        PROFILE_SYSTEM("RenderCommandSystem", entitySpan.size());

        auto &renderContext = getSingleton<components::RenderContext>();
        if (renderContext.sceneRendered == -1) return;

        const auto sceneRendered  = static_cast<unsigned int>(renderContext.sceneRendered);
        const SceneType sceneType = renderContext.sceneType;

        // Profile scene partition lookup
        const auto scenePartition = m_group->getPartitionView<components::SceneTag, unsigned int>(
            [](const components::SceneTag &tag) { return tag.id; });
        const auto *partition        = scenePartition.getPartition(sceneRendered);
        auto &app                    = Application::getInstance();
        const std::string &sceneName = app.getSceneManager().getScene(sceneRendered).getName();
        if (!partition) {
            LOG_ONCE(NEXO_WARN, "Nothing to render in scene {}, skipping", sceneName);
            return;
        }
        Logger::resetOnce(NEXO_LOG_ONCE_KEY("Nothing to render in scene {}, skipping", sceneName));

        // Profile component span retrieval
        const auto transformSpan = get<components::TransformComponent>();
        const auto meshSpan      = get<components::StaticMeshComponent>();
        const auto materialSpan  = get<components::MaterialComponent>();

        // Profile draw command creation
        std::vector<renderer::DrawCommand> drawCommands;
        {
            PROFILE_SCOPE_ENTITIES("RenderCommandSystem::DrawCommandCreation", partition->count);
            drawCommands.reserve(partition->count * 2); // Pre-allocate for potential selected entities

            for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
                const ecs::Entity entity = entitySpan[i];
                if (coord->entityHasComponent<components::CameraComponent>(entity) && sceneType != SceneType::EDITOR)
                    continue;

                const auto &transform     = transformSpan[i];
                const auto &materialAsset = materialSpan[i].material.lock();

                std::string shaderStr;
                std::shared_ptr<renderer::NxShader> shader;
                {
                    PROFILE_SCOPE("RenderCommandSystem::ShaderLookup");
                    shaderStr = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->shader : "";
                    const auto &mesh = meshSpan[i];
                    shader = renderer::ShaderLibrary::getInstance().get(shaderStr);
                    if (!shader) continue;
                }

                drawCommands.push_back(createDrawCommand(entity, shader, meshSpan[i], materialAsset, transform));

                if (coord->entityHasComponent<components::SelectedTag>(entity)) {
                    drawCommands.push_back(createSelectedDrawCommand(meshSpan[i], materialAsset, transform));
                }
            }
        }

        // Profile camera and uniform setup
        {
            PROFILE_SCOPE_ENTITIES("RenderCommandSystem::CameraSetup", renderContext.cameras.size());
            for (auto &camera : renderContext.cameras) {
                {
                    PROFILE_SCOPE_ENTITIES("RenderCommandSystem::UniformUpdates", drawCommands.size());
                }

                {
                    PROFILE_SCOPE("RenderCommandSystem::PipelineAddCommands");
                    camera.pipeline.addDrawCommands(drawCommands);
                }

                if (sceneType == SceneType::EDITOR && renderContext.gridParams.enabled) {
                    PROFILE_SCOPE("RenderCommandSystem::GridDrawCommand");
                    camera.pipeline.addDrawCommand(createGridDrawCommand(camera, renderContext));
                }

                if (sceneType == SceneType::EDITOR) {
                    PROFILE_SCOPE("RenderCommandSystem::OutlineDrawCommand");
                    camera.pipeline.addDrawCommand(createOutlineDrawCommand(camera));
                }
            }
        }
    }
} // namespace nexo::system
