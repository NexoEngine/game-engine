//// Camera.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/03/2025
//  Description: Header file for the camera component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Transform.hpp"
#include "math/Vector.hpp"
#include "core/event/Input.hpp"
#include "renderer/Framebuffer.hpp"
#include "ecs/Entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace nexo::components {

	enum class CameraType {
		PERSPECTIVE,
		ORTHOGRAPHIC
	};

	/**
     * @brief Represents the camera component.
     *
     * Contains settings for viewport dimensions, field-of-view,
     * near/far plane distances, clear color, and projection type.
     * Also stores the render target and flags for active state and resizing.
     */
    struct CameraComponent {
        unsigned int width;                 ///< Width of the camera's viewport.
        unsigned int height;                ///< Height of the camera's viewport.
        bool viewportLocked = false;        ///< If true, the viewport dimensions are locked.
        float fov = 45.0f;                  ///< Field of view (in degrees) for perspective cameras.
        float nearPlane = 0.1f;             ///< Near clipping plane distance.
        float farPlane = 1000.0f;           ///< Far clipping plane distance.
        CameraType type = CameraType::PERSPECTIVE;  ///< The type of the camera (perspective or orthographic).

        glm::vec4 clearColor = {37.0f/255.0f, 35.0f/255.0f, 50.0f/255.0f, 111.0f/255.0f};  ///< Background clear color.

        bool active = true;                 ///< Indicates if the camera is active.
        bool main = true;                   ///< Indicates if the camera is the main camera.
        bool resizing = false;              ///< Internal flag indicating if the camera is resizing.

        std::shared_ptr<renderer::Framebuffer> m_renderTarget = nullptr; ///< The render target framebuffer.

        /**
         * @brief Retrieves the projection matrix for this camera.
         *
         * For perspective cameras, returns a perspective projection matrix.
         * For orthographic cameras, returns an orthographic projection matrix.
         *
         * @return glm::mat4 The projection matrix.
         */
        [[nodiscard]] glm::mat4 getProjectionMatrix() const
        {
            if (type == CameraType::PERSPECTIVE) {
                return glm::perspective(glm::radians(fov), static_cast<float>(width) / static_cast<float>(height), nearPlane, farPlane);
            }
            return glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, nearPlane, farPlane);
        }

        /**
         * @brief Computes the view matrix for the camera.
         *
         * Given a transform component that represents the camera's current position and orientation,
         * this function calculates the view matrix using a look-at approach.
         *
         * @param transf The transform component of the camera.
         * @return glm::mat4 The view matrix.
         */
        [[nodiscard]] glm::mat4 getViewMatrix(const TransformComponent &transf) const
        {
            const glm::vec3 forward = transf.quat * glm::vec3(0.0f, 0.0f, -1.0f);
            const glm::vec3 upVec = transf.quat * glm::vec3(0.0f, 1.0f, 0.0f);
            return glm::lookAt(transf.pos, transf.pos + forward, upVec);
        }

        /**
         * @brief Resizes the camera's viewport.
         *
         * Updates the width and height, marks the camera as resizing, and resizes the associated render target.
         *
         * @param newWidth The new width for the viewport.
         * @param newHeight The new height for the viewport.
         */
        void resize(const unsigned int newWidth, const unsigned int newHeight)
        {
            width = newWidth;
            height = newHeight;
            resizing = true;
            if (m_renderTarget) {
                m_renderTarget->resize(newWidth, newHeight);
            }
        }
    };

    /**
     * @brief Component used to control a perspective camera using mouse input.
     *
     * Stores the last known mouse position and settings for mouse sensitivity,
     * yaw, and pitch.
     */
    struct PerspectiveCameraController {
        PerspectiveCameraController() { lastMousePosition = event::getMousePosition(); }

        glm::vec2 lastMousePosition{};  ///< Last recorded mouse position.
        float mouseSensitivity = 0.1f;///< Sensitivity factor for mouse movement.
        float yaw = 0.0f;             ///< Yaw angle in degrees.
        float pitch = 0.0f;           ///< Pitch angle in degrees.
    };

    /**
     * @brief Component that defines the target and settings for a perspective camera.
     *
     * This component allows the camera to orbit around a target entity based on mouse input.
     */
    struct PerspectiveCameraTarget {
        glm::vec2 lastMousePosition = event::getMousePosition();  ///< Last recorded mouse position.
        float mouseSensitivity = 0.1f;                            ///< Sensitivity factor for mouse movement.
        float distance = 5.0f;                                    ///< Distance from the camera to the target entity.
        ecs::Entity targetEntity;                                 ///< The target entity the camera is focusing on.
    };

    /**
     * @brief Encapsulates the overall camera context.
     *
     * Includes the view-projection matrix, camera position, clear color,
     * and the render target used for rendering.
     */
    struct CameraContext {
        glm::mat4 viewProjectionMatrix;                      ///< Combined view and projection matrix.
        glm::vec3 cameraPosition;                            ///< The position of the camera.
        glm::vec4 clearColor;                                ///< Clear color used for rendering.
        std::shared_ptr<renderer::Framebuffer> renderTarget; ///< The render target framebuffer.
    };
}
