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
#include <vector>
#include <string>
#include <imgui.h>

namespace nexo::editor {

    class AssetManagerWindow final : public ADocumentWindow {
    public:
        AssetManagerWindow() = default;

        void setup() override;
        void shutdown() override;
        void show() override;
        void update() override;

    private:
        struct Asset {
            std::string name;
            int type; // 0: default, 1: red overlay, 2: green overlay
        };

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
			} size;

            struct LayoutColors {
                ImU32 thumbnailBg;
                ImU32 thumbnailBgHovered;
                ImU32 thumbnailBgSelected;
                ImU32 thumbnailBgSelectedHovered;

                ImU32 titleBg;
                ImU32 titleBgHovered;
                ImU32 titleBgSelected;
                ImU32 titleBgSelectedHovered;

                ImU32 titleText;
            } color;
        };

        std::vector<Asset> assets;
        std::vector<int> selectedAssets;
        LayoutSettings layout;

        void calculateLayout(float availWidth);
        void drawMenuBar();
        void drawAssetsGrid();
        void drawAsset(const Asset& asset, int index, const ImVec2& itemPos, const ImVec2& itemSize);
        void handleSelection(int index, bool isSelected);
        ImU32 getAssetTypeOverlayColor(int type) const;
    };

} // namespace nexo::editor