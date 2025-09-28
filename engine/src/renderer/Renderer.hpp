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
        /**
         * @brief Initializes the renderer and its associated resources.
         *
         * This method sets up the rendering context and prepares any necessary resources
         * for rendering operations. It should be called once during application initialization.
         *
         * Usage:
         * - Call this method before performing any rendering operations.
         */
        static void init();

        /**
         * @brief Handles window resize events to adjust the viewport.
         *
         * This method should be called whenever the application window is resized.
         * It updates the rendering viewport to match the new window dimensions,
         * ensuring that rendered content scales appropriately.
         *
         * @param width The new width of the window in pixels.
         * @param height The new height of the window in pixels.
         *
         * Usage:
         * - Typically called from the windowing system's resize callback.
         */
        static void onWindowResize(unsigned int width, unsigned int height);

        struct NxSceneData {
            glm::mat4 projectionMatrix;
        };
        static NxSceneData *_sceneData;
    };
} // namespace nexo::renderer
