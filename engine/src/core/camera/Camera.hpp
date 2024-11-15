//// Camera.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the camera class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>

#include "core/Timestep.hpp"

namespace nexo::camera {

    using CameraId = unsigned int;

    enum class CameraMode {
        ORTHOGRAPHIC,
        PERSPECTIVE
    };

    class Camera {
        public:
            Camera() = default;

            virtual ~Camera() = default;

            virtual void setPosition(const glm::vec3 &position) { m_position = position; recalculateViewMatrix(); };
            [[nodiscard]] virtual const glm::vec3 &getPosition() const { return m_position; };

            virtual void setRotation(const float rotation) { m_rotation = rotation; recalculateViewMatrix(); };
            [[nodiscard]] virtual float getRotation() const { return m_rotation; };

            [[nodiscard]] virtual const glm::mat4 &getProjectionMatrix() const { return m_projectionMatrix; };
            [[nodiscard]] virtual const glm::mat4 &getViewMatrix() const { return m_viewMatrix; };
            [[nodiscard]] virtual const glm::mat4 &getViewProjectionMatrix() const { return m_viewProjectionMatrix; };

            [[nodiscard]] CameraMode getMode() const { return m_mode; };

            [[nodiscard]] CameraId getCameraID() const { return m_id; };

            virtual void onUpdate([[maybe_unused]] core::Timestep ts) {};
            static CameraId nextCameraId;
        protected:
            CameraId m_id{};

            glm::mat4 m_projectionMatrix{};
            glm::mat4 m_viewMatrix{};
            glm::mat4 m_viewProjectionMatrix{};

            glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
            float m_rotation = 0.0f;
            CameraMode m_mode{};
        private:
            void recalculateViewMatrix();
    };
}
