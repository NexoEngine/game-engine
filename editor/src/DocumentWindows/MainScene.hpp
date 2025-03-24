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
            explicit MainScene(WindowRegistry &windowRegistry,std::string sceneName, bool defaultScene = false);
            /**
 * @brief Default destructor for MainScene.
 *
 * This destructor is defaulted and performs no custom cleanup.
 */
~MainScene() = default;

            void setup();
            void shutdown() const;
            void show();
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

            void renderToolbar();
            void renderGizmo();
            void renderView();
    };
}
