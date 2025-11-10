//// PointLightsSystem.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Source file for the point lights system
//
///////////////////////////////////////////////////////////////////////////////

#include "PointLightsSystem.hpp"
#include "Application.hpp"
#include "Exception.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "core/exceptions/Exceptions.hpp"
#include "SystemProfiler.hpp"
#include "renderPasses/GPUResources.hpp"

namespace nexo::system {
    PointLightContext PointLightsSystem::update()
    {
        const std::span<const ecs::Entity> entitySpan = m_group->entities();
        const auto pointLightsSpan = get<components::PointLightComponent>();
        const auto transformComponentArray = get<components::TransformComponent>();
        PROFILE_SYSTEM("PointLightSystem", entitySpan.size());
        auto &renderContext = getSingleton<components::RenderContext>();
        if (renderContext.sceneRendered == -1) return {};

        const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

        const auto scenePartition = m_group->getPartitionView<components::SceneTag, unsigned int>(
            [](const components::SceneTag &tag) { return tag.id; });

        const auto *partition = scenePartition.getPartition(sceneRendered);

        auto &app                    = Application::getInstance();
        const std::string &sceneName = app.getSceneManager().getScene(sceneRendered).getName();
        if (!partition) {
            LOG_ONCE(NEXO_WARN, "No point light found in scene {}, skipping", sceneName);
            return {};
        }
        nexo::Logger::resetOnce(NEXO_LOG_ONCE_KEY("No point light found in scene {}, skipping", sceneName));

        if (partition->count > MAX_POINT_LIGHTS)
            THROW_EXCEPTION(core::TooManyPointLightsException, sceneRendered, partition->count);

        int shadowMapIndex = 0;

        PointLightContext out{};
        for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
            renderer::GpuPointLight gpuPointLight{};
            const ecs::Entity entity = entitySpan[i];
            const auto &transformComponent = transformComponentArray->get(entity);
            const auto &pointLightComponent = pointLightsSpan[i];

            gpuPointLight.position = transformComponent.pos;
            gpuPointLight.color = glm::vec4(pointLightComponent.color, 1.0f);
            gpuPointLight.constant = pointLightComponent.constant;
            gpuPointLight.linear = pointLightComponent.linear;
            gpuPointLight.quadratic = pointLightComponent.quadratic;

            // Choose a far plane for this light's shadow volume (tune as needed / make per-light later)
            gpuPointLight.farPlane = 25.0f; // or pointLightComponent.shadowFar; etc

            // Enable shadows for all point lights for now
            gpuPointLight.hasShadow      = 1;
            gpuPointLight.shadowMapIndex = shadowMapIndex++;

            out.pointLights[out.nbPointLights++] = gpuPointLight;

            // Safety: don't exceed MAX_POINT_LIGHTS for shadow maps either
            if (shadowMapIndex >= MAX_POINT_LIGHTS) break;
        }
        for (auto &camera : renderContext.cameras) {
            for (unsigned int i = 0; i < out.nbPointLights; ++i) {
                camera.pipeline.addPointLight(out.pointLights[i].position, out.pointLights[i].farPlane);
            }
        }
        return out;
    }
} // namespace nexo::system
