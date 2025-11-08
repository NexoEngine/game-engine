//// OpenGlShaderUniformBuffer.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Source file for the OpenGL implementation of the UBO
//
///////////////////////////////////////////////////////////////////////////////////////////

#include "OpenGlShaderUniformBuffer.hpp"

#include <glad/glad.h>

namespace nexo::renderer {

    NxOpenGlShaderUniformBuffer::NxOpenGlShaderUniformBuffer(const unsigned int size)
    {
        glCreateBuffers(1, &m_id);
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        m_capacity = size;
    }

    void NxOpenGlShaderUniformBuffer::bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    }

    void NxOpenGlShaderUniformBuffer::bindBase(const unsigned int bindingLocation) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingLocation, m_id);
    }

    void NxOpenGlShaderUniformBuffer::unbind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void NxOpenGlShaderUniformBuffer::setData(void* data, unsigned int size)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        if (size > m_capacity) {
            size_t newCapacity = std::max(size, m_capacity * 2);
            glBufferData(GL_UNIFORM_BUFFER, newCapacity, data, GL_DYNAMIC_DRAW);
            m_capacity = newCapacity;
        } else {
            glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

} // namespace nexo::renderer
