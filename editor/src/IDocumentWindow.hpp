//// IDocumentWindow.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/11/2024
//  Description: Header file for the document window interface
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SceneManagerBridge.hpp"

namespace nexo::editor {
    class IDocumentWindow {
        public:
        virtual ~IDocumentWindow() = default;
        virtual void setup() = 0;
        virtual void shutdown() = 0;
        virtual void show() = 0;
        virtual void update() = 0;

        virtual void setSceneManager(std::shared_ptr<SceneManagerBridge> bridge) = 0;

        [[nodiscard]] virtual bool isFocused() const = 0;
        [[nodiscard]] virtual bool isOpened() const = 0;
        [[nodiscard]] virtual bool &getOpened() = 0;
    };
}