/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** SceneTreeWindow.cpp
*/

#include "imgui.h"
#include "rlImGui.h"
#include "game_engine/editor/DocumentWindows/EntityPropertiesWindow.hpp"

#include <EntityHandling.hpp>

#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/GameEngine.hpp"

#include "game_engine/editor/DocumentWindows/EntityProperties/TransformProperty.hpp"
#include "game_engine/editor/DocumentWindows/EntityProperties/RenderProperty.hpp"

namespace engine::editor {

    EntityPropertiesWindow::EntityPropertiesWindow() : position_{0}, rotation_{0}, scale_{1},
        _entityPropertiesMap  {
            {typeid(TransformProperty::ComponentType), std::make_shared<TransformProperty>("Transform")},
            {typeid(RenderProperty::ComponentType), std::make_shared<RenderProperty>("Render")}
        }
    {

        // Constructor
    }

    EntityPropertiesWindow::~EntityPropertiesWindow() {
        // Destructor
    }

    void EntityPropertiesWindow::setup() {
        // Setup code
    }

    void EntityPropertiesWindow::shutdown() {
        // Shutdown code
    }

    void EntityPropertiesWindow::show() {
        ImVec2 pos = ImVec2(1507, 825);
        ImVec2 size = ImVec2(415, 272);
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Properties", &_opened, ImGuiWindowFlags_NoCollapse) && _sceneManagerBridge.isEntitySelected()) {

            const auto &components = engine::getAllComponents(_sceneManagerBridge.getSelectedEntity());

            for (const auto &[component, _] : components) {
                if (!_entityPropertiesMap.contains(component))
                    continue;
                if (_entityPropertiesMap[component]->show()) {

                }
                _entityPropertiesMap[component]->showEnd();

            }


        }
        ImGui::End();



        /*
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            // Position
            ImGui::Text("Position");
            ImGui::SameLine(); ImGui::DragFloat3("##Position", &position_.x, 0.1f);

            // Rotation
            ImGui::Text("Rotation");
            ImGui::SameLine(); ImGui::DragFloat3("##Rotation", &rotation_.x, 0.1f);

            // Scale
            ImGui::Text("Scale");
            ImGui::SameLine(); ImGui::DragFloat3("##Scale", &scale_.x, 0.1f);
        }

        // Add more properties for other components here...
        */

    }

    void EntityPropertiesWindow::update() {
        const auto &components = engine::getAllComponents(_sceneManagerBridge.getSelectedEntity());

        for (const auto &[component, _] : components) {
            if (!_entityPropertiesMap.contains(component))
                continue;
            _entityPropertiesMap[component]->update();
        }

    }

    void EntityPropertiesWindow::SetSelectedEntityTransform(const Vector3& position, const Vector3& rotation, const Vector3& scale) {
        position_ = position;
        rotation_ = rotation;
        scale_ = scale;
    }
}
