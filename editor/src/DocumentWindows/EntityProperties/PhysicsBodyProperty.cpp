//// PhysicsBodyProperty.cpp //////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        09/01/2025
//  Description: Implementation file for the physics body property class
//               Handles proper synchronization with Jolt Physics
//
///////////////////////////////////////////////////////////////////////////////

#include "PhysicsBodyProperty.hpp"

#include "Application.hpp"
#include "ImNexo/Components.hpp"
#include "ImNexo/Elements.hpp"
#include "components/Transform.hpp"
#include "systems/PhysicsSystem.hpp"

namespace nexo::editor {

    void PhysicsBodyProperty::show(const ecs::Entity entity)
    {
        const auto& coordinator = Application::m_coordinator;

        const auto physicsBodyOpt = coordinator->tryGetComponent<components::PhysicsBodyComponent>(entity);
        if (!physicsBodyOpt) {
            return;
        }

        const auto& physicsBody = physicsBodyOpt->get();

        if (ImNexo::Header("##PhysicsBody", "Physics Body Component")) {
            const auto currentType = physicsBody.type;

            constexpr std::array typeNames = {"Static", "Dynamic"};
            const int currentTypeIndex = (currentType == components::PhysicsBodyComponent::Type::Static) ? 0 : 1;
            int newTypeIndex           = currentTypeIndex;

            if (ImGui::Combo("Physics Type", &newTypeIndex, typeNames.data(), typeNames.size())) {
                const auto newType = (newTypeIndex == 0) ? components::PhysicsBodyComponent::Type::Static :
                                                           components::PhysicsBodyComponent::Type::Dynamic;

                if (newType != currentType) {
                    recreatePhysicsBody(entity, newType);
                }
            }

            ImGui::Separator();
            ImGui::Text("Body ID: %u", physicsBody.bodyID.GetIndex());

            const auto& app = Application::getInstance();
            if (const auto physicsSystem = app.getPhysicsSystem()) {
                const bool isActive = physicsSystem->getBodyInterface()->IsActive(physicsBody.bodyID);
                ImGui::Text("Active: %s", isActive ? "Yes" : "No");
            }

            ImGui::TreePop();
        }
    }

    void PhysicsBodyProperty::recreatePhysicsBody(const ecs::Entity entity,
                                                  const components::PhysicsBodyComponent::Type newType)
    {
        const auto& coordinator  = Application::m_coordinator;
        const auto& app          = Application::getInstance();
        const auto physicsSystem = app.getPhysicsSystem();

        if (!physicsSystem) {
            LOG(NEXO_ERROR, "PhysicsSystem not available");
            return;
        }

        const auto physicsBodyOpt = coordinator->tryGetComponent<components::PhysicsBodyComponent>(entity);
        const auto transformOpt   = coordinator->tryGetComponent<components::TransformComponent>(entity);

        if (!physicsBodyOpt || !transformOpt) {
            LOG(NEXO_ERROR, "Entity {} missing required components for physics body recreation", entity);
            return;
        }

        auto& physicsBody     = physicsBodyOpt->get();
        const auto& transform = transformOpt->get();

        const JPH::BodyID oldBodyID = physicsBody.bodyID;

        try {
            if (!oldBodyID.IsInvalid()) {
                physicsSystem->getBodyInterface()->RemoveBody(oldBodyID);
                physicsSystem->getBodyInterface()->DestroyBody(oldBodyID);
            }

            JPH::BodyID newBodyID;
            if (newType == components::PhysicsBodyComponent::Type::Static) {
                newBodyID = physicsSystem->createStaticBody(entity, transform);
            } else {
                newBodyID = physicsSystem->createDynamicBody(entity, transform);
            }

            if (newBodyID.IsInvalid()) {
                LOG(NEXO_ERROR, "Failed to create new physics body for entity {}", entity);
                return;
            }

            physicsBody.bodyID = newBodyID;
            physicsBody.type   = newType;

            LOG(NEXO_INFO, "Successfully recreated physics body for entity {} (type: {})", entity,
                (newType == components::PhysicsBodyComponent::Type::Static) ? "Static" : "Dynamic");

        } catch (const std::exception& e) {
            THROW_EXCEPTION(PhysicBodyCreationException, entity, e.what());
        }
    }

    void PhysicsBodyProperty::addPhysicsComponentToEntity(const ecs::Entity entity, const bool isDynamic)
    {
        const auto& coordinator  = Application::m_coordinator;
        const auto& app          = Application::getInstance();
        const auto physicsSystem = app.getPhysicsSystem();

        if (!physicsSystem) {
            LOG(NEXO_ERROR, "PhysicsSystem not available");
            return;
        }

        const auto transformOpt = coordinator->tryGetComponent<components::TransformComponent>(entity);
        if (!transformOpt) {
            LOG(NEXO_ERROR, "Entity {} missing TransformComponent for physics body creation", entity);
            return;
        }

        const auto& transform = transformOpt->get();
        const auto type       = isDynamic ? components::PhysicsBodyComponent::Type::Dynamic :
                                            components::PhysicsBodyComponent::Type::Static;

        try {
            JPH::BodyID bodyID;
            if (isDynamic) {
                bodyID = physicsSystem->createDynamicBody(entity, transform);
            } else {
                bodyID = physicsSystem->createStaticBody(entity, transform);
            }

            if (bodyID.IsInvalid()) {
                LOG(NEXO_ERROR, "Failed to create physics body for entity {}", entity);
                return;
            }

            coordinator->addComponent(entity, components::PhysicsBodyComponent{bodyID, type});

            LOG(NEXO_INFO, "Added physics component to entity {} (type: {})", entity, isDynamic ? "Dynamic" : "Static");

        } catch (const std::exception& e) {
            THROW_EXCEPTION(PhysicComponentCreationException, entity, e.what());
        }
    }

} // namespace nexo::editor
