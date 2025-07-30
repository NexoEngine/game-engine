//// GridPass.cpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Source file for the grid render pass
//
///////////////////////////////////////////////////////////////////////////////

#include "GridPass.hpp"
#include "DrawCommand.hpp"
#include "Framebuffer.hpp"
#include "RenderCommand.hpp"
#include "renderer/RenderPipeline.hpp"
#include "Masks.hpp"
#include "Passes.hpp"

#include <glad/glad.h>

namespace nexo::renderer {
    GridPass::GridPass() : RenderPass(Passes::GRID, "Grid pass")
    {

    }

    void GridPass::execute(RenderPipeline& pipeline)
    {
        std::shared_ptr<renderer::NxFramebuffer> renderTarget = pipeline.getRenderTarget();
        if (!renderTarget)
            return;

        renderTarget->bind();
        constexpr GLenum singleDrawBuffer[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, singleDrawBuffer);
        renderer::NxRenderCommand::setDepthMask(false);
        renderer::NxRenderCommand::setCulling(false);
        const auto &drawCommands = pipeline.getDrawCommands();
        for (const auto &cmd : drawCommands) {
            if (cmd.filterMask & F_GRID_PASS) {
                cmd.execute();
            }
        }
        renderer::NxRenderCommand::setDepthMask(true);
        renderer::NxRenderCommand::setCulling(true);
        renderer::NxRenderCommand::setCulledFace(CulledFace::BACK);

        constexpr GLenum allBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, allBuffers);
        renderTarget->unbind();
    }
}
