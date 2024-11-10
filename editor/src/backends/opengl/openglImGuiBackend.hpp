//// openglImGuiBackend.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the open gl imgui backend implementation
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <GLFW/glfw3.h>

namespace nexo::editor {
    class OpenGLImGuiBackend {
        public:
            static void init(GLFWwindow *window);
            static void shutdown();
            static void initFontAtlas();
            static void begin();
            static void end(GLFWwindow *window);
            static void *getErrorCallback();
    };
}