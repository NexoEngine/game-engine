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

namespace nexo::camera {

    using CameraId = unsigned int;
    static inline CameraId NextId = 0;

    class Camera {
        public:
            Camera() = default;
            ~Camera() = default;

            void setPosition(const glm::vec3 &position) { m_position = position; recalculateViewMatrix(); };
            [[nodiscard]] const glm::vec3 &getPosition() const { return m_position; };

            void setRotation(const float rotation) { m_rotation = rotation; recalculateViewMatrix(); };
            [[nodiscard]] float getRotation() const { return m_rotation; };

            [[nodiscard]] const glm::mat4 &getProjectionMatrix() const { return m_projectionMatrix; };
            [[nodiscard]] const glm::mat4 &getViewMatrix() const { return m_viewMatrix; };
            [[nodiscard]] const glm::mat4 &getViewProjectionMatrix() const { return m_viewProjectionMatrix; };

            [[nodiscard]] CameraId getCameraID() const { return m_id; };

        protected:
            CameraId m_id{};

            glm::mat4 m_projectionMatrix{};
            glm::mat4 m_viewMatrix{};
            glm::mat4 m_viewProjectionMatrix{};

            glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
            float m_rotation = 0.0f;
        private:
            void recalculateViewMatrix();
    };
}
