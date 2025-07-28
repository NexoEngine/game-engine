//// DragDrop.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date:        2025-06-30
//  Description: Implementation of drag and drop functionality for the scene tree
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "DocumentWindows/AssetManager/AssetManagerWindow.hpp"
#include "components/Uuid.hpp"
#include "context/ActionManager.hpp"
#include "context/Selector.hpp"
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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

namespace nexo::editor {

    void SceneTreeWindow::handleDragSource(const SceneObject& object)
    {
        if (object.type == SelectionType::SCENE || object.type == SelectionType::NONE)
            return;

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            const SceneTreeDragDropPayload payload{
                object.data.entity,
                object.data.sceneProperties.sceneId,
                object.type,
                object.uuid,
                object.uiName
            };

            ImGui::SetDragDropPayload("SCENE_TREE_NODE", &payload, sizeof(payload));
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
                    handleDropFromSceneTree(object, payload);
            }

            // Handle drops from asset manager
            if (const ImGuiPayload* assetPayload = ImGui::AcceptDragDropPayload("ASSET_DRAG"))
            {
                IM_ASSERT(assetPayload->DataSize == sizeof(AssetDragDropPayload));
                const auto& payload = *static_cast<const AssetDragDropPayload*>(assetPayload->Data);

                handleDropFromAssetManager(object, payload);
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

        return true;
    }

    void SceneTreeWindow::handleDropFromSceneTree(const SceneObject& dropTarget, const SceneTreeDragDropPayload& payload)
    {
        auto& app = Application::getInstance();
        auto& sceneManager = app.getSceneManager();
        auto& coordinator = *Application::m_coordinator;

        auto& sourceScene = sceneManager.getScene(payload.sourceSceneId);

        if (dropTarget.type == SelectionType::SCENE)
        {
            // Dropping onto a scene - move entity to that scene
            if (payload.sourceSceneId != dropTarget.data.sceneProperties.sceneId)
            {
                sourceScene.removeEntity(payload.entity);

                auto& targetScene = sceneManager.getScene(dropTarget.data.sceneProperties.sceneId);
                targetScene.addEntity(payload.entity);

                // Remove parent relationship if moving to different scene
                auto parentComp = coordinator.tryGetComponent<components::ParentComponent>(payload.entity);
                if (parentComp.has_value())
                {
                    auto parentTransform = coordinator.tryGetComponent<components::TransformComponent>(parentComp->get().parent);
                    if (parentTransform.has_value())
                        parentTransform->get().removeChild(payload.entity);

                    coordinator.removeComponent<components::ParentComponent>(payload.entity);
                }

                // Record action for undo/redo
                // TODO: Create a specific action for moving entities between scenes
                // For now, we just perform the operation without undo support for scene moves
            }
        }
        else if (dropTarget.type == SelectionType::ENTITY)
        {
            // Dropping onto an entity - create parent-child relationship
            ecs::Entity parentEntity = dropTarget.data.entity;
            ecs::Entity childEntity  = payload.entity;

            auto childTransformOpt = coordinator.tryGetComponent<components::TransformComponent>(childEntity);
            if (!childTransformOpt.has_value())
                return;
            auto &childTransform = childTransformOpt->get();
            glm::mat4 childWorldMat = childTransform.worldMatrix;

            auto parentTransformOpt = coordinator.tryGetComponent<components::TransformComponent>(parentEntity);
            if (!parentTransformOpt.has_value())
                return;
            auto& parentTransform = parentTransformOpt->get();
            glm::mat4 parentWorldMat = parentTransform.worldMatrix;

            // Compute the new localMatrix so that parentWorldMat * local = old world
            glm::mat4 invParent = glm::inverse(parentWorldMat);
            glm::mat4 newLocalMat = invParent * childWorldMat;

            glm::vec3 skew, scale, translation;
            glm::quat rotation;
            glm::vec4 perspective;
            glm::decompose(
                newLocalMat,
                scale,
                rotation,
                translation,
                skew,
                perspective
            );

            childTransform.pos  = translation;
            childTransform.quat = rotation;
            childTransform.size = scale;

            ecs::Entity oldParent = ecs::INVALID_ENTITY;
            auto oldParentComp = coordinator.tryGetComponent<components::ParentComponent>(childEntity);
            if (oldParentComp.has_value())
            {
                oldParent = oldParentComp->get().parent;

                if (auto oldPT = coordinator.tryGetComponent<components::TransformComponent>(oldParent)) {
                    oldPT->get().removeChild(childEntity);
                    if (oldPT->get().children.empty() && coordinator.entityHasComponent<components::RootComponent>(oldParent))
                        coordinator.removeComponent<components::RootComponent>(oldParent);
                }
            }

            if (!oldParentComp.has_value())
                coordinator.addComponent(childEntity, components::ParentComponent{parentEntity});
            else
                oldParentComp->get().parent = parentEntity;

            if (!coordinator.entityHasComponent<components::TransformComponent>(parentEntity))
                coordinator.addComponent(parentEntity, components::TransformComponent{});
            auto& pt = coordinator.getComponent<components::TransformComponent>(parentEntity);
            pt.addChild(childEntity);

            if (!coordinator.entityHasComponent<components::ParentComponent>(parentEntity) &&
                !coordinator.entityHasComponent<components::RootComponent>(parentEntity))
            {
                std::string name = dropTarget.uiName;
                const auto it = ObjectTypeToIcon.find(dropTarget.type);
                if (it != ObjectTypeToIcon.end())
                {
                    const std::string& icon = it->second;
                    if (name.rfind(icon, 0) == 0)
                        name.erase(0, icon.size());
                }

                coordinator.addComponent(
                    parentEntity,
                    components::RootComponent{ name, nullptr, 1 }
                );
            }

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

    void SceneTreeWindow::handleDropFromAssetManager(const SceneObject& dropTarget, const AssetDragDropPayload& payload)
    {
        if (dropTarget.type == SelectionType::SCENE)
        {
            auto& app = Application::getInstance();
            auto& sceneManager = app.getSceneManager();

            if (payload.type == assets::AssetType::MODEL)
            {
                const auto modelRef = assets::AssetCatalog::getInstance().getAsset(payload.id);
                if (!modelRef)
                    return;
                if (const auto model = modelRef.as<assets::Model>(); model)
                {
                    ecs::Entity newEntity = EntityFactory3D::createModel(
                        model,
                        {0.0f, 0.0f, 0.0f},
                        {1.0f, 1.0f, 1.0f},
                        {0.0f, 0.0f, 0.0f}
                    );
                    auto& scene = sceneManager.getScene(dropTarget.data.sceneProperties.sceneId);
                    scene.addEntity(newEntity);

                    // Record action for undo/redo TODO: Fix undo for models, it does not seem to work properly
                    auto action = std::make_unique<EntityCreationAction>(newEntity);
                    ActionManager::get().recordAction(std::move(action));
                }
            }
            else if (payload.type == assets::AssetType::TEXTURE)
            {
                const auto textureRef = assets::AssetCatalog::getInstance().getAsset(payload.id);
                if (!textureRef)
                    return;
                if (const auto texture = textureRef.as<assets::Texture>(); texture)
                {
                    components::Material material;
                    material.albedoTexture = texture;
                    material.albedoColor = glm::vec4(1.0f);

                    ecs::Entity newEntity = EntityFactory3D::createBillboard(
                        {0.0f, 0.0f, 0.0f},
                        {1.0f, 1.0f, 1.0f},
                        material
                    );

                    auto& scene = sceneManager.getScene(dropTarget.data.sceneProperties.sceneId);
                    scene.addEntity(newEntity);

                    auto action = std::make_unique<EntityCreationAction>(newEntity);
                    ActionManager::get().recordAction(std::move(action));
                }
            }
        }
        else if (dropTarget.type == SelectionType::ENTITY)
        {
            const auto matCompOpt = Application::m_coordinator->tryGetComponent<components::MaterialComponent>(dropTarget.data.entity);
            if (!matCompOpt)
                { ImGui::EndDragDropTarget(); return; }

            auto& matComp = matCompOpt->get();

            if (payload.type == assets::AssetType::TEXTURE)
            {
                const auto texRef = assets::AssetCatalog::getInstance().getAsset(payload.id);
                if (const auto tex = texRef.as<assets::Texture>(); tex)
                {
                    const auto mat = matComp.material.lock();
                    if (!mat)
                        return;
                    mat->getData()->albedoTexture = tex;
                }
            }
            else if (payload.type == assets::AssetType::MATERIAL)
            {
                auto const matRef = assets::AssetCatalog::getInstance().getAsset(payload.id);
                if (const auto m = matRef.as<assets::Material>(); m)
                {
                    auto oldMat = matComp.material;
                    matComp.material = m;
                }
            }
        }
    }

} // namespace nexo::editor
