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

#include <unordered_map>

namespace nexo::editor {

    const std::unordered_map<SelectionType, std::string> ObjectTypeToIcon = {
        {SelectionType::SCENE, ICON_FA_MAP_O " "},
        {SelectionType::LAYER, ICON_FA_BARS " "},
        {SelectionType::CAMERA, ICON_FA_CAMERA " "},
        {SelectionType::ENTITY, ICON_FA_CUBES " "},
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

            SceneObject root_;
            std::optional<std::pair<SelectionType, int>> m_renameTarget;
            std::string m_renameBuffer;

            void handleRename(SceneObject &obj);
            void ShowNode(SceneObject &object);
    };
}
