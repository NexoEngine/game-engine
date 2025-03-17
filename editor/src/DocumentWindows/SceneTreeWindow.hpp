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
#include "IconsFontAwesome.h"
#include "Nexo.hpp"
#include "core/scene/SceneManager.hpp"
#include "context/Selector.hpp"

#include "PopupManager.hpp"

#include <unordered_map>
#include <utility>
#include <imgui.h>
#include <map>

namespace nexo::editor {

	struct SceneProperties {
        scene::SceneId sceneId;
        WindowId windowId;
    };

    struct EntityProperties {
        SceneProperties sceneProperties;
        ecs::Entity entity;
    };

    const std::unordered_map<SelectionType, std::string> ObjectTypeToIcon = {
        {SelectionType::SCENE, ICON_FA_MAP_O " "},
        {SelectionType::CAMERA, ICON_FA_CAMERA " "},
        {SelectionType::ENTITY, ICON_FA_CUBES " "},
        {SelectionType::AMBIENT_LIGHT, ICON_FA_ADJUST " "},
        {SelectionType::DIR_LIGHT, ICON_FA_SUN_O " "},
        {SelectionType::POINT_LIGHT, ICON_FA_LIGHTBULB_O " "},
        {SelectionType::SPOT_LIGHT, ICON_FA_ARROW_CIRCLE_DOWN " "}
    };

    struct SceneObject {
        std::string uiName;
        std::string uuid;
        SelectionType type;
        EntityProperties data;
        std::vector<SceneObject> children;

        explicit SceneObject(std::string name = "", std::vector<SceneObject> children = {},
                    SelectionType type = SelectionType::NONE, EntityProperties data = {})
            : uiName(std::move(name)), type(type), data(std::move(data)), children(std::move(children)) {}
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
            unsigned int m_nbDirLights = 0;
            unsigned int m_nbPointLights = 0;
            unsigned int m_nbSpotLights = 0;
            std::optional<std::pair<SelectionType, std::string>> m_renameTarget;
            std::string m_renameBuffer;
            PopupManager m_popupManager;


            template<typename... Components>
            void generateNodes(
            	std::map<scene::SceneId, SceneObject> &scenes,
            	std::function<SceneObject(scene::SceneId, WindowId, ecs::Entity)> nodeCreator)
            {
            	auto &app = getApp();
            	std::set<ecs::Entity> entities = app.m_coordinator->getAllEntitiesWith<Components...>();
             	for (ecs::Entity entity : entities)
				{
					const auto& sceneTag = app.m_coordinator->getComponent<components::SceneTag>(entity);
					auto it = scenes.find(sceneTag.id);
					if (it != scenes.end())
					{
						// Use the provided node-creation function to create a new SceneObject for this entity.
						SceneObject newNode = nodeCreator(it->second.data.sceneProperties.sceneId, it->second.data.sceneProperties.windowId, entity);

						// Add the new node to the children of the corresponding scene node.
						it->second.children.push_back(newNode);
					}
				}
            }

            SceneObject newSceneNode(scene::SceneId sceneId, WindowId uiId);

            void newLightNode(SceneObject &lightNode, scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity, const std::string &uiName);
            SceneObject newAmbientLightNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity);
            SceneObject newDirectionalLightNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity);
            SceneObject newSpotLightNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity);
            SceneObject newPointLightNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity);

            SceneObject newCameraNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity cameraEntity);

            SceneObject newEntityNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity entity);

            void handleRename(SceneObject &obj);
            bool handleSelection(const SceneObject &obj, const std::string &uniqueLabel, ImGuiTreeNodeFlags baseFlags) const;
            void sceneSelected(const SceneObject &obj);
            void lightSelected(const SceneObject &obj);
            void cameraSelected(const SceneObject &obj) const;
            void entitySelected(const SceneObject &obj) const;
            void showNode(SceneObject &object);
            void sceneContextMenu();
            void sceneCreationMenu();
    };
}
