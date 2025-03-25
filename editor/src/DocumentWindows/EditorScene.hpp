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
#include "IDocumentWindow.hpp"
#include "core/scene/SceneManager.hpp"
#include <imgui.h>
#include <ImGuizmo.h>

namespace nexo::editor {
    class EditorScene : public ADocumentWindow {
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
			* @brief Removes a camera from the scene and updates the active camera.
			*
			* Removes the specified camera entity from the collection. If the removed camera was the active one,
			* it resets the active camera to an invalid state (-1) and, if any cameras remain, sets the active camera
			* to the first available camera in the collection.
			*
			* @param cameraId The identifier of the camera entity to delete.
			*/
            void deleteCamera(ecs::Entity cameraId);

            void setDefault() { m_defaultScene = true; };

        private:
            bool m_defaultScene = false;
            ImVec2 m_viewSize = {0, 0};
            ImVec2 m_viewPosition = {0, 0};
            ImVec2 m_viewportBounds[2];
            ImGuizmo::OPERATION m_currentGizmoOperation = ImGuizmo::UNIVERSAL;
            ImGuizmo::MODE m_currentGizmoMode = ImGuizmo::WORLD;

            int m_sceneId = -1;
            std::string m_sceneUuid;
            std::set<ecs::Entity> m_cameras;
            int m_activeCamera = -1;

            /**
             * @brief Sets the main scene window's view size.
             *
             * Configures the view to a default size of 1280x720 pixels.
             */
            void setupWindow();
            void setupImguizmo() const;
            void setupScene();
            void loadDefaultEntities() const;

            void handleKeyEvents() const;

            /**
             * @brief Renders the toolbar overlay within the main scene view.
             *
             * This method uses ImGui to display a toolbar that includes buttons for switching between orthographic and perspective camera modes,
             * a popup placeholder for adding primitive entities, and a draggable input for adjusting the target frames per second (FPS).
             * The toolbar is positioned relative to the current view to align with the scene layout.
             */
            void renderToolbar() const;

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
            void renderGizmo() const;
            void renderView();
    };
}
