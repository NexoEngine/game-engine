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
#include "renderer/RendererExceptions.hpp"
#include "Logger.hpp"

#include <glad/glad.h>
#include <iostream>
#include <utility>

namespace nexo::renderer {

    static constexpr unsigned int sMaxFramebufferSize = 8192;

    static GLenum textureTarget(const bool multisampled)
    {
        return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void createTextures(const bool multisampled, unsigned int *outId, const unsigned int count)
    {
        glCreateTextures(textureTarget(multisampled), static_cast<int>(count), outId);
    }

    static void bindTexture(const bool multisampled, const unsigned int id)
    {
        glBindTexture(textureTarget(multisampled), id);
    }

    static void attachColorTexture(const unsigned int id, const unsigned int samples, const GLenum format,
                                   const unsigned int width, const unsigned int height, const unsigned int index)
    {
        const bool multisample = samples > 1;
        if (multisample)
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, static_cast<int>(samples), format,
                                    static_cast<int>(width), static_cast<int>(height), GL_TRUE);
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(format), static_cast<int>(width), static_cast<int>(height),
                         0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget(multisample), id, 0);
    }

    static void attachDepthTexture(const unsigned int id, const unsigned int samples, const GLenum format,
                                   const GLenum attachmentType, const unsigned int width, const unsigned int height)
    {
        const bool multisample = samples > 1;
        if (multisample)
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, static_cast<int>(samples), format,
                                    static_cast<int>(width), static_cast<int>(height), GL_TRUE);
        else
        {
            glTexStorage2D(GL_TEXTURE_2D, 1, format, static_cast<int>(width), static_cast<int>(height));

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisample), id, 0);
    }

    static bool isDepthFormat(const FrameBufferTextureFormats format)
    {
        switch (format)
        {
            case FrameBufferTextureFormats::DEPTH24STENCIL8: return true;
            default: return false;
        }
    }


    OpenGlFramebuffer::OpenGlFramebuffer(FramebufferSpecs specs) : m_specs(std::move(specs))
    {
        for (auto format: m_specs.attachments.attachments)
        {
            if (!isDepthFormat(format.textureFormat))
                m_colorAttachmentsSpecs.emplace_back(format);
            else
                m_depthAttachmentSpec = format;
        }
        invalidate();
    }

    OpenGlFramebuffer::~OpenGlFramebuffer()
    {
        glDeleteFramebuffers(1, &m_id);
        glDeleteTextures(static_cast<int>(m_colorAttachments.size()), m_colorAttachments.data());
        glDeleteTextures(1, &m_depthAttachment);
    }

    void OpenGlFramebuffer::invalidate()
    {
        if (m_id)
        {
            glDeleteFramebuffers(1, &m_id);
            glDeleteTextures(static_cast<int>(m_colorAttachments.size()), m_colorAttachments.data());
            glDeleteTextures(1, &m_depthAttachment);

            m_colorAttachments.clear();
            m_depthAttachment = 0;
        }

        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        const bool multisample = m_specs.samples > 1;

        if (!m_colorAttachmentsSpecs.empty())
        {
            m_colorAttachments.resize(m_colorAttachmentsSpecs.size());
            // Fill the color attachments vector with open gl texture ids
            createTextures(multisample, m_colorAttachments.data(), m_colorAttachmentsSpecs.size());

            for (unsigned int i = 0; i < m_colorAttachments.size(); ++i)
            {
                bindTexture(multisample, m_colorAttachments[i]);
                switch (m_colorAttachmentsSpecs[i].textureFormat)
                {
                    case FrameBufferTextureFormats::RGBA8:
                        attachColorTexture(m_colorAttachments[i], m_specs.samples, GL_RGBA8, m_specs.width,
                                           m_specs.height, i);
                        break;
                    case FrameBufferTextureFormats::DEPTH24STENCIL8:
                        break;
                    default:
                        LOG(NEXO_WARN, "[OPENGL] Failed to create framebuffer: unsupported color texture format");
                        return;
                }
            }
        }

        if (m_depthAttachmentSpec.textureFormat != FrameBufferTextureFormats::NONE)
        {
            createTextures(multisample, &m_depthAttachment, 1);
            bindTexture(multisample, m_depthAttachment);
            switch (m_depthAttachmentSpec.textureFormat)
            {
                case FrameBufferTextureFormats::DEPTH24STENCIL8:
                    attachDepthTexture(m_depthAttachment, m_specs.samples, GL_DEPTH24_STENCIL8,
                                       GL_DEPTH_STENCIL_ATTACHMENT, m_specs.width, m_specs.height);
                    break;
                default:
                    LOG(NEXO_WARN, "[OPENGL] Failed to create framebuffer: unsupported depth texture format");
                return;
            }
        }

        if (m_colorAttachments.size() > 1)
        {
            if (m_colorAttachments.size() >= 4)
                THROW_EXCEPTION(FramebufferCreationFailed, "OPENGL");
            constexpr GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
            glDrawBuffers(static_cast<int>(m_colorAttachments.size()), buffers);
        }
        else if (m_colorAttachments.empty())
            glDrawBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            THROW_EXCEPTION(FramebufferCreationFailed, "OPENGL");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGlFramebuffer::bind()
    {
        if (toResize)
            invalidate();

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
        if (!width || !height)
            THROW_EXCEPTION(FramebufferResizingFailed, "OPENGL", false, width, height);
        if (width > sMaxFramebufferSize || height > sMaxFramebufferSize)
            THROW_EXCEPTION(FramebufferResizingFailed, "OPENGL", true, width, height);

        m_specs.width = width;
        m_specs.height = height;
        toResize = true;
    }

}
