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

#include <components/Name.hpp>

#include "Nexo.hpp"
#include "Path.hpp"
#include "Renderer3D.hpp"
#include "assets/AssetCatalog.hpp"
#include "components/BillboardMesh.hpp"
#include "components/Editor.hpp"
#include "components/Render3D.hpp"

namespace nexo::editor::utils {

    /**
     * @brief Adds camera props to the specified entity.
     *
     * This function attaches a billboard mesh and a material component to the given entity,
     * representing a camera icon in the editor. The billboard uses a predefined camera icon texture
     * and is set up to always face the camera in the 3D scene.
     *
     * @param entity The ECS entity to which the camera props will be added.
     */
    static void addCameraProps(const ecs::Entity entity)
    {
        auto& catalog = assets::AssetCatalog::getInstance();

        static const assets::AssetRef<assets::Texture> cameraIconTexture = catalog.createAsset<assets::Texture>(
            assets::AssetLocation("_internal::cameraIcon@_internal"),
            Path::resolvePathRelativeToExe("../resources/textures/cameraIcon.png"));

        static const assets::AssetRef<assets::Material> materialRef = [&catalog]() {
            auto billboardMat           = std::make_unique<components::Material>();
            billboardMat->isOpaque      = false;
            billboardMat->albedoTexture = cameraIconTexture;
            billboardMat->shader        = "Albedo unshaded transparent";
            return catalog.createAsset<assets::Material>(assets::AssetLocation("_internal::CameraPropMat@_internal"),
                                                         std::move(billboardMat));
        }();

        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        components::BillboardComponent billboardMesh;
        billboardMesh.vao = renderer::NxRenderer3D::getBillboardVAO();

        Application::m_coordinator->addComponent(entity, billboardMesh);
        Application::m_coordinator->addComponent(entity, matComponent);
        components::EditorTag tag;
        Application::m_coordinator->addComponent(entity, tag);
    }

    /**
     * @brief Adds point light props to the specified entity.
     *
     * This function attaches a billboard mesh and a material component to the given entity,
     * representing a point light icon in the editor. The billboard uses a predefined point light icon texture
     * and is set up to always face the camera in the 3D scene.
     *
     * @param entity The ECS entity to which the point light props will be added.
     */
    static void addPointLightProps(const ecs::Entity entity)
    {
        auto& catalog = assets::AssetCatalog::getInstance();

        static const assets::AssetRef<assets::Texture> pointLightIconTexture = catalog.createAsset<assets::Texture>(
            assets::AssetLocation("_internal::pointLightIcon@_internal"),
            Path::resolvePathRelativeToExe("../resources/textures/pointLightIcon.png"));

        static const assets::AssetRef<assets::Material> materialRef = [&catalog]() {
            auto billboardMat           = std::make_unique<components::Material>();
            billboardMat->isOpaque      = false;
            billboardMat->albedoTexture = pointLightIconTexture;
            billboardMat->shader        = "Albedo unshaded transparent";
            return catalog.createAsset<assets::Material>(
                assets::AssetLocation("_internal::PointLightPropMat@_internal"), std::move(billboardMat));
        }();

        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        components::BillboardComponent billboardMesh;
        billboardMesh.vao = renderer::NxRenderer3D::getBillboardVAO();

        Application::m_coordinator->addComponent(entity, billboardMesh);
        Application::m_coordinator->addComponent(entity, matComponent);
        components::EditorTag tag;
        Application::m_coordinator->addComponent(entity, tag);
    }

    /**
     * @brief Adds spotlight props to the specified entity.
     *
     * This function attaches a billboard mesh and a material component to the given entity,
     * representing a spotlight icon in the editor. The billboard uses a predefined spotlight icon texture
     * and is set up to always face the camera in the 3D scene.
     *
     * @param entity The ECS entity to which the spotlight props will be added.
     */
    static void addSpotLightProps(const ecs::Entity entity)
    {
        auto& catalog = assets::AssetCatalog::getInstance();

        static const assets::AssetRef<assets::Texture> spotLightIconTexture = catalog.createAsset<assets::Texture>(
            assets::AssetLocation("_internal::spotLightIcon@_internal"),
            Path::resolvePathRelativeToExe("../resources/textures/spotLightIcon.png"));

        static const assets::AssetRef<assets::Material> materialRef = [&catalog]() {
            auto billboardMat           = std::make_unique<components::Material>();
            billboardMat->isOpaque      = false;
            billboardMat->albedoTexture = spotLightIconTexture;
            billboardMat->shader        = "Albedo unshaded transparent";
            return catalog.createAsset<assets::Material>(assets::AssetLocation("_internal::SpotLightPropMat@_internal"),
                                                         std::move(billboardMat));
        }();

        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        components::BillboardComponent billboardMesh;
        billboardMesh.vao = renderer::NxRenderer3D::getBillboardVAO();

        Application::m_coordinator->addComponent(entity, billboardMesh);
        Application::m_coordinator->addComponent(entity, matComponent);
        components::EditorTag tag;
        Application::m_coordinator->addComponent(entity, tag);
    }

    void addPropsTo(const ecs::Entity entity, const PropsType type)
    {
        switch (type) {
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

    ecs::Entity FindEntityByName(const char* name)
    {
        if (!name) {
            LOG(NEXO_ERROR, "NxFindEntityByName: name is null");
            return static_cast<ecs::Entity>(-1);
        }

        auto& app            = Application::getInstance();
        auto& coordinator    = *Application::m_coordinator;
        const auto& scene    = app.getSceneManager().getScene(0);
        const auto& entities = scene.getEntities();

        for (const auto entity : entities) {
            if (coordinator.entityHasComponent<components::NameComponent>(entity)) {
                const auto& nameComponent = coordinator.getComponent<components::NameComponent>(entity);
                if (nameComponent.name == name) {
                    return entity;
                }
            }
        }

        LOG(NEXO_WARN, "FindEntityByName: Entity with name '{}' not found", name);
        return static_cast<ecs::Entity>(-1);
    }

} // namespace nexo::editor::utils
