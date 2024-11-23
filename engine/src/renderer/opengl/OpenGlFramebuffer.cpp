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

    static int framebufferTextureFormatToOpenGlFormat(FrameBufferTextureFormats format)
    {
        constexpr GLenum internalFormats[] = {GL_NONE, GL_RGBA8, GL_RGBA16, GL_DEPTH24_STENCIL8, GL_DEPTH24_STENCIL8};
        if (static_cast<unsigned int>(format) == 0 || format >= FrameBufferTextureFormats::NB_TEXTURE_FORMATS)
            return -1;
        return static_cast<int>(internalFormats[static_cast<unsigned int>(format)]);
    }

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
        if (!m_specs.width || !m_specs.height)
            THROW_EXCEPTION(FramebufferResizingFailed, "OPENGL", false, m_specs.width, m_specs.height);
        if (m_specs.width > sMaxFramebufferSize || m_specs.height > sMaxFramebufferSize)
            THROW_EXCEPTION(FramebufferResizingFailed, "OPENGL", true, m_specs.width, m_specs.height);
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
                const int glTextureFormat = framebufferTextureFormatToOpenGlFormat(
                    m_colorAttachmentsSpecs[i].textureFormat);
                if (glTextureFormat == -1)
                    THROW_EXCEPTION(FramebufferUnsupportedColorFormat, "OPENGL");
                attachColorTexture(m_colorAttachments[i], m_specs.samples, glTextureFormat, m_specs.width,
                                   m_specs.height, i);
            }
        }

        if (m_depthAttachmentSpec.textureFormat != FrameBufferTextureFormats::NONE)
        {
            createTextures(multisample, &m_depthAttachment, 1);
            bindTexture(multisample, m_depthAttachment);
            int glDepthFormat = framebufferTextureFormatToOpenGlFormat(m_depthAttachmentSpec.textureFormat);
            if (glDepthFormat == -1)
                THROW_EXCEPTION(FramebufferUnsupportedDepthFormat, "OPENGL");
            attachDepthTexture(m_depthAttachment, m_specs.samples, glDepthFormat, GL_DEPTH_STENCIL_ATTACHMENT,
                               m_specs.width, m_specs.height);
        }

        if (m_colorAttachments.size() > 1)
        {
            if (m_colorAttachments.size() >= 4)
                THROW_EXCEPTION(FramebufferCreationFailed, "OPENGL");
            constexpr GLenum buffers[4] = {
                GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
            };
            glDrawBuffers(static_cast<int>(m_colorAttachments.size()), buffers);
        } else if (m_colorAttachments.empty())
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
