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

namespace nexo::renderer {

    struct FramebufferSpecs {
        unsigned int width;
        unsigned int height;

        unsigned int samples = 1;

        bool swapChainTarget = false;
    };

    class Framebuffer {
        public:
            virtual ~Framebuffer() = default;

            virtual void bind() = 0;
            virtual void unbind() = 0;

            virtual unsigned int getFramebufferId() const = 0;

            virtual void resize(unsigned int width, unsigned int height) = 0;

            [[nodiscard]] virtual FramebufferSpecs &getSpecs() = 0;
            [[nodiscard]] virtual const FramebufferSpecs &getSpecs() const = 0;

            [[nodiscard]] virtual unsigned int getColorAttachmentId() const = 0;

            static std::shared_ptr<Framebuffer> create(const FramebufferSpecs& specs);
    };


}
