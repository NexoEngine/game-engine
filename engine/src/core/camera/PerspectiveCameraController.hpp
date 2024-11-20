//// PerspectiveCameraController.hpp //////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Header for the perspective camera controller
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "PerspectiveCamera.hpp"
#include "core/Timestep.hpp"
#include "core/event/Event.hpp"
#include "core/event/WindowEvent.hpp"

namespace nexo::camera {

    class PerspectiveCameraController final : public Camera, LISTENS_TO(
        event::EventMouseScroll,
        event::EventWindowResize
    ) {
        public:
            explicit PerspectiveCameraController(float aspectRatio, float fovY = 45.0f);

            void onUpdate(core::Timestep ts) override;

            PerspectiveCamera &getCamera() { return m_camera; }
            [[nodiscard]] const PerspectiveCamera &getCamera() const { return m_camera; }

            void handleEvent(event::EventMouseScroll &event) override;
            void handleEvent(event::EventWindowResize &event) override;

            void setPosition(const glm::vec3 &position) override { m_camera.setPosition(position); };
            void setRotation(const float rotation) override { m_camera.setRotation(rotation); };
            [[nodiscard]] const glm::vec3 &getPosition() const override { return m_camera.getPosition(); };
            [[nodiscard]] float getRotation() const override { return m_camera.getRotation(); };

            [[nodiscard]] const glm::mat4 &getProjectionMatrix() const override { return m_camera.getProjectionMatrix(); };
            [[nodiscard]] const glm::mat4 &getViewMatrix() const override { return m_camera.getViewMatrix(); };
            [[nodiscard]] const glm::mat4 &getViewProjectionMatrix() const override { return m_camera.getViewProjectionMatrix(); }

            [[nodiscard]] CameraMode getMode() const override {return m_camera.getMode();}

            bool zoomOn = true;

        private:
            float m_aspectRatio;
            float m_fovY;
            float m_zoomLevel = 1.0f;
            PerspectiveCamera m_camera;

            glm::vec3 m_cameraPosition = {0.0f, 0.0f, 5.0f};
            glm::vec3 m_cameraRotation = {0.0f, 0.0f, 0.0f};
            float m_cameraTranslationSpeed = 1.0f;
            float m_cameraRotationSpeed = 100.0f;
    };

}
