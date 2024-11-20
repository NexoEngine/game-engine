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

namespace nexo::renderer {

    enum class FrameBufferTextureFormats {
        NONE = 0,

        RGBA8,

        DEPTH24STENCIL8,

        Depth = DEPTH24STENCIL8
    };

    struct FrameBufferTextureSpecifications {
        FrameBufferTextureSpecifications() = default;
        FrameBufferTextureSpecifications(const FrameBufferTextureFormats format) : textureFormat(format) {};

        FrameBufferTextureFormats textureFormat = FrameBufferTextureFormats::NONE;
    };

    struct FrameBufferAttachmentsSpecifications {
        FrameBufferAttachmentsSpecifications() = default;
        FrameBufferAttachmentsSpecifications(std::initializer_list<FrameBufferTextureSpecifications> attachments) : attachments(attachments) {};

        std::vector<FrameBufferTextureSpecifications> attachments;
    };

    struct FramebufferSpecs {
        unsigned int width{};
        unsigned int height{};
        FrameBufferAttachmentsSpecifications attachments;

        unsigned int samples = 1;

        bool swapChainTarget = false;
    };

    class Framebuffer {
        public:
            virtual ~Framebuffer() = default;

            virtual void bind() = 0;
            virtual void unbind() = 0;

            [[nodiscard]] virtual unsigned int getFramebufferId() const = 0;

            virtual void resize(unsigned int width, unsigned int height) = 0;

            [[nodiscard]] virtual FramebufferSpecs &getSpecs() = 0;
            [[nodiscard]] virtual const FramebufferSpecs &getSpecs() const = 0;

            [[nodiscard]] virtual unsigned int getColorAttachmentId(unsigned int index = 0) const = 0;

            static std::shared_ptr<Framebuffer> create(const FramebufferSpecs& specs);
    };


}
