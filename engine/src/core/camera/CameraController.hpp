//// CameraController.hpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        22/11/2024
//  Description: Header file for the camera controller interface
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Camera.hpp"
#include "Timestep.hpp"

namespace nexo::camera {

    class CameraController : public Camera {
        public:
        virtual ~CameraController() = default;

        virtual void onUpdate(Timestep ts) = 0;
        bool zoomOn = true;
    };

}
