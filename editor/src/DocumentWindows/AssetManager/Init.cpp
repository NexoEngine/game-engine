//// Init.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the setup of the asset manager window
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManagerWindow.hpp"
#include "assets/AssetCatalog.hpp"
#include "assets/AssetImporter.hpp"
#include "assets/Assets/Model/ModelImporter.hpp"
#include "assets/Assets/Texture/TextureImporter.hpp"
#include "Path.hpp"

namespace nexo::editor {
    void AssetManagerWindow::setup()
    {
        // Internal resources
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/icon_folder.png");
            assets::ImporterFileInput fileInput{path};
            m_folderIcon = importer.importAsset<assets::Texture>(assets::AssetLocation("icon_folder@_internal"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/logoNexo.png");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("nexo_logo@Random"), fileInput);
        }
        // Models
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Avocado/Avocado.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::Avocado@Models"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/SmilingFace/SmilingFace.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::SmilingFace@Models"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Sword/scene.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::Sword@Models"), fileInput);
        }
        // Textures
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/grass.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::grass@Textures"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/rock.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::rock@Textures"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/wood.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::wood@Textures"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/dirt.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::dirt@Textures"), fileInput);
        }
        // Register for file drop events
        Application::getInstance().getEventManager()->registerListener<event::EventFileDrop>(this);

        m_layout.color.thumbnailBg = ImGui::GetColorU32(ImGuiCol_Button);
        m_layout.color.thumbnailBgHovered = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
        m_layout.color.thumbnailBgSelected = ImGui::GetColorU32(ImGuiCol_Header);
        m_layout.color.thumbnailBgSelectedHovered = ImGui::GetColorU32(ImGuiCol_HeaderHovered);

        m_layout.color.selectedBoxColor = ImGui::GetColorU32(ImGuiCol_TabSelectedOverline);

        m_layout.color.titleBg = ImGui::GetColorU32(ImGuiCol_Header);
        m_layout.color.titleBgHovered = ImGui::GetColorU32(ImGuiCol_HeaderHovered);
        m_layout.color.titleBgSelected = ImGui::GetColorU32(ImGuiCol_Header);
        m_layout.color.titleBgSelectedHovered = ImGui::GetColorU32(ImGuiCol_HeaderHovered);

        m_layout.color.titleText = ImGui::GetColorU32(ImGuiCol_Text);
        buildFolderStructure();
    }
}
