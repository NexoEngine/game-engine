//// SceneTreeWindow.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/11/2024
//  Description: Header file for the scene tree document window
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ADocumentWindow.hpp"
#include "core/camera/Camera.hpp"
#include "IconsFontAwesome.h"
#include "PopupManager.hpp"

#include <unordered_map>
#include <imgui.h>

namespace nexo::editor {

    const std::unordered_map<SelectionType, std::string> ObjectTypeToIcon = {
        {SelectionType::SCENE, ICON_FA_MAP_O " "},
        {SelectionType::LAYER, ICON_FA_BARS " "},
        {SelectionType::CAMERA, ICON_FA_CAMERA " "},
        {SelectionType::ENTITY, ICON_FA_CUBES " "},
    };

    struct SceneObject {
        std::string name;
        std::vector<SceneObject> children;
        int id;
        SelectionType type;
        VariantData data;

        SceneObject(const std::string &name = "", std::vector<SceneObject> children = {}, int id = 0,
                    SelectionType type = SelectionType::NONE, VariantData data = {})
            : name(name), children(std::move(children)), id(id), type(type), data(std::move(data)) {}
    };

    class SceneTreeWindow : public ADocumentWindow {
        public:
            SceneTreeWindow();

            ~SceneTreeWindow() override;

            void setup() override;

            void shutdown() override;

            void show() override;

            void update() override;

        private:
            SceneObject root_;
            std::optional<std::pair<SelectionType, int>> m_renameTarget;
            std::string m_renameBuffer;
            PopupManager m_popupManager;

            SceneObject newSceneNode(scene::SceneId id, const std::string &uiName);
            SceneObject newLayerNode(scene::SceneId id, std::shared_ptr<layer::Layer> layer);
            SceneObject newCameraNode(scene::SceneId id, std::shared_ptr<layer::Layer> layer);
            SceneObject newEntityNode(ecs::Entity entity);

            void handleRename(SceneObject &obj);
            bool handleSelection(SceneObject &obj, std::string &uniqueLabel, ImGuiTreeNodeFlags baseFlags);
            void sceneSelected(SceneObject &obj);
            void layerSelected(SceneObject &obj);
            void cameraSelected(SceneObject &obj);
            void entitySelected(SceneObject &obj);
            void showNode(SceneObject &object);
            void sceneContextMenu();
            void sceneCreationMenu();
            void layerCreationMenu();
    };
}
