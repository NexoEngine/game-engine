//// AssetManagerWindow.hpp ///////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
#include <core/event/WindowEvent.hpp>
#include "assets/Asset.hpp"

namespace nexo::editor {

    class AssetManagerWindow final : public ADocumentWindow, LISTENS_TO(event::EventFileDrop) {
        public:
            using ADocumentWindow::ADocumentWindow;

            void setup() override;
            void shutdown() override;
            void show() override;
            void update() override;

            void handleEvent(event::EventFileDrop& event) override;

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

            std::set<unsigned int> m_selectedAssets;
            std::unordered_map<std::string, std::vector<std::string>, TransparentStringHash, std::equal_to<>> m_folderChildren;
            LayoutSettings m_layout;

            void calculateLayout(float availWidth);
            void drawMenuBar();
            void drawAssetsGrid();
            void drawAsset(const assets::GenericAssetRef& asset, unsigned int index, const ImVec2& itemPos, const ImVec2& itemSize);
            void handleSelection(unsigned int index, bool isSelected);

            assets::AssetType m_selectedType = assets::AssetType::UNKNOWN;
            std::string m_currentFolder;  // Currently selected folder
            std::string m_hoveredFolder;  // Currently hovered folder
            std::vector<std::pair<std::string, std::string>> m_folderStructure;  // Pairs of (path, name)
            char m_searchBuffer[256] = "";

            void buildFolderStructure();
            void drawFolderTree();
            void drawFolderTreeItem(const std::string& name, const std::string& path);

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

            ImTextureID getFolderIconTexture() const;

            void handleNewFolderCreation();
            void drawFolder(
                const std::string& folderPath,
                const std::string& folderName,
                const ImVec2& itemPos,
                const ImVec2& itemSize
            );

            std::vector<std::string> m_pendingDroppedFiles;
            bool m_showDropIndicator = false;

            void handleDroppedFiles();
            assets::AssetLocation getAssetLocation(const std::filesystem::path &path) const;
            void importDroppedFile(const std::string& filePath) const;
    };

    /**
     * @brief Payload structure for drag and drop operations from asset manager.
     *
     * Contains information about the asset being dragged.
     */
    struct AssetDragDropPayload
    {
        assets::AssetType type; ///< Type of the asset
        assets::AssetID id; ///< ID of the asset
        std::string path; ///< Path to the asset
        std::string name; ///< Display name of the asset
    };
}
