/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** Main3DScene.hpp
*/

#pragma once

#include <cstdint>

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

            std::shared_ptr<engine::core::EngineCamera> _camera;
            float _nearPlane = 0.01f;
            float _farPlane = 1000.0f;
            ecs::SceneID _sceneID;

            Shader _gridShader;
            int _matViewLoc;
            int _matProjectionLoc;

            // ---------------------- //
            // --- Internal logic --- //
            // ---------------------- //
            void setupWindow();
            void setupScene();
            void setupCamera();
            void setupGridShader();
            void loadEntities();

            [[nodiscard]] bool isWindowResized() const;
            void handleWindowResize();
            void handleKeyEvents();

            void renderGrid();
            void renderToolbar();
            void renderGizmo();
            void renderView();
    };

} // namespace engine::editor
