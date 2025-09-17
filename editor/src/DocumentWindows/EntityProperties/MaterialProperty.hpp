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
        ecs::Entity m_selectedEntity{};
        assets::AssetRef<assets::Material> material;
    };

    class MaterialProperty final : public AEntityProperty {
       public:
        using AEntityProperty::AEntityProperty;

        /**
         * @brief Displays and manages the material properties of an entity.
         *
         * This function checks if the specified entity has a material component and, if so, displays its properties
         * in a collapsible header. It includes a thumbnail of the material, options to create a new material or
         * modify the existing one, and handles pop-up dialogs for material creation. The function also integrates
         * with the inspector window to allow detailed editing of the material properties.
         *
         * @param entity The entity whose material properties are to be displayed.
         */
        void show(ecs::Entity entity) override;

       private:
        /**
         * @brief Creates the material creation popup UI.
         *
         * This function sets up the user interface for creating a new material. It includes a preview of the material,
         * options to edit its properties, and buttons to apply or cancel the creation process. The function manages
         * temporary assets and scene previews to facilitate the material creation workflow.
         *
         * @param entity The entity for which the new material is being created.
         */
        static void createMaterialPopup(ecs::Entity entity);

        /**
         * @brief Cleans up resources used in the material creation popup.
         *
         * This function deletes temporary material assets and scene previews created during the material creation
         * process. It ensures that resources are properly released and closes the popup dialog.
         *
         * @param materialRef A reference to the temporary material asset to be deleted.
         * @param out A reference to the scene preview output structure to manage scene cleanup.
         */
        static void cleanupPopup(assets::AssetRef<assets::Material> &materialRef, utils::ScenePreviewOut &out);

        PopupManager m_popupManager;
    };
} // namespace nexo::editor
