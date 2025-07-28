//// MaskPass.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Header file for the mask render pass
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "renderer/RenderPass.hpp"
#include "renderer/Framebuffer.hpp"

namespace nexo::renderer {

    class MaskPass : public RenderPass {
        public:
            MaskPass(unsigned int width, unsigned int height);
            ~MaskPass() override = default;

            void execute(RenderPipeline& pipeline) override;
            void resize(unsigned int width, unsigned int height) override;
        private:
            std::shared_ptr<NxFramebuffer> m_output;
    };
}
