//// Window.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for window abstraction
//
///////////////////////////////////////////////////////////////////////////////

#include "Window.hpp"
#include "core/exceptions/Exceptions.hpp"
#ifdef GRAPHICS_API_OPENGL
    #include "renderer/opengl/OpenGlWindow.hpp"
#endif

namespace nexo::renderer {
    std::shared_ptr<Window> Window::create(int width, int height, const char *title)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlWindow>(width, height, title);
        #endif
        THROW_EXCEPTION(core::UnknownGraphicsApi, "UNKNOWN");
    }

}
