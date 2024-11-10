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
            MainScene();
            ~MainScene() override;

            void setup() override;
            void shutdown() override;
            void show() override;
            void update() override;


        private:
            ImVec2 _currentWindowSize = {0, 0};
            ImVec2 _prevWindowSize = {0, 0};
            ImVec2 _viewSize = {0, 0};
            ImVec2 _viewPosition = {0, 0};
            ecs::Entity _selectedEntity = 0;
            int _targetFPS = 60;
            ImGuizmo::OPERATION _currentGizmoOperation = ImGuizmo::UNIVERSAL;
            ImGuizmo::MODE _currentGizmoMode = ImGuizmo::WORLD;

            //std::shared_ptr<engine::core::EngineCamera> _camera;
            float _nearPlane = 0.01f;
            float _farPlane = 1000.0f;
            scene::SceneId _sceneID;

            //Shader _gridShader;
            int _matViewLoc;
            int _matProjectionLoc;

            std::shared_ptr<renderer::Framebuffer> m_framebuffer;

            // ---------------------- //
            // --- Internal logic --- //
            // ---------------------- //
            void setupWindow();
            void setupCamera();
            void setupGridShader();
            void loadEntities();

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

            [[nodiscard]] bool isWindowResized() const;
            void handleWindowResize();
            void handleKeyEvents();

            void renderGrid();
            void renderToolbar();
            void renderGizmo();
            void renderView();

            void rayPicking(void);
    };
}