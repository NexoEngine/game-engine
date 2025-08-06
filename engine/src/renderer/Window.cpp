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
#include "renderer/RendererExceptions.hpp"
#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/OpenGlWindow.hpp"
#endif
#include <tracy/Tracy.hpp>

namespace nexo::renderer {
    std::shared_ptr<NxWindow> NxWindow::create(int width, int height, const std::string &title)
    {
        ZoneScoped;
        ZoneName("Window Create", 13);
        ZoneValue(static_cast<int64_t>(width * height));
        ZoneText(title.c_str(), title.length());

        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlWindow>(width, height, title);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }

}
