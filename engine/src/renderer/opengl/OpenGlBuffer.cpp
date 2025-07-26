//// OpenGlBuffer.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        08/11/2024
//  Description: Source file for the opengl buffer classes (vertex and index)
//
///////////////////////////////////////////////////////////////////////////////

#include "OpenGlBuffer.hpp"

#include <glad/glad.h>

namespace nexo::renderer {
    // VERTEX BUFFER

    NxOpenGlVertexBuffer::NxOpenGlVertexBuffer(const float *vertices, const unsigned int size)
    {
        glGenBuffers(1, &_id);
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    NxOpenGlVertexBuffer::NxOpenGlVertexBuffer(const unsigned int size)
    {
        glGenBuffers(1, &_id);
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    NxOpenGlVertexBuffer::~NxOpenGlVertexBuffer()
    {
        glDeleteBuffers(1, &_id);
    }

    void NxOpenGlVertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _id);
    }

    void NxOpenGlVertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void NxOpenGlVertexBuffer::setData(void *data, size_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }


    // INDEX BUFFER

    NxOpenGlIndexBuffer::NxOpenGlIndexBuffer()
    {
        glGenBuffers(1, &_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    }

    NxOpenGlIndexBuffer::~NxOpenGlIndexBuffer()
    {
        glDeleteBuffers(1, &_id);
    }

    void NxOpenGlIndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    }

    void NxOpenGlIndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void NxOpenGlIndexBuffer::setData(unsigned int *indices, size_t count)
    {
        _count = count;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    }

    unsigned int NxOpenGlIndexBuffer::getCount() const
    {
        return _count;
    }
}
