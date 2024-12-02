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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtx/quaternion.hpp>


#include "Timestep.hpp"

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

            virtual void setPosition(const glm::vec3 &position) {m_position = position; recalculateViewMatrix();};
            [[nodiscard]] virtual const glm::vec3 &getPosition() const { return m_position; };

            virtual void setRotation(const glm::vec3 &rotation) {m_rotation = glm::quat(glm::radians(rotation)); recalculateViewMatrix();};
            [[nodiscard]] virtual glm::vec3 getRotation() const { return glm::degrees(glm::eulerAngles(m_rotation)); };
            virtual void rotate(const glm::vec3 &deltaRotation);

            [[nodiscard]] virtual const glm::mat4 &getProjectionMatrix() const { return m_projectionMatrix; };
            [[nodiscard]] virtual const glm::mat4 &getViewMatrix() const { return m_viewMatrix; };
            [[nodiscard]] virtual const glm::mat4 &getViewProjectionMatrix() const { return m_viewProjectionMatrix; };

            [[nodiscard]] virtual CameraMode getMode() const = 0;

            [[nodiscard]] CameraId getCameraID() const { return m_id; };

            virtual void onUpdate([[maybe_unused]] Timestep ts) {};
            static CameraId nextCameraId;
        protected:
            CameraId m_id{};
            CameraMode m_mode;

            glm::mat4 m_projectionMatrix{};
            glm::mat4 m_viewMatrix{};
            glm::mat4 m_viewProjectionMatrix{};

            glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
            glm::quat m_rotation = {1.0f, 0.0f, 0.0f, 0.0f};
        private:
            void recalculateViewMatrix();
    };
}
