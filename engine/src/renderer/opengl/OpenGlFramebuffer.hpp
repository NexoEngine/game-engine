//// OpenGlFramebuffer.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for open gl framebuffer class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "renderer/Framebuffer.hpp"
#include "renderer/RendererExceptions.hpp"

#include <glad/glad.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <iostream>

namespace nexo::renderer {

    template<typename T>
    constexpr GLenum getGLTypeFromTemplate()
    {
        if constexpr (std::is_same_v<T, float>)
            return GL_FLOAT;
        else if constexpr (std::is_same_v<T, int>)
            return GL_INT;
        else if constexpr (std::is_same_v<T, unsigned int>)
            return GL_UNSIGNED_INT;
        else if constexpr (std::is_same_v<T, glm::vec4>)
        	return GL_RGBA;
        return 0;
    }

    static int framebufferTextureFormatToOpenGlFormat(FrameBufferTextureFormats format)
    {
        constexpr GLenum formats[] = {GL_NONE, GL_RGBA, GL_RGBA, GL_RED_INTEGER};
        if (static_cast<unsigned int>(format) == 0 || format >= FrameBufferTextureFormats::DEPTH24STENCIL8) // Maybe change that later
            return -1;
        return static_cast<int>(formats[static_cast<unsigned int>(format)]);
    }

    class OpenGlFramebuffer final : public Framebuffer {
        public:
            /**
             * @brief Constructs an OpenGL framebuffer with the specified specifications.
             *
             * Initializes an OpenGL framebuffer object, sets up texture attachments based
             * on the provided specifications, and validates the framebuffer.
             *
             * @param specs The specifications for the framebuffer, including dimensions,
             *              attachments, and sampling options.
             *
             * Throws:
             * - FramebufferResizingFailed if the dimensions are invalid (e.g., zero or exceeding limits).
             * - FramebufferUnsupportedColorFormat if the color attachment format is unsupported.
             * - FramebufferUnsupportedDepthFormat if the depth attachment format is unsupported.
             * - FramebufferCreationFailed if the framebuffer status is not complete.
             */
            explicit OpenGlFramebuffer(FramebufferSpecs specs);

            /**
             * @brief Destroys the OpenGL framebuffer and releases associated resources.
             *
             * Deletes the framebuffer object and all associated texture attachments
             * (color and depth) from GPU memory.
             *
             * OpenGL Calls:
             * - `glDeleteFramebuffers`: Deletes the framebuffer object.
             * - `glDeleteTextures`: Deletes the textures associated with color and depth attachments.
             */
            ~OpenGlFramebuffer() override;

            /**
             * @brief Recreates the OpenGL framebuffer and its attachments.
             *
             * Invalidates the current framebuffer, releasing all associated resources,
             * and recreates the framebuffer based on the current specifications.
             * This method is useful when resizing the framebuffer or changing its attachments.
             *
             * OpenGL Operations:
             * - Deletes existing framebuffer and textures.
             * - Generates a new framebuffer object.
             * - Allocates and binds color and depth textures as specified.
             * - Validates the framebuffer status.
             *
             * Throws:
             * - FramebufferUnsupportedColorFormat if a specified color format is unsupported.
             * - FramebufferUnsupportedDepthFormat if a specified depth format is unsupported.
             * - FramebufferCreationFailed if the framebuffer is not complete.
             */
            void invalidate();

            /**
             * @brief Binds the framebuffer as the current rendering target.
             *
             * Activates the framebuffer for rendering, ensuring that subsequent draw calls
             * output to the framebuffer's attachments instead of the default framebuffer.
             * If the framebuffer is flagged for resizing, it is invalidated first.
             *
             * OpenGL Operations:
             * - `glBindFramebuffer`: Binds the framebuffer object.
             * - `glViewport`: Sets the viewport dimensions to match the framebuffer.
             * - `glClear`: Clears the framebuffer with a default black color.
             */
            void bind() override;
            /**
             * @brief Unbinds the framebuffer and restores the default framebuffer.
             *
             * Deactivates the current framebuffer, ensuring that subsequent draw calls
             * output to the default framebuffer (e.g., the screen or swap chain).
             *
             * OpenGL Operation:
             * - `glBindFramebuffer`: Binds the default framebuffer (ID = 0).
             */
            void unbind() override;

            void setClearColor(const glm::vec4 &color) override {m_clearColor = color;}

            [[nodiscard]] unsigned int getFramebufferId() const override;

            /**
             * @brief Resizes the framebuffer to new dimensions.
             *
             * Updates the width and height of the framebuffer and flags it for resizing.
             * The framebuffer will be invalidated and recreated during the next `bind()` call.
             *
             * @param width The new width of the framebuffer in pixels.
             * @param height The new height of the framebuffer in pixels.
             *
             * Throws:
             * - FramebufferResizingFailed if the new dimensions are zero or exceed the maximum supported size.
             */
            void resize(unsigned int width, unsigned int height) override;


            /**
             * @brief Reads a pixel value from a specified attachment.
             *
             * Template helper that retrieves the pixel value from the given attachment at (x,y).
             *
             * @tparam T The expected pixel data type.
             * @param attachmentIndex The index of the attachment.
             * @param x X-coordinate.
             * @param y Y-coordinate.
             * @return T The pixel data.
             */
            template<typename T>
            T getPixelImpl(unsigned int attachmentIndex, int x, int y) const
            {
                if (attachmentIndex >= m_colorAttachments.size())
                    THROW_EXCEPTION(FramebufferInvalidIndex, "OPENGL", attachmentIndex);

                glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

                auto &textureFormat = m_colorAttachmentsSpecs[attachmentIndex].textureFormat;
                GLenum format = framebufferTextureFormatToOpenGlFormat(textureFormat);
                GLenum type = getGLTypeFromTemplate<T>();

                T pixelData;
                glReadPixels(x, y, 1, 1, format, type, &pixelData);

                return pixelData;
            }
            void getPixelWrapper(unsigned int attachementIndex, int x, int y, void *result, const std::type_info &ti) const override;


            /**
             * @brief Clears the specified attachment with a given value.
             *
             * Template helper that clears the texture attached at the given index.
             *
             * @tparam T The type of the clear value.
             * @param attachmentIndex The index of the attachment.
             * @param value The value to clear the attachment to.
             */
            template<typename T>
            void clearAttachmentImpl(unsigned int attachmentIndex, const void *value) const
            {
                if (attachmentIndex >= m_colorAttachments.size())
                    THROW_EXCEPTION(FramebufferInvalidIndex, "OPENGL", attachmentIndex);
                auto &spec = m_colorAttachmentsSpecs[attachmentIndex];
                constexpr GLenum type = getGLTypeFromTemplate<T>();

                glClearTexImage(m_colorAttachments[attachmentIndex], 0, framebufferTextureFormatToOpenGlFormat(spec.textureFormat), type, value);
            }
            void clearAttachmentWrapper(unsigned int attachmentIndex, const void *value, const std::type_info &ti) const override;

            FramebufferSpecs &getSpecs() override {return m_specs;};
            [[nodiscard]] const FramebufferSpecs &getSpecs() const override {return m_specs;};

            [[nodiscard]] unsigned int getColorAttachmentId(const unsigned int index = 0) const override {return m_colorAttachments[index];};
            [[nodiscard]] unsigned int getDepthAttachmentId() const override { return m_depthAttachment; }
        private:
            unsigned int m_id = 0;
            bool toResize = false;
            FramebufferSpecs m_specs;

            glm::vec4 m_clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            std::vector<FrameBufferTextureSpecifications> m_colorAttachmentsSpecs;
            FrameBufferTextureSpecifications m_depthAttachmentSpec;

            std::vector<unsigned int> m_colorAttachments;
            unsigned int m_depthAttachment = 0;
    };
}
