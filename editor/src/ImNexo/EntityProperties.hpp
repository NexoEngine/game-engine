//// EntityPropertiesComponents.hpp ///////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        22/05/2025
//  Description: Header file for the entity properties components
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <imgui.h>
#include <string>
#include <vector>

#include "components/Light.hpp"
#include "components/Transform.hpp"
#include "components/Camera.hpp"

namespace ImNexo {

    void Ambient(nexo::components::AmbientLightComponent &ambientComponent);

    void DirectionalLight(nexo::components::DirectionalLightComponent &directionalComponent);

    void PointLight(nexo::components::PointLightComponent &pointComponent, nexo::components::TransformComponent &pointTransform);

    void SpotLight(nexo::components::SpotLightComponent &spotComponent, nexo::components::TransformComponent &spotTransform);


    /**
     * @brief Renders and handles the transform component editor UI.
     *
     * Creates a table-based editor for position, rotation, and scale values of a transform component.
     * Rotation is handled specially to convert between quaternion (internal) and euler angles (UI display).
     * When the user modifies euler angles, the function calculates the delta from the last displayed euler
     * angles and applies a corresponding rotation to the master quaternion.
     *
     * @param transformComponent Reference to the transform component being edited
     * @param lastDisplayedEuler Reference to vector storing the last displayed euler angles for computing deltas
     */
    void Transform(nexo::components::TransformComponent &transformComponent, glm::vec3 &lastDisplayedEuler);


    /**
     * @brief Renders and handles the camera component editor UI.
     *
     * Creates a table-based editor for camera parameters, including:
     * - Viewport size (width/height) with optional locking
     * - Field of view (FOV) adjustment
     * - Near and far clipping planes
     * - Camera clear color with color picker
     *
     * The viewport size can be locked to prevent accidental changes, which is useful
     * when the camera is being used in a specific context that requires fixed dimensions.
     *
     * @param cameraComponent Reference to the camera component being edited
     */
    void Camera(nexo::components::CameraComponent &cameraComponent);

    /**
     * @brief Renders and handles the camera target component editor UI.
     *
     * Creates a table-based editor for a camera target component, which controls
     * a camera that orbits around a target entity. The editor includes:
     * - Mouse sensitivity for orbit control
     * - Distance from camera to target
     * - Target entity selection dropdown showing available entities
     *
     * The entity dropdown filters out cameras and lights to show only valid targets.
     *
     * @param cameraTargetComponent Reference to the camera target component being edited
     */
    void CameraTarget(nexo::components::PerspectiveCameraTarget &cameraTargetComponent);

    /**
     * @brief Renders and handles the camera controller component editor UI.
     *
     * Creates a table-based editor for a free-moving camera controller component.
     * Currently includes only mouse sensitivity adjustment, which controls how
     * quickly the camera rotates in response to mouse movement.
     *
     * @param cameraControllerComponent Reference to the camera controller component being edited
     */
    InteractionState CameraController(
        nexo::components::PerspectiveCameraController &cameraControllerComponent,
        nexo::components::PerspectiveCameraController::Memento &beforeState
    );

}
