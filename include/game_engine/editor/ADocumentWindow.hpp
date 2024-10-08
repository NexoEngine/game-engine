/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** ADocumentWindow.hpp
*/

#pragma once

#include <GameEngine.hpp>
#include <SceneHandling.hpp>

#include "IDocumentWindow.hpp"
#include "SceneManagerBridge.hpp"

namespace engine::editor {
    class ADocumentWindow : public IDocumentWindow {
        public:
            ADocumentWindow() : _sceneManagerBridge(SceneManagerBridge::getInstance(engine::getSceneManager())) {}
            ~ADocumentWindow() override = default;

            [[nodiscard]] bool isFocused() const override { return _focused; }
            [[nodiscard]] bool isOpened() const override { return _opened; }

            /**
             * @brief Get the opened value
             * @note Used to give a ref to ImGui
             * @return ref to bool _opened
             */
            [[nodiscard]] bool &getOpened() override { return _opened; }
        protected:
            bool _opened = true;
            bool _focused = false;

            /**
             * @brief Ref to singleton SceneManagerBridge
             * Responsible of the communication between the editor and the engine
             */
            SceneManagerBridge &_sceneManagerBridge;
    };
} // namespace engine::editor
