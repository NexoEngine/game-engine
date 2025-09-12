//// MainScene.hpp ////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the main document window
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <imgui.h>

#include <ImGuizmo.h>
#include "../PopupManager.hpp"
#include "ADocumentWindow.hpp"
#include "Definitions.hpp"
#include "DocumentWindows/AssetManager/AssetManagerWindow.hpp"
#include "ImNexo/Widgets.hpp"
#include "core/scene/SceneManager.hpp"
#include "inputs/WindowState.hpp"

namespace nexo::editor {
    class EditorScene final : public ADocumentWindow {
       public:
        using ADocumentWindow::ADocumentWindow;

        /**
         * @brief Initializes the main scene.
         *
         * Configures essential components of the main scene by sequentially:
         * - Setting up ImGuizmo parameters,
         * - Initializing the window settings, and
         * - Creating and configuring the scene.
         */
        void setup() override;

        /**
         * @brief Shuts down the main scene.
         *
         * This method is a no-op in the EditorScene class, as there are no specific shutdown
         * procedures required for this scene type.
         */
        void shutdown() override;

        /**
         * @brief Displays the main scene window and updates the active scene selection.
         *
         * This method creates an ImGui window with specific size constraints and zero padding,
         * then determines the window's focus status to update the scene's active state. When focused,
         * it sets the current scene as selected in the scene view manager and clears any entity selection.
         * Finally, it renders both the scene view and transformation gizmos within the window.
         */
        void show() override;

        bool showToolbar      = true;
        bool isPhysicsRunning = false;

        /**
         * @brief Updates the scene by processing input events and rendering the current frame.
         *
         * This function handles key events and updates the scene by executing the rendering engine in framebuffer mode.
         * It processes left mouse clicks when the scene view is focused and ImGuizmo is not active. The mouse position
         * is adjusted relative to the viewport and its y-coordinate is flipped to match OpenGL's texture format. If the
         * click falls within valid bounds and corresponds to a valid entity (pixel value not equal to -1), the entity
         * is selected and the SceneViewManager is notified of the active scene; otherwise, any existing selection is
         * cleared.
         *
         * The update is skipped entirely if the scene window is not open.
         */
        void update() override;

        /**
         * @brief Retrieves the unique identifier of the scene.
         *
         * @return scene::SceneId The identifier of this scene.
         */
        [[nodiscard]] scene::SceneId getSceneId() const
        {
            return m_sceneId;
        };

        /**
         * @brief Sets the active camera for this scene.
         *
         * Deactivates the current camera and switches to the specified camera entity.
         * The previously active camera will have its render and active flags set to false.
         *
         * @param cameraId Entity ID of the camera to set as active.
         */
        void setCamera(ecs::Entity cameraId);

        /**
         * @brief Marks this scene as the default scene.
         *
         * When a scene is set as the default, it will be populated with
         * default entities (lights, basic geometry) during setup.
         */
        void setDefault()
        {
            m_defaultScene = true;
        };

       private:
        // Scene properties
        bool m_defaultScene = false;
        int m_sceneId       = -1;
        std::string m_sceneUuid;

        // Camera management
        int m_activeCamera = -1;
        int m_editorCamera = -1;

        // Viewport and rendering
        ImVec2 m_viewportBounds[2];
        bool m_wireframeEnabled = false;

        // Entity selection
        ecs::Entity m_entityHovered = ecs::INVALID_ENTITY;

        // Gizmo settings
        ImGuizmo::OPERATION m_currentGizmoOperation = ImGuizmo::UNIVERSAL;
        ImGuizmo::MODE m_currentGizmoMode           = ImGuizmo::WORLD;

        // Snapping settings
        bool m_snapTranslateOn    = false;
        glm::vec3 m_snapTranslate = {10.0f, 10.0f, 10.0f};
        bool m_snapRotateOn       = false;
        float m_angleSnap         = 90.0f;
        bool m_snapToGrid         = false;

        // UI management
        PopupManager m_popupManager;

        // Button styling
        const std::vector<ImNexo::GradientStop> m_buttonGradient   = {{0.0f, IM_COL32(50, 50, 70, 230)},
                                                                      {1.0f, IM_COL32(30, 30, 45, 230)}};
        const std::vector<ImNexo::GradientStop> m_selectedGradient = {{0.0f, IM_COL32(70, 70, 120, 230)},
                                                                      {1.0f, IM_COL32(50, 50, 100, 230)}};

        // Game window management
        bool m_shouldFocusGameWindow = false;
        std::string m_gameWindowToFocus;
        bool m_shouldSplitDock = false;
        std::string m_gameWindowNameToSplit;

        // Timecode management
        std::vector<float> m_timecodeSeconds{5.0f, 8.0f, 5.0f, 10.0f};
        int m_currentTimecodeIndex = 0;
        bool m_isTimecodeActive    = false;
        float m_timecodeElapsed    = 0.0f;

        /**
         * @brief Sets the main scene window's view size.
         *
         * Configures the view to a default size of 1280x720 pixels.
         */
        void setupWindow();

        /**
         * @brief Creates and initializes a scene with basic components.
         *
         * Sets up the scene with a framebuffer, editor camera, and loads default
         * entities if this is the default scene.
         */
        void setupScene();

        /**
         * @brief Handles drag-and-drop events for adding models and textures to the scene.
         *
         * This method checks if a drag-and-drop operation is occurring over the scene viewport.
         * If a model or texture asset is dropped, it creates the corresponding entity in the scene
         * at a default position. The method also records the creation action for undo/redo functionality.
         */
        void hideAllButSelectionCallback() const;

        /**
         * @brief Selects all entities in the scene.
         *
         * Iterates through all entities in the current scene and adds them to the selection
         * context, excluding the editor camera. Updates the window state to the gizmo state
         * after selection.
         */
        void selectAllCallback();

        /**
         * @brief Unhides all entities in the scene.
         *
         * Iterates through all entities in the current scene and sets their RenderComponent's
         * isRendered property to true, effectively unhiding them. Records the changes as actions
         * for undo/redo functionality.
         */
        void unhideAllCallback() const;

        /**
         * @brief Deletes the currently selected entities from the scene.
         *
         * Removes all selected entities from the scene and records the deletions as actions
         * for undo/redo functionality. Clears the selection context after deletion and resets
         * the window state to the global state.
         */
        void deleteCallback();

        /** @
         * brief Sets up command bindings for various editor states.
         *
         * Initializes command mappings for the global state, gizmo state, and specific
         * gizmo modes (translate, rotate, scale). Also sets up keyboard shortcuts for
         * common actions within the editor.
         */
        void setupGlobalState();

        /**
         * @brief Sets up command bindings for the gizmo state.
         *
         * Initializes command mappings for the gizmo state, allowing users to switch
         * between different transformation modes (translate, rotate, scale) and modify
         * gizmo behavior using keyboard shortcuts. Includes commands for toggling snapping,
         * locking axes, and changing the gizmo operation.
         */
        void setupGizmoState();

        /**
         * @brief Sets up command bindings for the gizmo translate state.
         *
         * Initializes command mappings for the translation state of the gizmo,
         * allowing users to switch between different transformation modes (translate, rotate, scale)
         * and modify gizmo behavior using keyboard shortcuts. It includes commands for toggling
         * snapping, locking axes, and changing the gizmo operation.
         */
        void setupGizmoTranslateState();

        /**
         * @brief Sets up command bindings for the gizmo scale state.
         *
         * Initializes command mappings for the scale state of the gizmo,
         * allowing users to switch between different transformation modes (translate, rotate, scale)
         * and modify gizmo behavior using keyboard shortcuts. It includes commands for toggling
         * snapping, locking axes, and changing the gizmo operation.
         */
        void setupGizmoRotateState();

        /**
         * @brief Sets up command bindings for the gizmo scale state.
         *
         * Initializes command mappings for the scale state of the gizmo,
         * allowing users to switch between different transformation modes (translate, rotate, scale)
         * and modify gizmo behavior using keyboard shortcuts. It includes commands for toggling
         * snapping, locking axes, and changing the gizmo operation.
         */
        void setupGizmoScaleState();

        /**
         * @brief Sets up all keyboard shortcuts for the editor scene.
         *
         * This function initializes the global state and various gizmo states (translate, rotate, scale)
         * by registering commands with their associated key bindings and actions. It configures how
         * different key combinations affect the editor's behavior, such as selecting all entities,
         * deleting entities, switching between gizmo modes, and toggling snapping.
         */
        void setupShortcuts();

        /**
         * @brief Populates the scene with default entities.
         *
         * Creates standard light sources (ambient, directional, point, spot)
         * and a simple ground plane in the scene.
         */
        static void loadDefaultEntities();

        /**
         * @brief Creates a physics-enabled entity in the scene.
         *
         * This method creates an entity with a mesh, material, transform, and physics components
         * based on the provided parameters. The entity is added to the current scene and configured
         * with the specified position, size, rotation, color, shape type, and motion type.
         *
         * @param pos The position of the entity in world space.
         * @param size The size (scale) of the entity.
         * @param rotation The rotation of the entity in Euler angles (degrees).
         * @param color The color of the entity as a glm::vec4 (RGBA).
         * @param shapeType The shape type for the physics collider (e.g., box, sphere).
         * @param motionType The motion type for the physics body (e.g., static, dynamic).
         */
        void createEntityWithPhysic(const glm::vec3& pos, const glm::vec3& size, const glm::vec3& rotation,
                                    const glm::vec4& color, system::ShapeType shapeType,
                                    JPH::EMotionType motionType) const;

        /**
         * @brief Adds a 3D model to the scene at the specified position, scale, and rotation.
         *
         * This method creates an entity with a model component using the provided model path,
         * and sets its transform based on the given position, scale, and rotation. The entity
         * is then added to the current scene.
         *
         * @param modelPath The file path to the 3D model asset.
         * @param position The position of the model in world space.
         * @param scale The scale of the model.
         * @param rotation The rotation of the model in Euler angles (degrees).
         */
        void addModelToScene(const std::string& modelPath, const glm::vec3& position,
                             const glm::vec3& scale    = {1.0f, 1.0f, 1.0f},
                             const glm::vec3& rotation = {0.0f, 0.0f, 0.0f}) const;

        /**
         * @brief Renders the toolbar overlay within the main scene view.
         *
         * This method uses ImGui to display a toolbar that includes buttons for switching between orthographic and
         * perspective camera modes, a popup placeholder for adding primitive entities, and a draggable input for
         * adjusting the target frames per second (FPS). The toolbar is positioned relative to the current view to align
         * with the scene layout.
         */
        void renderToolbar();

        /**
         * @brief Sets up the initial layout and style for the toolbar.
         *
         * Creates a child window with specific size and style settings for the toolbar,
         * positions it at the top of the viewport, and configures spacing.
         *
         * @param buttonWidth Standard width for toolbar buttons
         */
        void initialToolbarSetup(float buttonWidth) const;

        /**
         * @brief Renders the editor camera button in the toolbar.
         *
         * Shows either a camera settings button (when editor camera is active) or a
         * "switch back to editor camera" button (when a different camera is active).
         */
        void renderEditorCameraToolbarButton();

        /**
         * @brief Renders the button to toggle between world/local coordinate modes.
         *
         * Updates the button props based on the current mode and renders the appropriate
         * button with correct styling.
         *
         * @param showGizmoModeMenu Flag indicating if the mode dropdown menu is visible
         * @param activeGizmoMode Reference to store the active mode button properties
         * @param inactiveGizmoMode Reference to store the inactive mode button properties
         * @return true if the button was clicked
         */
        bool renderGizmoModeToolbarButton(bool showGizmoModeMenu, ImNexo::ButtonProps& activeGizmoMode,
                                          ImNexo::ButtonProps& inactiveGizmoMode);

        /**
         * @brief Renders the primitive creation dropdown menu.
         *
         * Creates a dropdown with buttons for adding primitive shapes like cubes,
         * spheres, etc. to the scene.
         *
         * @param primitiveButtonPos Position of the parent button that opened this menu
         * @param buttonSize Size of buttons in the dropdown
         * @param showPrimitiveMenu Reference to the flag controlling menu visibility
         */
        void renderPrimitiveSubMenu(const ImVec2& primitiveButtonPos, const ImVec2& buttonSize,
                                    bool& showPrimitiveMenu);

        /**
         * @brief Renders the snap settings dropdown menu.
         *
         * Creates a dropdown with toggles for different snapping modes (translate, rotate)
         * and their settings.
         *
         * @param snapButtonPos Position of the parent button that opened this menu
         * @param buttonSize Size of buttons in the dropdown
         * @param showSnapMenu Reference to the flag controlling menu visibility
         */
        void renderSnapSubMenu(const ImVec2& snapButtonPos, const ImVec2& buttonSize, bool& showSnapMenu);

        /**
         * @brief Handles the snap settings popup dialog.
         *
         * Creates a modal popup allowing users to configure fine-grained snap settings
         * like translate values and rotation angles.
         */
        void snapSettingsPopup();

        /**
         * @brief Handles the grid settings popup dialog.
         *
         * Creates a modal popup allowing users to configure grid visibility and size.
         */
        void gridSettingsPopup();

        /**
         * @brief Renders a standard toolbar button with optional tooltip and styling.
         *
         * Creates a gradient button with the specified icon and shows a tooltip on hover.
         *
         * @param uniqueId Unique identifier for the ImGui control
         * @param icon Font icon to display on the button
         * @param tooltip Text to show when hovering over the button
         * @param gradientStop Color gradient for the button background
         * @param rightClicked Optional pointer to track right-click events
         * @return true if the button was clicked
         */
        static bool renderToolbarButton(const std::string& uniqueId, const std::string& icon,
                                        const std::string& tooltip,
                                        const std::vector<ImNexo::GradientStop>& gradientStop,
                                        bool* rightClicked = nullptr);

        /**
         * @brief Retrieves the last used gizmo operation mode.
         *
         * @return ImGuizmo::OPERATION The last gizmo operation mode (translate, rotate, scale).
         */
        ImGuizmo::OPERATION getLastGuizmoOperation();

        /**
         * @brief Renders the transformation gizmo for the selected entity.
         *
         * This method displays an interactive ImGuizmo tool to manipulate the translation, rotation, and scale
         * of the currently selected entity. It first verifies that the selection is an entity and that the active
         * scene corresponds to the one managed by this instance. The method then retrieves the view and projection
         * matrices from the active camera, configures ImGuizmo to match the view's dimensions, and constructs the
         * entity's transformation matrix from its current translation, rotation, and scale.
         *
         * If the gizmo is actively manipulated, the entity's transform component is updated with the new values.
         */
        void renderGizmo();

        /**
         * @brief Configures ImGuizmo with the camera's parameters.
         *
         * Sets up ImGuizmo to use the provided camera's view and projection matrices,
         * and configures it for orthographic or perspective mode based on the camera type.
         *
         * @param camera The camera component whose parameters will be used to set up ImGuizmo.
         */
        void setupGizmoContext(const components::CameraComponent& camera) const;

        /**  * @brief Retrieves the snap settings for a given gizmo operation.
         *
         * This function checks if snapping is enabled for the specified operation
         * (translation or rotation) and returns a pointer to the corresponding snap
         * value array. If snapping is not enabled for the operation, it returns nullptr.
         *
         * @param operation The ImGuizmo operation type (TRANSLATE or ROTATE).
         * @return A pointer to the snap settings array if snapping is enabled; otherwise, nullptr.
         */
        float* getSnapSettingsForOperation(ImGuizmo::OPERATION operation);

        /**  * @brief Captures the initial transform states of multiple entities.
         *
         * This method stores the current state of the TransformComponent for each entity
         * in the provided list. The states are saved in a static map, allowing for later
         * comparison to detect changes after transformations are applied.
         *
         * @param entities A vector of entity IDs whose transform states will be captured.
         */
        static void captureInitialTransformStates(const std::vector<int>& entities);

        /**  * @brief Applies a world transformation delta to multiple entities.
         *
         * This function takes a source entity's old and new world transformation matrices,
         * computes the delta transformation, and applies this delta to a list of target entities.
         * Each target entity's TransformComponent is updated to reflect the new world position,
         * rotation, and scale based on the computed delta.
         *
         * @param sourceEntity The entity whose transformation change is the basis for the delta.
         * @param oldWorldMatrix The original world transformation matrix of the source entity.
         * @param newWorldMatrix The new world transformation matrix of the source entity.
         * @param targetEntities A vector of entity IDs to which the delta transformation will be applied.
         */
        static void applyTransformToEntities(ecs::Entity sourceEntity, const glm::mat4& oldWorldMatrix,
                                             const glm::mat4& newWorldMatrix, const std::vector<int>& targetEntities);

        /**  * @brief Creates undo actions for transform changes on multiple entities.
         *
         * This method generates and records undo actions for a set of entities that have undergone
         * transformation changes. It compares the initial and current states of each entity's
         * TransformComponent and, if a change is detected, creates a TransformChangeAction to
         * encapsulate the before-and-after states. All generated actions are then recorded with
         * the ActionManager to enable undo/redo functionality.
         *
         * @param entities A vector of entity IDs that have been transformed.
         */
        static void createTransformUndoActions(const std::vector<int>& entities);

        static bool s_wasUsingGizmo;
        static ImGuizmo::OPERATION s_lastOperation;
        static std::unordered_map<ecs::Entity, components::TransformComponent::Memento> s_initialTransformStates;

        /**
         * @brief Renders the main viewport showing the 3D scene.
         *
         * Handles resizing of the viewport, draws the framebuffer texture containing the
         * rendered scene, and updates viewport bounds for input handling.
         */
        void renderView();

        /**
         * @brief Renders a message when no active camera is set in the scene.
         *
         * Displays a centered message within the viewport area indicating that no active
         * camera is available for rendering the scene.
         */
        void renderNoActiveCamera() const;

        /**
         * @brief Renders the popup for creating primitive shapes.
         *
         * Displays a modal popup allowing users to select and create various primitive
         * shapes (cube, sphere, plane, etc.) in the scene.
         *
         * @param primitive The type of primitive to create.
         */
        void renderPrimitiveCreationPopup(const Primitives& primitive) const;

        /**
         * @brief Renders the popup for creating new entities.
         *
         * Displays a modal popup with options to create different types of entities,
         * including primitives, models, lights, and cameras. Handles user interactions
         * to instantiate the selected entity type in the scene.
         */
        void renderNewEntityPopup();

        /**
         * @brief Renders the popup for creating a sphere primitive.
         *
         * Displays a modal popup allowing users to specify parameters for creating
         * a sphere primitive in the scene, such as radius, segments, and color.
         */
        void handleSelection();

        /**
         * @brief Handles drag-and-drop events for adding models and textures to the scene.
         *
         * This method checks if a drag-and-drop operation is occurring over the scene viewport.
         * If a model or texture asset is dropped, it creates the corresponding entity in the scene
         * at a default position. The method also records the creation action for undo/redo functionality.
         */
        void handleDropTarget();

        /**  * @brief Handles the drop of a model asset into the scene.
         *
         * Creates a new model entity in the scene at a default position when a model
         * asset is dropped onto the viewport. Records the creation action for undo/redo.
         *
         * @param payload The drag-and-drop payload containing model asset information.
         */
        void handleDropModel(const AssetDragDropPayload& payload) const;

        /**  * @brief Handles the drop of a texture asset into the scene.
         *
         * Applies the dropped texture to the currently selected entity if it has
         * a material component. If no entity is selected, it samples the entity
         * under the mouse cursor and applies the texture to it. Records the change
         * as an action for undo/redo functionality.
         *
         * @param payload The drag-and-drop payload containing texture asset information.
         */
        void handleDropTexture(const AssetDragDropPayload& payload) const;

        /**
         * @brief Handles the drop of a material asset into the scene.
         *
         * Applies the dropped material to the currently selected entity if it has
         * a material component. If no entity is selected, it samples the entity
         * under the mouse cursor and applies the material to it. Records the change
         * as an action for undo/redo functionality.
         *
         * @param payload The drag-and-drop payload containing material asset information.
         */
        void handleDropMaterial(const AssetDragDropPayload& payload) const;

        /**  * @brief Samples the entity ID at the given mouse coordinates in the viewport.
         *
         * Reads the pixel value from the framebuffer at the specified mouse coordinates
         * to determine which entity is under the cursor. The pixel value corresponds to
         * the entity ID, with -1 indicating no entity is present at that location.
         *
         * @param mx The x-coordinate of the mouse within the viewport.
         * @param my The y-coordinate of the mouse within the viewport.
         * @return The entity ID under the mouse cursor, or -1 if no entity is present.
         */
        [[nodiscard]] int sampleEntityTexture(float mx, float my) const;

        /**
         * @brief Finds the root parent entity of the given entity.
         *
         * Traverses the entity hierarchy upwards to locate the top-most parent entity.
         * Returns the root parent entity ID, or the input entity ID if it has no parent.
         *
         * @param entityId The entity whose root parent is to be found.
         * @return The root parent entity ID.
         */
        static ecs::Entity findRootParent(ecs::Entity entityId);

        /**
         * @brief Selects the entire hierarchy of the given entity.
         *
         * Selects the entity and all its children in the hierarchy. If Ctrl is pressed,
         * the selection is added to the current selection context.
         *
         * @param entityId The entity whose hierarchy will be selected.
         * @param isCtrlPressed True if Ctrl is pressed for multi-selection.
         */
        void selectEntityHierarchy(ecs::Entity entityId, bool isCtrlPressed);

        /**
         * @brief Selects all child entities of a model.
         *
         * Adds all children of the given model entities to the selection context.
         * If Ctrl is pressed, the selection is added to the current selection.
         *
         * @param children Vector of child entity IDs to select.
         * @param isCtrlPressed True if Ctrl is pressed for multi-selection.
         */
        void selectModelChildren(const std::vector<ecs::Entity>& children, bool isCtrlPressed);

        /**
         * @brief Updates the selection context based on user input.
         *
         * Handles selection logic for entities, supporting Shift and Ctrl modifiers
         * for multi-selection and range selection.
         *
         * @param entityId The entity to update selection for.
         * @param isShiftPressed True if Shift is pressed for range selection.
         * @param isCtrlPressed True if Ctrl is pressed for multi-selection.
         */
        void updateSelection(int entityId, bool isShiftPressed, bool isCtrlPressed);

        /**
         * @brief Updates the window state after selection changes.
         *
         * Synchronizes the window state with the current selection context,
         * ensuring the UI reflects the latest selection.
         */
        void updateWindowState();

        /**
         * @brief Updates the timecode for video textures in the scene.
         *
         * Advances the timecode and updates playback for all video textures
         * according to the current timecode state.
         */
        void handleTimecodeUpdate();

        /**
         * @brief Skips all video textures to the previous keyframe.
         *
         * Iterates through all video textures in the scene and rewinds their
         * playback to the previous keyframe.
         */
        void skipVideosToPreviousKeyframe() const;

        /**
         * @brief Skips all video textures in the scene to the next keyframe.
         *
         * Iterates through all entities in the current scene, identifies those with
         * VideoTextureComponent, and advances their playback to the next keyframe.
         */
        void skipVideosToNextKeyframe() const;

        /**
         * @brief Creates a new game window or focuses an existing one.
         *
         * Checks if a game window for the current scene already exists. If it does,
         * makes it visible and focuses it. Otherwise, creates a new game window,
         * configures it with the scene ID and UUID, and schedules it for docking
         * and focusing on the next frame.
         */
        void createOrFocusGameWindow();

        /** @brief Spawns a scene with multiple balls for physics testing.
         *
         * Creates a grid of sphere entities with physics properties, arranged in
         * a 5x5x5 formation. Each sphere is given a random color and positioned
         * based on the provided offset.
         *
         * @param offset A glm::vec3 representing the positional offset to apply to all spheres.
         */
        void spawnBallsScene(const glm::vec3& offset = {0.0f, 0.0f, 0.0f}) const;

        /**
         * @brief Creates a scene with multiple light sources.
         *
         * Adds various types of lights (directional, point, spot, etc.) to the scene,
         * positioned according to the given offset. Useful for testing lighting and visual effects.
         *
         * @param offset Position offset to apply to all lights.
         */
=======
        void spawnBallsScene(const glm::vec3& offset = {0.0f, 0.0f, 0.0f}) const;

>>>>>>> 48d37ed4 (feat(demo): physics ball spawn by pressing L)
        void lightsScene(const glm::vec3& offset = {0.0f, 0.0f, 0.0f}) const;

        /**
         * @brief Creates a scene with physics entities.
         *
         * Adds objects with physical properties (rigid, dynamic) to the scene,
         * positioned according to the given offset. Useful for testing physics interactions.
         *
         * @param offset Position offset to apply to all physics entities.
         */
        void physicScene(const glm::vec3& offset = {0.0f, 0.0f, 0.0f}) const;

        /**
         * @brief Creates a scene containing video textures.
         *
         * Adds entities with animated (video) textures to the scene, positioned according to the offset.
         * Useful for testing video playback and synchronization in the engine.
         *
         * @param offset Position offset to apply to all video textures.
         */
        void videoScene(const glm::vec3& offset = {0.0f, 0.0f, 0.0f}) const;

        /**
         * @brief Creates a forest-type scene.
         *
         * Adds many vegetation objects (trees, bushes) to the scene, positioned according to the offset.
         * Useful for testing rendering and management of large numbers of entities.
         *
         * @param offset Position offset to apply to all forest elements.
         */
        void forestScene(const glm::vec3& offset = {0.0f, 0.0f, 0.0f}) const;

        enum class EditorState { GLOBAL, GIZMO, GIZMO_TRANSLATE, GIZMO_ROTATE, GIZMO_SCALE, NB_STATE };

        WindowState m_globalState;
        WindowState m_gizmoState;
        WindowState m_gizmoTranslateState;
        WindowState m_gizmoRotateState;
        WindowState m_gizmoScaleState;
    };
} // namespace nexo::editor
