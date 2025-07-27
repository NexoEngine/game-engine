//// ForwardPass.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Source file for the forward render pass
//
///////////////////////////////////////////////////////////////////////////////

#include "ForwardPass.hpp"
#include "DrawCommand.hpp"
#include "Framebuffer.hpp"
#include "RenderCommand.hpp"
#include "renderPasses/Masks.hpp"
#include "renderer/RenderPipeline.hpp"
#include "renderer/Renderer3D.hpp"
#include "Passes.hpp"

#include <glad/glad.h>

namespace nexo::renderer {
    ForwardPass::ForwardPass() : RenderPass(Passes::FORWARD, "Forward Pass")
    {

    }

    void ForwardPass::execute(RenderPipeline& pipeline)
    {
        const std::shared_ptr<renderer::NxFramebuffer> renderTarget = pipeline.getRenderTarget();
        renderTarget->bind();
       	NxRenderCommand::setClearColor(pipeline.getCameraClearColor());
       	NxRenderCommand::clear();
        renderTarget->clearAttachment<int>(1, -1);
        NxRenderer3D::get().bindTextures();
        const std::vector<DrawCommand> &drawCommands = pipeline.getDrawCommands();
        for (const auto &cmd : drawCommands) {
            if (cmd.filterMask & F_FORWARD_PASS)
                cmd.execute();
        }
        renderTarget->unbind();
    }
}
