//// DirectionalLights.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Source file for the directional lights system
//
///////////////////////////////////////////////////////////////////////////////

#include "DirectionalLightsSystem.hpp"
#include "Application.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "renderPasses/GPUResources.hpp"

namespace nexo::system {
    static glm::mat4 buildLightViewProjectionMatrix(const components::DirectionalLightComponent &dirLight)
    {
        const glm::vec3 sceneCenter(0.0f);
        const float sceneRadius = 20.0f;

        const glm::vec3 lightDir = glm::normalize(dirLight.direction);
        const glm::vec3 lightPos = sceneCenter - lightDir * sceneRadius * 2.0f;

        glm::mat4 lightView = glm::lookAt(lightPos,
                                          sceneCenter,
                                          glm::vec3(0.0f, 1.0f, 0.0f));

        float orthoSize = sceneRadius * 0.75f;
        glm::mat4 lightProj = glm::ortho(-orthoSize, orthoSize,
                                         -orthoSize, orthoSize,
                                         0.1f, sceneRadius * 4.0f);

        return lightProj * lightView;
    }

    renderer::GpuDirectionalLight DirectionalLightsSystem::update()
    {
        auto &renderContext = getSingleton<components::RenderContext>();
        if (renderContext.sceneRendered == -1) return {};

        const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

        const auto scenePartition = m_group->getPartitionView<components::SceneTag, unsigned int>(
            [](const components::SceneTag &tag) { return tag.id; });

        const auto *partition = scenePartition.getPartition(sceneRendered);

        auto &app                    = Application::getInstance();
        const std::string &sceneName = app.getSceneManager().getScene(sceneRendered).getName();
        if (!partition) {
            LOG_ONCE(NEXO_WARN, "No directional light found in scene {}, skipping", sceneName);
            return {};
        }
        nexo::Logger::resetOnce(NEXO_LOG_ONCE_KEY("No directional light found in scene {}, skipping", sceneName));

        if (partition->count != 1)
            LOG_ONCE(NEXO_WARN,
                     "For scene {}, found {} directional lights, only one is supported, picking the first one",
                     sceneName, partition->count);
        else
            nexo::Logger::resetOnce(NEXO_LOG_ONCE_KEY(
                "For scene {}, found {} directional lights, only one is supported, picking the first one", sceneName,
                partition->count));

        const auto &lights   = get<components::DirectionalLightComponent>(); // now 'lights' names the container
        const auto &dirLight = lights[0];
        renderer::GpuDirectionalLight gpuDirLight{};
        gpuDirLight.color = glm::vec4(dirLight.color, 1.0f);
        gpuDirLight.direction = dirLight.direction;

        const auto lightVp = buildLightViewProjectionMatrix(dirLight);
        for (auto &camera : renderContext.cameras) {
            camera.pipeline.setGlobalUniform("uDirLightViewProj", lightVp);
            camera.pipeline.setGlobalUniform("uDirShadowTexIndex", 31);
        }
        return gpuDirLight;
    }
} // namespace nexo::system
