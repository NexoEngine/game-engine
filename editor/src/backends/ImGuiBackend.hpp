//// ImGuiBackend.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the imgui backend interface class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderer/Window.hpp"

namespace nexo::editor {
    class ImGuiBackend {
        public:
            static void init(const std::shared_ptr<renderer::Window>& window);
            static void shutdown();
            static void initFontAtlas();
            static void begin();
            static void end(const std::shared_ptr<renderer::Window>& window);
            static void setErrorCallback(const std::shared_ptr<renderer::Window>& window);
    };
}