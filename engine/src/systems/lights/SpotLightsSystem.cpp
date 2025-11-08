//// SpotLightsSystem.cpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Source file for the spot lights system
//
///////////////////////////////////////////////////////////////////////////////

#include "SpotLightsSystem.hpp"
#include "Application.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "core/exceptions/Exceptions.hpp"
#include "SystemProfiler.hpp"
#include "renderPasses/GPUResources.hpp"

namespace nexo::system {
    SpotLightContext SpotLightsSystem::update()
    {
        const std::span<const ecs::Entity> entitySpan = m_group->entities();
        const auto spotLightsSpan = get<components::SpotLightComponent>();
        const auto transformComponentArray = get<components::TransformComponent>();
        PROFILE_SYSTEM("SpotLightSystem", entitySpan.size());

        auto &renderContext = getSingleton<components::RenderContext>();
        if (renderContext.sceneRendered == -1) return {};

        const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

        const auto scenePartition = m_group->getPartitionView<components::SceneTag, unsigned int>(
            [](const components::SceneTag &tag) { return tag.id; });

        const auto *partition = scenePartition.getPartition(sceneRendered);

        auto &app                    = Application::getInstance();
        const std::string &sceneName = app.getSceneManager().getScene(sceneRendered).getName();
        if (!partition) {
            LOG_ONCE(NEXO_WARN, "No spot light found in scene {}, skipping", sceneName);
            return {};
        }
        nexo::Logger::resetOnce(NEXO_LOG_ONCE_KEY("No spot light found in scene {}, skipping", sceneName));

        if (partition->count > MAX_SPOT_LIGHTS)
            THROW_EXCEPTION(core::TooManySpotLightsException, sceneRendered, partition->count);

        SpotLightContext out{};
        for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
            const ecs::Entity entity = entitySpan[i];
            const auto &transform = transformComponentArray->get(entity);
            const auto &spotLight = spotLightsSpan[i];

            renderer::GpuSpotLight gpuSpotLight{};
            gpuSpotLight.color = glm::vec4(spotLight.color, 1.0f);
            gpuSpotLight.constant = spotLight.constant;
            gpuSpotLight.linear = spotLight.linear;
            gpuSpotLight.quadratic = spotLight.quadratic;
            gpuSpotLight.cutOff = spotLight.cutOff;
            gpuSpotLight.direction = spotLight.direction;
            gpuSpotLight.outerCutoff = spotLight.outerCutoff;
            gpuSpotLight.position = transform.pos;

            out.spotLights[out.nbSpotLights++] = gpuSpotLight;
        }
        return out;
    }
} // namespace nexo::system
