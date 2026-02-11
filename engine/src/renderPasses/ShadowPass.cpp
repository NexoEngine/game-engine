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
        specs.attachments = NxFrameBufferAttachmentsSpecifications({
            NxFrameBufferTextureSpecifications(NxFrameBufferTextureFormats::DEPTH24STENCIL8)  // depth-only
        });
        specs.width  = size;
        specs.height = size;

        m_shadowMap = NxFramebuffer::create(specs);
    }

    void ShadowPass::execute(RenderPipeline &pipeline)
    {
        const auto &globalUniforms = pipeline.getGlobalUniforms();

        // Skip entirely if there is no directional light
        if (globalUniforms.find("uDirLightViewProj") == globalUniforms.end()) return;

        auto shader = ShaderLibrary::getInstance().get("Shadow depth");
        if (!shader || !m_shadowMap) return;

        m_shadowMap->bind();
        glViewport(0, 0, m_size, m_size);
        NxRenderCommand::clear();

        // Save cull face state before overriding for shadow rendering
        GLint prevCullFaceMode;
        glGetIntegerv(GL_CULL_FACE_MODE, &prevCullFaceMode);
        GLboolean wasCullingEnabled = glIsEnabled(GL_CULL_FACE);

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

            auto offsetIt = cmd.uniforms.find("uInstanceOffset");
            if (offsetIt != cmd.uniforms.end()) {
                offsetIt->second.applyToShader(shader.get(), "uInstanceOffset");
            }

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

        // Restore previous cull face state
        glCullFace(prevCullFaceMode);
        if (!wasCullingEnabled)
            glDisable(GL_CULL_FACE);

        m_shadowMap->unbind();
    }

    void ShadowPass::resize(unsigned int, unsigned int)
    {
        // Shadow map resolution is fixed at construction time, independent of screen size.
    }

} // namespace nexo::renderer
