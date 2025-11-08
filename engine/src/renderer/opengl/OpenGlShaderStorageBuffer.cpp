//// OpenGlShaderStorageBuffer.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Source file for the opengl implementation of the SSBOs
//
///////////////////////////////////////////////////////////////////////////////

#include "OpenGlShaderStorageBuffer.hpp"
#include <glad/glad.h>

namespace nexo::renderer {
    NxOpenGlShaderStorageBuffer::NxOpenGlShaderStorageBuffer(const unsigned int size)
    {
        glCreateBuffers(1, &m_id);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
        glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        m_capacity = size;
    }

    void NxOpenGlShaderStorageBuffer::bind() const
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
    }

    void NxOpenGlShaderStorageBuffer::bindBase(const unsigned int bindingLocation) const
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingLocation, m_id);
    }

    void NxOpenGlShaderStorageBuffer::unbind() const
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void NxOpenGlShaderStorageBuffer::setData(void* data, unsigned int size)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
        if (size > m_capacity) {
            size_t newCapacity = std::max(size, m_capacity * 2);
            glBufferData(GL_SHADER_STORAGE_BUFFER, newCapacity, data, GL_DYNAMIC_DRAW);
            m_capacity = newCapacity;
        } else {
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
        }
        m_size = size;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void NxOpenGlShaderStorageBuffer::appendData(void* data, unsigned int size)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);

        unsigned int required = m_size + size;
        if (required > m_capacity) {
            unsigned int newCapacity = std::max(required, m_capacity * 2);

            GLuint newId = 0;
            glCreateBuffers(1, &newId);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, newId);
            glBufferData(GL_SHADER_STORAGE_BUFFER, newCapacity, nullptr, GL_DYNAMIC_DRAW);

            // Copy old contents
            if (m_size > 0) {
                glBindBuffer(GL_COPY_READ_BUFFER, m_id);
                glBindBuffer(GL_COPY_WRITE_BUFFER, newId);
                glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER,
                                    0, 0, m_size);
            }

            glDeleteBuffers(1, &m_id);
            m_id       = newId;
            m_capacity = newCapacity;

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
        }

        // Write new chunk after old data
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_size, size, data);
        m_size += size;

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
} // namespace nexo::renderer
