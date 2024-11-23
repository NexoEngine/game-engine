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
            explicit OpenGlFramebuffer(FramebufferSpecs specs);
            ~OpenGlFramebuffer() override;

            void invalidate();

            void bind() override;
            void unbind() override;

            [[nodiscard]] unsigned int getFramebufferId() const override;

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