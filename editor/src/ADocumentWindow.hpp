//// ADocumentWindow.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the document window abstract class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IDocumentWindow.hpp"
#include "SceneManagerBridge.hpp"
#include "Nexo.hpp"

namespace nexo::editor {
    class ADocumentWindow : public IDocumentWindow {
        public:
            explicit ADocumentWindow() : m_sceneManagerBridge(SceneManagerBridge::getInstance()) {};
            ~ADocumentWindow() override = default;

            [[nodiscard]] bool isFocused() const override { return m_focused; }
            [[nodiscard]] bool isOpened() const override { return m_opened; }

            /**
             * @brief Get the opened value
             * @note Used to give a ref to ImGui
             * @return ref to bool _opened
             */
            [[nodiscard]] bool &getOpened() override { return m_opened; }
        protected:
            bool m_opened = true;
            bool m_focused = false;

            /**
             * @brief Ref to singleton SceneManagerBridge
             * Responsible of the communication between the editor and the engine
             */
            SceneManagerBridge &m_sceneManagerBridge;
    };
}