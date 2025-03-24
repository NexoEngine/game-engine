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
        	using AEntityProperty::AEntityProperty;
            bool show(ecs::Entity entity) override;
            void createMaterialPopup(ecs::Entity entity) const;
        private:
        	PopupManager m_popupManager;
    };
}
