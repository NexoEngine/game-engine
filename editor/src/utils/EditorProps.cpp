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

    static void addCameraProps(ecs::Entity entity)
    {
        auto &app = getApp();
        nexo::components::Material billboardMat{};
        billboardMat.isOpaque = false;
        static const std::shared_ptr<nexo::renderer::NxTexture2D> cameraIconTexture =
            nexo::renderer::NxTexture2D::create(nexo::Path::resolvePathRelativeToExe("../resources/textures/cameraIcon.png").string());
        billboardMat.albedoTexture = cameraIconTexture;
        billboardMat.shader = "Albedo unshaded transparent";
        auto billboard = std::make_shared<nexo::components::BillBoard>();
        auto renderable = std::make_shared<nexo::components::Renderable3D>(billboardMat, billboard);
        nexo::components::RenderComponent renderComponent(renderable, nexo::components::RenderType::RENDER_3D);
        app.m_coordinator->addComponent(entity, renderComponent);
    }

    static void addPointLightProps(ecs::Entity entity)
    {
        auto &app = getApp();
        nexo::components::Material billboardMat{};
        billboardMat.isOpaque = false;
        static const std::shared_ptr<renderer::NxTexture2D> pointLightIconTexture =
            renderer::NxTexture2D::create(Path::resolvePathRelativeToExe("../resources/textures/pointLightIcon.png").string());
        billboardMat.albedoTexture = pointLightIconTexture;
        billboardMat.shader = "Albedo unshaded transparent";
        auto billboard = std::make_shared<nexo::components::BillBoard>();
        auto renderable = std::make_shared<nexo::components::Renderable3D>(billboardMat, billboard);
        nexo::components::RenderComponent renderComponent(renderable, nexo::components::RenderType::RENDER_3D);
        app.m_coordinator->addComponent(entity, renderComponent);
    }

    static void addSpotLightProps(ecs::Entity entity)
    {
        auto &app = getApp();
        nexo::components::Material billboardMat{};
        billboardMat.isOpaque = false;
        static const std::shared_ptr<renderer::NxTexture2D> spotLightIconTexture =
            renderer::NxTexture2D::create(Path::resolvePathRelativeToExe("../resources/textures/spotLightIcon.png").string());
        billboardMat.albedoTexture = spotLightIconTexture;
        billboardMat.shader = "Albedo unshaded transparent";
        auto billboard = std::make_shared<nexo::components::BillBoard>();
        auto renderable = std::make_shared<nexo::components::Renderable3D>(billboardMat, billboard);
        nexo::components::RenderComponent renderComponent(renderable, nexo::components::RenderType::RENDER_3D);
        app.m_coordinator->addComponent(entity, renderComponent);
    }

    void addPropsTo(ecs::Entity entity, PropsType type)
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
