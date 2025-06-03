//// MainScene.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the main document window
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ADocumentWindow.hpp"
#include "inputs/WindowState.hpp"
#include "core/scene/SceneManager.hpp"
#include "../PopupManager.hpp"
#include <imgui.h>
#include <ImGuizmo.h>
#include "ImNexo/Widgets.hpp"

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

            // No-op method in this class
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

            /**
             * @brief Updates the scene by processing input events and rendering the current frame.
             *
             * This function handles key events and updates the scene by executing the rendering engine in framebuffer mode.
             * It processes left mouse clicks when the scene view is focused and ImGuizmo is not active. The mouse position is
             * adjusted relative to the viewport and its y-coordinate is flipped to match OpenGL's texture format. If the click
             * falls within valid bounds and corresponds to a valid entity (pixel value not equal to -1), the entity is selected
             * and the SceneViewManager is notified of the active scene; otherwise, any existing selection is cleared.
             *
             * The update is skipped entirely if the scene window is not open.
             */
            void update() override;

                  /**
            * @brief Retrieves the unique identifier of the scene.
            *
            * @return scene::SceneId The identifier of this scene.
            */
            [[nodiscard]] scene::SceneId getSceneId() const {return m_sceneId;};

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
            void setDefault() { m_defaultScene = true; };

        private:
            bool m_defaultScene = false;
            ImVec2 m_viewportBounds[2];
            ImGuizmo::OPERATION m_currentGizmoOperation = ImGuizmo::UNIVERSAL;
            ImGuizmo::MODE m_currentGizmoMode = ImGuizmo::WORLD;
            bool m_snapTranslateOn = false;
            glm::vec3 m_snapTranslate = {10.0f, 10.0f, 10.0f};
            bool m_snapRotateOn = false;
            float m_angleSnap = 90.0f;
            bool m_snapToGrid = false;
            bool m_wireframeEnabled = false;

            int m_sceneId = -1;
            std::string m_sceneUuid;
            int m_activeCamera = -1;
            int m_editorCamera = -1;

            PopupManager m_popupManager;

            const std::vector<ImNexo::GradientStop> m_buttonGradient = {
                {0.0f, IM_COL32(50, 50, 70, 230)},
                {1.0f, IM_COL32(30, 30, 45, 230)}
            };

            // Selected button gradient - lighter blue gradient
            const std::vector<ImNexo::GradientStop> m_selectedGradient = {
                {0.0f, IM_COL32(70, 70, 120, 230)},
                {1.0f, IM_COL32(50, 50, 100, 230)}
            };

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

            void hideAllButSelectionCallback() const;
            void selectAllCallback();
            void unhideAllCallback() const;
            void deleteCallback();

            void setupGlobalState();
            void setupGizmoState();
            void setupGizmoTranslateState();
            void setupGizmoRotateState();
            void setupGizmoScaleState();
            void setupShortcuts();

            /**
             * @brief Populates the scene with default entities.
             *
             * Creates standard light sources (ambient, directional, point, spot)
             * and a simple ground plane in the scene.
             */
            void loadDefaultEntities() const;

            /**
             * @brief Renders the toolbar overlay within the main scene view.
             *
             * This method uses ImGui to display a toolbar that includes buttons for switching between orthographic and perspective camera modes,
             * a popup placeholder for adding primitive entities, and a draggable input for adjusting the target frames per second (FPS).
             * The toolbar is positioned relative to the current view to align with the scene layout.
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
            bool renderGizmoModeToolbarButton(
            bool showGizmoModeMenu,
                ImNexo::ButtonProps &activeGizmoMode,
                ImNexo::ButtonProps &inactiveGizmoMode
            );

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
            void renderPrimitiveSubMenu(const ImVec2 &primitiveButtonPos, const ImVec2 &buttonSize, bool &showPrimitiveMenu) const;

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
            void renderSnapSubMenu(const ImVec2 &snapButtonPos, const ImVec2 &buttonSize, bool &showSnapMenu);

            /**
             * @brief Handles the snap settings popup dialog.
             *
             * Creates a modal popup allowing users to configure fine-grained snap settings
             * like translate values and rotation angles.
             */
            void snapSettingsPopup();

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
             * @return true if the button was clicked
             */
            static bool renderToolbarButton(
                const std::string &uniqueId,
                const std::string &icon,
                const std::string &tooltip,
                const std::vector<ImNexo::GradientStop> & gradientStop,
                bool *rightClicked = nullptr
            );

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
            void setupGizmoContext(const components::CameraComponent& camera) const;
            float* getSnapSettingsForOperation(ImGuizmo::OPERATION operation);
            static void captureInitialTransformStates(const std::vector<int>& entities);
            void applyTransformToEntities(
                const ecs::Entity sourceEntity,
                const glm::mat4& oldWorldMatrix,
                const glm::mat4& newWorldMatrix,
                const std::vector<int>& targetEntities) const;
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
            void renderNoActiveCamera() const;
            void renderNewEntityPopup();

            void handleSelection();
            int sampleEntityTexture(float mx, float my) const;
            ecs::Entity findRootParent(ecs::Entity entityId) const;
            void selectEntityHierarchy(ecs::Entity entityId, const bool isCtrlPressed);
            void selectModelChildren(const std::vector<components::SubMeshIndex>& children, const bool isCtrlPressed);
            void updateSelection(int entityId, bool isShiftPressed, bool isCtrlPressed);
            void updateWindowState();

            enum class EditorState {
                GLOBAL,
                GIZMO,
                GIZMO_TRANSLATE,
                GIZMO_ROTATE,
                GIZMO_SCALE,
                NB_STATE
            };

            WindowState m_globalState;
            WindowState m_gizmoState;
            WindowState m_gizmoTranslateState;
            WindowState m_gizmoRotateState;
            WindowState m_gizmoScaleState;
    };
}
