// ShadowPass.cpp
#include "ShadowPass.hpp"
#include "DrawCommand.hpp"
#include "Passes.hpp"
#include "Masks.hpp"
#include "renderer/RenderCommand.hpp"
#include "renderer/RenderPipeline.hpp"
#include "renderer/ShaderLibrary.hpp"

#include <glad/glad.h>

namespace nexo::renderer {

    ShadowPass::ShadowPass(const unsigned int size)
        : RenderPass(Passes::SHADOW, "Shadow pass"), m_size(size)
    {
        NxFramebufferSpecs specs;
        specs.attachments = {
            NxFrameBufferTextureFormats::DEPTH24STENCIL8  // depth-only
        };
        specs.width  = size;
        specs.height = size;

        m_shadowMap = NxFramebuffer::create(specs);
    }

    void ShadowPass::execute(RenderPipeline &pipeline)
    {
        const auto &globalUniforms = pipeline.getGlobalUniforms();
        auto shader = ShaderLibrary::getInstance().get("Shadow depth");
        if (!shader || !m_shadowMap) return;

        m_shadowMap->bind();
        glViewport(0, 0, m_size, m_size);
        NxRenderCommand::clear(); // or clear() if your FBO only has depth
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        shader->bind();

        // Set uLightViewProjection from global uniforms
        auto it = globalUniforms.find("uDirLightViewProj");
        if (it != globalUniforms.end()) {
            it->second.applyToShader(shader.get(), "uLightViewProjection");
        }

        auto &drawCommands = pipeline.getDrawCommands();

        // We reuse the same instance SSBO / VAOs, but a single shadow shader
        for (auto &cmd : drawCommands) {
            if (!(cmd.filterMask & F_FORWARD_PASS)) continue; // only real geometry

            // Bind VAO
            if (cmd.vao) cmd.vao->bind();

            cmd.uniforms["uInstanceOffset"].applyToShader(shader.get(), "uInstanceOffset");

            // Draw instanced
            const auto indexBuffer = cmd.vao->getIndexBuffer();
            const int indexCount   = static_cast<int>(indexBuffer->getCount());
            glDrawElementsInstanced(GL_TRIANGLES,
                                    indexCount,
                                    GL_UNSIGNED_INT,
                                    nullptr,
                                    cmd.instanceCount);
        }

        shader->unbind();
        glCullFace(GL_BACK);    // restore
        glDisable(GL_CULL_FACE);

        m_shadowMap->unbind();
    }

    void ShadowPass::resize(unsigned int width, unsigned int height)
    {
        // For directional shadows we usually keep it square and independent of screen size.
        m_size = std::max(width, height);
        if (m_shadowMap) m_shadowMap->resize(m_size, m_size);
    }

} // namespace nexo::renderer
