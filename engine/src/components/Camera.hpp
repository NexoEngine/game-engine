//// Camera.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/03/2025
//  Description: Header file for the camera component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Transform.hpp"
#include "core/event/Input.hpp"
#include "renderer/Framebuffer.hpp"
#include "ecs/Definitions.hpp"
#include "renderer/RenderPipeline.hpp"
#include <glm/glm.hpp>

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
        bool render = false;                ///< Indicates if the camera has to be rendered.
        bool main = true;                   ///< Indicates if the camera is the main camera.
        bool resizing = false;              ///< Internal flag indicating if the camera is resizing.

        std::shared_ptr<renderer::NxFramebuffer> m_renderTarget = nullptr; ///< The render target framebuffer.

        renderer::RenderPipeline pipeline;

        /**
         * @brief Retrieves the projection matrix for this camera.
         *
         * For perspective cameras, returns a perspective projection matrix.
         * For orthographic cameras, returns an orthographic projection matrix.
         *
         * @return glm::mat4 The projection matrix.
         */
        [[nodiscard]] glm::mat4 getProjectionMatrix() const;

        /**
         * @brief Computes the view matrix for the camera.
         *
         * Given a transform component that represents the camera's current position and orientation,
         * this function calculates the view matrix using a look-at approach.
         *
         * @param transf The transform component of the camera.
         * @return glm::mat4 The view matrix.
         */
        [[nodiscard]] glm::mat4 getViewMatrix(const TransformComponent &transf) const;

        /**
         * @brief Resizes the camera's viewport.
         *
         * Updates the width and height, marks the camera as resizing, and resizes the associated render target.
         *
         * @param newWidth The new width for the viewport.
         * @param newHeight The new height for the viewport.
         */
        void resize(unsigned int newWidth, unsigned int newHeight);

        struct Memento {
            unsigned int width;
            unsigned int height;
            bool viewportLocked;
            float fov;
            float nearPlane;
            float farPlane;
            CameraType type;
            glm::vec4 clearColor;
            bool main;
            std::shared_ptr<renderer::NxFramebuffer> renderTarget;
        };

        void restore(const Memento& memento);
        [[nodiscard]] Memento save() const;
    };

    struct EditorCameraTag {};

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
        float translationSpeed = 5.0f; ///< Camera speed
        bool wasMouseReleased = true;
        bool wasActiveLastFrame = true;

        struct Memento {
            float mouseSensitivity;
            float translationSpeed;
        };

        void restore(const Memento& memento)
        {
            mouseSensitivity = memento.mouseSensitivity;
            translationSpeed = memento.translationSpeed;
        }

        [[nodiscard]] Memento save() const
        {
            return {
                mouseSensitivity,
                translationSpeed
            };
        }
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

        struct Memento {
            float mouseSensitivity;
            float distance;
            ecs::Entity targetEntity;
        };

        void restore(const Memento& memento)
        {
            mouseSensitivity = memento.mouseSensitivity;
            distance = memento.distance;
            targetEntity = memento.targetEntity;
        }

        [[nodiscard]] Memento save() const
        {
            return {
                mouseSensitivity,
                distance,
                targetEntity
            };
        }
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
        std::shared_ptr<renderer::NxFramebuffer> renderTarget; ///< The render target framebuffer.
        renderer::RenderPipeline pipeline;
    };
}
