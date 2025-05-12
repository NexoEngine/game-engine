//// NodeHandling.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the node handling in the scene tree window
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "components/Uuid.hpp"

namespace nexo::editor {

    // Node creation methods
    SceneObject SceneTreeWindow::newSceneNode(const std::string &sceneName, const scene::SceneId sceneId, const WindowId uiId) const
    {
        SceneObject sceneNode;
        const std::string uiName = ObjectTypeToIcon.at(SelectionType::SCENE) + sceneName;
        sceneNode.data.sceneProperties = SceneProperties{sceneId, uiId};
        sceneNode.data.entity = sceneId;
        sceneNode.type = SelectionType::SCENE;
        auto &app = Application::getInstance();
        auto &selector = Selector::get();
        sceneNode.uuid = app.getSceneManager().getScene(sceneId).getUuid();
        sceneNode.uiName = selector.getUiHandle(sceneNode.uuid, uiName);
        return sceneNode;
    }

    void SceneTreeWindow::newLightNode(SceneObject &lightNode, const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity lightEntity, const std::string &uiName) const
    {
        const SceneProperties sceneProperties{sceneId, uiId};
        lightNode.data.sceneProperties = sceneProperties;
        lightNode.data.entity = lightEntity;
        auto &selector = Selector::get();
        const auto entityUuid = Application::m_coordinator->tryGetComponent<components::UuidComponent>(lightEntity);
        if (entityUuid)
        {
            lightNode.uuid = entityUuid->get().uuid;
            lightNode.uiName = selector.getUiHandle(entityUuid->get().uuid, uiName);
        } else
            lightNode.uiName = uiName;
    }

    SceneObject SceneTreeWindow::newAmbientLightNode(const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity lightEntity) const
    {
        SceneObject lightNode;
        lightNode.type = SelectionType::AMBIENT_LIGHT;
        const std::string uiName = std::format("{}Ambient light ", ObjectTypeToIcon.at(lightNode.type));
        newLightNode(lightNode, sceneId, uiId, lightEntity, uiName);
        return lightNode;
    }

    SceneObject SceneTreeWindow::newDirectionalLightNode(const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity lightEntity)
    {
        SceneObject lightNode;
        lightNode.type = SelectionType::DIR_LIGHT;
        const std::string uiName = std::format("{}Directional light {}", ObjectTypeToIcon.at(lightNode.type), ++m_nbDirLights);
        newLightNode(lightNode, sceneId, uiId, lightEntity, uiName);
        return lightNode;
    }

    SceneObject SceneTreeWindow::newSpotLightNode(const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity lightEntity)
    {
        SceneObject lightNode;
        lightNode.type = SelectionType::SPOT_LIGHT;
        const std::string uiName = std::format("{}Spot light {}", ObjectTypeToIcon.at(lightNode.type), ++m_nbSpotLights);
        newLightNode(lightNode, sceneId, uiId, lightEntity, uiName);
        return lightNode;
    }

    SceneObject SceneTreeWindow::newPointLightNode(const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity lightEntity)
    {
        SceneObject lightNode;
        lightNode.type = SelectionType::POINT_LIGHT;
        const std::string uiName = std::format("{}Point light {}", ObjectTypeToIcon.at(lightNode.type), ++m_nbPointLights);
        newLightNode(lightNode, sceneId, uiId, lightEntity, uiName);
        return lightNode;
    }

    SceneObject SceneTreeWindow::newCameraNode(const scene::SceneId sceneId, const WindowId uiId,
                                               const ecs::Entity cameraEntity) const
    {
        SceneObject cameraNode;
        const std::string uiName = ObjectTypeToIcon.at(SelectionType::CAMERA) + std::string("Camera");
        cameraNode.type = SelectionType::CAMERA;
        const SceneProperties sceneProperties{sceneId, uiId};
        cameraNode.data.sceneProperties = sceneProperties;
        cameraNode.data.entity = cameraEntity;
        auto &selector = Selector::get();
        const auto entityUuid = nexo::Application::m_coordinator->tryGetComponent<components::UuidComponent>(cameraEntity);
        if (entityUuid)
        {
            cameraNode.uuid = entityUuid->get().uuid;
            cameraNode.uiName = selector.getUiHandle(entityUuid->get().uuid, uiName);
        } else
            cameraNode.uiName = uiName;
        return cameraNode;
    }

    SceneObject SceneTreeWindow::newEntityNode(const scene::SceneId sceneId, const WindowId uiId,
                                               const ecs::Entity entity) const
    {
        auto &selector = Selector::get();
        SceneObject entityNode;
        const std::string uiName = std::format("{}{}", ObjectTypeToIcon.at(SelectionType::ENTITY), entity);
        entityNode.type = SelectionType::ENTITY;
        entityNode.data.sceneProperties = SceneProperties{sceneId, uiId};
        entityNode.data.entity = entity;
        const auto entityUuid = nexo::Application::m_coordinator->tryGetComponent<components::UuidComponent>(entity);
        if (entityUuid)
        {
            entityNode.uuid = entityUuid->get().uuid;
            entityNode.uiName = selector.getUiHandle(entityUuid->get().uuid, uiName);
        }
        else
            entityNode.uiName = uiName;
        return entityNode;
    }
}
