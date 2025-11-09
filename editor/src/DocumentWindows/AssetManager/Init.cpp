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
#include "Path.hpp"
#include "assets/AssetCatalog.hpp"
#include "assets/AssetImporter.hpp"
#include "assets/Assets/Model/ModelImporter.hpp"
#include "assets/Assets/Texture/TextureImporter.hpp"

namespace nexo::editor {
    void AssetManagerWindow::setup()
    {
        // Internal resources
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/icon_folder.png");
            assets::ImporterFileInput fileInput{path};
            m_folderIcon =
                importer.importAsset<assets::Texture>(assets::AssetLocation("icon_folder@_internal"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/logo_nexo.png");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("logo_nexo@Random"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/logo_nexo_bg.png");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("logo_nexo_bg@Random"), fileInput);
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
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/corvette/scene.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::corvette@Models"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/diablo/scene.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::diablo@Models"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/study_room/scene.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::study_room@Models"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path =
                Path::resolvePathRelativeToExe("../resources/models/SmilingFace/SmilingFace.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::SmilingFace@Models"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Sword/scene.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::Sword@Models"), fileInput);
        }
        // Demo Models
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/bretzel/bretzel.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::bretzel@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/chair/chair.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::chair@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/cork_board/cork_board.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::cork_board@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/domino/domino_vertical.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::dominoV@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/domino/domino_horizontal.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::dominoH@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/desk/desk.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::desk@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/duck/duck.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::duck@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/fan/fan.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::fan@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/full_room/full_room.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::full_room@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/laptop/laptop.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::laptop@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/polaroid/polaroid.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::polaroid@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/shelf/shelf.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::shelf@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/bin.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::bin@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/coffee.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::coffee@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/desk_frame.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::desk_frame@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/desk_light.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::desk_light@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/earth.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::earth@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/epitech.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::epitech@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/frame.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::frame@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/garland.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::garland@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/nexo/nexo_gradient.gltf");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::NEXO@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/plane.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::plane@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/plant.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::plant@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/rubix_cube.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::rubix_cube@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/small_plant.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::small_plant@Demo"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/models/Demo/stand_light.glb");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Model>(assets::AssetLocation("my_package::stand_light@Demo"), fileInput);
        }
        // // Textures
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
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/Checkerboard.png");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::Checkerboard@Textures"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/container2.png");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::container2@Textures"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/campus.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::campus@Textures/Picture"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/cathedral.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::cathedral@Textures/Picture"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/Thomas_Guigs.png");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::Thomas_Guigs@Textures/Picture"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/guillaume.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::guillaume@Textures/Picture"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/jean.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::jean@Textures/Picture"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/mehdy.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::mehdy@Textures/Picture"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/fdlm.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::fdlm@Textures/Picture"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/guizmo.png");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::guizmo@Textures/Picture"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/leo.png");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::leo@Textures/Picture"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/kaben.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::kaben@Textures/Picture"), fileInput);
        }
        {
            assets::AssetImporter importer;
            std::filesystem::path path = Path::resolvePathRelativeToExe("../resources/textures/baz.jpg");
            assets::ImporterFileInput fileInput{path};
            importer.importAsset<assets::Texture>(assets::AssetLocation("my_package::baz@Textures/Picture"), fileInput);
        }
        // Register for file drop events
        Application::getInstance().getEventManager()->registerListener<event::EventFileDrop>(this);

        m_layout.color.thumbnailBg                = ImGui::GetColorU32(ImGuiCol_Button);
        m_layout.color.thumbnailBgHovered         = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
        m_layout.color.thumbnailBgSelected        = ImGui::GetColorU32(ImGuiCol_Header);
        m_layout.color.thumbnailBgSelectedHovered = ImGui::GetColorU32(ImGuiCol_HeaderHovered);

        m_layout.color.selectedBoxColor = ImGui::GetColorU32(ImGuiCol_TabSelectedOverline);

        m_layout.color.titleBg                = ImGui::GetColorU32(ImGuiCol_Header);
        m_layout.color.titleBgHovered         = ImGui::GetColorU32(ImGuiCol_HeaderHovered);
        m_layout.color.titleBgSelected        = ImGui::GetColorU32(ImGuiCol_Header);
        m_layout.color.titleBgSelectedHovered = ImGui::GetColorU32(ImGuiCol_HeaderHovered);

        m_layout.color.titleText = ImGui::GetColorU32(ImGuiCol_Text);
        buildFolderStructure();
    }
} // namespace nexo::editor
