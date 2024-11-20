//// PerspectiveCamera.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Header file for the perspective camera
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Camera.hpp"

namespace nexo::camera {

    class PerspectiveCamera final : public Camera {
        public:
            PerspectiveCamera(float fovY, float aspectRatio, float nearClip, float farClip);

            void setProjection(float fovY, float aspectRatio, float nearClip, float farClip);
            void setAspectRatio(float aspectRatio);

            [[nodiscard]] CameraMode getMode() const override {return m_mode;}

        private:
            float m_fovY = 45.0f;
            float m_aspectRatio = 16.0f / 9.0f;
            float m_nearClip = 0.1f;
            float m_farClip = 1000.0f;
    };

}