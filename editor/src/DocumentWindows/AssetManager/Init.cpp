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
        auto& catalog = assets::AssetCatalog::getInstance();
        auto asset = std::make_unique<assets::Model>();
        assets::AssetLocation location{"my_package::My_Model@Random"};
        catalog.registerAsset(location, std::move(asset));

        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/9mn/scene.gltf");
            assets::ImporterFileInput fileInput{path};
            auto assetRef9mn = importer.importAsset<assets::Model>(assets::AssetLocation("my_package::9mn@DefaultScene"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/logo_nexo.png");
            assets::ImporterFileInput fileInput{path};
            auto textureRef = importer.importAsset<assets::Texture>(assets::AssetLocation("nexo_logo@Random"), fileInput);
        }

        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/icon_folder.png");
            assets::ImporterFileInput fileInput{path};
            m_folderIcon = importer.importAsset<assets::Texture>(assets::AssetLocation("icon_folder@_internal"), fileInput);
        }
        // Register for file drop events
        Application::getInstance().getEventManager()->registerListener<event::EventFileDrop>(this);
    }
}
