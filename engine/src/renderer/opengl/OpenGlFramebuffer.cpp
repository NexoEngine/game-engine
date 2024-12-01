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

    /**
     * @brief Converts a framebuffer texture format to its OpenGL equivalent.
     *
     * Maps internal framebuffer texture formats (e.g., RGBA8, DEPTH24STENCIL8) to their
     * corresponding OpenGL formats. Used during texture attachment creation.
     *
     * @param format The `FrameBufferTextureFormats` value to convert.
     * @return The OpenGL format (GLenum) corresponding to the specified texture format,
     *         or -1 if the format is invalid or unsupported.
     */
    static int framebufferTextureFormatToOpenGlFormat(FrameBufferTextureFormats format)
    {
        constexpr GLenum internalFormats[] = {GL_NONE, GL_RGBA8, GL_RGBA16, GL_DEPTH24_STENCIL8, GL_DEPTH24_STENCIL8};
        if (static_cast<unsigned int>(format) == 0 || format >= FrameBufferTextureFormats::NB_TEXTURE_FORMATS)
            return -1;
        return static_cast<int>(internalFormats[static_cast<unsigned int>(format)]);
    }

    /**
     * @brief Determines the OpenGL texture target based on multisampling.
     *
     * Returns the appropriate OpenGL texture target (e.g., `GL_TEXTURE_2D` or `GL_TEXTURE_2D_MULTISAMPLE`)
     * depending on whether multisampling is enabled.
     *
     * @param multisampled Indicates whether the texture is multisampled.
     * @return The OpenGL texture target as a GLenum.
     */
    static GLenum textureTarget(const bool multisampled)
    {
        return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    /**
     * @brief Creates OpenGL texture objects.
     *
     * Allocates one or more texture IDs in OpenGL, with support for multisampling.
     *
     * @param multisampled Indicates whether the textures are multisampled.
     * @param outId Pointer to an array where the generated texture IDs will be stored.
     * @param count The number of textures to create.
     *
     * OpenGL Operation:
     * - `glCreateTextures`: Allocates texture IDs for the specified texture target.
     */
    static void createTextures(const bool multisampled, unsigned int *outId, const unsigned int count)
    {
        glCreateTextures(textureTarget(multisampled), static_cast<int>(count), outId);
    }

    /**
     * @brief Binds an OpenGL texture to the current context.
     *
     * Activates the specified texture ID for subsequent OpenGL operations.
     *
     * @param multisampled Indicates whether the texture is multisampled.
     * @param id The OpenGL ID of the texture to bind.
     *
     * OpenGL Operation:
     * - `glBindTexture`: Binds the texture to the current context.
     */
    static void bindTexture(const bool multisampled, const unsigned int id)
    {
        glBindTexture(textureTarget(multisampled), id);
    }

    /**
     * @brief Attaches a color texture to the framebuffer.
     *
     * Configures and attaches a color texture to the framebuffer at the specified index.
     * Supports both multisampled and non-multisampled textures.
     *
     * @param id The OpenGL ID of the texture to attach.
     * @param samples The number of samples for multisampling (1 for no multisampling).
     * @param format The OpenGL internal format of the texture (e.g., GL_RGBA8).
     * @param width The width of the texture in pixels.
     * @param height The height of the texture in pixels.
     * @param index The attachment index (e.g., GL_COLOR_ATTACHMENT0 + index).
     *
     * OpenGL Operations:
     * - Configures the texture using `glTexImage2D` or `glTexImage2DMultisample`.
     * - Sets texture parameters for filtering and wrapping.
     * - Attaches the texture to the framebuffer using `glFramebufferTexture2D`.
     */
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

    /**
     * @brief Attaches a depth texture to the framebuffer.
     *
     * Configures and attaches a depth texture to the framebuffer. Supports both multisampled
     * and non-multisampled textures.
     *
     * @param id The OpenGL ID of the texture to attach.
     * @param samples The number of samples for multisampling (1 for no multisampling).
     * @param format The OpenGL internal format of the depth texture (e.g., GL_DEPTH24_STENCIL8).
     * @param attachmentType The framebuffer attachment type (e.g., GL_DEPTH_ATTACHMENT).
     * @param width The width of the texture in pixels.
     * @param height The height of the texture in pixels.
     *
     * OpenGL Operations:
     * - Configures the texture using `glTexStorage2D` or `glTexImage2DMultisample`.
     * - Sets texture parameters for filtering and wrapping.
     * - Attaches the texture to the framebuffer using `glFramebufferTexture2D`.
     */
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

    /**
     * @brief Checks if a texture format is a depth format.
     *
     * Determines whether the specified `FrameBufferTextureFormats` value corresponds to
     * a depth or depth-stencil format.
     *
     * @param format The texture format to check.
     * @return True if the format is a depth format, false otherwise.
     */
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
