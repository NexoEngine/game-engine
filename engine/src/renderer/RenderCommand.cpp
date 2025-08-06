//// RenderCommand.cpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for render command class
//
///////////////////////////////////////////////////////////////////////////////
#include "RenderCommand.hpp"
#include "renderer/RendererExceptions.hpp"
#include <tracy/Tracy.hpp>
#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/OpenGlRendererAPI.hpp"
#endif

namespace nexo::renderer {

    #ifdef NX_GRAPHICS_API_OPENGL
        NxRendererApi *NxRenderCommand::_rendererApi = new NxOpenGlRendererApi;
    #endif

    void NxRenderCommand::init()
    {
        ZoneScoped;
        ZoneName("Render Command Init", 18);

        if (!_rendererApi)
            THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
        _rendererApi->init();
    }
}
