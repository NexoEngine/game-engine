//// RenderProperty.hpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/03/2025
//  Description: Header file for the render property inspector
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AEntityProperty.hpp"
#include "DocumentWindows/PopupManager.hpp"

namespace nexo::editor {
    class RenderProperty : public nexo::editor::AEntityProperty {
        public:
            explicit RenderProperty(const std::string& name);
            ~RenderProperty() override;

            void update() final;

            static int show(ecs::Entity entity);
            static void createMaterialPopup(ecs::Entity entity);
            void showEnd() final;

            static bool showMaterialInspector;
            static components::Material *selectedMaterial;
            static PopupManager popupManager;
    };
}
