//// ForwardPass.cpp //////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Source file for the forward render pass
//
///////////////////////////////////////////////////////////////////////////////

#include "ForwardPass.hpp"
#include "DrawCommand.hpp"
#include "Passes.hpp"
#include "RenderCommand.hpp"
#include "renderPasses/Masks.hpp"
#include "renderer/RenderPipeline.hpp"
#include "renderer/Renderer3D.hpp"

namespace nexo::renderer {

    namespace {
        constexpr int DIR_SHADOW_TEX_UNIT      = 31;
        constexpr int POINT_SHADOW_TEX_UNIT_BASE = 32;
        constexpr int MAX_POINT_SHADOW_LIGHTS  = 10;
    }

    ForwardPass::ForwardPass() : RenderPass(Passes::FORWARD, "Forward Pass")
    {}

    void ForwardPass::execute(RenderPipeline &pipeline)
    {
        const auto &globalUniforms = pipeline.getGlobalUniforms();

        const auto renderTarget = pipeline.getRenderTarget();
        if (!renderTarget) return;

        renderTarget->bind();
        NxRenderCommand::setClearColor(pipeline.getCameraClearColor());
        NxRenderCommand::clear();
        renderTarget->clearAttachment<int>(1, -1);
        auto &renderer3D = NxRenderer3D::get();

        auto shadowMapFb = getPrerequisiteOutput(pipeline, Passes::SHADOW);

        if (shadowMapFb) {
            shadowMapFb->bindDepthAsTexture(DIR_SHADOW_TEX_UNIT);
        }

        for (int i = 0; i < static_cast<int>(pipeline.getNbPointLights()) && i < MAX_POINT_SHADOW_LIGHTS; ++i) {
            const auto& sm = pipeline.getShadowMap(i);
            if (!sm.depthCube) continue;

            glActiveTexture(GL_TEXTURE0 + POINT_SHADOW_TEX_UNIT_BASE + i);
            glBindTexture(GL_TEXTURE_CUBE_MAP, sm.depthCube);
        }

        // Disable face culling for double-sided model support
        glDisable(GL_CULL_FACE);

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

        glEnable(GL_CULL_FACE);
        renderTarget->unbind();
    }
} // namespace nexo::renderer
