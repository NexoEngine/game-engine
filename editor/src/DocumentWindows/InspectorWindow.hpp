//// InspectorWindow.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie Giacomel
//  Date:        23/11/2024
//  Description: Header file for the inspector window
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ADocumentWindow.hpp"

#include <imgui.h>

namespace nexo::editor {
    class InspectorWindow : public ADocumentWindow {
        public:
            InspectorWindow();
            ~InspectorWindow() override;

            void setup() override;
            void shutdown() override;

            void show() override;
            void update() override;

        private:
            void showTransformComponent();
            void showRendererComponent();
    };
}