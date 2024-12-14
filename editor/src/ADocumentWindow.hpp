//// ADocumentWindow.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
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
            explicit ADocumentWindow()
            {
                windowId = nextWindowId++;
            };
            ~ADocumentWindow() override = default;

            [[nodiscard]] bool isFocused() const override { return m_focused; }
            [[nodiscard]] bool isOpened() const override { return m_opened; }

            /**
             * @brief Get the opened value
             * @note Used to give a ref to ImGui
             * @return ref to bool _opened
             */
            [[nodiscard]] bool &getOpened() override { return m_opened; }

            void setSceneManager(std::shared_ptr<SceneManagerBridge> bridge) override
            {
                m_sceneManagerBridge = bridge;
            };
            WindowId windowId;
        protected:
            bool m_opened = true;
            bool m_focused = false;

            std::shared_ptr<SceneManagerBridge> m_sceneManagerBridge;
    };
}
