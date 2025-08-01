//// DragDrop.cpp /////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        12/07/2025
//  Description: Source file for the drag and drop feature of the editor scene
//
///////////////////////////////////////////////////////////////////////////////

#include <imgui.h>
#include "Definitions.hpp"
#include "EditorScene.hpp"
#include "assets/Asset.hpp"
#include "assets/AssetCatalog.hpp"
#include "components/Editor.hpp"
#include "components/MaterialComponent.hpp"
#include "context/ActionManager.hpp"

namespace nexo::editor {

    void EditorScene::handleDropModel(const AssetDragDropPayload &payload) const
    {
        const auto modelRef = assets::AssetCatalog::getInstance().getAsset(payload.id);
        if (!modelRef)
            return;
        if (const auto model = modelRef.as<assets::Model>(); model)
        {
            auto& sceneManager = Application::getInstance().getSceneManager();
            // Create entity with the model
            ecs::Entity newEntity = EntityFactory3D::createModel(
                model,
                {0.0f, 0.0f, 0.0f}, // position
                {1.0f, 1.0f, 1.0f}, // scale
                {0.0f, 0.0f, 0.0f}  // rotation
            );

            // Add to the scene
            auto& scene = sceneManager.getScene(m_sceneId);
            scene.addEntity(newEntity);

            // Record action for undo/redo TODO: Fix undo for models, it does not seem to work properly
            auto action = std::make_unique<EntityCreationAction>(newEntity);
            ActionManager::get().recordAction(std::move(action));
        }
    }

    void EditorScene::handleDropTexture(const AssetDragDropPayload &payload) const
    {
        const auto textureRef = assets::AssetCatalog::getInstance().getAsset(payload.id);
        if (!textureRef)
            return;
        if (const auto texture = textureRef.as<assets::Texture>(); texture)
        {
            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_viewportBounds[0].x;
            my -= m_viewportBounds[0].y;

            // Flip the y-coordinate to match opengl texture format
            my = m_contentSize.y - my;

            // Check if mouse is inside viewport
            if (!(mx >= 0 && my >= 0 && mx < m_contentSize.x && my < m_contentSize.y))
                return;
            const int entityId = sampleEntityTexture(mx, my);
            if (entityId == -1) {
                auto& sceneManager = Application::getInstance().getSceneManager();
                components::Material material;
                material.albedoTexture = texture;
                material.albedoColor = glm::vec4(1.0f); // White to show texture colors

                // Create billboard entity
                ecs::Entity newEntity = EntityFactory3D::createBillboard(
                    {0.0f, 0.0f, 0.0f}, // position
                    {1.0f, 1.0f, 1.0f}, // size
                    material
                );

                // Add to the scene
                auto& scene = sceneManager.getScene(m_sceneId);
                scene.addEntity(newEntity);

                // Record action for undo/redo
                auto action = std::make_unique<EntityCreationAction>(newEntity);
                ActionManager::get().recordAction(std::move(action));
                return;
            }
            const auto matComponent = Application::m_coordinator->tryGetComponent<components::MaterialComponent>(entityId);
            if (!matComponent)
                return;
            const auto material = matComponent->get().material.lock();
            if (!material)
                return;
            material->getData()->albedoTexture = texture;
        }
    }

    void EditorScene::handleDropMaterial(const AssetDragDropPayload &payload) const
    {
        const auto materialRef = assets::AssetCatalog::getInstance().getAsset(payload.id);
        if (!materialRef)
            return;
        if (const auto material = materialRef.as<assets::Material>(); material)
        {
            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_viewportBounds[0].x;
            my -= m_viewportBounds[0].y;

            // Flip the y-coordinate to match opengl texture format
            my = m_contentSize.y - my;

            // Check if mouse is inside viewport
            if (!(mx >= 0 && my >= 0 && mx < m_contentSize.x && my < m_contentSize.y))
                return;
            const int entityId = sampleEntityTexture(mx, my);
            if (entityId == -1)
                return;
            const auto matComponent = Application::m_coordinator->tryGetComponent<components::MaterialComponent>(entityId);
            if (!matComponent)
                return;
            matComponent->get().material = material;
        }
    }

    void EditorScene::handleDropTarget()
    {
        if (ImGui::BeginDragDropTarget())
        {
            // Handle drops from asset manager
            if (const ImGuiPayload* assetPayload = ImGui::AcceptDragDropPayload("ASSET_DRAG", ImGuiDragDropFlags_AcceptBeforeDelivery))
            {
                IM_ASSERT(assetPayload->DataSize == sizeof(AssetDragDropPayload));
                auto [mx, my] = ImGui::GetMousePos();
                mx -= m_viewportBounds[0].x;
                my -= m_viewportBounds[0].y;

                // Flip the y-coordinate to match opengl texture format
                my = m_contentSize.y - my;

                // Check if mouse is inside viewport
                if (!(mx >= 0 && my >= 0 && mx < m_contentSize.x && my < m_contentSize.y))
                    return;
                const int entityId = sampleEntityTexture(mx, my);
                if (entityId != -1 && static_cast<ecs::Entity>(entityId) != m_entityHovered)
                {
                    m_entityHovered = static_cast<ecs::Entity>(entityId);
                    Application::m_coordinator->addComponent(m_entityHovered, components::SelectedTag{});
                }
                if (entityId == -1 && m_entityHovered != ecs::INVALID_ENTITY)
                {
                    Application::m_coordinator->removeComponent<components::SelectedTag>(m_entityHovered);
                    m_entityHovered = ecs::INVALID_ENTITY;
                }
                if (!assetPayload->IsDelivery())
                {
                    return;
                }
                if (m_entityHovered != ecs::INVALID_ENTITY)
                    Application::m_coordinator->removeComponent<components::SelectedTag>(m_entityHovered);
                m_entityHovered = ecs::INVALID_ENTITY;
                const auto& payload = *static_cast<const AssetDragDropPayload*>(assetPayload->Data);

                switch(payload.type)
                {
                    case assets::AssetType::MODEL:
                        handleDropModel(payload);
                        break;
                    case assets::AssetType::TEXTURE:
                        handleDropTexture(payload);
                        break;
                    case assets::AssetType::MATERIAL:
                        handleDropMaterial(payload);
                        break;
                    default:
                        break;
                }
            }
            ImGui::EndDragDropTarget();
        }
    }
}
