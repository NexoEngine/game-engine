//// OrthographicCamera.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: header file for the orthographic camera class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Camera.hpp"

namespace nexo::camera {

    // 2D Rendering only (rotation only around Z-axis)
    class OrthographicCamera : public Camera {
        public:
            OrthographicCamera(float left, float right, float bottom, float top);

            void setProjection(float left, float right, float bottom, float top);
    };

}
