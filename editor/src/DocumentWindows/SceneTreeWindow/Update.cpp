//// Update.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the update of the scene tree window
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "components/Render3D.hpp"
#include "components/StaticMesh.hpp"

namespace nexo::editor {
    void SceneTreeWindow::update()
    {
        root_.uiName = "Scene Tree";
        root_.data.entity = -1;
        root_.type = SelectionType::NONE;
        root_.children.clear();
        m_nbPointLights = 0;
        m_nbDirLights = 0;
        m_nbSpotLights = 0;

        if (m_resetExpandState) {
            m_forceExpandAll = false;
            m_forceCollapseAll = false;
            m_resetExpandState = false;
        }

        // Retrieves the scenes that are displayed on the GUI
        const auto &scenes = m_windowRegistry.getWindows<EditorScene>();
        std::map<scene::SceneId, SceneObject> sceneNodes;
        for (const auto &scene : scenes)
        {
            sceneNodes[scene->getSceneId()] = newSceneNode(scene->getWindowName(), scene->getSceneId(), windowId);
        }

        generateNodes<components::AmbientLightComponent, components::SceneTag>(
            sceneNodes,
            [](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return newAmbientLightNode(sceneId, uiId, entity);
         });
        generateNodes<components::DirectionalLightComponent, components::SceneTag>(
            sceneNodes,
            [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return newDirectionalLightNode(sceneId, uiId, entity);
         });
        generateNodes<components::PointLightComponent, components::SceneTag>(
            sceneNodes,
            [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return newPointLightNode(sceneId, uiId, entity);
         });
        generateNodes<components::SpotLightComponent, components::SceneTag>(
            sceneNodes,
            [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return newSpotLightNode(sceneId, uiId, entity);
         });

        generateNodes<components::CameraComponent, components::SceneTag, ecs::Exclude<components::EditorCameraTag>>(
            sceneNodes,
            [](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return newCameraNode(sceneId, uiId, entity);
         });

        generateNodes<
        components::StaticMeshComponent,
        components::MaterialComponent,
        components::TransformComponent,
        components::SceneTag,
        ecs::Exclude<components::CameraComponent>,
        ecs::Exclude<components::SpotLightComponent>,
        ecs::Exclude<components::PointLightComponent>>(
            sceneNodes,
            [](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
              return newEntityNode(sceneId, uiId, entity);
         });

        for (const auto &sceneNode: sceneNodes | std::views::values)
        {
            root_.children.push_back(sceneNode);
        }
    }
}
