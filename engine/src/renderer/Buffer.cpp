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

#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable: 4702) // Unreachable code
#endif

    std::shared_ptr<NxVertexBuffer> createVertexBuffer(float *vertices, unsigned int size)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlVertexBuffer>(vertices, size);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }

    std::shared_ptr<NxVertexBuffer> createVertexBuffer(unsigned int size)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlVertexBuffer>(size);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }

    std::shared_ptr<NxIndexBuffer> createIndexBuffer()
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlIndexBuffer>();
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }

#if defined(_MSC_VER)
    #pragma warning(pop) // Unreachable code
#endif

}
