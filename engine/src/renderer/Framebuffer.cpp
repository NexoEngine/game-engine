//// Framebuffer.cpp //////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for framebuffer asbtract class
//
///////////////////////////////////////////////////////////////////////////////
#include "Framebuffer.hpp"
#include "renderer/RendererExceptions.hpp"
#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/OpenGlFramebuffer.hpp"
#endif

namespace nexo::renderer {

    std::shared_ptr<NxFramebuffer> NxFramebuffer::create(const NxFramebufferSpecs &specs)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlFramebuffer>(specs);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }

}
