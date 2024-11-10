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
    }


    void OpenGlFramebuffer::invalidate()
    {
        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        glGenTextures(1, &m_colorAttachments);
        glBindTexture(GL_TEXTURE_2D, m_colorAttachments);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specs.width, m_specs.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachments, 0);

        glGenTextures(1, &m_depthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specs.width, m_specs.height, 0, GL_DEPTH_STENCIL,
                     GL_UNSIGNED_INT_24_8, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw core::FramebufferCreationFailed("OPENGL");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGlFramebuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    }

    void OpenGlFramebuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}