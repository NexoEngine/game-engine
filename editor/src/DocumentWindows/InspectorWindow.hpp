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
    class InspectorWindow final : public ADocumentWindow {
    public:
        InspectorWindow();
        ~InspectorWindow() override;

	        void setup() override;
	        void shutdown() override;

	        void show() override;
	        void update() override;

    private:
        // std::unordered_map<std::type_index, void (InspectorWindow::*)()> m_componentShowFunctions;
        std::unordered_map<std::type_index, int (*)(ecs::Entity)> m_componentShowFunctions;

        void showEntityProperties();
        void showComponent(const std::type_index& type);
        // void showTransformComponent();
        // void showRendererComponent();
    };
};
