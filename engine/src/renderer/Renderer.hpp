//// Renderer.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for renderer class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>

namespace nexo::renderer {

    class NxRenderer {
        public:
            static void init();
            static void onWindowResize(unsigned int width, unsigned int height);

            struct NxSceneData {
                glm::mat4 projectionMatrix;
            };
            static NxSceneData *_sceneData;

    };
}
