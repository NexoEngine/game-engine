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
#include <set>
#include <imgui.h>
#include <assets/AssetRef.hpp>
#include "utils/TransparentStringHash.hpp"

namespace nexo::editor {

    class AssetManagerWindow final : public ADocumentWindow {
        public:
            using ADocumentWindow::ADocumentWindow;

            void setup() override;
            void shutdown() override;
            void show() override;
            void update() override;

        private:
            struct LayoutSettings {
                struct LayoutSizes {
                    float iconSize = 64.0f;
                    int iconSpacing = 8;
                    ImVec2 itemSize;
                    ImVec2 itemStep;
                    int columnCount;
                    float thumbnailHeightRatio = 0.8f;
                    float titlePadding = 5.0f;
                    float overlaySize = 6.0f;
                    float overlayPadding = 5.0f;
                    float cornerRadius = 5.0f;
                    float selectedBoxThickness = 4.0f;
                } size;

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
                } color;
            };

            std::set<int> m_selectedAssets;
            std::unordered_map<std::string, std::vector<std::string>, TransparentStringHash, std::equal_to<>> m_folderChildren;
            LayoutSettings m_layout;

            void calculateLayout(float availWidth);
            void drawMenuBar();
            void drawAssetsGrid();
            void drawAsset(const assets::GenericAssetRef& asset, int index, const ImVec2& itemPos, const ImVec2& itemSize);
            void handleSelection(int index, bool isSelected);

            assets::AssetType m_selectedType = assets::AssetType::UNKNOWN;
            std::string m_currentFolder;
            std::vector<std::pair<std::string, std::string>> m_folderStructure;  // Pairs of (path, name)
            char m_searchBuffer[256] = "";

            void buildFolderStructure();
            void drawFolderTree();
            void drawFolderTreeItem(const std::string& name, const std::string& path);

            void drawTextureThumbnail(
                ImDrawList *drawList,
                ImTextureID textureId,
                const ImVec2 &itemPos,
                const ImVec2& thumbnailEnd
            ) const ;

            struct FolderCreationState {
                bool isCreatingFolder = false;
                char folderName[256] = "";
                std::string parentPath;
                bool showError = false;
                std::string errorMessage;
                float errorTimer = 3.0f;
            };

            FolderCreationState m_folderCreationState;
            assets::AssetRef<assets::Texture> m_folderIcon;

            ImTextureID getFolderIconTexture();

            void handleNewFolderCreation();
            void drawFolder(
                const std::string& folderPath,
                const std::string& folderName,
                const ImVec2& itemPos,
                const ImVec2& itemSize
            );

            // File drop handling
            std::vector<std::string> m_pendingDroppedFiles;
            bool m_showDropIndicator = false;

            void handleDroppedFiles();
            void importDroppedFile(const std::string& filePath);
    };

    /**
     * @brief Payload structure for drag and drop operations from asset manager.
     *
     * Contains information about the asset being dragged.
     */
    struct AssetDragDropPayload
    {
        assets::AssetType type; ///< Type of the asset
        char path[256]; ///< Path to the asset
        char name[128]; ///< Display name of the asset
    };
}
