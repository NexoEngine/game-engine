//// Framebuffer.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for framebuffer abstract class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace nexo::renderer {

    /**
     * @enum NxFrameBufferTextureFormats
     * @brief Enum representing the various texture formats supported for framebuffer attachments.
     *
     * Texture formats define how data is stored in framebuffer attachments. These include color,
     * depth, and stencil formats.
     *
     * Types:
     * - NONE: No specific format, used as a default.
     * - RGBA8: 8-bit per channel RGBA texture.
     * - RGBA16: 16-bit per channel RGBA texture.
     * - DEPTH24STENCIL8: 24-bit depth buffer with an 8-bit stencil buffer.
     * - Depth: Alias for DEPTH24STENCIL8.
     * - NB_TEXTURE_FORMATS: Tracks the number of texture formats (for internal use).
     */
    enum class NxFrameBufferTextureFormats {
        NONE = 0,

        RGBA8,
        RGBA16,

        RED_INTEGER,

        DEPTH24STENCIL8,

        Depth = DEPTH24STENCIL8,
        NB_TEXTURE_FORMATS
    };

    /**
     * @struct NxFrameBufferTextureSpecifications
     * @brief Defines the format for a single framebuffer texture attachment.
     *
     * This struct specifies the properties of a single texture attachment, such as its format.
     *
     * Members:
     * - @param textureFormat Specifies the format of the texture (e.g., RGBA8 or Depth).
     *
     * Constructors:
     * - FrameBufferTextureSpecifications(): Default constructor with no format.
     * - FrameBufferTextureSpecifications(const NxFrameBufferTextureFormats format): Initializes with a specified texture format.
     */
    struct NxFrameBufferTextureSpecifications {
        NxFrameBufferTextureSpecifications() = default;
        NxFrameBufferTextureSpecifications(const NxFrameBufferTextureFormats format) : textureFormat(format) {};

        NxFrameBufferTextureFormats textureFormat = NxFrameBufferTextureFormats::NONE;
    };

    /**
     * @struct NxFrameBufferAttachmentsSpecifications
     * @brief Defines the list of texture attachments for a framebuffer.
     *
     * A framebuffer can have multiple attachments (e.g., color and depth textures). This struct
     * holds a list of all texture specifications for a framebuffer.
     *
     * Members:
     * - @param attachments A vector containing the specifications for each texture attachment.
     *
     * Constructors:
     * - FrameBufferAttachmentsSpecifications(): Default constructor with no attachments.
     * - FrameBufferAttachmentsSpecifications(std::initializer_list<FrameBufferTextureSpecifications> attachments):
     *   Initializes the list with a set of predefined texture specifications.
     */
    struct NxFrameBufferAttachmentsSpecifications {
        NxFrameBufferAttachmentsSpecifications() = default;
        NxFrameBufferAttachmentsSpecifications(std::initializer_list<NxFrameBufferTextureSpecifications> attachments) : attachments(attachments) {};

        std::vector<NxFrameBufferTextureSpecifications> attachments;
    };

    /**
     * @struct NxFramebufferSpecs
     * @brief Represents the specifications for creating a framebuffer.
     *
     * NxFramebufferSpecs encapsulates all the necessary properties for initializing a framebuffer,
     * including dimensions, attachments, sampling, and swap chain behavior.
     *
     * Members:
     * - @param width The width of the framebuffer in pixels.
     * - @param height The height of the framebuffer in pixels.
     * - @param attachments A list of texture attachment specifications.
     * - @param samples The number of samples for multisampling (default is 1).
     * - @param swapChainTarget Indicates whether the framebuffer is part of the swap chain (default is false).
     */
    struct NxFramebufferSpecs {
        unsigned int width{};
        unsigned int height{};
        NxFrameBufferAttachmentsSpecifications attachments;

        unsigned int samples = 1;

        bool swapChainTarget = false;
    };

    /**
     * @class NxFramebuffer
     * @brief Abstract class representing a framebuffer in the rendering pipeline.
     *
     * A framebuffer is an off-screen rendering target that stores the results of
     * rendering operations. It can have multiple texture attachments, such as color,
     * depth, and stencil buffers. The `NxFramebuffer` class provides an abstraction layer
     * for creating and managing framebuffers across different graphics APIs (e.g., OpenGL, Vulkan).
     *
     * Key Features:
     * - Off-screen Rendering: Framebuffers are used for techniques like shadow mapping,
     *   post-processing effects, and deferred rendering.
     * - Multiple Attachments: Framebuffers can have multiple color attachments and
     *   additional depth/stencil attachments.
     * - Resizable: The framebuffer can be resized dynamically to match the viewport dimensions.
     *
     * Usage:
     * - The `NxFramebuffer` class is an abstract base class. Platform or API-specific
     *   implementations (e.g., NxOpenGLFramebuffer) must inherit and implement the
     *   pure virtual methods.
     *
     * Responsibilities:
     * - Manage GPU resources associated with framebuffers and their attachments.
     * - Bind and unbind framebuffers for rendering.
     * - Provide access to framebuffer specifications and texture attachment IDs.
     *
     * Example Workflow:
     * 1. Create a framebuffer with desired specifications using the `create` static method.
     * 2. Bind the framebuffer before issuing rendering commands.
     * 3. Access texture attachments (e.g., for post-processing) using `getColorAttachmentId`.
     * 4. Unbind the framebuffer to render to the default framebuffer (screen).
     */
    class NxFramebuffer {
        public:
            /**
            * @brief Destroys the framebuffer and releases associated resources.
            *
            * This virtual destructor ensures that derived classes properly clean up
            * framebuffer resources (e.g., OpenGL framebuffers).
            */
            virtual ~NxFramebuffer() = default;

            /**
             * @brief Binds the framebuffer as the active rendering target.
             *
             * When a framebuffer is bound, subsequent rendering operations will be directed
             * to this framebuffer instead of the default frame buffer (e.g., the screen).
             */
            virtual void bind() = 0;

            /**
             * @brief Unbinds the current framebuffer, reverting to the default framebuffer.
             *
             * After unbinding, subsequent rendering operations will target the default framebuffer
             * (usually the screen or swap chain).
             */
            virtual void unbind() = 0;

            virtual void setClearColor(const glm::vec4 &color) = 0;

            /**
             * @brief Retrieves the unique OpenGL ID of the framebuffer.
             *
             * The framebuffer ID is used internally by OpenGL to identify the framebuffer object.
             *
             * @return The OpenGL ID of the framebuffer.
             */
            [[nodiscard]] virtual unsigned int getFramebufferId() const = 0;

            /**
             * @brief Resizes the framebuffer to new dimensions.
             *
             * Resizing the framebuffer adjusts the dimensions of all associated texture attachments.
             * This is commonly used when the viewport size changes.
             *
             * @param width The new width of the framebuffer in pixels.
             * @param height The new height of the framebuffer in pixels.
             */
            virtual void resize(unsigned int width, unsigned int height) = 0;

            virtual const glm::vec2 getSize() const = 0;

            virtual void getPixelWrapper(unsigned int attachementIndex, int x, int y, void *result, const std::type_info &ti) const = 0;


            /**
             * @brief Retrieves the pixel data from a specified attachment.
             *
             * Template version of getPixelWrapper.
             *
             * @tparam T The expected type of the pixel data.
             * @param attachmentIndex The index of the attachment.
             * @param x X-coordinate of the pixel.
             * @param y Y-coordinate of the pixel.
             * @return T The pixel data.
             */
            template<typename T>
            T getPixel(unsigned int attachmentIndex, int x, int y) const
            {
                 T result;
                 getPixelWrapper(attachmentIndex, x, y, &result, typeid(T));
                 return result;
            }

            virtual void clearAttachmentWrapper(unsigned int attachmentIndex, const void *value, const std::type_info &ti) const = 0;


            /**
             * @brief Clears a specified attachment to a given value.
             *
             * Template version of clearAttachmentWrapper.
             *
             * @tparam T The type of the clear value.
             * @param attachmentIndex The index of the attachment.
             * @param value The value to clear the attachment to.
             */
            template<typename T>
            void clearAttachment(unsigned int attachmentIndex, T value) const
            {
                 clearAttachmentWrapper(attachmentIndex, &value, typeid(T));
            }

            /**
             * @brief Retrieves the specifications of the framebuffer.
             *
             * This method provides access to the framebuffer's specifications, including
             * dimensions, attachments, and sampling options.
             *
             * @return A reference to the NxFramebufferSpecs struct.
             */
            [[nodiscard]] virtual NxFramebufferSpecs &getSpecs() = 0;

            /**
            * @brief Retrieves the specifications of the framebuffer (const version).
            *
            * This method provides read-only access to the framebuffer's specifications.
            *
            * @return A constant reference to the NxFramebufferSpecs struct.
            */
            [[nodiscard]] virtual const NxFramebufferSpecs &getSpecs() const = 0;
            /**
             * @brief Retrieves the OpenGL ID of a specific color attachment.
             *
             * Color attachments are textures that store the rendered color output. This method
             * retrieves the OpenGL ID of the texture for the specified attachment index.
             *
             * @param index The index of the color attachment (default is 0).
             * @return The OpenGL ID of the texture attachment.
             *
             * Notes:
             * - If the framebuffer has multiple color attachments, specify the index accordingly.
             * - An invalid index may result in undefined behavior.
             */
            [[nodiscard]] virtual unsigned int getColorAttachmentId(unsigned int index = 0) const = 0;

            virtual unsigned int getDepthAttachmentId() const = 0;

            /**
             * @brief Creates a framebuffer based on the provided specifications.
             *
             * This static method abstracts the creation of a framebuffer, delegating the actual
             * implementation to the active graphics API (e.g., OpenGL).
             *
             * @param specs The specifications for creating the framebuffer, including dimensions,
             *              attachments, and sampling options.
             * @return A shared pointer to the created NxFramebuffer instance.
             *
             * Throws:
             * - Implementation-specific exceptions if framebuffer creation fails.
             *
             * Notes:
             * - This function is typically implemented in a platform-specific or API-specific source file.
             */
            static std::shared_ptr<NxFramebuffer> create(const NxFramebufferSpecs& specs);
    };


}
