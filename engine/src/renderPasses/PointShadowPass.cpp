#include "PointShadowPass.hpp"
#include "RenderPipeline.hpp"
#include "ShaderLibrary.hpp"
#include "DrawCommand.hpp"
#include "Passes.hpp"
#include "Masks.hpp"
#include "renderPasses/GPUResources.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace nexo::renderer {

    PointShadowPass::PointShadowPass()
        : RenderPass(Passes::POINT_SHADOW, "Point shadow pass")
    {}

    void PointShadowPass::execute(RenderPipeline& pipeline)
    {
        auto shader = ShaderLibrary::getInstance().get("Point shadow depth");
        if (!shader) return;

        auto& drawCommands       = pipeline.getDrawCommands();
        const uint64_t currentGen = pipeline.getGeometryGeneration();

        for (int i = 0; i < static_cast<int>(pipeline.getNbPointLights()) && i < MAX_POINT_LIGHTS; ++i) {
            const auto& light = pipeline.getPointLight(i);
            auto& sm         = pipeline.getShadowMap(i);

            if (!sm.depthCube || !sm.fbo) continue;

            // Skip re-rendering if light and geometry haven't changed
            if (!sm.dirty && sm.cachedGeometryGeneration == currentGen) {
                continue;
            }

            const glm::vec3 lightPos  = light.position;
            const float     nearPlane = 0.1f;
            const float     farPlane  = light.farPlane;

            sm.farPlane = farPlane;

            glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);
            glm::mat4 shadowTransforms[6];
            shadowTransforms[0] = proj * glm::lookAt(lightPos, lightPos + glm::vec3( 1,  0,  0), glm::vec3(0, -1,  0));
            shadowTransforms[1] = proj * glm::lookAt(lightPos, lightPos + glm::vec3(-1,  0,  0), glm::vec3(0, -1,  0));
            shadowTransforms[2] = proj * glm::lookAt(lightPos, lightPos + glm::vec3( 0,  1,  0), glm::vec3(0,  0,  1));
            shadowTransforms[3] = proj * glm::lookAt(lightPos, lightPos + glm::vec3( 0, -1,  0), glm::vec3(0,  0, -1));
            shadowTransforms[4] = proj * glm::lookAt(lightPos, lightPos + glm::vec3( 0,  0,  1), glm::vec3(0, -1,  0));
            shadowTransforms[5] = proj * glm::lookAt(lightPos, lightPos + glm::vec3( 0,  0, -1), glm::vec3(0, -1,  0));

            glViewport(0, 0, RenderPipeline::POINT_SHADOW_SIZE, RenderPipeline::POINT_SHADOW_SIZE);

            shader->bind();
            shader->setUniformFloat3("uLightPos", lightPos);
            shader->setUniformFloat("uFarPlane", farPlane);

            glBindFramebuffer(GL_FRAMEBUFFER, sm.fbo);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);

            for (int face = 0; face < 6; ++face) {
                glFramebufferTexture2D(GL_FRAMEBUFFER,
                                       GL_DEPTH_ATTACHMENT,
                                       GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                                       sm.depthCube,
                                       0);

                glClear(GL_DEPTH_BUFFER_BIT);

                shader->setUniformMatrix("uShadowMatrix", shadowTransforms[face]);

                for (auto& cmd : drawCommands) {
                    if (!(cmd.filterMask & F_FORWARD_PASS)) continue;
                    if (!cmd.vao) continue;

                    cmd.vao->bind();

                    // Forward the instance offset from the forward pass
                    auto it = cmd.uniforms.find("uInstanceOffset");
                    if (it != cmd.uniforms.end()) {
                        it->second.applyToShader(shader.get(), "uInstanceOffset");
                    }

                    auto indexBuffer = cmd.vao->getIndexBuffer();
                    int indexCount   = static_cast<int>(indexBuffer->getCount());
                    glDrawElementsInstanced(GL_TRIANGLES,
                                            indexCount,
                                            GL_UNSIGNED_INT,
                                            nullptr,
                                            cmd.instanceCount);
                }
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            shader->unbind();

            // Mark this shadow map as clean for this geometry state
            sm.dirty = false;
            sm.cachedGeometryGeneration = currentGen;
        }
    }

}
