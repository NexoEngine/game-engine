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
#include "core/scene/SceneManager.hpp"
#include "renderer/Framebuffer.hpp"

#include <imgui.h>
#include <ImGuizmo.h>

namespace nexo::editor {
        class MainScene : public ADocumentWindow {
        public:
            MainScene(std::string sceneName, bool defaultScene = false);
            ~MainScene() override;

            void setup() override;
            void shutdown() override;
            void show() override;
            void update() override;

            const std::string &getName() const {return m_sceneName;};
            void setName(const std::string &name) {m_sceneName = name; };

            void setupFramebuffer();

            scene::SceneId getSceneId() {return _sceneID;};

            void hideLayer(scene::LayerId id) { m_hiddenLayers.insert(id); };
            void showLayer(scene::LayerId id) { m_hiddenLayers.erase(id); };
            bool isLayerHidden(scene::LayerId id) { return m_hiddenLayers.contains(id); };
            void addDefaultCameraToLayer(scene::LayerId id) const;

            // Strictly used for display purposes when having multiple view of one scene
            unsigned int idView = 0;
        private:
            std::string m_sceneName;
            bool m_defaultScene = false;
            ImVec2 _viewSize = {0, 0};
            ImVec2 _viewPosition = {0, 0};
            int _targetFPS = 60;
            ImGuizmo::OPERATION _currentGizmoOperation = ImGuizmo::UNIVERSAL;
            ImGuizmo::MODE _currentGizmoMode = ImGuizmo::WORLD;

            std::shared_ptr<camera::OrthographicCameraController> m_camera;
            std::set<scene::LayerId> m_hiddenLayers;
            scene::SceneId _sceneID;

            std::shared_ptr<renderer::Framebuffer> m_framebuffer;

            // ---------------------- //
            // --- Internal logic --- //
            // ---------------------- //
            void setupWindow();
            void setupImguizmo();
            void setupScene();
            void loadDefaultEntities(scene::LayerId defaultLayerId);
            void setHiddenLayerStatus(bool status);

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

            void handleKeyEvents();

            void renderToolbar();
            void renderGizmo();
            void renderView();

            glm::vec2 getMouseWorldPosition() const;

            void rayPicking(void);
    };
}