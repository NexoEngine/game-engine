//// RenderSystem.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/03/2025
//  Description: Source file for the render system
//
///////////////////////////////////////////////////////////////////////////////

#include "RenderSystem.hpp"
#include "RendererContext.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"
#include "components/Render.hpp"
#include "renderer/RenderCommand.hpp"
#include "ecs/Coordinator.hpp"
#include "core/exceptions/Exceptions.hpp"
#include "Application.hpp"

#include <glm/gtc/type_ptr.hpp>


namespace nexo::system {

	/**
     * @brief Sets up the lighting uniforms in the given shader.
     *
     * This static helper function binds the provided shader and sets uniforms for ambient, directional,
     * point, and spot lights based on the current lightContext data. After updating the uniforms, the shader is unbound.
     *
     * @param shader Shared pointer to the shader used for rendering.
     * @param lightContext The light context containing lighting information for the scene.
     *
     * @note The light context must contain valid values for:
     *  - ambientLight
     *  - directionalLights (and directionalLightCount)
     *  - pointLights (and pointLightCount)
     *  - spotLights (and spotLightCount)
     */
	static void setupLights(const std::shared_ptr<renderer::Shader>& shader, const components::LightContext& lightContext)
	{
        shader->bind();
        shader->setUniformFloat3("ambientLight", lightContext.ambientLight);
        shader->setUniformInt("numDirLights", lightContext.directionalLightCount);
        shader->setUniformInt("numPointLights", lightContext.pointLightCount);
        shader->setUniformInt("numSpotLights", lightContext.spotLightCount);

        for (unsigned int i = 0; i < lightContext.directionalLightCount; ++i)
        {
            auto directionalLight = lightContext.directionalLights[i];
            shader->setUniformFloat3(std::format("dirLights[{}].direction", i), directionalLight.direction);
            shader->setUniformFloat4(std::format("dirLights[{}].color", i), glm::vec4(directionalLight.color, 1.0f));
        }

        for (unsigned int i = 0; i < lightContext.pointLightCount; ++i)
        {
            auto pointLight = lightContext.pointLights[i];
            shader->setUniformFloat3(std::format("pointLights[{}].position", i), pointLight.pos);
            shader->setUniformFloat4(std::format("pointLights[{}].color", i), glm::vec4(pointLight.color, 1.0f));
            shader->setUniformFloat(std::format("pointLights[{}].constant", i), pointLight.constant);
            shader->setUniformFloat(std::format("pointLights[{}].linear", i), pointLight.linear);
            shader->setUniformFloat(std::format("pointLights[{}].quadratic", i), pointLight.quadratic);
        }

        for (unsigned int i = 0; i < lightContext.spotLightCount; ++i)
        {
            auto spotLight = lightContext.spotLights[i];
            shader->setUniformFloat3(std::format("spotLights[{}].position", i), spotLight.pos);
            shader->setUniformFloat4(std::format("spotLights[{}].color", i), glm::vec4(spotLight.color, 1.0f));
            shader->setUniformFloat(std::format("spotLights[{}].constant", i), spotLight.constant);
            shader->setUniformFloat(std::format("spotLights[{}].linear", i), spotLight.linear);
            shader->setUniformFloat(std::format("spotLights[{}].quadratic", i), spotLight.quadratic);
            shader->setUniformFloat3(std::format("spotLights[{}].direction", i), spotLight.direction);
            shader->setUniformFloat(std::format("spotLights[{}].cutOff", i), spotLight.cutOff);
            shader->setUniformFloat(std::format("spotLights[{}].outerCutoff", i), spotLight.outerCutoff);
        }
        shader->unbind();
	}

	void RenderSystem::update()
	{
		auto &renderContext = getSingleton<components::RenderContext>();
		if (renderContext.sceneRendered == -1)
			return;

		const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

		setupLights(renderContext.renderer3D.getShader(), renderContext.sceneLights);

		const auto scenePartition = m_group->getPartitionView<components::SceneTag, unsigned int>(
			[](const components::SceneTag& tag) { return tag.id; }
		);

		const auto *partition = scenePartition.getPartition(sceneRendered);

		auto &app = Application::getInstance();
        const std::string &sceneName = app.getSceneManager().getScene(sceneRendered).getName();

		const auto transformSpan = get<components::TransformComponent>();
		const auto renderSpan = get<components::RenderComponent>();
		const std::span<const ecs::Entity> entitySpan = m_group->entities();

		while (!renderContext.cameras.empty())
		{
			const components::CameraContext &camera = renderContext.cameras.front();
			if (camera.renderTarget != nullptr)
			{
				camera.renderTarget->bind();
				//TODO: Find a way to automatically clear color attachments
				renderer::RenderCommand::setClearColor(camera.clearColor);
				renderer::RenderCommand::clear();
				camera.renderTarget->clearAttachment<int>(1, -1);
            }

            if (!partition) {
                LOG_ONCE(NEXO_WARN, "Nothing to render in scene {}, skipping", sceneName);
                camera.renderTarget->unbind();
                renderContext.cameras.pop();
                continue;
            }
            nexo::Logger::resetOnce(NEXO_LOG_ONCE_KEY("Nothing to render in scene {}, skipping", sceneName));

            for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i)
            {
				const auto &transform = transformSpan[i];
				const auto &render = renderSpan[i];
				const ecs::Entity entity = entitySpan[i];
				if (render.isRendered)
				{
					renderContext.renderer3D.beginScene(camera.viewProjectionMatrix, camera.cameraPosition);
					auto context = std::make_shared<renderer::RendererContext>();
					context->renderer3D = renderContext.renderer3D;
					render.draw(context, transform, static_cast<int>(entity));
					renderContext.renderer3D.endScene();
               }
            }

			if (camera.renderTarget != nullptr)
			{
				camera.renderTarget->unbind();

			}
			renderContext.cameras.pop();
		}
	}
}
