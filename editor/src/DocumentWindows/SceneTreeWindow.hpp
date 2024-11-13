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

namespace nexo::editor {
    enum SceneObjectType {
        SCENE_OBJECT_TYPE_ENTITY,
        SCENE_OBJECT_TYPE_SCENE,
        SCENE_OBJECT_TYPE_LAYER,
        SCENE_OBJECT_TYPE_FOLDER,
        SCENE_OBJECT_TYPE_UNKNOWN
    };

    struct SceneObject {
        std::string name;
        std::vector<SceneObject> children;
        ecs::Entity id; // For entities, use their ID; for scenes, use the SceneID
        SceneObjectType type;

        SceneObject(const std::string &name = "", std::vector<SceneObject> children = {}, ecs::Entity id = 0,
                    SceneObjectType type = SCENE_OBJECT_TYPE_ENTITY)
            : name(name), children(std::move(children)), id(id), type(type) {}
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
                uint32_t id;
                SceneObjectType type = SCENE_OBJECT_TYPE_UNKNOWN;
            };

            SceneObject root_;

            void ShowNode(SceneObject &object);
    };
}
