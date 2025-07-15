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
#include "RenderCommand.hpp"
#include "renderPasses/Masks.hpp"
#include "renderer/RenderPipeline.hpp"
#include "renderer/Renderer3D.hpp"
#include "Passes.hpp"

#include <glad/glad.h>

namespace nexo::renderer {
    ForwardPass::ForwardPass(unsigned int width, unsigned int height) : RenderPass(Passes::FORWARD, "Forward Pass")
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

    void ForwardPass::execute(RenderPipeline& pipeline)
    {
        auto output = (m_isFinal) ? pipeline.getFinalRenderTarget() : m_output;
        output->bind();
    	renderer::NxRenderCommand::setClearColor(pipeline.getCameraClearColor());
    	renderer::NxRenderCommand::clear();
        output->clearAttachment(1, -1);

        //IMPORTANT: Bind textures after binding the framebuffer, since binding can trigger a resize and invalidate the
        // current texture slots
        renderer::NxRenderer3D::get().bindTextures();
        const std::vector<DrawCommand> &drawCommands = pipeline.getDrawCommands();
        for (const auto &cmd : drawCommands) {
            if (cmd.filterMask & F_FORWARD_PASS)
                cmd.execute();
        }
        output->unbind();
        pipeline.setOutput(id, output);
    }

    void ForwardPass::resize(unsigned int width, unsigned int height)
    {
        if (!m_output)
            return;
        m_output->resize(width, height);
    }
}
