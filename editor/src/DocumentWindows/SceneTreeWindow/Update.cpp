//// Update.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the update of the scene tree window
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "components/Render3D.hpp"
#include "components/StaticMesh.hpp"
#include "components/Name.hpp"
#include "components/Uuid.hpp"

namespace nexo::editor {

    void SceneTreeWindow::generateHierarchicalNodes(std::map<scene::SceneId, SceneObject> &scenes)
    {
        // First, find all root entities (those with RootComponent or standalone entities without a parent)
        const std::vector<ecs::Entity> rootEntities = Application::m_coordinator->getAllEntitiesWith<
            components::RootComponent,
            components::TransformComponent,
            components::SceneTag>();

        // Set to track entities that have been processed
        std::unordered_set<ecs::Entity> processedEntities;

        // Process each root entity
        for (const ecs::Entity rootEntity : rootEntities) {
            const auto& sceneTag = Application::m_coordinator->getComponent<components::SceneTag>(rootEntity);
            if (auto it = scenes.find(sceneTag.id); it != scenes.end()) {
                SceneObject rootNode = createEntityNode(it->second.data.sceneProperties.sceneId,
                                                      it->second.data.sceneProperties.windowId,
                                                      rootEntity);
                processedEntities.insert(rootEntity);
                buildChildNodesForEntity(rootEntity, rootNode, processedEntities);
                it->second.children.push_back(rootNode);
            }
        }

        // Find standalone entities (those with no parent but without RootComponent)
        const std::vector<ecs::Entity> standaloneEntities = Application::m_coordinator->getAllEntitiesWith<
            components::StaticMeshComponent,
            components::TransformComponent,
            components::SceneTag,
            ecs::Exclude<components::ParentComponent>,
            ecs::Exclude<components::RootComponent>>();

        for (const ecs::Entity entity : standaloneEntities) {
            if (processedEntities.find(entity) != processedEntities.end())
                continue; // Skip if already processed

            const auto& sceneTag = Application::m_coordinator->getComponent<components::SceneTag>(entity);
            if (auto it = scenes.find(sceneTag.id); it != scenes.end()) {
                SceneObject entityNode = createEntityNode(it->second.data.sceneProperties.sceneId,
                                                       it->second.data.sceneProperties.windowId,
                                                       entity);
                processedEntities.insert(entity);
                buildChildNodesForEntity(entity, entityNode, processedEntities);
                it->second.children.push_back(entityNode);
            }
        }
    }

    void SceneTreeWindow::buildChildNodesForEntity(
        ecs::Entity parentEntity,
        SceneObject& parentNode,
        std::unordered_set<ecs::Entity>& processedEntities)
    {
        const auto& transform = Application::m_coordinator->getComponent<components::TransformComponent>(parentEntity);

        for (const auto childEntity : transform.children) {
            if (childEntity == ecs::INVALID_ENTITY)
                continue;
            // Skip if already processed
            if (processedEntities.find(childEntity) != processedEntities.end())
                continue;

            SceneObject childNode = createEntityNode(
                parentNode.data.sceneProperties.sceneId,
                parentNode.data.sceneProperties.windowId,
                childEntity);

            processedEntities.insert(childEntity);
            buildChildNodesForEntity(childEntity, childNode, processedEntities);
            parentNode.children.push_back(childNode);
        }
    }

    SceneObject SceneTreeWindow::createEntityNode(const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity)
    {
        SceneProperties scene{sceneId, uiId};
        EntityProperties data{scene, entity};

        std::string name;

        // If it is a mesh node/mesh, get the name component
        if (Application::m_coordinator->entityHasComponent<components::NameComponent>(entity)) {
            const auto& nameComp = Application::m_coordinator->getComponent<components::NameComponent>(entity);
            if (!nameComp.name.empty()) {
                name = nameComp.name;
            }
        }

        // If it is a root component, get the name of the model
        if (name.empty() && Application::m_coordinator->entityHasComponent<components::RootComponent>(entity)) {
            const auto& rootComp = Application::m_coordinator->getComponent<components::RootComponent>(entity);
            if (!rootComp.name.empty()) {
                name = rootComp.name;
            }
        }

        // Fall back to entity ID if no name found
        if (name.empty()) {
            name = "Entity " + std::to_string(entity);
        }

        std::string uuid;
        if (Application::m_coordinator->entityHasComponent<components::UuidComponent>(entity)) {
            const auto& uuidComponent = Application::m_coordinator->getComponent<components::UuidComponent>(entity);
            uuid = uuidComponent.uuid;
        }

        // Create UI name with appropriate icon
        std::string uiName;
        if (Application::m_coordinator->entityHasComponent<components::RootComponent>(entity)) {
            uiName = ICON_FA_OBJECT_GROUP "  " + name; // Use a different icon for root/model entities
        } else if (Application::m_coordinator->entityHasComponent<components::StaticMeshComponent>(entity)) {
            uiName = ICON_FA_CUBE "  " + name;
        } else {
            uiName = ICON_FA_DOT_CIRCLE_O "  " + name; // Generic icon for other entities
        }

        SceneObject node(uiName, {}, SelectionType::ENTITY, data);
        node.uuid = uuid;

        return node;
    }


    void SceneTreeWindow::update()
    {
        root_.uiName = "Scene Tree";
        root_.data.entity = -1;
        root_.type = SelectionType::NONE;
        root_.children.clear();
        m_nbPointLights = 0;
        m_nbDirLights = 0;
        m_nbSpotLights = 0;

        if (m_resetExpandState) {
            m_forceExpandAll = false;
            m_forceCollapseAll = false;
            m_resetExpandState = false;
        }

        // Retrieves the scenes that are displayed on the GUI
        const auto &scenes = m_windowRegistry.getWindows<EditorScene>();
        std::map<scene::SceneId, SceneObject> sceneNodes;
        for (const auto &scene : scenes)
        {
            sceneNodes[scene->getSceneId()] = newSceneNode(scene->getWindowName(), scene->getSceneId(), windowId);
        }

        generateNodes<components::AmbientLightComponent, components::SceneTag>(
            sceneNodes,
            [](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
                return newAmbientLightNode(sceneId, uiId, entity);
            });
        generateNodes<components::DirectionalLightComponent, components::SceneTag>(
            sceneNodes,
            [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
                return newDirectionalLightNode(sceneId, uiId, entity);
            });
        generateNodes<components::PointLightComponent, components::SceneTag>(
            sceneNodes,
            [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
                return newPointLightNode(sceneId, uiId, entity);
            });
        generateNodes<components::SpotLightComponent, components::SceneTag>(
            sceneNodes,
            [this](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
                return newSpotLightNode(sceneId, uiId, entity);
            });

        generateNodes<components::CameraComponent, components::SceneTag, ecs::Exclude<components::EditorCameraTag>>(
            sceneNodes,
            [](const scene::SceneId sceneId, const WindowId uiId, const ecs::Entity entity) {
                return newCameraNode(sceneId, uiId, entity);
            });

        // Generate hierarchical entity nodes
        generateHierarchicalNodes(sceneNodes);

        for (const auto &sceneNode: sceneNodes | std::views::values)
        {
            root_.children.push_back(sceneNode);
        }
    }
}
