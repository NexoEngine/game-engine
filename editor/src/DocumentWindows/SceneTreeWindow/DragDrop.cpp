//// DragDrop.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date:        2025-06-30
//  Description: Implementation of drag and drop functionality for the scene tree
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "DocumentWindows/AssetManager/AssetManagerWindow.hpp"
#include "context/ActionManager.hpp"
#include "context/actions/EntityActions.hpp"
#include "components/Parent.hpp"
#include "components/Transform.hpp"
#include "components/Render3D.hpp"
#include "EntityFactory3D.hpp"
#include "assets/AssetCatalog.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "assets/Assets/Texture/Texture.hpp"
#include <imgui.h>
#include <algorithm>
#include <filesystem>

namespace nexo::editor {

    void SceneTreeWindow::handleDragSource(const SceneObject& object)
    {
        // Only allow dragging of entities, lights, and cameras
        if (object.type == SelectionType::SCENE || object.type == SelectionType::NONE)
            return;

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            // Create payload data
            SceneTreeDragDropPayload payload{
                object.data.entity,
                object.data.sceneProperties.sceneId,
                object.type,
                object.uuid,
                object.uiName
            };

            // Set the payload
            ImGui::SetDragDropPayload("SCENE_TREE_NODE", &payload, sizeof(payload));

            // Show preview text while dragging
            ImGui::Text("Moving: %s", object.uiName.c_str());

            ImGui::EndDragDropSource();
        }
    }

    void SceneTreeWindow::handleDropTarget(const SceneObject& object)
    {
        if (ImGui::BeginDragDropTarget())
        {
            // Handle drops from scene tree nodes
            if (const ImGuiPayload* imguiPayload = ImGui::AcceptDragDropPayload("SCENE_TREE_NODE"))
            {
                IM_ASSERT(imguiPayload->DataSize == sizeof(SceneTreeDragDropPayload));
                const auto& payload = *static_cast<const SceneTreeDragDropPayload*>(imguiPayload->Data);

                if (canAcceptDrop(object, payload))
                {
                    handleDrop(object, payload);
                }
            }

            // Handle drops from asset manager
            if (const ImGuiPayload* assetPayload = ImGui::AcceptDragDropPayload("ASSET_DRAG"))
            {
                IM_ASSERT(assetPayload->DataSize == sizeof(AssetDragDropPayload));
                const auto& payload = *static_cast<const AssetDragDropPayload*>(assetPayload->Data);

                // Handle different asset types
                if (object.type == SelectionType::SCENE)
                {
                    auto& app = Application::getInstance();
                    auto& sceneManager = app.getSceneManager();

                    if (payload.type == assets::AssetType::MODEL)
                    {
                        // Import the model
                        assets::AssetImporter importer;
                        std::filesystem::path path{payload.path};
                        assets::ImporterFileInput fileInput{path};
                        std::string assetLocationStr = std::string(payload.name) + "@DragDrop/";
                        auto modelRef = assets::AssetCatalog::getInstance().getAsset(payload.id);
                        if (!modelRef)
                            return;
                        if (auto model = modelRef.as<assets::Model>(); model)
                        {
                            // Create entity with the model
                            ecs::Entity newEntity = EntityFactory3D::createModel(
                                model,
                                {0.0f, 0.0f, 0.0f}, // position
                                {1.0f, 1.0f, 1.0f}, // scale
                                {0.0f, 0.0f, 0.0f}  // rotation
                            );

                            // Add to the scene
                            auto& scene = sceneManager.getScene(object.data.sceneProperties.sceneId);
                            scene.addEntity(newEntity);

                            // Record action for undo/redo TODO: Fix undo for models, it does not seem to work properly
                            auto action = std::make_unique<EntityCreationAction>(newEntity);
                            ActionManager::get().recordAction(std::move(action));
                        }
                    }
                    else if (payload.type == assets::AssetType::TEXTURE)
                    {
                        // Get the texture from the asset catalog
                        auto& catalog = assets::AssetCatalog::getInstance();
                        assets::AssetLocation location(payload.path);
                        auto textureRef = catalog.getAsset(location).as<assets::Texture>();

                        if (!textureRef)
                        {
                            // If not in catalog, try to import it
                            assets::AssetImporter importer;
                            std::filesystem::path path{payload.path};
                            assets::ImporterFileInput fileInput{path};
                            std::string assetLocationStr = std::string(payload.name) + "@DragDrop/";
                            textureRef = importer.importAsset<assets::Texture>(assets::AssetLocation(assetLocationStr), fileInput);
                        }

                        if (textureRef)
                        {
                            // Create material with the texture
                            components::Material material;
                            material.albedoTexture = textureRef;
                            material.albedoColor = glm::vec4(1.0f); // White to show texture colors

                            // Create billboard entity
                            ecs::Entity newEntity = EntityFactory3D::createBillboard(
                                {0.0f, 0.0f, 0.0f}, // position
                                {1.0f, 1.0f, 1.0f}, // size
                                material
                            );

                            // Add to the scene
                            auto& scene = sceneManager.getScene(object.data.sceneProperties.sceneId);
                            scene.addEntity(newEntity);

                            // Record action for undo/redo
                            auto action = std::make_unique<EntityCreationAction>(newEntity);
                            ActionManager::get().recordAction(std::move(action));
                        }
                    }
                }
            }

            ImGui::EndDragDropTarget();
        }
    }

    bool SceneTreeWindow::canAcceptDrop(const SceneObject& dropTarget, const SceneTreeDragDropPayload& payload)
    {
        // Can't drop on itself
        if (dropTarget.type != SelectionType::SCENE && dropTarget.data.entity == payload.entity)
            return false;

        // Can't drop a parent onto its child (prevent circular references)
        if (dropTarget.type != SelectionType::SCENE)
        {
            ecs::Entity currentEntity = dropTarget.data.entity;
            auto parentComp = Application::m_coordinator->tryGetComponent<components::ParentComponent>(currentEntity);
            while (parentComp.has_value())
            {
                if (parentComp->get().parent == payload.entity)
                    return false;
                currentEntity = parentComp->get().parent;
                parentComp = Application::m_coordinator->tryGetComponent<components::ParentComponent>(currentEntity);
            }
        }

        // Allow dropping entities onto scenes or other entities
        return true;
    }

    void SceneTreeWindow::handleDrop(const SceneObject& dropTarget, const SceneTreeDragDropPayload& payload)
    {
        auto& app = Application::getInstance();
        auto& sceneManager = app.getSceneManager();
        auto& coordinator = *Application::m_coordinator;

        // Get the source scene
        auto& sourceScene = sceneManager.getScene(payload.sourceSceneId);

        if (dropTarget.type == SelectionType::SCENE)
        {
            // Dropping onto a scene - move entity to that scene
            if (payload.sourceSceneId != dropTarget.data.sceneProperties.sceneId)
            {
                // Remove from source scene
                sourceScene.removeEntity(payload.entity);

                // Add to target scene
                auto& targetScene = sceneManager.getScene(dropTarget.data.sceneProperties.sceneId);
                targetScene.addEntity(payload.entity);

                // Update scene tag
                auto& sceneTag = coordinator.getComponent<components::SceneTag>(payload.entity);
                sceneTag.id = dropTarget.data.sceneProperties.sceneId;

                // Remove parent relationship if moving to different scene
                auto parentComp = coordinator.tryGetComponent<components::ParentComponent>(payload.entity);
                if (parentComp.has_value())
                {
                    // Update parent's children list
                    auto parentTransform = coordinator.tryGetComponent<components::TransformComponent>(parentComp->get().parent);
                    if (parentTransform.has_value())
                    {
                        parentTransform->get().removeChild(payload.entity);
                    }

                    coordinator.removeComponent<components::ParentComponent>(payload.entity);
                }

                // Record action for undo/redo
                // TODO: Create a specific action for moving entities between scenes
                // For now, we just perform the operation without undo support for scene moves
            }
        }
        else
        {
            // Dropping onto an entity - create parent-child relationship
            ecs::Entity parentEntity = dropTarget.data.entity;
            ecs::Entity childEntity = payload.entity;

            // Get old parent before modifications
            ecs::Entity oldParent = ecs::INVALID_ENTITY;
            auto oldParentComp = coordinator.tryGetComponent<components::ParentComponent>(childEntity);
            if (oldParentComp.has_value())
            {
                oldParent = oldParentComp->get().parent;

                // Update old parent's children list
                auto oldParentTransform = coordinator.tryGetComponent<components::TransformComponent>(oldParent);
                if (oldParentTransform.has_value())
                {
                    oldParentTransform->get().removeChild(childEntity);
                }
            }

            // Set new parent
            if (!oldParentComp.has_value())
            {
                coordinator.addComponent(childEntity, components::ParentComponent{parentEntity});
            }
            else
            {
                oldParentComp->get().parent = parentEntity;
            }

            // Update parent's children list
            auto parentTransform = coordinator.tryGetComponent<components::TransformComponent>(parentEntity);
            if (!parentTransform.has_value())
            {
                coordinator.addComponent(parentEntity, components::TransformComponent{});
                parentTransform = coordinator.tryGetComponent<components::TransformComponent>(parentEntity);
            }
            if (parentTransform.has_value())
            {
                parentTransform->get().addChild(childEntity);
            }

            // If moving to different scene, update scene tag
            if (payload.sourceSceneId != dropTarget.data.sceneProperties.sceneId)
            {
                sourceScene.removeEntity(childEntity);
                auto& targetScene = sceneManager.getScene(dropTarget.data.sceneProperties.sceneId);
                targetScene.addEntity(childEntity);

                auto& sceneTag = coordinator.getComponent<components::SceneTag>(childEntity);
                sceneTag.id = dropTarget.data.sceneProperties.sceneId;
            }
            auto action = std::make_unique<EntityParentChangeAction>(childEntity, oldParent, parentEntity);
            ActionManager::get().recordAction(std::move(action));
        }
    }

} // namespace nexo::editor
