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
#include "components/Editor.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"
#include "components/Render.hpp"
#include "core/event/Input.hpp"
#include "math/Projection.hpp"
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
    void RenderSystem::setupLights(const std::shared_ptr<renderer::Shader>& shader, const components::LightContext& lightContext)
    {
        static std::shared_ptr<renderer::Shader> lastShader = nullptr;
        if (lastShader == shader)
            return;
        lastShader = shader;
        if (!lastShader)
            return;
        shader->setUniformFloat3("uAmbientLight", lightContext.ambientLight);
        shader->setUniformInt("uNumPointLights", lightContext.pointLightCount);
        shader->setUniformInt("uNumSpotLights", lightContext.spotLightCount);

        const auto &directionalLight = lightContext.dirLight;
        shader->setUniformFloat3("uDirLight.direction", directionalLight.direction);
        shader->setUniformFloat4("uDirLight.color", glm::vec4(directionalLight.color, 1.0f));

        // Well we are doing something very stupid here, but any way this render system is fucked
        // In the future, we should have a material/light pre-pass that sets all uniforms of the the material
        // But for now the material is embedded into the renderable which is also scuffed
        const auto &pointLightComponentArray = coord->getComponentArray<components::PointLightComponent>();
        const auto &transformComponentArray = coord->getComponentArray<components::TransformComponent>();
        for (unsigned int i = 0; i < lightContext.pointLightCount; ++i)
        {
            const auto &pointLight = pointLightComponentArray->get(lightContext.pointLights[i]);
            const auto &transform = transformComponentArray->get(lightContext.pointLights[i]);
            shader->setUniformFloat3(std::format("uPointLights[{}].position", i), transform.pos);
            shader->setUniformFloat4(std::format("uPointLights[{}].color", i), glm::vec4(pointLight.color, 1.0f));
            shader->setUniformFloat(std::format("uPointLights[{}].constant", i), pointLight.constant);
            shader->setUniformFloat(std::format("uPointLights[{}].linear", i), pointLight.linear);
            shader->setUniformFloat(std::format("uPointLights[{}].quadratic", i), pointLight.quadratic);
        }

        const auto &spotLightComponentArray = coord->getComponentArray<components::SpotLightComponent>();
        for (unsigned int i = 0; i < lightContext.spotLightCount; ++i)
        {
            const auto &spotLight = spotLightComponentArray->get(lightContext.spotLights[i]);
            const auto &transform = transformComponentArray->get(lightContext.spotLights[i]);
            shader->setUniformFloat3(std::format("uSpotLights[{}].position", i), transform.pos);
            shader->setUniformFloat4(std::format("uSpotLights[{}].color", i), glm::vec4(spotLight.color, 1.0f));
            shader->setUniformFloat(std::format("uSpotLights[{}].constant", i), spotLight.constant);
            shader->setUniformFloat(std::format("uSpotLights[{}].linear", i), spotLight.linear);
            shader->setUniformFloat(std::format("uSpotLights[{}].quadratic", i), spotLight.quadratic);
            shader->setUniformFloat3(std::format("uSpotLights[{}].direction", i), spotLight.direction);
            shader->setUniformFloat(std::format("uSpotLights[{}].cutOff", i), spotLight.cutOff);
            shader->setUniformFloat(std::format("uSpotLights[{}].outerCutoff", i), spotLight.outerCutoff);
        }
    }

    void RenderSystem::renderGrid(const components::CameraContext &camera, components::RenderContext &renderContext)
    {
        //TODO: Implement a way to do this without relying on camera render target when none is bound
        if (!camera.renderTarget)
            return;
        renderContext.renderer3D.beginScene(camera.viewProjectionMatrix, camera.cameraPosition, "Grid shader");
        auto gridShader = renderContext.renderer3D.getShader();
        gridShader->bind();

        // Grid appearance
        const components::RenderContext::GridParams &gridParams = renderContext.gridParams;
        gridShader->setUniformFloat("uGridSize", gridParams.gridSize);  // Size of grid from center to edge
        gridShader->setUniformFloat("uGridCellSize", gridParams.cellSize); // Base size of each cell
        gridShader->setUniformFloat("uGridMinPixelsBetweenCells", gridParams.minPixelsBetweenCells); // For LOD calculation

        gridShader->setUniformFloat4("uGridColorThin", {0.5f, 0.55f, 0.7f, 0.6f});   // Soft blue-purple
        gridShader->setUniformFloat4("uGridColorThick", {0.7f, 0.75f, 0.9f, 0.8f});  // Lighter blue-purple
        const glm::vec2 &mousePos = event::getMousePosition();
        const glm::vec2 &screenSize = camera.renderTarget->getSize();
        const glm::vec3 &rayDir = math::projectRayToWorld(mousePos.x, mousePos.y, camera.viewProjectionMatrix, camera.cameraPosition, screenSize.x, screenSize.y);

        glm::vec3 mouseWorldPos = camera.cameraPosition;
        if (rayDir.y != 0.0f) {
            float t = -camera.cameraPosition.y / rayDir.y;
            if (t > 0.0f) {
                mouseWorldPos = camera.cameraPosition + rayDir * t;
            }
        }
        // For glowing effect
        gridShader->setUniformFloat3("uMouseWorldPos", mouseWorldPos);
        gridShader->setUniformFloat("uTime", static_cast<float>(glfwGetTime()));

        renderer::RenderCommand::setDepthMask(false);
        renderer::RenderCommand::drawUnIndexed(6);
        gridShader->unbind();
        renderer::RenderCommand::setDepthMask(true);
    }

	void RenderSystem::update()
	{
		auto &renderContext = getSingleton<components::RenderContext>();
		if (renderContext.sceneRendered == -1)
			return;

		const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);
		const SceneType sceneType = renderContext.sceneType;

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
                const ecs::Entity entity = entitySpan[i];
                if (coord->entityHasComponent<components::CameraComponent>(entity) && sceneType != SceneType::EDITOR)
                    continue;
				const auto &transform = transformSpan[i];
				const auto &render = renderSpan[i];
				// This needs to be changed, i guess we should go toward a static mesh/material components, way better
				const auto &material = std::dynamic_pointer_cast<components::Renderable3D>(render.renderable)->material;
				if (render.isRendered)
				{
				    bool isSelected = coord->entityHasComponent<components::SelectedTag>(entity);
				    if (isSelected)
					{
    					renderer::RenderCommand::setStencilFunc(GL_ALWAYS, 1, 0xFF);
                        renderer::RenderCommand::setStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
                        renderer::RenderCommand::setStencilMask(0xFF);
					}
					renderContext.renderer3D.beginScene(camera.viewProjectionMatrix, camera.cameraPosition, material.shader);
					auto shader = renderContext.renderer3D.getShader();
					setupLights(shader, renderContext.sceneLights);
					auto context = std::make_shared<renderer::RendererContext>();
					context->renderer3D = renderContext.renderer3D;
					render.draw(context, transform, static_cast<int>(entity));
					renderContext.renderer3D.endScene();
					if (isSelected)
					{
                        renderer::RenderCommand::setStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                        renderer::RenderCommand::setStencilMask(0x00);
                        renderer::RenderCommand::setDepthFunc(GL_LEQUAL);
                        float scaleFactor = 1.015f;
                        if (material.isOpaque)
                            renderContext.renderer3D.beginScene(camera.viewProjectionMatrix, camera.cameraPosition, "Outline pulse flat");
                        else {
                            renderContext.renderer3D.beginScene(camera.viewProjectionMatrix, camera.cameraPosition, "Outline pulse transparent flat");
                            scaleFactor = 1.1f; //Sligthly larger for transparency
                        }
                        auto outlineShader = renderContext.renderer3D.getShader();
                        outlineShader->setUniformFloat("uTime", static_cast<float>(glfwGetTime()));
                        render.draw(context, {transform.pos, transform.size * scaleFactor, transform.quat});
                        renderContext.renderer3D.endScene();
                        renderer::RenderCommand::setDepthFunc(GL_LESS);
                        renderer::RenderCommand::setStencilFunc(GL_ALWAYS, 0, 0xFF);
                        renderer::RenderCommand::setStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
                        renderer::RenderCommand::setStencilMask(0xFF);
					}
               }
            }

            if (sceneType == SceneType::EDITOR && renderContext.gridParams.enabled)
            {
                renderGrid(camera, renderContext);
            }

			if (camera.renderTarget != nullptr)
			{
				camera.renderTarget->unbind();

			}
			renderContext.cameras.pop();
		}
		// We have to do this for now to reset the shader stored as a static here, this will change later
		setupLights(nullptr, renderContext.sceneLights);
	}
}
