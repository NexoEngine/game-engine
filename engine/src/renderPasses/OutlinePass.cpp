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

#include <glad/glad.h>

namespace nexo::renderer {
    OutlinePass::OutlinePass(unsigned int width, unsigned int height) : RenderPass(OUTLINE_PASS)
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

    void OutlinePass::execute(RenderPipeline& pipeline)
    {
        auto output = (m_isFinal) ? pipeline.getFinalRenderTarget() : m_output;
        std::shared_ptr<NxFramebuffer> maskPass = nullptr;
        std::shared_ptr<NxFramebuffer> prevPass = nullptr;
        for (const auto &prereq : prerequisites) {
            const auto &prereqPass = pipeline.getRenderPass(prereq);
            if (prereqPass->name == MASK_PASS) {
                maskPass = pipeline.getOutput(prereqPass->id);
            } else {
                prevPass = pipeline.getOutput(prereqPass->id);
            }
        }
        if (!prevPass || !maskPass)
            return;
        output->copy(prevPass);
        output->bind();

        renderer::NxRenderCommand::setDepthTest(false);
        renderer::NxRenderCommand::setDepthMask(false);
        maskPass->bindAsTexture();
        output->bindDepthAsTexture(1);
        maskPass->bindDepthAsTexture(2);
        const auto &drawCommands = pipeline.getDrawCommands();
        for (const auto &cmd : drawCommands) {
            if (cmd.filterMask & F_OUTLINE_PASS) {
                cmd.execute();
            }
        }
        output->unbind();
        renderer::NxRenderCommand::setDepthMask(true);
        renderer::NxRenderCommand::setDepthTest(true);
        pipeline.setOutput(id, output);
    }

    void OutlinePass::resize(unsigned int width, unsigned int height)
    {
        if (!m_output)
            return;
        m_output->resize(width, height);
    }
}
