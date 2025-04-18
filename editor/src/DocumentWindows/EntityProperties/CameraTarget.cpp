//// CameraTarget.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        18/04/2025
//  Description: Source file for the camera target property
//
///////////////////////////////////////////////////////////////////////////////

#include "CameraTarget.hpp"
#include "Coordinator.hpp"
#include "Definitions.hpp"
#include "components/Camera.hpp"
#include "Components/EntityPropertiesComponents.hpp"
#include "components/Light.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "context/Selector.hpp"

namespace nexo::editor {

    /**
     * @brief Draw a dropdown to select an entity with proper UI name display
     *
     * @param label The label to display
     * @param targetEntity Reference to the entity ID that will be updated on selection
     * @param entities Vector of available entities to choose from
     * @param getNameFunc Function to get UI name from entity ID
     * @return true if the selection changed, false otherwise
     */
    static bool drawRowEntityDropdown(const std::string &label, ecs::Entity &targetEntity,
                                     const std::vector<ecs::Entity>& entities,
                                     const std::function<std::string(ecs::Entity)>& getNameFunc)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label.c_str());

        ImGui::TableNextColumn();

        // Create a unique ID for this widget
        ImGui::PushID(label.c_str());

        ImGui::SetNextItemWidth(-FLT_MIN); // Use all available width

        bool changed = false;

        // Build entity-name mapping
        static std::vector<std::pair<ecs::Entity, std::string>> entityNamePairs;
        static ecs::Entity lastTargetEntity = 0;
        static std::vector<ecs::Entity> lastEntities;

        // Only rebuild the mapping if entities list changed or target entity changed
        bool needRebuild = lastTargetEntity != targetEntity ||
                          lastEntities.size() != entities.size();

        if (!needRebuild) {
            for (size_t i = 0; i < entities.size() && !needRebuild; i++) {
                needRebuild = lastEntities[i] != entities[i];
            }
        }

        if (needRebuild) {
            entityNamePairs.clear();
            entityNamePairs.reserve(entities.size());
            lastEntities = entities;
            lastTargetEntity = targetEntity;

            for (ecs::Entity entity : entities) {
                std::string name = getNameFunc(entity);
                entityNamePairs.emplace_back(entity, name);
            }
        }

        // Find current index
        int currentIndex = -1;
        for (size_t i = 0; i < entityNamePairs.size(); i++) {
            if (entityNamePairs[i].first == targetEntity) {
                currentIndex = static_cast<int>(i);
                break;
            }
        }

        // Add a "None" option if we want to allow null selection
        std::string currentItemName = currentIndex >= 0 ? entityNamePairs[currentIndex].second : "None";

        // Draw the combo box
        if (ImGui::BeginCombo("##entity_dropdown", currentItemName.c_str()))
        {
            // Optional: Add a "None" option for clearing the target
            if (ImGui::Selectable("None", targetEntity == ecs::MAX_ENTITIES)) {
                targetEntity = ecs::MAX_ENTITIES;
                changed = true;
            }

            for (size_t i = 0; i < entityNamePairs.size(); i++)
            {
                const bool isSelected = (currentIndex == static_cast<int>(i));
                if (ImGui::Selectable(entityNamePairs[i].second.c_str(), isSelected))
                {
                    targetEntity = entityNamePairs[i].first;
                    changed = true;
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        ImGui::PopID();
        return changed;
    }

    void CameraTarget::show(const ecs::Entity entity)
    {
        auto& targetComponent = Application::getEntityComponent<components::PerspectiveCameraTarget>(entity);

        if (EntityPropertiesComponents::drawHeader("##ControllerNode", "Camera Controller"))
        {
            ImGui::Spacing();

			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
   			if (ImGui::BeginTable("InspectorControllerTable", 2,
                ImGuiTableFlags_SizingStretchProp))
            {
                auto &app = getApp();
                auto &selector = Selector::get();
                ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

                const std::vector<ecs::Entity> &entities = app.m_coordinator->getAllEntitiesWith<
                                                                components::TransformComponent,
                                                                ecs::Exclude<components::CameraComponent>,
                                                                ecs::Exclude<components::DirectionalLightComponent>,
                                                                ecs::Exclude<components::PointLightComponent>,
                                                                ecs::Exclude<components::AmbientLightComponent>,
                                                                ecs::Exclude<components::SpotLightComponent>>();

                EntityPropertiesComponents::drawRowDragFloat1("Mouse sensitivity", "", &targetComponent.mouseSensitivity, 0.1f);
                EntityPropertiesComponents::drawRowDragFloat1("Distance", "", &targetComponent.distance, 0.1f);
                drawRowEntityDropdown(
                    "Target Entity",
                    targetComponent.targetEntity, entities,
                    [&app, &selector](ecs::Entity e) {
                        return selector.getUiHandle(
                                app.m_coordinator->getComponent<components::UuidComponent>(e).uuid,
                                std::to_string(e)
                        );
                });

                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        	ImGui::TreePop();
        }
	}
}
