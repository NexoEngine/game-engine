//// AABBDebugSystem.cpp ///////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        05/10/2025
//  Description: Source file for the aabb debug system
//
///////////////////////////////////////////////////////////////////////////////

#include "AABBDebugSystem.hpp"
#include "DrawCommand.hpp"
#include "Logger.hpp"
#include "Renderer3D.hpp"
#include "ShaderLibrary.hpp"
#include "components/Parent.hpp"
#include "math/Bounds.hpp"
#include "renderPasses/Masks.hpp"

namespace nexo::system  {

    static const glm::mat4 modelFromAABB(const math::AABB &box)
    {
        const glm::vec3 center = 0.5f * (box.min + box.max);
        const glm::vec3 extent = 0.5f * (box.max - box.min);
        glm::mat4 M(1.0f);
        M = glm::translate(M, center);
        return glm::scale(M, extent);
    }

    static renderer::DrawCommand createDrawCommand(const std::shared_ptr<renderer::NxShader> &shader,
                                                   const math::AABB &box,
                                                   const components::TransformComponent &transform)
    {
        renderer::DrawCommand cmd;
        cmd.type = renderer::CommandType::LINE;
        cmd.vao                   = renderer::NxRenderer3D::getBoxVAO();
        cmd.shader                = shader;
        cmd.uniforms["uMatModel"] = transform.worldMatrix * modelFromAABB(box);
        cmd.uniforms["uColor"] = glm::vec3(1.0f, 0.0f, 0.0f);

        cmd.filterMask = 0;
        cmd.filterMask |= renderer::F_FORWARD_PASS;
        return cmd;
    }

    void AABBDebugSystem::update()
    {
        auto &renderContext = getSingleton<components::RenderContext>();
        if (renderContext.sceneRendered == -1) return;

        const auto sceneRendered  = static_cast<unsigned int>(renderContext.sceneRendered);

        std::vector<renderer::DrawCommand> drawCommands;
        auto shader = renderer::ShaderLibrary::getInstance().get("AABB Debug");
        if (!shader) {
            LOG(NEXO_ERROR, "Could not load AABB debug shader");
            return;
        }

        for (const ecs::Entity entity : entities) {
            auto &sceneTag = getComponent<components::SceneTag>(entity);
            if (sceneTag.id != sceneRendered) continue;
            auto &rootComponent = getComponent<components::RootComponent>(entity);
            auto &transform        = getComponent<components::TransformComponent>(entity);
            if (auto model = rootComponent.modelRef.lock()) {
                drawCommands.push_back(createDrawCommand(shader, model->rootBounds, transform));
            }
        }

        for (auto &camera : renderContext.cameras) {
            for (auto &cmd : drawCommands) {
                cmd.uniforms["uViewProjection"] = camera.viewProjectionMatrix;
            }
            camera.pipeline.addDrawCommands(drawCommands);
        }
    }
}
