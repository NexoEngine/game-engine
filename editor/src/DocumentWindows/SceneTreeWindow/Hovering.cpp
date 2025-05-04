//// Hovering.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the hovering handling of the scene tree window
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"

namespace nexo::editor {

    void SceneTreeWindow::cameraHovered(const SceneObject &obj) const
    {
        auto &app = Application::getInstance();
        auto &cameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(obj.data.entity);

        if (cameraComponent.m_renderTarget)
        {
            ImGui::BeginTooltip();
            constexpr float previewSize = 200.0f;
            cameraComponent.render = true;
            const unsigned int textureId = cameraComponent.m_renderTarget->getColorAttachmentId(0);

            ImGui::Image(
                static_cast<ImTextureID>(static_cast<intptr_t>(textureId)),
                ImVec2(previewSize, previewSize),
                ImVec2(0, 1), ImVec2(1, 0) // Flip Y coordinates for OpenGL texture
            );

            ImGui::EndTooltip();
        }
    }

    void SceneTreeWindow::handleHovering(const SceneObject &obj) const
    {
        if (obj.type == SelectionType::CAMERA) {
            static bool cameraHoveredLastFrame = false;
            if (ImGui::IsItemHovered()) {
                cameraHovered(obj);
                cameraHoveredLastFrame = true;
            } else if (cameraHoveredLastFrame) {
                cameraHoveredLastFrame = false;
                auto &cameraComponent = Application::getInstance().m_coordinator->getComponent<components::CameraComponent>(obj.data.entity);
                cameraComponent.render = false;
            }
        }
    }
}
