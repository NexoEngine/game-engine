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
#include "Passes.hpp"
#include "RenderCommand.hpp"
#include "renderPasses/Masks.hpp"
#include "renderer/RenderPipeline.hpp"
#include "renderer/Renderer3D.hpp"

namespace nexo::renderer {
    ForwardPass::ForwardPass() : RenderPass(Passes::FORWARD, "Forward Pass")
    {}

    void ForwardPass::execute(RenderPipeline &pipeline)
    {
        const std::unordered_map<std::string, UniformValue> globalUniforms = pipeline.getGlobalUniforms();

        const std::shared_ptr<renderer::NxFramebuffer> renderTarget = pipeline.getRenderTarget();
        renderTarget->bind();
        NxRenderCommand::setClearColor(pipeline.getCameraClearColor());
        NxRenderCommand::clear();
        renderTarget->clearAttachment<int>(1, -1);
        auto &renderer3D = NxRenderer3D::get();

        std::shared_ptr<NxFramebuffer> shadowMapFb = nullptr;
        for (auto prereq : prerequisites) {
            auto p = pipeline.getRenderPass(prereq);
            if (p && p->getId() == Passes::SHADOW) {
                shadowMapFb = p->getOutput();
            }
        }

        constexpr int DIR_SHADOW_TEX_UNIT = 31;
        if (shadowMapFb) {
            shadowMapFb->bindDepthAsTexture(DIR_SHADOW_TEX_UNIT);
        }

        for (int i = 0; i < pipeline.getNbPointLights() && i < 10; ++i) {
            const auto& sm = pipeline.getShadowMap(i);
            if (!sm.depthCube) continue;

            glActiveTexture(GL_TEXTURE0 + 32 + i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, sm.depthCube);
        }

        std::vector<DrawCommand> &drawCommands = pipeline.getDrawCommands();
        int currentBatch = -1;

        for (auto &cmd : drawCommands) {
            if (cmd.filterMask & F_FORWARD_PASS) {
                if (static_cast<int>(cmd.textureBatchIndex) != currentBatch) {
                    renderer3D.bindTextureBatch(cmd.textureBatchIndex);
                    currentBatch = static_cast<int>(cmd.textureBatchIndex);
                }
                for (const auto &[name, value] : globalUniforms) {
                    cmd.uniforms[name] = value;
                }
                cmd.uniforms["uDirShadowTexIndex"] = UniformValue{DIR_SHADOW_TEX_UNIT};

                cmd.execute();
            }
        }

        renderTarget->unbind();
    }
} // namespace nexo::renderer
