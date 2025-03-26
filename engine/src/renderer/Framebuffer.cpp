//// Framebuffer.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for framebuffer asbtract class
//
///////////////////////////////////////////////////////////////////////////////
#include "Framebuffer.hpp"
#include "renderer/RendererExceptions.hpp"
#ifdef GRAPHICS_API_OPENGL
    #include "opengl/OpenGlFramebuffer.hpp"
#endif

namespace nexo::renderer {

    std::shared_ptr<Framebuffer> Framebuffer::create(const FramebufferSpecs &specs)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlFramebuffer>(specs);
        #endif
        THROW_EXCEPTION(UnknownGraphicsApi, "UNKNOWN");

    }

}
