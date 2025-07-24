//// MaterialProperty.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/06/2025
//  Description: Source file for the material component ui display
//
///////////////////////////////////////////////////////////////////////////////

#include "MaterialProperty.hpp"
#include "ImNexo/Elements.hpp"
#include "Types.hpp"
#include "components/Camera.hpp"
#include "components/SceneComponents.hpp"
#include "context/ThumbnailCache.hpp"
#include "DocumentWindows/MaterialInspector/MaterialInspector.hpp"
#include "assets/AssetCatalog.hpp"
#include "ImNexo/Panels.hpp"
#include "utils/ScenePreview.hpp"
#include <imgui.h>

namespace nexo::editor {

    void MaterialProperty::cleanupPopup(assets::AssetRef<assets::Material> &materialRef, utils::ScenePreviewOut &out) const
    {
        assets::AssetCatalog::getInstance().deleteAsset(materialRef);
        materialRef = nullptr;
        if (out.sceneGenerated) {
            Application::getInstance().getSceneManager().deleteScene(out.sceneId);
            out.sceneGenerated = false;
        }
        ImGui::CloseCurrentPopup();
    }

    void MaterialProperty::createMaterialPopup(const ecs::Entity entity) const
    {
        ImGui::Text("Create New Material");
        ImGui::Separator();
        static assets::AssetRef<assets::Material> materialRef = nullptr;
        if (!materialRef) {
            auto material = std::make_unique<components::Material>();
            material->albedoColor = {0.05f * 1.7, 0.09f * 1.35, 0.13f * 1.45, 1.0f};
            materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
                                    assets::AssetLocation("_internal::TempMaterial@_internal"),
                                    std::move(material));
        }


        const ImVec2 availSize = ImGui::GetContentRegionAvail();
        const float totalWidth = availSize.x;
        float totalHeight = availSize.y - 40; // Reserve space for bottom buttons

        // Define layout: 40% for inspector, 60% for preview
        const float inspectorWidth = totalWidth * 0.4f;
        const float previewWidth = totalWidth - inspectorWidth - 8; // Subtract spacing between panels

        ImGui::Columns(2, "MaterialPreviewColumns", false);
        static char materialName[128] = "";


        ImGui::SetColumnWidth(0, inspectorWidth);
        // --- Left Side: Material Inspector ---
        {
            ImGui::BeginChild("MaterialInspector", ImVec2(inspectorWidth - 4, totalHeight), true);
            ImGui::InputText("Name", materialName, IM_ARRAYSIZE(materialName));
            ImGui::Spacing();

            auto material = materialRef.lock();
            components::Material& materialData = *material->getData();

            if (ImNexo::MaterialInspector(materialData))
                ThumbnailCache::getInstance().updateMaterialThumbnail(materialRef);

            ImGui::EndChild();
        }
        static utils::ScenePreviewOut out;
        ImGui::NextColumn();
        // --- Right Side: Material Preview ---
        {
            ImGui::BeginChild("MaterialPreview", ImVec2(previewWidth - 4, totalHeight), true);

            if (!out.sceneGenerated)
                utils::genScenePreview("Material Creation Scene", {previewWidth - 4, totalHeight}, entity, out);
            Application::SceneInfo sceneInfo{out.sceneId, RenderingType::FRAMEBUFFER};
            auto &materialComp = Application::m_coordinator->getComponent<components::MaterialComponent>(out.entityCopy);
            materialComp.material = materialRef;
            Application::getInstance().run(sceneInfo);
            const auto &cameraComp = Application::m_coordinator->getComponent<components::CameraComponent>(out.cameraId);
            const unsigned int textureId = cameraComp.m_renderTarget->getColorAttachmentId();


            const float aspectRatio = (previewWidth - 8) / totalHeight;

            const float displayHeight = totalHeight - 20;
            const float displayWidth = displayHeight * aspectRatio;

            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 4, ImGui::GetCursorPosY() + 4));
            ImNexo::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)),
                        ImVec2(displayWidth, displayHeight));

            ImGui::EndChild();
        }

        ImGui::Columns(1);
        ImGui::Spacing();

        constexpr float buttonWidth = 120.0f;

        if (ImNexo::Button("OK", ImVec2(buttonWidth, 0)))
        {
            // Create a new permanent material based on the preview material
            if (materialRef.isValid()) {
                auto name = std::string(materialName);
                if (name.empty()) {
                    name = std::format("NewMaterial_{}", entity);
                }
                const auto &sceneTag = Application::m_coordinator->getComponent<components::SceneTag>(entity);
                std::string sceneName = Application::getInstance().getSceneManager().getScene(sceneTag.id).getName();
                auto hashPos = sceneName.find('#');
                if (hashPos != std::string::npos) {
                    sceneName.erase(hashPos);
                }
                assets::AssetLocation finalLocation(std::format("{}@{}/", name, sceneName));

                // Create a new material asset by copying the preview material
                auto newMaterialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
                    finalLocation,
                    std::make_unique<components::Material>(*materialRef.lock()->getData())
                );

                auto& materialComponent = Application::m_coordinator->getComponent<components::MaterialComponent>(entity);
                materialComponent.material = newMaterialRef;
                LOG(NEXO_INFO, "Applied new material '{}' to entity {}", finalLocation.getFullLocation(), entity);
            }

            cleanupPopup(materialRef, out);
        }
        ImGui::SameLine();
        if (ImNexo::Button("Cancel", ImVec2(buttonWidth, 0)))
            cleanupPopup(materialRef, out);
    }


    void MaterialProperty::show(ecs::Entity entity)
    {
        if (Application::m_coordinator->entityHasComponent<components::CameraComponent>(entity) ||
            Application::m_coordinator->entityHasComponent<components::PointLightComponent>(entity) ||
            Application::m_coordinator->entityHasComponent<components::SpotLightComponent>(entity))
            return;
        const auto &materialComponent = Application::getEntityComponent<components::MaterialComponent>(entity);
        if (ImNexo::Header("##MaterialNode", "Material Component"))
        {
            ImTextureID textureID = ThumbnailCache::getInstance().getMaterialThumbnail(materialComponent.material);
            ImNexo::Image(textureID, ImVec2(64, 64));
            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                // --- Dropdown for Material Types ---
                static int selectedMaterialIndex = 0;
                const char* materialTypes[] = { "Default", "Metal", "Wood", "Plastic" };
                ImGui::Combo("##MaterialType", &selectedMaterialIndex, materialTypes, IM_ARRAYSIZE(materialTypes));

                // --- Material Action Buttons ---
                if (ImNexo::Button("Create new material"))
                {
                    m_popupManager.openPopup("Create new material", ImVec2(1440,900));
                }
                ImGui::SameLine();
                if (ImNexo::Button("Modify Material"))
                {
                    m_inspector.setSubInspectorVisibility<MaterialInspector>(true);
                }
                MaterialInspectorData data;
                data.m_selectedEntity = entity;
                data.material = materialComponent.material;
                m_inspector.setSubInspectorData<MaterialInspector>(data);

            }
            ImGui::EndGroup();
            const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        }
        ImGui::TreePop();

        if (m_popupManager.showPopupModal("Create new material"))
        {
            createMaterialPopup(entity);
            PopupManager::closePopup();
        }
    }
}
