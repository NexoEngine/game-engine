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
#include <tracy/Tracy.hpp>
#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/OpenGlBuffer.hpp"
#endif


namespace nexo::renderer {

    std::shared_ptr<NxVertexBuffer> createVertexBuffer(float *vertices, unsigned int size)
    {
        ZoneScoped;
        ZoneName("Create Vertex Buffer (Data)", 25);
        ZoneValue(static_cast<int64_t>(size));

        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlVertexBuffer>(vertices, size);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }

    std::shared_ptr<NxVertexBuffer> createVertexBuffer(unsigned int size)
    {
        ZoneScoped;
        ZoneName("Create Vertex Buffer", 19);
        ZoneValue(static_cast<int64_t>(size));

        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlVertexBuffer>(size);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }

    std::shared_ptr<NxIndexBuffer> createIndexBuffer()
    {
        ZoneScoped;
        ZoneName("Create Index Buffer", 18);

        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlIndexBuffer>();
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }
}
