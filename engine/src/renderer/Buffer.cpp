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
#include "opengl/OpenGlBuffer.hpp"


namespace nexo::renderer {

    std::shared_ptr<VertexBuffer> createVertexBuffer(float *vertices, unsigned int size)
    {
        return std::make_shared<OpenGlVertexBuffer>(vertices, size);
    }

    std::shared_ptr<VertexBuffer> createVertexBuffer(unsigned int size)
    {
        return std::make_shared<OpenGlVertexBuffer>(size);
    }

    std::shared_ptr<IndexBuffer> createIndexBuffer(unsigned int *indices, unsigned int count)
    {
        return std::make_shared<OpenGlIndexBuffer>(indices, count);
    }
}
