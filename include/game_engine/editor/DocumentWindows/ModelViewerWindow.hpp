/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** ModelViewerWindow.hpp
*/

#pragma once

#include "editor/ADocumentWindow.hpp"
#include "ecs/Scene.hpp"
#include <string>
#include <core/Camera.hpp>

namespace engine::editor {
    class ModelViewerWindow : public ADocumentWindow {
        public:
            ModelViewerWindow();
            ~ModelViewerWindow() override;

            void setup() override;
            void shutdown() override;
            void show() override;
            void update() override;
        private:
            std::string _assetPath = "";
            const char* _filterPatterns[1] = {"*.obj"};
            bool _modelLoaded = false;
            std::shared_ptr<engine::core::EngineCamera> _camera;
            ecs::SceneID _sceneID;
            ecs::Entity _importedEntity{};

            ImVec2 _prevWindowSize;

            ImVec2 _currentWindowSize;

            bool isWindowResized() const;
    };
}
