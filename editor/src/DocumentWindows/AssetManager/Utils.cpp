//// Utils.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        27/05/2025
//  Description: Source file for the utils function of the asset manager
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"

namespace nexo::editor {

    template <typename T>
    void AssetManagerWindow::drawErrorMessageInPopup(T& actionState)
    {
        if (!actionState.showError) return;

        ImGui::Separator();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
        ImGui::Text("%s", actionState.errorMessage.c_str());
        ImGui::PopStyleColor();

        if (actionState.errorTimer <= 0.0f) {
            actionState.showError  = false;
            actionState.errorTimer = ERROR_DISPLAY_TIMEOUT;
        } else {
            actionState.errorTimer -= ImGui::GetIO().DeltaTime;
        }
    }

    // Explicit template instantiation for FolderActionState and AssetActionState
    template void AssetManagerWindow::drawErrorMessageInPopup(FolderActionState& actionState);
    template void AssetManagerWindow::drawErrorMessageInPopup(AssetActionState& actionState);

    /**
     * @brief Retrieves the texture ID for the given texture asset.
     *
     * This method checks if the texture asset is valid and returns its texture ID.
     * If the texture is not valid, it returns 0.
     *
     * @param texture The asset reference to the texture.
     * @return ImTextureID The texture ID or 0 if the texture is invalid.
     */
    ImTextureID AssetManagerWindow::getIconTexture(const assets::AssetRef<assets::Texture> &texture)
    {
        if (const auto texRef = texture.lock()) {
            const auto &texData = texRef->getData();
            if (texData && texData->texture) {
                return texData->texture->getId();
            }
        }
        return 0;
    }
} // namespace nexo::editor
