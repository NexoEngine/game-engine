//// MaterialProperty.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/06/2025
//  Description: Header file for the material component display
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "AEntityProperty.hpp"
#include "DocumentWindows/InspectorWindow/InspectorWindow.hpp"
#include "DocumentWindows/PopupManager.hpp"
#include "assets/Assets/Material/Material.hpp"
#include "utils/ScenePreview.hpp"

namespace nexo::editor {

    struct MaterialInspectorData {
        ecs::Entity m_selectedEntity;
        assets::AssetRef<assets::Material> material;
    };

    class MaterialProperty final : public AEntityProperty {
        public:
            using AEntityProperty::AEntityProperty;

            void show(ecs::Entity entity) override;
        private:
            void createMaterialPopup(const ecs::Entity entity);
            void cleanupPopup(assets::AssetRef<assets::Material> &materialRef, utils::ScenePreviewOut &out) const;
            PopupManager m_popupManager;
    };
}
