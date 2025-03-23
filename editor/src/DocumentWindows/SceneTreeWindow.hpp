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

    /**
     * @brief Mapping from selection types to corresponding icon strings.
     */
    const std::unordered_map<SelectionType, std::string> ObjectTypeToIcon = {
        {SelectionType::SCENE, ICON_FA_MAP_O " "},
        {SelectionType::CAMERA, ICON_FA_CAMERA " "},
        {SelectionType::ENTITY, ICON_FA_CUBES " "},
        {SelectionType::AMBIENT_LIGHT, ICON_FA_ADJUST " "},
        {SelectionType::DIR_LIGHT, ICON_FA_SUN_O " "},
        {SelectionType::POINT_LIGHT, ICON_FA_LIGHTBULB_O " "},
        {SelectionType::SPOT_LIGHT, ICON_FA_ARROW_CIRCLE_DOWN " "}
    };

    /**
     * @brief Represents an object in the scene tree.
     *
     * Contains a UI name, UUID, selection type, associated data, and potential child nodes.
     */
    struct SceneObject {
        std::string uiName;                   ///< The UI display name for the object.
        std::string uuid;                     ///< The unique identifier (UUID) of the object.
        SelectionType type;                   ///< The type of the object.
        EntityProperties data;                ///< Associated data (scene properties and entity).
        std::vector<SceneObject> children;    ///< Child objects (if any).

        explicit SceneObject(std::string name = "", std::vector<SceneObject> children = {},
                    SelectionType type = SelectionType::NONE, EntityProperties data = {})
            : uiName(std::move(name)), type(type), data(std::move(data)), children(std::move(children)) {}
    };

    /**
     * @brief Document window for displaying and interacting with the scene tree.
     *
     * The SceneTreeWindow class is responsible for drawing the scene tree, handling selection,
     * renaming, context menus, and scene/node creation.
     */
    class SceneTreeWindow : public ADocumentWindow {
        public:
            SceneTreeWindow();

            ~SceneTreeWindow() override;

            void setup() override;

            void shutdown() override;

            void show() override;

            void update() override;

        private:
	        SceneObject root_;    ///< Root node of the scene tree.
	        unsigned int m_nbDirLights = 0;   ///< Counter for directional lights.
	        unsigned int m_nbPointLights = 0; ///< Counter for point lights.
	        unsigned int m_nbSpotLights = 0;  ///< Counter for spot lights.
	        std::optional<std::pair<SelectionType, std::string>> m_renameTarget; ///< Target for renaming.
	        std::string m_renameBuffer; ///< Buffer for rename input.
	        PopupManager m_popupManager; ///< Manages context and creation popups.

			/**
             * @brief Generates nodes for all entities matching the specified components.
             *
             * This template function iterates over all entities with the given components,
             * creates a SceneObject node using the provided nodeCreator function, and adds it
             * to the corresponding scene node.
             *
             * @tparam Components Component types to filter by.
             * @param scenes Map of scene IDs to their SceneObject nodes.
             * @param nodeCreator Function that creates a SceneObject given a scene ID, UI window ID, and entity.
             */
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

            /**
             * @brief Creates a new scene node.
             *
             * @param sceneId The ID of the scene.
             * @param uiId The UI window ID for the scene.
             * @return A SceneObject representing the scene.
             */
            SceneObject newSceneNode(scene::SceneId sceneId, WindowId uiId);

            /**
             * @brief Creates a new light node and adds properties to it.
             *
             * @param lightNode The SceneObject to populate.
             * @param sceneId The scene ID.
             * @param uiId The UI window ID.
             * @param lightEntity The light entity.
             * @param uiName The UI display name.
             */
            void newLightNode(SceneObject &lightNode, scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity, const std::string &uiName);

            /**
             * @brief Creates a new ambient light node.
             *
             * @param sceneId The scene ID.
             * @param uiId The UI window ID.
             * @param lightEntity The ambient light entity.
             * @return A SceneObject representing the ambient light.
             */
            SceneObject newAmbientLightNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity);
            /**
             * @brief Creates a new directional light node.
             *
             * @param sceneId The scene ID.
             * @param uiId The UI window ID.
             * @param lightEntity The directional light entity.
             * @return A SceneObject representing the directional light.
             */
            SceneObject newDirectionalLightNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity);

            /**
             * @brief Creates a new spot light node.
             *
             * @param sceneId The scene ID.
             * @param uiId The UI window ID.
             * @param lightEntity The spot light entity.
             * @return A SceneObject representing the spot light.
             */
            SceneObject newSpotLightNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity);

            /**
             * @brief Creates a new point light node.
             *
             * @param sceneId The scene ID.
             * @param uiId The UI window ID.
             * @param lightEntity The point light entity.
             * @return A SceneObject representing the point light.
             */
            SceneObject newPointLightNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity);

            /**
             * @brief Creates a new camera node.
             *
             * @param sceneId The scene ID.
             * @param uiId The UI window ID.
             * @param cameraEntity The camera entity.
             * @return A SceneObject representing the camera.
             */
            SceneObject newCameraNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity cameraEntity);

            /**
             * @brief Creates a new entity node.
             *
             * @param sceneId The scene ID.
             * @param uiId The UI window ID.
             * @param entity The entity.
             * @return A SceneObject representing the entity.
             */
            SceneObject newEntityNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity entity);

            /**
             * @brief Handles the renaming of a scene object.
             *
             * Displays an input text field to rename the object and updates the UI handle accordingly.
             *
             * @param obj The SceneObject to rename.
             */
            void handleRename(SceneObject &obj);

            /**
             * @brief Handles selection of a scene object.
             *
             * Opens a tree node for the object and, if clicked, updates selection state.
             *
             * @param obj The SceneObject.
             * @param uniqueLabel The unique label used for the tree node.
             * @param baseFlags Base ImGuiTreeNodeFlags for the node.
             * @return true if the node is open; false otherwise.
             */
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
