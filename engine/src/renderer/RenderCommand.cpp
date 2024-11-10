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
#include "core/exceptions/Exceptions.hpp"
#ifdef GRAPHICS_API_OPENGL
    #include "opengl/OpenGlRendererAPI.hpp"
#endif

namespace nexo::renderer {

    #ifdef GRAPHICS_API_OPENGL
        RendererApi *RenderCommand::_rendererApi = new OpenGlRendererApi;
    #endif

    void RenderCommand::init()
    {
        if (!_rendererApi)
            throw core::UnknownGraphicsApi("UNKNOWN");
        _rendererApi->init();
    }
}
