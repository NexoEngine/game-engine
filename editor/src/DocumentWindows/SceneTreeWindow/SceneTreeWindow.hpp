//// SceneTreeWindow.hpp //////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//
//  Author:      Guillaume HEIN
//  Date:        13/11/2024
//  Description: Header file for the scene tree document window
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ADocumentWindow.hpp"
#include "DocumentWindows/EditorScene/EditorScene.hpp"
#include "IconsFontAwesome.h"
#include "Nexo.hpp"
#include "core/scene/SceneManager.hpp"
#include "context/Selector.hpp"

#include "../PopupManager.hpp"

#include <unordered_map>
#include <utility>
#include <imgui.h>
#include <map>

namespace nexo::editor
{
    /**
     * @brief Stores scene identification information.
     *
     * Contains the scene's unique identifier and its associated window ID
     * to facilitate operations that require both scene and UI context.
     */
    struct SceneProperties
    {
        scene::SceneId sceneId; ///< The unique identifier for the scene
        WindowId windowId; ///< The associated window identifier in the UI
    };

    /**
     * @brief Links an entity with its parent scene information.
     *
     * Combines entity ID with scene properties to maintain the hierarchical
     * relationship between entities and their containing scenes.
     */
    struct EntityProperties
    {
        SceneProperties sceneProperties; ///< Properties of the scene containing this entity
        ecs::Entity entity; ///< The entity identifier
    };

    /**
     * @brief Mapping from selection types to corresponding icon strings.
     */
    const std::unordered_map<SelectionType, std::string> ObjectTypeToIcon = {
        {SelectionType::SCENE, ICON_FA_GLOBE "  "},
        {SelectionType::CAMERA, ICON_FA_CAMERA "  "},
        {SelectionType::ENTITY, ICON_FA_CUBE "  "},
        {SelectionType::AMBIENT_LIGHT, ICON_FA_ADJUST "  "},
        {SelectionType::DIR_LIGHT, ICON_FA_SUN_O "  "},
        {SelectionType::POINT_LIGHT, ICON_FA_LIGHTBULB_O "  "},
        {SelectionType::SPOT_LIGHT, ICON_FA_ARROW_CIRCLE_DOWN "  "}
    };

    /**
     * @brief Represents an object in the scene tree.
     *
     * Contains a UI name, UUID, selection type, associated data, and potential child nodes.
     */
    struct SceneObject
    {
        std::string uiName; ///< The UI display name for the object.
        std::string uuid; ///< The unique identifier (UUID) of the object.
        SelectionType type; ///< The type of the object.
        EntityProperties data; ///< Associated data (scene properties and entity).
        std::vector<SceneObject> children; ///< Child objects (if any).

        explicit SceneObject(std::string name = "", std::vector<SceneObject> children = {},
                             const SelectionType type = SelectionType::NONE, const EntityProperties data = {})
            : uiName(std::move(name)), type(type), data(data), children(std::move(children))
        {
        }
    };

    /**
     * @brief Document window for displaying and interacting with the scene tree.
     *
     * The SceneTreeWindow class is responsible for drawing the scene tree, handling selection,
     * renaming, context menus, and scene/node creation.
     */
    class SceneTreeWindow final : public ADocumentWindow {
        public:
            using ADocumentWindow::ADocumentWindow;
            ~SceneTreeWindow() override = default;

        // No-op method in this class
        void setup() override;

        // No-op method in this class
        void shutdown() override;

        /**
         * @brief Renders and manages the Scene Tree window.
         *
         * Initializes and displays the Scene Tree window using ImGui by setting its initial position and size.
         * On the first opening, it performs a docking setup, and then renders the hierarchical scene nodes
         * if present. Additionally, it handles right-click actions to open the context menu and invokes
         * context and creation menus for scene management.
         */
        void show() override;

        /**
         * @brief Rebuilds the scene tree GUI by clearing old nodes and repopulating it with current scene data.
         *
         * This function resets the root node and light counters, retrieves the list of open scenes from the
         * SceneViewManager via the window registry, and creates a new scene node for each open scene.
         * It then generates child nodes for ambient, directional, point, and spot lights, as well as cameras
         * and generic entities using a templated generateNodes method. Finally, it appends these nodes as children
         * to the root node, effectively updating the scene tree display.
         */
        void update() override;

            void generateHierarchicalNodes(std::map<scene::SceneId, SceneObject> &scenes);
            void buildChildNodesForEntity(
                ecs::Entity parentEntity,
                SceneObject& parentNode,
                std::unordered_set<ecs::Entity>& processedEntities);
            static SceneObject createEntityNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity entity) ;

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
         * @tparam NodeCreator Type of the node creator function.
         * @param scenes Map of scene IDs to their SceneObject nodes.
         * @param nodeCreator Function that creates a SceneObject given a scene ID, UI window ID, and entity.
         */
        template <typename... Components, typename NodeCreator>
        static void generateNodes(std::map<scene::SceneId, SceneObject>& scenes, NodeCreator nodeCreator)
        {
            const std::vector<ecs::Entity> entities = Application::m_coordinator->getAllEntitiesWith<Components...>();
            for (const ecs::Entity entity : entities)
            {
                const auto& sceneTag = Application::m_coordinator->getComponent<components::SceneTag>(entity);
                if (auto it = scenes.find(sceneTag.id); it != scenes.end())
                {
                    SceneObject newNode = nodeCreator(it->second.data.sceneProperties.sceneId,
                                                      it->second.data.sceneProperties.windowId, entity);
                    it->second.children.push_back(newNode);
                }
            }
        }

            /**
            * @brief Creates a new scene node for the scene tree.
            *
            * Constructs and initializes a SceneObject to represent a scene within the scene tree UI.
            * The node's properties are configured using the provided scene and UI identifiers: scene properties
            * and type are set from these identifiers, the scene's unique UUID is obtained from the Scene Manager,
            * and a UI handle is generated by concatenating a scene icon with the scene name from the Scene View Manager.
            *
            * @param sceneName The name of the scene.
            * @param sceneId The unique identifier of the scene.
            * @param uiId The identifier for the scene's UI element.
            * @return SceneObject The newly created scene node with initialized properties.
            */
            static SceneObject newSceneNode(const std::string &sceneName, scene::SceneId sceneId, WindowId uiId);

            /**
            * @brief Creates a new light node and adds properties to it.
            *
            * @param lightNode The SceneObject to populate.
            * @param sceneId The scene ID.
            * @param uiId The UI window ID.
            * @param lightEntity The light entity.
            * @param uiName The UI display name.
            */
            static void newLightNode(SceneObject &lightNode, scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity, const std::string &uiName);

            /**
            * @brief Creates a new ambient light node.
            *
            * @param sceneId The scene ID.
            * @param uiId The UI window ID.
            * @param lightEntity The ambient light entity.
            * @return A SceneObject representing the ambient light.
            */
            static SceneObject newAmbientLightNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity lightEntity);
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
        static SceneObject newCameraNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity cameraEntity);

        /**
         * @brief Creates a new entity node.
         *
         * @param sceneId The scene ID.
         * @param uiId The UI window ID.
         * @param entity The entity.
         * @return A SceneObject representing the entity.
         */
        static SceneObject newEntityNode(scene::SceneId sceneId, WindowId uiId, ecs::Entity entity);

        /**
         * @brief Handles the renaming of a scene object.
         *
         * Displays an input text field to rename the object and updates the UI handle accordingly.
         *
         * @param obj The SceneObject to rename.
         */
        void handleRename(SceneObject& obj);

        /**
         * @brief Renders a scene object as a selectable tree node.
         *
         * This function displays the scene object as a tree node using a unique label and specified flags.
         * If the node is expanded and clicked, it updates the global selection state—setting the selected
         * entity, its type, and the associated scene.
         *
         * @param obj The scene object to be displayed.
         * @param uniqueLabel A unique identifier used for the tree node label.
         * @param baseFlags ImGui tree node flags to customize the node's appearance.
         * @return true if the tree node is open (expanded); false otherwise.
         */
        static bool handleSelection(const SceneObject& obj, const std::string& uniqueLabel,
                                    ImGuiTreeNodeFlags baseFlags);

        /**
         * @brief Handles camera preview tooltips when hovering over camera nodes.
         *
         * When hovering over a camera node, renders a preview tooltip showing the camera's
         * view and enables rendering for that camera temporarily.
         *
         * @param obj The scene object (camera) being hovered over.
         */
        static void handleHovering(const SceneObject& obj);

        /**
         * @brief Displays a context menu option to delete a scene.
         *
         * Presents a "Delete Scene" menu item. When selected, it removes the scene associated
         * with the given scene object from the scene view manager using the scene's unique window ID.
         *
         * @param obj The scene object representing the scene to be deleted.
         */
        void sceneSelected(const SceneObject& obj);

        /**
         * @brief Displays a context menu option to delete a light node.
         *
         * When the "Delete Light" menu item is selected, this function unselects the currently selected entity
         * and deletes the light entity associated with the provided scene object.
         *
         * @param obj The scene object representing the light node to delete.
         */
        static void lightSelected(const SceneObject& obj);

        /**
         * @brief Displays a context menu option for deleting a camera.
         *
         * When the "Delete Camera" menu item is selected, the function retrieves the active SceneViewManager,
         * finds the scene associated with the camera's window identifier, and deletes the camera entity from that scene.
         * It also clears any current selection and notifies the application to delete the camera entity.
         *
         * @param obj The scene object representing the camera to be deleted.
         */
        void cameraSelected(const SceneObject& obj) const;

        /**
         * @brief Handles camera preview when hovering over a camera node.
         *
         * Renders a small preview of what the camera sees when the cursor
         * hovers over the camera node in the scene tree.
         *
         * @param obj The scene object representing the camera being hovered.
         */
        static void cameraHovered(const SceneObject& obj);

        /**
         * @brief Displays context menu options for an entity.
         *
         * Shows options like "Delete Entity" when right-clicking on an entity
         * in the scene tree. Handles entity deletion when selected.
         *
         * @param obj The scene object representing the entity.
         */
        static void entitySelected(const SceneObject& obj);

        /**
         * @brief Renders a node and its children in the scene tree.
         *
         * Handles the recursive display of scene tree nodes, including selection,
         * renaming, and context menus for each node type.
         *
         * @param object The scene object to render.
         */
        void showNode(SceneObject& object);

        /**
         * @brief Shows the context menu for the scene tree background.
         *
         * Displays options like "Create Scene" when right-clicking on an empty
         * area of the scene tree.
         */
        void sceneContextMenu();

        void showSceneSelectionContextMenu(scene::SceneId sceneId, const std::string& uuid = "",
                                           const std::string& uiName = "");

        /**
         * @brief Displays a modal popup for creating a new scene.
         *
         * Renders a popup with an input field prompting the user to enter a scene name. When the "Create"
         * button is pressed with a non-empty name, it attempts to add the new scene via the SceneViewManager
         * fetched from the window registry, logging a warning if the creation fails due to a non-unique name.
         * The popup is closed either upon successful scene creation or when the "Cancel" button is clicked.
         */
        void sceneCreationMenu();

        // /**
        //  * @brief Displays a popup menu for creating primitive entities.
        //  *
        //  * Renders a popup with options to create customisable primitive shapes (sphere, cylinder).
        //  */
        // void primitiveCreationMenu();

        /**
         * @brief Creates a new scene with the provided name.
         *
         * Validates the scene name, creates a new EditorScene object, and configures
         * appropriate docking settings for the new scene window.
         *
         * @param newSceneName The name for the new scene.
         * @return true if the scene was created successfully, false otherwise.
         */
        bool handleSceneCreation(const std::string& newSceneName) const;

        /**
         * @brief Sets up docking for a new scene window.
         *
         * Creates a new docking node and configures it to contain the specified windows,
         * positioning it at the location of the existing floating window.
         *
         * @param floatingWindowName The name of the existing floating window to use as a reference.
         * @param newSceneName The name of the new scene window to dock.
         * @return true if the docking setup was successful, false otherwise.
         */
        bool setupNewDockSpaceNode(const std::string& floatingWindowName, const std::string& newSceneName) const;

        enum class SceneTreeState
        {
            GLOBAL,
            NB_STATE
        };

        WindowState m_defaultState;
        bool m_forceExpandAll = false;
        bool m_forceCollapseAll = false;
        bool m_resetExpandState = false;

        // Add these method declarations
        void setupShortcuts();
        void setupDefaultState();

        // Add these callback methods
        void deleteSelectedCallback();
        void addEntityCallback();
        void expandAllCallback();
        void collapseAllCallback();
        void renameSelectedCallback();
        static void duplicateSelectedCallback();

        static void selectAllCallback();
        static void hideSelectedCallback();
        static void showAllCallback();

        /**
        * @brief Handles drag source setup for scene objects.
        * @param object The scene object being dragged.
        */
        static void handleDragSource(const SceneObject& object);

        /**
        * @brief Handles drop target setup for scene objects.
        * @param object The scene object that can receive drops.
        */
        static void handleDropTarget(const SceneObject& object);

        /**
        * @brief Processes the drop operation from the scene tree itself.
        * @param dropTarget The target scene object receiving the drop.
        * @param payload The drag-and-drop payload data.
        */
        static void handleDropFromSceneTree(const SceneObject& dropTarget, const struct SceneTreeDragDropPayload& payload);

        /**
        * @brief Processes the drop operation from the asset manager.
        * @param dropTarget The target scene object receiving the drop.
        * @param payload The drag-and-drop payload data.
        */
        static void handleDropFromAssetManager(const SceneObject& dropTarget, const AssetDragDropPayload& payload);

        /**
        * @brief Validates if a drop operation is allowed.
        * @param dropTarget The target scene object.
        * @param payload The drag-and-drop payload data.
        * @return true if the drop is valid, false otherwise.
        */
        static bool canAcceptDrop(const SceneObject& dropTarget, const SceneTreeDragDropPayload& payload);
    };

    /**
     * @brief Payload structure for drag and drop operations in the scene tree.
     *
     * Contains all necessary information to perform entity/scene drag and drop
     * operations including validation and hierarchy updates.
     */
    struct SceneTreeDragDropPayload
    {
        ecs::Entity entity; ///< The entity being dragged
        scene::SceneId sourceSceneId; ///< The scene the entity originated from
        SelectionType type; ///< The type of object being dragged
        std::string uuid; ///< UUID of the dragged object
        std::string name; ///< Display name of the dragged object
    };
}
