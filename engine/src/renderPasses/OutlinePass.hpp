//// OutlinePass.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Header file for the outline render pass
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "renderer/RenderPass.hpp"
#include "renderer/Framebuffer.hpp"

namespace nexo::renderer {

    #define OUTLINE_PASS "OutlinePass"

    class OutlinePass : public RenderPass {
        public:
            OutlinePass(unsigned int width, unsigned int height);
            virtual ~OutlinePass() = default;

            void execute(RenderPipeline& pipeline) override;
            void resize(unsigned int width, unsigned int height) override;
        private:
            std::shared_ptr<NxFramebuffer> m_output;
    };
}
