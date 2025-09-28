//// OpenGlFramebuffer.hpp ////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
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
    /**
     * @brief Retrieves the OpenGL type corresponding to the given template type.
     *
     * This template function maps a C++ type to its corresponding OpenGL type.
     * For example, `float` maps to `GL_FLOAT`, `int` maps to `GL_INT`, etc.
     *
     * @tparam T The C++ type to map to an OpenGL type.
     * @return GLenum The OpenGL type corresponding to the template type.
     */
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
        else
            return 0;
    }

    /**
     * @brief Converts a framebuffer texture format to its corresponding OpenGL format.
     *
     * This function maps a `NxFrameBufferTextureFormats` enum value to its
     * corresponding OpenGL format. If the format is invalid or unsupported,
     * the function returns -1.
     *
     * @param format The framebuffer texture format to convert.
     * @return int The OpenGL format corresponding to the given texture format,
     *             or -1 if the format is invalid or unsupported.
     */
    static int framebufferTextureFormatToOpenGlFormat(NxFrameBufferTextureFormats format)
    {
        constexpr GLenum formats[] = {GL_NONE, GL_RGBA, GL_RGBA, GL_RED_INTEGER};
        if (static_cast<unsigned int>(format) == 0 ||
            format >= NxFrameBufferTextureFormats::DEPTH24STENCIL8) // Maybe change that later
            return -1;
        return static_cast<int>(formats[static_cast<unsigned int>(format)]);
    }

    class NxOpenGlFramebuffer final : public NxFramebuffer {
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
         * - NxFramebufferResizingFailed if the dimensions are invalid (e.g., zero or exceeding limits).
         * - NxFramebufferUnsupportedColorFormat if the color attachment format is unsupported.
         * - NxFramebufferUnsupportedDepthFormat if the depth attachment format is unsupported.
         * - NxFramebufferCreationFailed if the framebuffer status is not complete.
         */
        explicit NxOpenGlFramebuffer(NxFramebufferSpecs specs);

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
        ~NxOpenGlFramebuffer() override;

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
         * - NxFramebufferUnsupportedColorFormat if a specified color format is unsupported.
         * - NxFramebufferUnsupportedDepthFormat if a specified depth format is unsupported.
         * - NxFramebufferCreationFailed if the framebuffer is not complete.
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
         * @brief Binds a specified color attachment as a texture to a texture slot.
         *
         * Activates the specified color attachment texture for use in shaders,
         * allowing it to be sampled during rendering. The texture is bound to the
         * specified texture unit.
         *
         * @param slot The texture unit slot to bind the texture to (default is 0).
         * @param attachment The index of the color attachment to bind (default is 0).
         *
         * Throws:
         * - NxFramebufferInvalidIndex if the attachment index is out of range.
         *
         * OpenGL Operations:
         * - `glActiveTexture`: Activates the specified texture unit.
         * - `glBindTexture`: Binds the color attachment texture to the active texture unit.
         */
        void bindAsTexture(unsigned int slot = 0, unsigned int attachment = 0) override;

        /**
         * @brief Binds the depth attachment as a texture to a specified texture slot.
         *
         * Activates the depth texture for use in shaders, allowing it to be sampled
         * during rendering. The texture is bound to the specified texture unit.
         *
         * @param slot The texture unit slot to bind the depth texture to (default is 0).
         *
         * OpenGL Operations:
         * - `glActiveTexture`: Activates the specified texture unit.
         * - `glBindTexture`: Binds the depth texture to the active texture unit.
         */
        void bindDepthAsTexture(unsigned int slot = 0) override;

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

        /**
         * @brief Sets the clear color for the framebuffer.
         *
         * Updates the color used when clearing the framebuffer. This color is applied
         * during the `bind()` operation to clear the framebuffer's color attachments.
         *
         * @param color A `glm::vec4` representing the RGBA clear color.
         */
        void setClearColor(const glm::vec4 &color) override
        {
            m_clearColor = color;
        }

        /**
         * @brief Copies the contents from another framebuffer into this framebuffer.
         *
         * Performs a blit operation to copy color, depth, and stencil data from the
         * source framebuffer to this framebuffer. Both framebuffers must have compatible
         * attachments for the copy to succeed.
         *
         * @param source The source framebuffer to copy from.
         *
         * Throws:
         * - NxFramebufferInvalidIndex if the source framebuffer has more color attachments
         *   than this framebuffer.
         *
         * OpenGL Operations:
         * - `glBindFramebuffer`: Binds the source and destination framebuffers for reading and drawing.
         * - `glBlitFramebuffer`: Copies the specified buffers from the source to the destination.
         * - `glReadBuffer` and `glDrawBuffers`: Configures which attachments to read from and draw to.
         */
        void copy(std::shared_ptr<NxFramebuffer> source) override;

        /**
         * @brief Retrieves the OpenGL ID of the framebuffer.
         *
         * @return The OpenGL ID of the framebuffer object.
         */
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
         * - NxFramebufferResizingFailed if the new dimensions are zero or exceed the maximum supported size.
         */
        void resize(unsigned int width, unsigned int height) override;

        /**
         * @brief Retrieves the current size of the framebuffer.
         *
         * @return A `glm::vec2` containing the width and height of the framebuffer.
         */
        [[nodiscard]] glm::vec2 getSize() const override;

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
        T getPixelImpl(const unsigned int attachmentIndex, const int x, const int y) const
        {
            if (attachmentIndex >= m_colorAttachments.size())
                THROW_EXCEPTION(NxFramebufferInvalidIndex, "OPENGL", attachmentIndex);

            glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

            auto &textureFormat = m_colorAttachmentsSpecs[attachmentIndex].textureFormat;
            const GLenum format = framebufferTextureFormatToOpenGlFormat(textureFormat);
            const GLenum type   = getGLTypeFromTemplate<T>();

            T pixelData;
            glReadPixels(x, y, 1, 1, format, type, &pixelData);

            return pixelData;
        }

        /**
         * @brief Wrapper to call the templated getPixelImpl based on type_info.
         *
         * @param attachementIndex The index of the attachment.
         * @param x X-coordinate.
         * @param y Y-coordinate.
         * @param result Pointer to store the result.
         * @param ti Type information of the expected pixel data type.
         */
        void getPixelWrapper(unsigned int attachementIndex, int x, int y, void *result,
                             const std::type_info &ti) const override;

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
        void clearAttachmentImpl(const unsigned int attachmentIndex, const void *value) const
        {
            if (attachmentIndex >= m_colorAttachments.size())
                THROW_EXCEPTION(NxFramebufferInvalidIndex, "OPENGL", attachmentIndex);
            auto &spec            = m_colorAttachmentsSpecs[attachmentIndex];
            constexpr GLenum type = getGLTypeFromTemplate<T>();

            glClearTexImage(m_colorAttachments[attachmentIndex], 0,
                            framebufferTextureFormatToOpenGlFormat(spec.textureFormat), type, value);
        }

        /**
         * @brief Wrapper to call the templated clearAttachmentImpl based on type_info.
         *
         * @param attachmentIndex The index of the attachment.
         * @param value Pointer to the value to clear the attachment to.
         * @param ti Type information of the clear value type.
         */
        void clearAttachmentWrapper(unsigned int attachmentIndex, const void *value,
                                    const std::type_info &ti) const override;

        /**
         * @brief Retrieves the specifications of the framebuffer.
         *
         * @return The `NxFramebufferSpecs` structure containing the framebuffer's specifications.
         */
        NxFramebufferSpecs &getSpecs() override
        {
            return m_specs;
        }

        /** @brief Retrieves the specifications of the framebuffer (const version).
         *
         * @return The `NxFramebufferSpecs` structure containing the framebuffer's specifications.
         */
        [[nodiscard]] const NxFramebufferSpecs &getSpecs() const override
        {
            return m_specs;
        }

        /**
         * @brief Retrieves the number of color attachments in the framebuffer.
         *
         * @return The number of color attachments.
         */
        [[nodiscard]] unsigned int getNbColorAttachments() const override
        {
            return static_cast<unsigned int>(m_colorAttachments.size());
        }

        /**
         * @brief Retrieves the OpenGL ID of a specific color attachment.
         *
         * @param index The index of the color attachment (default is 0).
         * @return The OpenGL ID of the specified color attachment.
         */
        [[nodiscard]] unsigned int getColorAttachmentId(const unsigned int index = 0) const override
        {
            return m_colorAttachments[index];
        }

        /**
         * @brief Retrieves the OpenGL ID of the depth attachment.
         *
         * @return The OpenGL ID of the depth attachment.
         */
        [[nodiscard]] unsigned int getDepthAttachmentId() const override
        {
            return m_depthAttachment;
        }

        /**
         * @brief Checks if the framebuffer has a depth attachment.
         *
         * @return True if a depth attachment exists, false otherwise.
         */
        [[nodiscard]] bool hasDepthAttachment() const override
        {
            return m_depthAttachmentSpec.textureFormat != NxFrameBufferTextureFormats::NONE;
        }

        /**
         * @brief Checks if the framebuffer has a stencil attachment.
         *
         * @return True if a stencil attachment exists, false otherwise.
         */
        [[nodiscard]] bool hasStencilAttachment() const override
        {
            return m_depthAttachmentSpec.textureFormat != NxFrameBufferTextureFormats::NONE;
        }

        /**
         * @brief Checks if the framebuffer has a combined depth-stencil attachment.
         *
         * @return True if a combined depth-stencil attachment exists, false otherwise.
         */
        [[nodiscard]] bool hasDepthStencilAttachment() const override
        {
            return m_depthAttachmentSpec.textureFormat != NxFrameBufferTextureFormats::NONE;
        }

       private:
        unsigned int m_id = 0;
        bool toResize     = false;
        NxFramebufferSpecs m_specs;

        glm::vec4 m_clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        std::vector<NxFrameBufferTextureSpecifications> m_colorAttachmentsSpecs;
        NxFrameBufferTextureSpecifications m_depthAttachmentSpec;

        std::vector<unsigned int> m_colorAttachments;
        unsigned int m_depthAttachment = 0;
    };
} // namespace nexo::renderer
