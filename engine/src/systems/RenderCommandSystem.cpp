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
#include "Logger.hpp"
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
#include "renderPasses/GPUResources.hpp"
#include "renderPasses/Masks.hpp"
#include "renderer/DrawCommand.hpp"
#include "renderer/ShaderLibrary.hpp"
#include "SystemProfiler.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <unistd.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <algorithm>

namespace nexo::system {

    static renderer::DrawCommand createOutlineDrawCommand(const components::CameraContext &camera)
    {
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

        cmd.setUniform("uTime", static_cast<float>(glfwGetTime()));
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

        const auto transformSpan = get<components::TransformComponent>();
        const auto meshSpan      = get<components::StaticMeshComponent>();
        const auto materialSpan  = get<components::MaterialComponent>();

        // Profile draw command creation
        std::vector<RenderItem> renderItems;
        renderItems.reserve(partition->count);
        for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
            const ecs::Entity entity = entitySpan[i];
            if (coord->entityHasComponent<components::CameraComponent>(entity) && sceneType != SceneType::EDITOR)
                continue;

            const auto &transform     = transformSpan[i];
            const auto &materialAsset = materialSpan[i].material.lock();

            std::string shaderStr;
            std::shared_ptr<renderer::NxShader> shader;
            shaderStr = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->shader : "";
            shader = renderer::ShaderLibrary::getInstance().get(shaderStr);
            if (!shader) continue;

            RenderItem item;
            item.entity        = entity;
            item.shader        = shader;
            item.mesh          = meshSpan[i].vao;
            item.material      = materialAsset;
            item.filterMask    = renderer::F_FORWARD_PASS;
            item.isTransparent = false;
            item.modelMatrix = transform.worldMatrix;
            renderItems.push_back(item);

            if (coord->entityHasComponent<components::SelectedTag>(entity)) {
                RenderItem sel = item;
                sel.filterMask = renderer::F_OUTLINE_MASK;
                sel.entity = entity;
                sel.mesh = meshSpan[i].vao;
                sel.modelMatrix = transform.worldMatrix;
                // Optional: use a dedicated mask shader if different
                if (materialAsset->getData()->isOpaque) {
                    sel.shader = renderer::ShaderLibrary::getInstance().get("Flat color");
                } else {
                    sel.shader = renderer::ShaderLibrary::getInstance().get("Albedo unshaded transparent");
                }
                sel.material = materialAsset;
                // material can be nullptr or same; we won’t use heavy material uniforms anyway
                renderItems.push_back(sel);
            }
        }

        std::sort(renderItems.begin(), renderItems.end(),
            [](const RenderItem& a, const RenderItem& b) {
                if (a.filterMask     != b.filterMask)     return a.filterMask     < b.filterMask;
                if (a.shader         != b.shader)         return a.shader         < b.shader;
                if (a.material       != b.material)       return a.material       < b.material;
                if (a.mesh           != b.mesh)           return a.mesh           < b.mesh;
                return false;
            });

        std::vector<renderer::GpuInstanceData> instances;
        instances.reserve(renderItems.size());
        uint32_t index = 0;
        for (auto& item : renderItems) {
            renderer::GpuInstanceData inst{};
            inst.model    = item.modelMatrix;
            inst.entityId = static_cast<int>(item.entity);
            instances.push_back(inst);

            item.instanceIndex = index;
            ++index;
        }

        std::vector<RenderBatch> batches;
        if (!renderItems.empty()) {
            RenderBatch current{};
            auto& first = renderItems[0];

            current.shader        = first.shader;
            current.mesh          = first.mesh;
            current.material      = first.material;
            current.filterMask    = first.filterMask;
            current.instanceOffset = first.instanceIndex;
            current.instanceCount  = 1;

            for (size_t i = 1; i < renderItems.size(); ++i) {
                const auto& item = renderItems[i];

                bool sameBatch =
                    item.shader     == current.shader &&
                    item.mesh->getId()       == current.mesh->getId() &&
                    item.material   == current.material &&
                    item.filterMask == current.filterMask;

                if (sameBatch) {
                    current.instanceCount++;
                } else {
                    batches.push_back(current);
                    current.shader        = item.shader;
                    current.mesh          = item.mesh;
                    current.material      = item.material;
                    current.filterMask    = item.filterMask;
                    current.instanceOffset = item.instanceIndex;
                    current.instanceCount  = 1;
                }
            }

            batches.push_back(current);
        }

        std::vector<renderer::DrawCommand> drawCommands;
        drawCommands.reserve(batches.size() * 2);
        for (const auto& batch : batches) {
            renderer::DrawCommand cmd;
            cmd.vao        = batch.mesh;
            cmd.shader     = batch.shader;
            cmd.filterMask = batch.filterMask;
            cmd.instanceOffset = batch.instanceOffset;
            cmd.instanceCount  = batch.instanceCount;
            cmd.instanced      = (batch.instanceCount > 1);
            if (cmd.instanced) {
                LOG(NEXO_INFO, "Instanced batch detected");
            }

            // Set per-batch uniforms:
            cmd.setUniform("uInstanceOffset", static_cast<int>(batch.instanceOffset));
            if (batch.filterMask == renderer::F_FORWARD_PASS) {
                cmd.setUniform("uMaterial.albedoColor",
                    batch.material && batch.material->isLoaded() ? batch.material->getData()->albedoColor : glm::vec4(0.0f));
                const auto albedoTextureAsset =
                    batch.material && batch.material->isLoaded() ? batch.material->getData()->albedoTexture.lock() : nullptr;
                const auto albedoTexture =
                    albedoTextureAsset && albedoTextureAsset->isLoaded() ? albedoTextureAsset->getData()->texture : nullptr;
                cmd.setUniform("uMaterial.albedoTexIndex", renderer::NxRenderer3D::get().getTextureIndex(albedoTexture));

                cmd.setUniform("uMaterial.specularColor",
                    batch.material && batch.material->isLoaded() ? batch.material->getData()->specularColor : glm::vec4(0.0f));
                const auto specularTextureAsset =
                    batch.material && batch.material->isLoaded() ? batch.material->getData()->metallicMap.lock() : nullptr;
                const auto specularTexture = specularTextureAsset && specularTextureAsset->isLoaded() ?
                                                 specularTextureAsset->getData()->texture : nullptr;
                cmd.setUniform("uMaterial.specularTexIndex", renderer::NxRenderer3D::get().getTextureIndex(specularTexture));

                cmd.setUniform("uMaterial.emissiveColor",
                    batch.material && batch.material->isLoaded() ? batch.material->getData()->emissiveColor : glm::vec3(0.0f));
                const auto emissiveTextureAsset =
                    batch.material && batch.material->isLoaded() ? batch.material->getData()->emissiveMap.lock() : nullptr;
                const auto emissiveTexture = emissiveTextureAsset && emissiveTextureAsset->isLoaded() ?
                                                 emissiveTextureAsset->getData()->texture : nullptr;
                cmd.setUniform("uMaterial.emissiveTexIndex", renderer::NxRenderer3D::get().getTextureIndex(emissiveTexture));

                cmd.setUniform("uMaterial.roughness",
                    batch.material && batch.material->isLoaded() ? batch.material->getData()->roughness : 1.0f);
                const auto roughnessTextureAsset =
                    batch.material && batch.material->isLoaded() ? batch.material->getData()->roughnessMap.lock() : nullptr;
                const auto roughnessTexture = roughnessTextureAsset && roughnessTextureAsset->isLoaded() ?
                                                  roughnessTextureAsset->getData()->texture : nullptr;
                cmd.setUniform("uMaterial.roughnessTexIndex", renderer::NxRenderer3D::get().getTextureIndex(roughnessTexture));
            } else if (batch.filterMask == renderer::F_OUTLINE_MASK) {
                cmd.setUniform("uMaterial.albedoColor",
                    batch.material && batch.material->isLoaded() ? batch.material->getData()->albedoColor : glm::vec4(0.0f));
                const auto albedoTextureAsset =
                    batch.material && batch.material->isLoaded() ? batch.material->getData()->albedoTexture.lock() : nullptr;
                const auto albedoTexture =
                    albedoTextureAsset && albedoTextureAsset->isLoaded() ? albedoTextureAsset->getData()->texture : nullptr;
                cmd.setUniform("uMaterial.albedoTexIndex", renderer::NxRenderer3D::get().getTextureIndex(albedoTexture));
            }

            drawCommands.push_back(cmd);
        }

        std::cout << " Draw count " << drawCommands.size() << " commands" << std::endl;
        for (auto &camera : renderContext.cameras) {
            camera.pipeline.setStorageBufferData(INSTANCE_BUFFER, instances.data(), sizeof(renderer::GpuInstanceData) * instances.size());
            camera.pipeline.addDrawCommands(drawCommands);

            if (sceneType == SceneType::EDITOR && renderContext.gridParams.enabled)
                camera.pipeline.addDrawCommand(createGridDrawCommand(camera, renderContext));

            if (sceneType == SceneType::EDITOR)
                camera.pipeline.addDrawCommand(createOutlineDrawCommand(camera));
        }
    }
} // namespace nexo::system
