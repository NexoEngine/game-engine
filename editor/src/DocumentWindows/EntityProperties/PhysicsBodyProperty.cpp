//// PhysicsBodyProperty.cpp //////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      AI Assistant
//  Date:        08/01/2025
//  Description: Source file for the physics body property inspector
//
///////////////////////////////////////////////////////////////////////////////

#include <imgui.h>

#include "PhysicsBodyProperty.hpp"
#include "ImNexo/ImNexo.hpp"
#include "ImNexo/Components.hpp"
#include "context/ActionManager.hpp"
#include "context/actions/EntityActions.hpp"
#include "systems/PhysicsSystem.hpp"
#include "IconsFontAwesome.h"

namespace nexo::editor {

    bool PhysicsBodyProperty::addPhysicsComponentToEntity(ecs::Entity entity, bool isDynamic)
    {
        JPH::EMotionType motionType = isDynamic ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static;
        auto& coordinator = *Application::m_coordinator;
        auto& transformComponent = coordinator.getComponent<components::TransformComponent>(entity);
        nexo::system::ShapeType shapeType = nexo::system::ShapeType::Box;
        
        auto& app = Application::getInstance();
        JPH::BodyID bodyId = app.getPhysicsSystem()->createBodyFromShape(
            entity, 
            transformComponent, 
            shapeType, 
            motionType
        );
        
        if (!bodyId.IsInvalid()) {
            auto action = std::make_unique<ComponentAddAction<components::PhysicsBodyComponent>>(entity);
            ActionManager::get().recordAction(std::move(action));
            return true;
        } else {
            LOG(NEXO_ERROR, "Failed to create physics body for entity {}", entity);
            return false;
        }
    }

    void PhysicsBodyProperty::show(ecs::Entity entity)
    {
        auto& coordinator = *Application::m_coordinator;
        
        if (!coordinator.entityHasComponent<components::PhysicsBodyComponent>(entity)) {
            if (ImNexo::Header("##AddPhysicsNode", ICON_FA_PLUS " Add Physics Component"))
            {
                ImGui::Spacing();
                ImGui::Text("Add physics simulation to this entity:");
                ImGui::Spacing();
                
                static int physicsType = 1;
                ImGui::RadioButton("Static Body", &physicsType, 0);
                ImGui::SameLine();
                ImGui::TextDisabled("(?)");
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Static bodies don't move but can be collided with");
                }
                
                ImGui::RadioButton("Dynamic Body", &physicsType, 1);
                ImGui::SameLine();
                ImGui::TextDisabled("(?)");
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Dynamic bodies are affected by gravity and forces");
                }
                
                ImGui::Spacing();
                
                if (ImGui::Button("Add Physics Component")) {
                    addPhysicsComponentToEntity(entity, physicsType == 1);
                }
                
                ImGui::TreePop();
            }
        } else {
            auto& physicsComponent = coordinator.getComponent<components::PhysicsBodyComponent>(entity);
            
            if (ImNexo::Header("##PhysicsBodyNode", ICON_FA_CUBE " Physics Body Component"))
            {
                ImGui::Spacing();
                
                const char* typeStr = (physicsComponent.type == components::PhysicsBodyComponent::Type::Dynamic) ? "Dynamic" : "Static";
                ImGui::Text("Type: %s", typeStr);
                
                ImGui::Spacing();
                
                const char* convertButtonText = (physicsComponent.type == components::PhysicsBodyComponent::Type::Dynamic) 
                    ? "Convert to Static" 
                    : "Convert to Dynamic";
                    
                if (ImGui::Button(convertButtonText)) {
                    auto& app = Application::getInstance();
                    auto* bodyInterface = app.getPhysicsSystem()->getBodyInterface();
                    
                    JPH::EMotionType newMotionType = (physicsComponent.type == components::PhysicsBodyComponent::Type::Dynamic) 
                        ? JPH::EMotionType::Static 
                        : JPH::EMotionType::Dynamic;
                        
                    bodyInterface->SetMotionType(physicsComponent.bodyID, newMotionType, JPH::EActivation::Activate);
                    
                    physicsComponent.type = (physicsComponent.type == components::PhysicsBodyComponent::Type::Dynamic) 
                        ? components::PhysicsBodyComponent::Type::Static 
                        : components::PhysicsBodyComponent::Type::Dynamic;
                }
                
                ImGui::SameLine();
                
                if (ImGui::Button("Remove Physics")) {
                    auto& app = Application::getInstance();
                    auto* bodyInterface = app.getPhysicsSystem()->getBodyInterface();
                    bodyInterface->RemoveBody(physicsComponent.bodyID);
                    bodyInterface->DestroyBody(physicsComponent.bodyID);
                    
                    auto action = std::make_unique<ComponentRemoveAction<components::PhysicsBodyComponent>>(entity);
                    ActionManager::get().recordAction(std::move(action));
                }
                
                ImGui::Spacing();
                ImGui::Text("Body ID: %u", physicsComponent.bodyID.GetIndexAndSequenceNumber());
                
                ImGui::TreePop();
            }
        }
    }
}
