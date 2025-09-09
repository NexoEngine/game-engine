//// Buffer.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for vertex buffer abstract class
//
///////////////////////////////////////////////////////////////////////////////
#include "Buffer.hpp"
#include "renderer/RendererExceptions.hpp"
#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/OpenGlBuffer.hpp"
#endif


namespace nexo::renderer {

    std::shared_ptr<NxVertexBuffer> createVertexBuffer(float *vertices, unsigned int size)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlVertexBuffer>(vertices, size);
        #else
            THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
        #endif
    }

    std::shared_ptr<NxVertexBuffer> createVertexBuffer(unsigned int size)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlVertexBuffer>(size);
        #else
            THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
        #endif
    }

    std::shared_ptr<NxIndexBuffer> createIndexBuffer()
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlIndexBuffer>();
        #else
            THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
        #endif
    }

}
