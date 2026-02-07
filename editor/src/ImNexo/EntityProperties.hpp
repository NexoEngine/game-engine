//// EntityPropertiesComponents.hpp ///////////////////////////////////////////
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
//  Date:        22/05/2025
//  Description: Header file for the entity properties components
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "components/Transform.hpp"

namespace ImNexo {

    /**
     * @brief Renders and handles the ambient light component editor UI.
     *
     * Creates a color picker for adjusting the ambient light color.
     * The color is represented as a vec3 in the component, but displayed
     * as a vec4 in the UI for compatibility with ImGui's color picker.
     *
     * @param ambientComponent Reference to the ambient light component being edited
     */
    void Ambient(nexo::components::AmbientLightComponent &ambientComponent);

    /**
     * @brief Renders and handles the directional light component editor UI.
     *
     * Creates a color picker for adjusting the directional light color,
     * and a table-based editor for the light's direction vector (x, y, z).
     * The direction is represented as a vec3 in the component.
     *
     * @param directionalComponent Reference to the directional light component being edited
     */
    void DirectionalLight(nexo::components::DirectionalLightComponent &directionalComponent);

    /**
     * @brief Renders and handles the point light component editor UI.
     *
     * Creates a color picker for adjusting the point light color,
     * and a table-based editor for the light's position (x, y, z)
     * and attenuation parameters (constant, linear, quadratic).
     * The position is represented as a vec3 in the transform component,
     * while attenuation parameters are floats in the point light component.
     *
     * @param pointComponent Reference to the point light component being edited
     * @param pointTransform Reference to the transform component for position editing
     */
    void PointLight(nexo::components::PointLightComponent &pointComponent,
                    nexo::components::TransformComponent &pointTransform);

    /**
     * @brief Renders and handles the spotlight component editor UI.
     *
     * Creates a color picker for adjusting the spotlight color,
     * and a table-based editor for the light's position (x, y, z),
     * direction (x, y, z), and cutoff angles (inner and outer).
     * The position is represented as a vec3 in the transform component,
     * while direction and cutoff angles are part of the spotlight component.
     *
     * @param spotComponent Reference to the spotlight component being edited
     * @param spotTransform Reference to the transform component for position editing
     */
    void SpotLight(nexo::components::SpotLightComponent &spotComponent,
                   nexo::components::TransformComponent &spotTransform);

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
     * Currently, includes only mouse sensitivity adjustment, which controls how
     * quickly the camera rotates in response to mouse movement.
     *
     * @param cameraControllerComponent Reference to the camera controller component being edited
     */
    void CameraController(nexo::components::PerspectiveCameraController &cameraControllerComponent);

} // namespace ImNexo
