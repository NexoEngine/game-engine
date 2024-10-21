/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** Main3DScene.hpp
*/

#pragma once

#include <cstdint>
#include <EntityHandling.hpp>

#include "ImGuizmo.h"

#include "game_engine/editor/ADocumentWindow.hpp"

#include "game_engine/GameEngine.hpp"

#include <raylib.h>

namespace engine::editor {
    class Main3DScene : public ADocumentWindow {
        public:
            Main3DScene();
            ~Main3DScene() override;

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
            ImGuizmo::OPERATION _lastGizmoOperationOver = ImGuizmo::SCALE;

            std::shared_ptr<engine::core::EngineCamera> _camera;
            ecs::SceneID _sceneID;

            // ---------------------- //
            // --- Internal logic --- //
            // ---------------------- //
            void setupWindow();
            void setupScene();
            void setupCamera();
            void loadEntities();

            void add_cube()
            {
                ecs::Entity cube = engine::createCube({0, 0, 0}, 1, 1, 1, WHITE, true);
                engine::addEntityToScene(cube, _sceneID);
            }

            void add_plan()
            {
                // ecs::Entity plan = engine::createPlan({0, 0, 0}, 1, 1, WHITE, true);
                // engine::addEntityToScene(plan, engine::editor::Main3DScene::_sceneID);
            }

            void add_sphere()
            {
                // ecs::Entity sphere = engine::createSphere({0, 0, 0}, 1, 1, 1, WHITE, true);
                // engine::addEntityToScene(sphere, engine::editor::Main3DScene::_sceneID);
            }

            void add_cylinder()
            {
                // ecs::Entity cylinder = engine::createCylinder({0, 0, 0}, 1, 1, 1, 1, WHITE, true);
                // engine::addEntityToScene(cylinder, engine::editor::Main3DScene::_sceneID);
            }

            void add_cone()
            {
                // ecs::Entity cone = engine::createCone({0, 0, 0}, 1, 1, 1, 1, WHITE, true);
                // engine::addEntityToScene(cone, engine::editor::Main3DScene::_sceneID);
            }

            void add_polygon()
            {
                // ecs::Entity polygon = engine::createPolygon({0, 0, 0}, 1, 1, 1, 1, WHITE, true);
                // engine::addEntityToScene(polygon, engine::editor::Main3DScene::_sceneID);
            }

            void add_torus()
            {
                // ecs::Entity torus = engine::createTorus({0, 0, 0}, 1, 1, 1, 1, WHITE, true);
                // engine::addEntityToScene(torus, engine::editor::Main3DScene::_sceneID);
            }

            void add_knot()
            {
                // ecs::Entity knot = engine::createKnot({0, 0, 0}, 1, 1, 1, 1, WHITE, true);
                // engine::addEntityToScene(knot, engine::editor::Main3DScene::_sceneID);
            }

            void add_hemisphere()
            {
                // ecs::Entity hemisphere = engine::createHemisphere({0, 0, 0}, 1, 1, 1, 1, WHITE, true);
                // engine::addEntityToScene(hemisphere, engine::editor::Main3DScene::_sceneID);
            }

            [[nodiscard]] bool isWindowResized() const;
            void handleWindowResize();

            void renderToolbar();
            void renderGizmo();
            void renderView();
    };

} // namespace engine::editor
