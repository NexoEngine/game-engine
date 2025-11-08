//// RenderBillboardSystem.cpp ////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/05/2025
//  Description: Source file for the billboard rendering system
//
///////////////////////////////////////////////////////////////////////////////

#include "RenderBillboardSystem.hpp"
#include "Application.hpp"
#include "components/BillboardMesh.hpp"
#include "components/Editor.hpp"
#include "renderPasses/GPUResources.hpp"
#include "renderPasses/Masks.hpp"
#include "renderer/Renderer3D.hpp"
#include "renderer/ShaderLibrary.hpp"
#include "SystemProfiler.hpp"

namespace nexo::system {

    static glm::mat4 createBillboardTransformMatrix(const glm::vec3 &cameraPosition,
                                                    const components::TransformComponent &transform,
                                                    const glm::vec3 &cameraUp = glm::vec3(0.0f, 1.0f, 0.0f),
                                                    bool constrainToY         = false)
    {
        glm::vec3 look = glm::normalize(cameraPosition - transform.pos);

        if (constrainToY) {
            look.y = 0.0f;
            look   = glm::normalize(look);

            glm::vec3 right = glm::normalize(glm::cross(cameraUp, look));
            glm::vec3 up    = glm::cross(look, right);

            return {glm::vec4(right, 0.0f), glm::vec4(up, 0.0f),
                    glm::vec4(-look, 0.0f), // Negative look preserves winding
                    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)};
        }
        const glm::vec3 right = glm::normalize(glm::cross(cameraUp, look));
        const glm::vec3 up    = glm::cross(look, right);

        return {glm::vec4(right, 0.0f), glm::vec4(up, 0.0f), glm::vec4(-look, 0.0f), // Negative look preserves winding
                glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)};
    }

    void RenderBillboardSystem::update()
    {
        const std::span<const ecs::Entity> entitySpan = m_group->entities();
        PROFILE_SYSTEM("RenderBillboardSystem", entitySpan.size());

        auto& renderContext = getSingleton<components::RenderContext>();
        if (renderContext.sceneRendered == -1) return;

        const auto sceneRendered  = static_cast<unsigned int>(renderContext.sceneRendered);
        const SceneType sceneType = renderContext.sceneType;

        const auto scenePartition = m_group->getPartitionView<components::SceneTag, unsigned int>(
            [](const components::SceneTag& tag) { return tag.id; });
        const auto* partition        = scenePartition.getPartition(sceneRendered);
        auto& app                    = Application::getInstance();
        const std::string& sceneName = app.getSceneManager().getScene(sceneRendered).getName();
        if (!partition) {
            LOG_ONCE(NEXO_WARN, "Nothing to render (billboards) in scene {}, skipping", sceneName);
            return;
        }
        Logger::resetOnce(NEXO_LOG_ONCE_KEY("Nothing to render (billboards) in scene {}, skipping", sceneName));

        const auto transformComponentArray = get<components::TransformComponent>();
        const auto billboardSpan = get<components::BillboardComponent>();
        const auto materialComponentArray  = get<components::MaterialComponent>();

        for (auto& camera : renderContext.cameras) {
            std::vector<RenderItem> renderItems;
            renderItems.reserve(partition->count);
            std::vector<renderer::GpuMaterial> gpuMaterials;
            gpuMaterials.reserve(partition->count);
            std::unordered_map<const void*, uint32_t> materialToIndex;
            materialToIndex.reserve(partition->count);
            for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
                const ecs::Entity entity = entitySpan[i];

                if (coord->entityHasComponent<components::CameraComponent>(entity) && sceneType != SceneType::EDITOR)
                    continue;

                const auto& transform     = transformComponentArray->get(entity);
                const auto& billboard     = billboardSpan[i];
                const auto& materialAsset = materialComponentArray->get(entity).material.lock();

                const auto* matKey = materialAsset.get();
                uint32_t localMaterialIndex;
                auto it = materialToIndex.find(matKey);
                if (it == materialToIndex.end()) {
                    // New material for this frame
                    renderer::GpuMaterial gpuMat{};

                    const auto& src = *materialAsset->getData();

                    gpuMat.albedoColor      = src.albedoColor;
                    gpuMat.albedoTexIndex   = renderer::NxRenderer3D::get().getTextureIndex(
                                                  src.albedoTexture.lock() && src.albedoTexture.lock()->isLoaded()
                                                      ? src.albedoTexture.lock()->getData()->texture
                                                      : nullptr);

                    gpuMat.specularColor    = src.specularColor;
                    gpuMat.specularTexIndex = renderer::NxRenderer3D::get().getTextureIndex(
                                                  src.metallicMap.lock() && src.metallicMap.lock()->isLoaded()
                                                      ? src.metallicMap.lock()->getData()->texture
                                                      : nullptr);

                    gpuMat.emissiveColor    = src.emissiveColor;
                    gpuMat.emissiveTexIndex = renderer::NxRenderer3D::get().getTextureIndex(
                                                  src.emissiveMap.lock() && src.emissiveMap.lock()->isLoaded()
                                                      ? src.emissiveMap.lock()->getData()->texture
                                                      : nullptr);

                    gpuMat.roughness        = src.roughness;
                    gpuMat.roughnessTexIndex = renderer::NxRenderer3D::get().getTextureIndex(
                                                   src.roughnessMap.lock() && src.roughnessMap.lock()->isLoaded()
                                                       ? src.roughnessMap.lock()->getData()->texture
                                                       : nullptr);

                    gpuMat.metallic         = src.metallic;
                    gpuMat.metallicTexIndex = renderer::NxRenderer3D::get().getTextureIndex(
                                                   src.metallicMap.lock() && src.metallicMap.lock()->isLoaded()
                                                       ? src.metallicMap.lock()->getData()->texture
                                                       : nullptr);

                    gpuMat.opacity          = src.opacity;

                    localMaterialIndex = static_cast<uint32_t>(gpuMaterials.size());
                    gpuMaterials.push_back(gpuMat);
                    materialToIndex[matKey] = localMaterialIndex;
                } else {
                    localMaterialIndex = it->second;
                }

                std::string shaderStr;
                std::shared_ptr<renderer::NxShader> shader;
                shaderStr = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->shader : "";
                shader    = renderer::ShaderLibrary::getInstance().get(shaderStr);
                if (!shader) continue;

                RenderItem item;
                item.entity        = entity;
                item.shader        = shader;
                item.mesh          = billboard.vao;
                item.materialIndex      = localMaterialIndex;
                item.filterMask    = renderer::F_FORWARD_PASS;
                item.isTransparent = !(materialAsset && materialAsset->isLoaded() &&
                                        materialAsset->getData()->isOpaque);

                const glm::mat4 billboardRotation =
                    createBillboardTransformMatrix(camera.cameraPosition, transform);
                glm::mat4 model =
                    glm::translate(glm::mat4(1.0f), transform.pos) *
                    billboardRotation *
                    glm::scale(glm::mat4(1.0f),
                                glm::vec3(transform.size.x, transform.size.y, 1.0f));

                item.modelMatrix = model;

                renderItems.push_back(item);
                if (coord->entityHasComponent<components::SelectedTag>(entity)) {
                    RenderItem sel = item;
                    sel.filterMask = renderer::F_OUTLINE_MASK;
                    sel.entity = entity;
                    sel.mesh = billboard.vao;
                    sel.modelMatrix = model;
                    if (materialAsset->getData()->isOpaque) {
                        sel.shader = renderer::ShaderLibrary::getInstance().get("Flat color");
                    } else {
                        sel.shader = renderer::ShaderLibrary::getInstance().get("Albedo unshaded transparent");
                    }
                    sel.materialIndex = localMaterialIndex;
                    renderItems.push_back(sel);
                }
            }

            if (renderItems.empty())
                continue;

            std::sort(renderItems.begin(), renderItems.end(),
                [](const RenderItem& a, const RenderItem& b) {
                    if (a.filterMask != b.filterMask) return a.filterMask < b.filterMask;
                    if (a.shader     != b.shader)     return a.shader     < b.shader;
                    if (a.materialIndex   != b.materialIndex)   return a.materialIndex   < b.materialIndex;
                    if (a.mesh       != b.mesh)       return a.mesh       < b.mesh;
                    return false;
                });


            const std::size_t currentBufferSizeBytes =
                camera.pipeline.getStorageBufferSize(INSTANCE_BUFFER);
            const std::uint32_t baseInstanceOffset =
                static_cast<std::uint32_t>(currentBufferSizeBytes / sizeof(renderer::GpuInstanceData));

            const std::size_t currentMaterialBufferBytes =
                camera.pipeline.getStorageBufferSize(MATERIAL_BUFFER);
            const std::uint32_t baseMaterialOffset =
                static_cast<std::uint32_t>(currentMaterialBufferBytes / sizeof(renderer::GpuMaterial));;

            std::vector<renderer::GpuInstanceData> billboardInstances;
            billboardInstances.reserve(renderItems.size());

            std::uint32_t localIndex = 0;
            for (auto& item : renderItems) {
                renderer::GpuInstanceData inst{};
                inst.model    = item.modelMatrix;
                inst.entityId = static_cast<int>(item.entity);
                uint32_t globalMaterialIndex = baseMaterialOffset + item.materialIndex;
                inst.materialIndex = static_cast<int>(globalMaterialIndex);
                billboardInstances.push_back(inst);

                item.instanceIndex = baseInstanceOffset + localIndex;
                ++localIndex;
            }

            std::vector<RenderBatch> batches;
            batches.reserve(renderItems.size());

            RenderBatch current{};
            auto& first = renderItems[0];

            current.shader         = first.shader;
            current.mesh           = first.mesh;
            current.materialIndex       = first.materialIndex;
            current.filterMask     = first.filterMask;
            current.instanceOffset = first.instanceIndex;
            current.instanceCount  = 1;

            for (std::size_t i = 1; i < renderItems.size(); ++i) {
                const auto& item = renderItems[i];

                const bool sameBatch =
                    item.shader     == current.shader &&
                    item.mesh       == current.mesh &&
                    item.materialIndex   == current.materialIndex &&
                    item.filterMask == current.filterMask;

                if (sameBatch) {
                    ++current.instanceCount;
                } else {
                    batches.push_back(current);

                    current.shader         = item.shader;
                    current.mesh           = item.mesh;
                    current.materialIndex       = item.materialIndex;
                    current.filterMask     = item.filterMask;
                    current.instanceOffset = item.instanceIndex;
                    current.instanceCount  = 1;
                }
            }

            batches.push_back(current);

            std::vector<renderer::DrawCommand> drawCommands;
            drawCommands.reserve(batches.size());

            for (const auto& batch : batches) {
                renderer::DrawCommand cmd;
                cmd.vao           = batch.mesh;
                cmd.shader        = batch.shader;
                cmd.filterMask    = batch.filterMask;
                cmd.instanceOffset = batch.instanceOffset;
                cmd.instanceCount  = batch.instanceCount;
                cmd.instanced      = (batch.instanceCount > 1);

                if (cmd.instanced) {
                    LOG(NEXO_INFO, "Billboard instanced batch detected (instances = {})",
                        batch.instanceCount);
                }

                cmd.setUniform("uInstanceOffset", static_cast<int>(batch.instanceOffset));
                drawCommands.push_back(cmd);
            }

            if (!billboardInstances.empty()) {
                camera.pipeline.appendStorageBufferData(
                    INSTANCE_BUFFER,
                    billboardInstances.data(),
                    billboardInstances.size() * sizeof(renderer::GpuInstanceData));
            }

            if (!gpuMaterials.empty()) {
                camera.pipeline.appendStorageBufferData(MATERIAL_BUFFER, gpuMaterials.data(), sizeof(renderer::GpuMaterial) * gpuMaterials.size());
            }

            camera.pipeline.addDrawCommands(drawCommands);
        }
    }

} // namespace nexo::system
