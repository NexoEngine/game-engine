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

#include <Jolt/Physics/Body/BodyLockInterface.h>
#include <Jolt/Physics/Body/MotionProperties.h>

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

            if (ImGui::Combo("Physics Type", &newTypeIndex, typeNames.data(), static_cast<int>(typeNames.size()))) {
                const auto newType = (newTypeIndex == 0) ? components::PhysicsBodyComponent::Type::Static :
                                                           components::PhysicsBodyComponent::Type::Dynamic;

                if (newType != currentType) {
                    recreatePhysicsBody(entity, newType);
                }
            }

            ImGui::Separator();

            const auto& app = Application::getInstance();
            if (const auto physicsSystem = app.getPhysicsSystem()) {
                auto* bodyInterface = physicsSystem->getBodyInterface();
                const JPH::BodyID bodyID = physicsBody.bodyID;

                // Debug Info Section
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                const bool debugOpen = ImGui::CollapsingHeader("Debug Information", ImGuiTreeNodeFlags_DefaultOpen);
                ImGui::PopStyleColor(4);

                if (debugOpen) {
                    ImGui::Text("Body ID: %u", bodyID.GetIndex());
                    const bool isActive = bodyInterface->IsActive(bodyID);
                    ImGui::Text("Active: %s", isActive ? "Yes" : "No");

                    // Position (read-only, comes from transform)
                    const JPH::Vec3 pos = bodyInterface->GetPosition(bodyID);
                    ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.GetX(), pos.GetY(), pos.GetZ());
                }

                // Only show physics properties for dynamic bodies
                if (currentType == components::PhysicsBodyComponent::Type::Dynamic) {

                    // Mass Properties
                    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    const bool massOpen = ImGui::CollapsingHeader("Mass Properties", ImGuiTreeNodeFlags_DefaultOpen);
                    ImGui::PopStyleColor(4);

                    if (massOpen) {
                        // Get mass info (read-only for now as Jolt mass editing requires shape recreation)
                        const auto lockInterface = physicsSystem->getBodyLockInterface();
                        JPH::BodyLockRead lock(*lockInterface, bodyID);
                        if (lock.Succeeded()) {
                            const JPH::Body& body = lock.GetBody();
                            const JPH::MotionProperties* motionProps = body.GetMotionProperties();

                            if (motionProps) {
                                const float invMass = motionProps->GetInverseMass();
                                const float mass = invMass > 0.0f ? 1.0f / invMass : 0.0f;

                                ImGui::Text("Mass: %.3f kg", mass);
                                ImGui::Text("Inverse Mass: %.6f", invMass);

                                if (ImGui::IsItemHovered()) {
                                    ImGui::SetTooltip("Mass is determined by shape and density.\nRecreate body to change mass.");
                                }
                            }
                        }
                    }

                    // Velocity Controls
                    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    const bool velocityOpen = ImGui::CollapsingHeader("Velocity", ImGuiTreeNodeFlags_DefaultOpen);
                    ImGui::PopStyleColor(4);

                    if (velocityOpen) {
                        // Linear Velocity
                        JPH::Vec3 linVel = bodyInterface->GetLinearVelocity(bodyID);
                        float linVelArray[3] = {linVel.GetX(), linVel.GetY(), linVel.GetZ()};

                        if (ImGui::DragFloat3("Linear Velocity", linVelArray, 0.1f, -100.0f, 100.0f, "%.2f m/s")) {
                            bodyInterface->SetLinearVelocity(bodyID, JPH::Vec3(linVelArray[0], linVelArray[1], linVelArray[2]));
                        }

                        // Angular Velocity
                        JPH::Vec3 angVel = bodyInterface->GetAngularVelocity(bodyID);
                        float angVelArray[3] = {angVel.GetX(), angVel.GetY(), angVel.GetZ()};

                        if (ImGui::DragFloat3("Angular Velocity", angVelArray, 0.1f, -10.0f, 10.0f, "%.2f rad/s")) {
                            bodyInterface->SetAngularVelocity(bodyID, JPH::Vec3(angVelArray[0], angVelArray[1], angVelArray[2]));
                        }

                        // Quick reset button
                        if (ImGui::Button("Reset Velocities")) {
                            bodyInterface->SetLinearVelocity(bodyID, JPH::Vec3::sZero());
                            bodyInterface->SetAngularVelocity(bodyID, JPH::Vec3::sZero());
                        }
                    }

                    // Physics Material Properties
                    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    const bool materialOpen = ImGui::CollapsingHeader("Material Properties", ImGuiTreeNodeFlags_DefaultOpen);
                    ImGui::PopStyleColor(4);

                    if (materialOpen) {
                        const auto lockInterface = physicsSystem->getBodyLockInterface();
                        JPH::BodyLockRead lock(*lockInterface, bodyID);
                        if (lock.Succeeded()) {
                            const JPH::Body& body = lock.GetBody();

                            // Friction
                            float friction = body.GetFriction();
                            if (ImGui::SliderFloat("Friction", &friction, 0.0f, 1.0f, "%.3f")) {
                                bodyInterface->SetFriction(bodyID, friction);
                            }
                            if (ImGui::IsItemHovered()) {
                                ImGui::SetTooltip("Surface friction (0 = ice, 1 = rubber)");
                            }

                            // Restitution (Bounciness)
                            float restitution = body.GetRestitution();
                            if (ImGui::SliderFloat("Restitution", &restitution, 0.0f, 1.0f, "%.3f")) {
                                bodyInterface->SetRestitution(bodyID, restitution);
                            }
                            if (ImGui::IsItemHovered()) {
                                ImGui::SetTooltip("Bounciness (0 = no bounce, 1 = perfect bounce)");
                            }
                        }
                    }

                    // Constraints and Limits
                    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    const bool constraintsOpen = ImGui::CollapsingHeader("Constraints and Limits");
                    ImGui::PopStyleColor(4);

                    if (constraintsOpen) {
                        const auto lockInterface = physicsSystem->getBodyLockInterface();

                        // Read current values
                        float linearDamping = 0.0f;
                        float angularDamping = 0.0f;
                        float maxLinVel = 0.0f;
                        float maxAngVel = 0.0f;

                        {
                            JPH::BodyLockRead readLock(*lockInterface, bodyID);
                            if (readLock.Succeeded()) {
                                const JPH::Body& body = readLock.GetBody();
                                const JPH::MotionProperties* motionProps = body.GetMotionProperties();

                                if (motionProps) {
                                    linearDamping = motionProps->GetLinearDamping();
                                    angularDamping = motionProps->GetAngularDamping();
                                    maxLinVel = motionProps->GetMaxLinearVelocity();
                                    maxAngVel = motionProps->GetMaxAngularVelocity();
                                }
                            }
                        }

                        // Display and edit
                        bool needsUpdate = false;
                        float newLinearDamping = linearDamping;
                        float newAngularDamping = angularDamping;

                        if (ImGui::SliderFloat("Linear Damping", &newLinearDamping, 0.0f, 1.0f, "%.3f")) {
                            needsUpdate = true;
                        }
                        if (ImGui::IsItemHovered()) {
                            ImGui::SetTooltip("Resistance to linear motion (air resistance)");
                        }

                        if (ImGui::SliderFloat("Angular Damping", &newAngularDamping, 0.0f, 1.0f, "%.3f")) {
                            needsUpdate = true;
                        }
                        if (ImGui::IsItemHovered()) {
                            ImGui::SetTooltip("Resistance to rotation");
                        }

                        // Apply changes if needed
                        if (needsUpdate) {
                            JPH::BodyLockWrite writeLock(*lockInterface, bodyID);
                            if (writeLock.Succeeded()) {
                                JPH::Body& body = writeLock.GetBody();
                                JPH::MotionProperties* motionProps = body.GetMotionProperties();

                                if (motionProps) {
                                    motionProps->SetLinearDamping(newLinearDamping);
                                    motionProps->SetAngularDamping(newAngularDamping);
                                }
                            }
                        }

                        ImGui::Text("Max Linear Velocity: %.1f m/s", maxLinVel);
                        if (ImGui::IsItemHovered()) {
                            ImGui::SetTooltip("Maximum linear velocity (set by physics system)");
                        }

                        ImGui::Text("Max Angular Velocity: %.1f rad/s", maxAngVel);
                        if (ImGui::IsItemHovered()) {
                            ImGui::SetTooltip("Maximum angular velocity (set by physics system)");
                        }
                    }

                    // Real-time Visual Feedback
                    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    const bool debugVizOpen = ImGui::CollapsingHeader("Real-time Debug Visualization");
                    ImGui::PopStyleColor(4);

                    if (debugVizOpen) {
                        static bool showVelocity = false;
                        static bool showForces = false;

                        ImGui::Checkbox("Show Velocity Arrows", &showVelocity);
                        ImGui::Checkbox("Show Applied Forces", &showForces);

                        if (showVelocity) {
                            const JPH::Vec3 linVel = bodyInterface->GetLinearVelocity(bodyID);
                            const float speed = linVel.Length();
                            ImGui::Text("Current Speed: %.2f m/s", speed);
                            ImGui::ProgressBar(speed / 50.0f, ImVec2(-1, 0));
                        }
                    }
                }
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

