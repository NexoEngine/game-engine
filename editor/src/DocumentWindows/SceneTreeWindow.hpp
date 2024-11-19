//// SceneTreeWindow.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        13/11/2024
//  Description: Header file for the scene tree document window
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ADocumentWindow.hpp"
#include "IconsFontAwesome.h"
#include "PopupManager.hpp"

#include <unordered_map>
#include <utility>
#include <imgui.h>

namespace nexo::editor {

    const std::unordered_map<SelectionType, std::string> ObjectTypeToIcon = {
        {SelectionType::SCENE, ICON_FA_MAP_O " "},
        {SelectionType::LAYER, ICON_FA_BARS " "},
        {SelectionType::CAMERA, ICON_FA_CAMERA " "},
        {SelectionType::ENTITY, ICON_FA_CUBES " "},
    };

    using NodeId = unsigned int;

    struct SceneObject {
        NodeId nodeId;
        std::string uiName;
        SelectionType type;
        VariantData data;
        std::vector<SceneObject> children;

        explicit SceneObject(std::string name = "", std::vector<SceneObject> children = {}, NodeId id = 0,
                    SelectionType type = SelectionType::NONE, VariantData data = {})
            : nodeId(id), uiName(std::move(name)), type(type), data(std::move(data)), children(std::move(children)) {}
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
            NodeId nextNodeId = 0;
            std::optional<std::pair<SelectionType, int>> m_renameTarget;
            std::string m_renameBuffer;
            PopupManager m_popupManager;

            SceneObject newSceneNode(scene::SceneId sceneId, WindowId uiId);
            SceneObject newLayerNode(scene::SceneId sceneId, WindowId uiId, layer::LayerId layerId, const std::string &layerName);
            SceneObject newCameraNode(scene::SceneId sceneId, WindowId uiId, layer::LayerId layerId);
            SceneObject newEntityNode(scene::SceneId sceneId, WindowId uiId, unsigned int layerId, ecs::Entity entity);

            void handleRename(SceneObject &obj);
            bool handleSelection(const SceneObject &obj, const std::string &uniqueLabel, ImGuiTreeNodeFlags baseFlags) const;
            void sceneSelected(const SceneObject &obj);
            void layerSelected(const SceneObject &obj) const;
            void cameraSelected(const SceneObject &obj) const;
            void entitySelected(const SceneObject &obj) const;
            void showNode(SceneObject &object);
            void sceneContextMenu();
            void sceneCreationMenu();
            void layerCreationMenu();
    };
}
