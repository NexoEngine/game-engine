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
#include "WindowRegistry.hpp"
#include "core/scene/SceneManager.hpp"
#include <imgui.h>
#include <ImGuizmo.h>

namespace nexo::editor {
    class MainScene {
        public:
			/**
			* @brief Constructs a MainScene.
			*
			* Initializes the main scene with a specified name and default scene flag,
			* and stores a reference to the window registry for managing window functionalities.
			*
			* @param sceneName Name of the scene.
			* @param defaultScene Set to true if this scene should be considered the default.
			*/
            explicit MainScene(WindowRegistry &windowRegistry,std::string sceneName, bool defaultScene = false);
            ~MainScene() = default;

            /**
             * @brief Initializes the main scene.
             *
             * Configures essential components of the main scene by sequentially:
             * - Setting up ImGuizmo parameters,
             * - Initializing the window settings, and
             * - Creating and configuring the scene.
             */
            void setup();
            void shutdown() const;

            /**
             * @brief Displays the main scene window and updates the active scene selection.
             *
             * This method creates an ImGui window with specific size constraints and zero padding,
             * then determines the window's focus status to update the scene's active state. When focused,
             * it sets the current scene as selected in the scene view manager and clears any entity selection.
             * Finally, it renders both the scene view and transformation gizmos within the window.
             */
            void show();

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
            void update();

            /**
			 * @brief Indicates whether the scene is currently open.
			 *
			 * This function returns the state of the scene, reflecting if it is actively open and available for interaction.
			 *
			 * @return true if the scene is open; false otherwise.
			 */
			bool isOpened() const { return m_opened; }

            /**
			 * @brief Retrieves the name of the scene.
			 *
			 * @return A constant reference to the scene name.
			 */
			[[nodiscard]] const std::string &getName() const {return m_sceneName;};

            /**
			 * @brief Sets the scene's name.
			 *
			 * Updates the internal identifier of the scene to the provided value.
			 *
			 * @param name New name for the scene.
			 */
			void setName(const std::string_view name) { m_sceneName = name; };

            /**
			 * @brief Retrieves the unique identifier of the scene.
			 *
			 * @return scene::SceneId The identifier of this scene.
			 */
			[[nodiscard]] scene::SceneId getSceneId() const {return m_sceneId;};

            /**
			 * @brief Retrieves the unique identifier for the window.
			 *
			 * This function returns the unique window ID, which is used to reference and manage the associated window within the application.
			 *
			 * @return WindowId Unique identifier for the window.
			 */
			[[nodiscard]] WindowId getWindowId() const { return windowId; }

            /**
			 * @brief Sets the unique identifier for the window.
			 *
			 * Updates the internal window ID to the specified value, which is used to uniquely identify the window associated with the scene.
			 *
			 * @param id The new window identifier.
			 */
			void setWindowId(WindowId id) { windowId = id; }

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

        private:
        	WindowId windowId = nextWindowId++;
            std::string m_sceneName;
            bool m_defaultScene = false;
            bool m_opened = true;
            bool m_focused = false;
            ImVec2 _viewSize = {0, 0};
            ImVec2 _viewPosition = {0, 0};
            ImVec2 m_viewportBounds[2];
            int _targetFPS = 60;
            ImGuizmo::OPERATION _currentGizmoOperation = ImGuizmo::UNIVERSAL;
            ImGuizmo::MODE _currentGizmoMode = ImGuizmo::WORLD;

            WindowRegistry &m_windowRegistry;

            int m_sceneId = -1;
            std::set<ecs::Entity> m_cameras;
            unsigned int m_activeCamera = -1;

            // ---------------------- //
            // --- Internal logic --- //
            // ---------------------- //

            /**
             * @brief Sets the main scene window's view size.
             *
             * Configures the view to a default size of 1280x720 pixels.
             */
            void setupWindow();
            void setupImguizmo() const;
            void setupScene();
            void loadDefaultEntities() const;

            // void add_cube()
            // {
            //     ecs::Entity cube = engine::createCube({0, 0, 0}, 1, 1, 1, WHITE, true);
            //     engine::addEntityToScene(cube, _sceneID);
            // }
            //
            // void add_plan()
            // {
            //     ecs::Entity plan = engine::createPlane({0, 0, 0}, 1, 1, WHITE, true);
            //     engine::addEntityToScene(plan, engine::editor::Main3DScene::_sceneID);
            // }
            //
            // void add_sphere()
            // {
            //     ecs::Entity sphere = engine::createSphere({0, 0, 0}, 1, WHITE, true);
            //     engine::addEntityToScene(sphere, engine::editor::Main3DScene::_sceneID);
            // }
            //
            // void add_cylinder()
            // {
            //     ecs::Entity cylinder = engine::createCylinder({0, 0, 0}, 1, 1, WHITE, true);
            //     engine::addEntityToScene(cylinder, engine::editor::Main3DScene::_sceneID);
            // }
            //
            // void add_cone()
            // {
            //     ecs::Entity cone = engine::createCone({0, 0, 0}, 1, 1, 16, WHITE, true);
            //     engine::addEntityToScene(cone, engine::editor::Main3DScene::_sceneID);
            // }
            //
            // void add_polygon()
            // {
            //     ecs::Entity polygon = engine::createPolygon({0, 0, 0}, 6, 1, 1, WHITE, true);
            //     engine::addEntityToScene(polygon, engine::editor::Main3DScene::_sceneID);
            // }
            //
            // void add_torus()
            // {
            //     ecs::Entity torus = engine::createTorus({0, 0, 0}, 1, 1, 16, 16, WHITE, true);
            //     engine::addEntityToScene(torus, engine::editor::Main3DScene::_sceneID);
            // }
            //
            // void add_knot()
            // {
            //     ecs::Entity knot = engine::createKnot({0, 0, 0}, 1, 1, 16, 16, WHITE, true);
            //     engine::addEntityToScene(knot, engine::editor::Main3DScene::_sceneID);
            // }
            //
            // void add_hemisphere()
            // {
            //     ecs::Entity hemisphere = engine::createHemisphere({0, 0, 0}, 1, 16, 16, WHITE, true);
            //     engine::addEntityToScene(hemisphere, engine::editor::Main3DScene::_sceneID);
            // }

            void handleKeyEvents() const;

            /**
             * @brief Renders the toolbar overlay within the main scene view.
             *
             * This method uses ImGui to display a toolbar that includes buttons for switching between orthographic and perspective camera modes,
             * a popup placeholder for adding primitive entities, and a draggable input for adjusting the target frames per second (FPS).
             * The toolbar is positioned relative to the current view to align with the scene layout.
             */
            void renderToolbar();

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
            void renderView();
    };
}
