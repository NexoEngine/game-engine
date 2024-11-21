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

    OpenGlVertexBuffer::OpenGlVertexBuffer(const float *vertices, const unsigned int size)
    {
        glGenBuffers(1, &_id);
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGlVertexBuffer::OpenGlVertexBuffer(const unsigned int size)
    {
        glGenBuffers(1, &_id);
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGlVertexBuffer::~OpenGlVertexBuffer()
    {
        glDeleteBuffers(1, &_id);
    }

    void OpenGlVertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _id);
    }

    void OpenGlVertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGlVertexBuffer::setData(void *data, const unsigned int size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }


    // INDEX BUFFER

    OpenGlIndexBuffer::OpenGlIndexBuffer()
    {
        glGenBuffers(1, &_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    }

    OpenGlIndexBuffer::~OpenGlIndexBuffer()
    {
        glDeleteBuffers(1, &_id);
    }

    void OpenGlIndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
    }

    void OpenGlIndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OpenGlIndexBuffer::setData(unsigned int *indices, unsigned int count)
    {
        _count = count;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    }

    unsigned int OpenGlIndexBuffer::getCount() const
    {
        return _count;
    }
}
