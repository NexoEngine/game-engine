//// AssetManagerWindow.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        18/11/2024
//  Description: Header file for the AssetManagerWindow class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <ADocumentWindow.hpp>
#include <assets/AssetRef.hpp>
#include <core/event/WindowEvent.hpp>
#include <imgui.h>
#include <set>
#include "DocumentWindows/PopupManager.hpp"
#include "FolderManager.hpp"
#include "assets/Asset.hpp"
#include "utils/TransparentStringHash.hpp"

namespace nexo::editor {

    static constexpr std::string_view INTERNAL_FOLDER_PREFIX = "_internal";
    static constexpr float ERROR_DISPLAY_TIMEOUT             = 3.0f;

    struct FolderCreationState {
        bool isCreatingFolder  = false;
        std::string folderName = "New Folder";
        std::string parentPath;
        bool showError = false;
        std::string errorMessage;
        float errorTimer = ERROR_DISPLAY_TIMEOUT;

        void reset()
        {
            isCreatingFolder = false;
            folderName       = "New Folder";
            parentPath       = "";
            showError        = false;
            errorMessage     = "";
            errorTimer       = 3.0f;
        }
    };

    struct GridLayoutSizes {
        float iconSize  = 64.0f;
        int iconSpacing = 8;

        ImVec2 itemSize;
        ImVec2 itemStep;
        int columnCount;

        static constexpr float THUMBNAIL_HEIGHT_RATIO = 0.8f;
        static constexpr float TITLE_PADDING          = 5.0f;
        static constexpr float OVERLAY_SIZE           = 24.0f;
        static constexpr float OVERLAY_PADDING        = 5.0f;
        static constexpr float CORNER_RADIUS          = 5.0f;
        static constexpr float SELECTED_BOX_THICKNESS = 4.0f;
    };

    struct LayoutColors {
        ImU32 thumbnailBg;
        ImU32 thumbnailBgHovered;
        ImU32 thumbnailBgSelected;
        ImU32 thumbnailBgSelectedHovered;

        ImU32 selectedBoxColor;

        ImU32 titleBg;
        ImU32 titleBgHovered;
        ImU32 titleBgSelected;
        ImU32 titleBgSelectedHovered;

        ImU32 titleText;
    };

    struct LayoutSettings {
        GridLayoutSizes size;
        LayoutColors color;

        float leftPanelWidth = 200.0f;
    };

    struct AssetTypeInfo {
        ImTextureID iconTexture;
        ImU32 backgroundColor;
        std::string tooltip;
    };

    struct AssetLayoutParams {
        ImVec2 itemPos;
        ImVec2 itemSize;
        ImVec2 itemEnd;
        ImVec2 thumbnailEnd;
    };

    class AssetManagerWindow final : public ADocumentWindow, LISTENS_TO(event::EventFileDrop) {
       public:
        using ADocumentWindow::ADocumentWindow;

        void setup() override;
        void shutdown() override;
        void show() override;
        void update() override;

        void handleEvent(event::EventFileDrop& event) override;

       private:
        std::set<unsigned int> m_selectedAssets;

        LayoutSettings m_layout;

        void drawMenuBar();
        void drawPanelSplitter();
        void drawBreadcrumbs();
        void sceneContextMenu();
        void handleRightClick();

        void drawAssetsGrid();
        void drawAssetTitle(const std::shared_ptr<assets::IAsset>& assetData, const AssetLayoutParams& params,
                            bool isHovered) const;
        void drawAsset(const assets::GenericAssetRef& asset, unsigned int index, const ImVec2& itemPos,
                       const ImVec2& itemSize);
        void handleSelection(unsigned int index, bool isSelected);

        assets::AssetType m_selectedType = assets::AssetType::UNKNOWN;
        std::string m_currentFolder; // Currently selected folder
        std::string m_hoveredFolder; // Currently hovered folder
        std::string m_searchBuffer;

        PopupManager m_popupManager;

        void buildFolderStructure();

        void folderTreeContextMenu();
        void drawFolderTree();
        void drawFolderTreeItem(const std::string& name, const std::string& path);

        FolderCreationState m_folderCreationState;
        assets::AssetRef<assets::Texture> m_folderIcon;

        ImTextureID getIconTexture(const assets::AssetRef<assets::Texture>& texture) const;

        void newFolderMenu();
        bool handleNewFolderCreation();
        void drawFolderIcon(const AssetLayoutParams& params) const;
        void drawFolder(const std::string& folderPath, const std::string& folderName, const ImVec2& itemPos,
                        const ImVec2& itemSize);

        std::vector<std::string> m_pendingDroppedFiles;

        void handleDroppedFiles();
        void handleAssetDrop(const std::string& path) const;
        assets::AssetLocation getAssetLocation(const std::filesystem::path& path) const;
        void importDroppedFile(const std::string& filePath) const;

        FolderManager m_folderManager;
    };

    /**
     * @brief Payload structure for drag and drop operations from asset manager.
     *
     * Contains information about the asset being dragged.
     */
    struct AssetDragDropPayload {
        assets::AssetType type; ///< Type of the asset
        assets::AssetID id;     ///< ID of the asset
        char path[256];         ///< Path to the asset
        char name[64];          ///< Display name of the asset
    };
} // namespace nexo::editor
