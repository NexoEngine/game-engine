//// EditorProps.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        19/04/2025
//  Description: Source file for the editor props utils
//
///////////////////////////////////////////////////////////////////////////////

#include "EditorProps.hpp"
#include "renderer/Texture.hpp"
#include "components/Render3D.hpp"
#include "components/Shapes3D.hpp"
#include "Path.hpp"
#include "Nexo.hpp"
#include "assets/AssetCatalog.hpp"

namespace nexo::editor::utils {

    static void addCameraProps(const ecs::Entity entity)
    {
        auto& catalog = assets::AssetCatalog::getInstance();
        components::Material billboardMat{};
        billboardMat.isOpaque = false;
        static const assets::AssetRef<assets::Texture> cameraIconTexture = catalog.createAsset<assets::Texture>(
            assets::AssetLocation("_internal::cameraIcon@_internal"),
            Path::resolvePathRelativeToExe("../resources/textures/cameraIcon.png")
        );
        billboardMat.albedoTexture = cameraIconTexture;
        billboardMat.shader = "Albedo unshaded transparent";
        auto billboard = std::make_shared<components::BillBoard>();
        auto renderable = std::make_shared<components::Renderable3D>(billboardMat, billboard);
        const components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);
        Application::m_coordinator->addComponent(entity, renderComponent);
    }

    static void addPointLightProps(const ecs::Entity entity)
    {
        auto& catalog = assets::AssetCatalog::getInstance();
        components::Material billboardMat{};
        billboardMat.isOpaque = false;
        static const assets::AssetRef<assets::Texture> pointLightIconTexture = catalog.createAsset<assets::Texture>(
            assets::AssetLocation("_internal::pointLightIcon@_internal"),
            Path::resolvePathRelativeToExe("../resources/textures/pointLightIcon.png")
        );
        billboardMat.albedoTexture = pointLightIconTexture;
        billboardMat.shader = "Albedo unshaded transparent";
        auto billboard = std::make_shared<components::BillBoard>();
        auto renderable = std::make_shared<components::Renderable3D>(billboardMat, billboard);
        const components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);
        Application::m_coordinator->addComponent(entity, renderComponent);
    }

    static void addSpotLightProps(const ecs::Entity entity)
    {
        auto& catalog = assets::AssetCatalog::getInstance();
        components::Material billboardMat{};
        billboardMat.isOpaque = false;
        static const assets::AssetRef<assets::Texture> spotLightIconTexture = catalog.createAsset<assets::Texture>(
            assets::AssetLocation("_internal::spotLightIcon@_internal"),
            Path::resolvePathRelativeToExe("../resources/textures/spotLightIcon.png")
        );
        billboardMat.albedoTexture = spotLightIconTexture;
        billboardMat.shader = "Albedo unshaded transparent";
        auto billboard = std::make_shared<components::BillBoard>();
        auto renderable = std::make_shared<components::Renderable3D>(billboardMat, billboard);
        const components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);
        Application::m_coordinator->addComponent(entity, renderComponent);
    }

    void addPropsTo(const ecs::Entity entity, const PropsType type)
    {
        switch (type)
        {
            case PropsType::CAMERA:
                addCameraProps(entity);
                break;
            case PropsType::POINT_LIGHT:
                addPointLightProps(entity);
                break;
            case PropsType::SPOT_LIGHT:
                addSpotLightProps(entity);
                break;
            default:
                break;
        }
    }

}
