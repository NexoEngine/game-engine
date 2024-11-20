//// OpenGlFramebuffer.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the opengl frambuffer class
//
///////////////////////////////////////////////////////////////////////////////

#include "OpenGlFramebuffer.hpp"
#include "core/exceptions/Exceptions.hpp"

#include <glad/glad.h>
#include <iostream>

namespace nexo::renderer {
    OpenGlFramebuffer::OpenGlFramebuffer(const FramebufferSpecs &specs) : m_specs(specs)
    {
        invalidate();
    }

    OpenGlFramebuffer::~OpenGlFramebuffer()
    {
        glDeleteFramebuffers(1, &m_id);
        glDeleteTextures(1, &m_colorAttachments);
        glDeleteTextures(1, &m_depthAttachment);
    }


    void OpenGlFramebuffer::invalidate()
    {
        if (m_id)
        {
            glDeleteFramebuffers(1, &m_id);
            glDeleteTextures(1, &m_colorAttachments);
            glDeleteTextures(1, &m_depthAttachment);
        }

        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        glGenTextures(1, &m_colorAttachments);
        glBindTexture(GL_TEXTURE_2D, m_colorAttachments);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<int>(m_specs.width), static_cast<int>(m_specs.height), 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachments, 0);

        glGenTextures(1, &m_depthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<int>(m_specs.width),
                     static_cast<int>(m_specs.height), 0, GL_DEPTH_STENCIL,
                     GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            THROW_EXCEPTION(core::FramebufferCreationFailed, "OPENGL");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGlFramebuffer::bind()
    {
        if (toResize)
        {
            glBindTexture(GL_TEXTURE_2D, m_colorAttachments);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<int>(m_specs.width), static_cast<int>(m_specs.height),
                         0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<int>(m_specs.width),
                         static_cast<int>(m_specs.height), 0, GL_DEPTH_STENCIL,
                         GL_UNSIGNED_INT_24_8, nullptr);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        glViewport(0, 0, static_cast<int>(m_specs.width), static_cast<int>(m_specs.height));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGlFramebuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    unsigned int OpenGlFramebuffer::getFramebufferId() const
    {
        return m_id;
    }

    void OpenGlFramebuffer::resize(const unsigned int width, const unsigned int height)
    {
        m_specs.width = width;
        m_specs.height = height;
        toResize = true;
    }

}
