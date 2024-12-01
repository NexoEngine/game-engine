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

namespace nexo::renderer {
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

            FramebufferSpecs &getSpecs() override {return m_specs;};
            [[nodiscard]] const FramebufferSpecs &getSpecs() const override {return m_specs;};

            [[nodiscard]] unsigned int getColorAttachmentId(const unsigned int index = 0) const override {return m_colorAttachments[index];};
        private:
            unsigned int m_id = 0;
            bool toResize = false;
            FramebufferSpecs m_specs;

            std::vector<FrameBufferTextureSpecifications> m_colorAttachmentsSpecs;
            FrameBufferTextureSpecifications m_depthAttachmentSpec;

            std::vector<unsigned int> m_colorAttachments;
            unsigned int m_depthAttachment = 0;
    };
}