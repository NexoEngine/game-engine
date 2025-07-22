//// MaskPass.cpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Source file for the mask render pass
//
///////////////////////////////////////////////////////////////////////////////

#include "MaskPass.hpp"
#include "DrawCommand.hpp"
#include "Framebuffer.hpp"
#include "renderer/RenderPipeline.hpp"
#include "renderer/RenderCommand.hpp"
#include "renderer/Renderer3D.hpp"
#include "Masks.hpp"
#include "Passes.hpp"

namespace nexo::renderer {
    MaskPass::MaskPass(unsigned int width, unsigned int height) : RenderPass(Passes::MASK, "Mask pass")
    {
        renderer::NxFramebufferSpecs maskFramebufferSpecs;
        maskFramebufferSpecs.attachments = { renderer::NxFrameBufferTextureFormats::RGBA8, renderer::NxFrameBufferTextureFormats::DEPTH24STENCIL8 };
        maskFramebufferSpecs.width = width;  // Default size, will be resized as needed
        maskFramebufferSpecs.height = height;
        m_output = renderer::NxFramebuffer::create(maskFramebufferSpecs);
    }

    void MaskPass::execute(RenderPipeline& pipeline)
    {
        auto output = (m_isFinal) ? pipeline.getFinalRenderTarget() : m_output;
        output->bind();
        renderer::NxRenderCommand::setClearColor({0.0f, 0.0f, 0.0f, 0.0f});
        renderer::NxRenderCommand::clear();

        //IMPORTANT: Bind textures after binding the framebuffer, since binding can trigger a resize and invalidate the
        // current texture slots
        renderer::NxRenderer3D::get().bindTextures();
        const std::vector<DrawCommand> &drawCommands = pipeline.getDrawCommands();
        for (const auto &cmd : drawCommands) {
            if (cmd.filterMask & F_OUTLINE_MASK)
                cmd.execute();
        }
        output->unbind();
        pipeline.setOutput(id, output);
    }

    void MaskPass::resize(unsigned int width, unsigned int height)
    {
        if (!m_output)
            return;
        m_output->resize(width, height);
    }
}
