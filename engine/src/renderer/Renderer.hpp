//// Renderer.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
