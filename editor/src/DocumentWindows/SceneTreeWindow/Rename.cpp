//// Rename.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the rename handling of the scene tree window
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"

namespace nexo::editor {

    void SceneTreeWindow::handleRename(SceneObject &obj)
    {
        ImGui::BeginGroup();
        ImGui::TextUnformatted(ObjectTypeToIcon.at(obj.type).c_str());
        ImGui::SameLine();

        const std::string editableName = obj.uiName.substr(ObjectTypeToIcon.at(obj.type).size());
        char buffer[256];
        auto result = std::format_to_n(buffer, sizeof(buffer) - 1, "{}", editableName);
        // Null-terminate at the position where format_to_n stopped writing
        *(result.out) = '\0';

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f); // Remove border
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f); // No rounding
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

        if (ImGui::InputText("##Rename", buffer, sizeof(buffer),
                             ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            obj.uiName = ObjectTypeToIcon.at(obj.type) + std::string(buffer);
            auto &selector = Selector::get();
            selector.setUiHandle(obj.uuid, obj.uiName);
            if (obj.type == SelectionType::SCENE)
            {
                auto &app = getApp();
                app.getSceneManager().getScene(obj.data.sceneProperties.sceneId).setName(obj.uiName);
            }
            m_renameTarget.reset();
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
            m_renameTarget.reset();
        ImGui::PopStyleVar(3);
        ImGui::EndGroup();
    }

}
