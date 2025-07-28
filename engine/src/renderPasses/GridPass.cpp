//// GridPass.cpp /////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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

namespace nexo::renderer {
    GridPass::GridPass(unsigned int width, unsigned int height) : RenderPass(Passes::GRID, "Grid pass")
    {
        renderer::NxFramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {
            renderer::NxFrameBufferTextureFormats::RGBA8,
            renderer::NxFrameBufferTextureFormats::RED_INTEGER,
            renderer::NxFrameBufferTextureFormats::Depth
        };
        framebufferSpecs.width = width;
        framebufferSpecs.height = height;
        m_output = renderer::NxFramebuffer::create(framebufferSpecs);
    }

    void GridPass::execute(RenderPipeline& pipeline)
    {
        auto output = (m_isFinal) ? pipeline.getFinalRenderTarget() : m_output;
        std::shared_ptr<NxFramebuffer> prevPass = nullptr;
        for (const auto &prereq : prerequisites) {
            const auto &prereqPass = pipeline.getRenderPass(prereq);
            prevPass = pipeline.getOutput(prereqPass->getId());
            break;
        }
        if (!prevPass)
            return;

        output->copy(prevPass);
        output->bind();

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

        output->unbind();
        pipeline.setOutput(id, output);
    }

    void GridPass::resize(unsigned int width, unsigned int height)
    {
        if (!m_output)
            return;
        m_output->resize(width, height);
    }
}
