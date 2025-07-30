//// OutlinePass.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Source file for the outline render pass
//
///////////////////////////////////////////////////////////////////////////////

#include "OutlinePass.hpp"
#include "DrawCommand.hpp"
#include "Framebuffer.hpp"
#include "RenderCommand.hpp"
#include "renderPasses/MaskPass.hpp"
#include "renderer/RenderPipeline.hpp"
#include "renderer/Renderer3D.hpp"
#include "Masks.hpp"
#include "Passes.hpp"

#include <glad/glad.h>

namespace nexo::renderer {
    OutlinePass::OutlinePass() : RenderPass(Passes::OUTLINE, "Outline pass")
    {

    }

    void OutlinePass::execute(RenderPipeline& pipeline)
    {
        const std::shared_ptr<renderer::NxFramebuffer> renderTarget = pipeline.getRenderTarget();
        std::shared_ptr<NxFramebuffer> maskPass = nullptr;
        for (const auto &prereq : prerequisites) {
            const auto &prereqPass = pipeline.getRenderPass(prereq);
            if (prereqPass->getId() == Passes::MASK) {
                maskPass = prereqPass->getOutput();
            }
        }
        if (!renderTarget || !maskPass)
            return;
        renderTarget->bind();
        constexpr GLenum singleDrawBuffer[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, singleDrawBuffer);

        renderer::NxRenderCommand::setDepthTest(false);
        renderer::NxRenderCommand::setDepthMask(false);
        maskPass->bindAsTexture();
        renderTarget->bindDepthAsTexture(1);
        maskPass->bindDepthAsTexture(2);
        const auto &drawCommands = pipeline.getDrawCommands();
        for (const auto &cmd : drawCommands) {
            if (cmd.filterMask & F_OUTLINE_PASS) {
                cmd.execute();
            }
        }
        constexpr GLenum allBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, allBuffers);
        renderTarget->unbind();
        renderer::NxRenderCommand::setDepthMask(true);
        renderer::NxRenderCommand::setDepthTest(true);
    }
}
