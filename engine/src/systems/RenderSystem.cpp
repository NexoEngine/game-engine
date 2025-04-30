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
#include "math/Vector.hpp"
#include "renderer/RenderCommand.hpp"
#include "ecs/Coordinator.hpp"
#include "core/exceptions/Exceptions.hpp"
#include "Application.hpp"

#include <glm/gtc/type_ptr.hpp>


namespace nexo::system {

    RenderSystem::RenderSystem()
    {
        renderer::FramebufferSpecs maskFramebufferSpecs;
        maskFramebufferSpecs.attachments = { renderer::FrameBufferTextureFormats::RGBA8 };
        maskFramebufferSpecs.width = 1280;  // Default size, will be resized as needed
        maskFramebufferSpecs.height = 720;
        m_maskFramebuffer = renderer::Framebuffer::create(maskFramebufferSpecs);

        // Create fullscreen quad for post-processing
        m_fullscreenQuad = renderer::createVertexArray();

        // Define fullscreen quad vertices (position and texture coordinates)
        float quadVertices[] = {
            // positions        // texture coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f
        };

        auto quadVB = renderer::createVertexBuffer(sizeof(quadVertices));
        quadVB->setData(quadVertices, sizeof(quadVertices));

        renderer::BufferLayout quadLayout = {
            {renderer::ShaderDataType::FLOAT3, "aPosition"},
            {renderer::ShaderDataType::FLOAT2, "aTexCoord"}
        };

        quadVB->setLayout(quadLayout);
        m_fullscreenQuad->addVertexBuffer(quadVB);
    }

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
        if (!camera.renderTarget)
            return;

        renderContext.renderer3D.beginScene(camera.viewProjectionMatrix, camera.cameraPosition, "Grid shader");
        auto gridShader = renderContext.renderer3D.getShader();
        gridShader->bind();

        // Grid appearance
        const components::RenderContext::GridParams &gridParams = renderContext.gridParams;
        gridShader->setUniformFloat("uGridSize", gridParams.gridSize);
        gridShader->setUniformFloat("uGridCellSize", gridParams.cellSize);
        gridShader->setUniformFloat("uGridMinPixelsBetweenCells", gridParams.minPixelsBetweenCells);

        gridShader->setUniformFloat4("uGridColorThin", {0.5f, 0.55f, 0.7f, 0.6f});
        gridShader->setUniformFloat4("uGridColorThick", {0.7f, 0.75f, 0.9f, 0.8f});

        const glm::vec2 globalMousePos = event::getMousePosition();
        glm::vec3 mouseWorldPos = camera.cameraPosition;  // Default position (camera position)
        const glm::vec2 renderTargetSize = camera.renderTarget->getSize();

        if (renderContext.isChildWindow) {
            // viewportBounds[0] is min (top-left), viewportBounds[1] is max (bottom-right)
            const glm::vec2& viewportMin = renderContext.viewportBounds[0];
            const glm::vec2& viewportMax = renderContext.viewportBounds[1];
            const glm::vec2 viewportSize(viewportMax.x - viewportMin.x, viewportMax.y - viewportMin.y);

            // Check if mouse is within the viewport bounds
            if (math::isPosInBounds(globalMousePos, viewportMin, viewportMax)) {

                // Calculate relative mouse position within the viewport
                glm::vec2 relativeMousePos(
                    globalMousePos.x - viewportMin.x,
                    globalMousePos.y - viewportMin.y
                );

                // Convert to normalized coordinates [0,1]
                glm::vec2 normalizedPos(
                    relativeMousePos.x / viewportSize.x,
                    relativeMousePos.y / viewportSize.y
                );

                // Convert to framebuffer coordinates
                glm::vec2 framebufferPos(
                    normalizedPos.x * renderTargetSize.x,
                    normalizedPos.y * renderTargetSize.y
                );

                // Project ray
                const glm::vec3 rayDir = math::projectRayToWorld(
                    framebufferPos.x, framebufferPos.y,
                    camera.viewProjectionMatrix, camera.cameraPosition,
                    renderTargetSize.x, renderTargetSize.y
                );

                // Calculate intersection with y=0 plane (grid plane)
                if (rayDir.y != 0.0f) {
                    float t = -camera.cameraPosition.y / rayDir.y;
                    if (t > 0.0f) {
                        mouseWorldPos = camera.cameraPosition + rayDir * t;
                    }
                }
            }
        } else {
            const glm::vec3 rayDir = math::projectRayToWorld(
                globalMousePos.x, globalMousePos.y,
                camera.viewProjectionMatrix, camera.cameraPosition,
                renderTargetSize.x, renderTargetSize.y
            );

            if (rayDir.y != 0.0f) {
                float t = -camera.cameraPosition.y / rayDir.y;
                if (t > 0.0f) {
                    mouseWorldPos = camera.cameraPosition + rayDir * t;
                }
            }
        }

        // Set uniforms for grid highlighting
        gridShader->setUniformFloat3("uMouseWorldPos", mouseWorldPos);
        gridShader->setUniformFloat("uTime", static_cast<float>(glfwGetTime()));

        // Render the grid
        renderer::RenderCommand::setDepthMask(false);
        glDisable(GL_CULL_FACE);
        renderer::RenderCommand::drawUnIndexed(6);
        gridShader->unbind();
        renderer::RenderCommand::setDepthMask(true);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    void RenderSystem::renderOutline(
        components::RenderContext &renderContext,
        const components::CameraContext &camera,
        const components::RenderComponent &renderComponent,
        const components::TransformComponent &transformComponent
    ) {
        if (m_maskFramebuffer->getSize().x != camera.renderTarget->getSize().x ||
            m_maskFramebuffer->getSize().y != camera.renderTarget->getSize().y) {
                m_maskFramebuffer->resize(
                    static_cast<unsigned int>(camera.renderTarget->getSize().x),
                    static_cast<unsigned int>(camera.renderTarget->getSize().y)
                );
        }

        // Step 2: Render selected object to mask texture
        m_maskFramebuffer->bind();
        renderer::RenderCommand::setClearColor({0.0f, 0.0f, 0.0f, 0.0f});
        renderer::RenderCommand::clear();
        const auto &material = std::dynamic_pointer_cast<components::Renderable3D>(renderComponent.renderable)->material;


        std::string maskShaderName = "Flat color";
        if (!material.isOpaque)
            maskShaderName = "Albedo unshaded transparent";

        renderContext.renderer3D.beginScene(camera.viewProjectionMatrix, camera.cameraPosition, maskShaderName);
        auto context = std::make_shared<renderer::RendererContext>();
        context->renderer3D = renderContext.renderer3D;
        renderComponent.draw(context, transformComponent);
        renderContext.renderer3D.endScene();


        m_maskFramebuffer->unbind();
        if (camera.renderTarget != nullptr)
            camera.renderTarget->bind();

        // Step 3: Draw full-screen quad with outline post-process shader
        renderer::RenderCommand::setDepthMask(false);
        renderContext.renderer3D.beginScene(camera.viewProjectionMatrix, camera.cameraPosition, "Outline pulse flat");
        auto outlineShader = renderContext.renderer3D.getShader();
        outlineShader->bind();
        unsigned int maskTexture = m_maskFramebuffer->getColorAttachmentId(0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, maskTexture);
        outlineShader->setUniformInt("uMaskTexture", 0);
        outlineShader->setUniformFloat("uTime", static_cast<float>(glfwGetTime()));
        outlineShader->setUniformFloat2("uScreenSize", {camera.renderTarget->getSize().x, camera.renderTarget->getSize().y});
        outlineShader->setUniformFloat("uOutlineWidth", 10.0f);

        m_fullscreenQuad->bind();
        renderer::RenderCommand::drawUnIndexed(6);
        m_fullscreenQuad->unbind();
        renderContext.renderer3D.endScene();

        outlineShader->unbind();
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

		while (!renderContext.cameras.empty()) {
			const components::CameraContext &camera = renderContext.cameras.front();
			if (camera.renderTarget != nullptr) {
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

            if (sceneType == SceneType::EDITOR && renderContext.gridParams.enabled)
                renderGrid(camera, renderContext);

            for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
                const ecs::Entity entity = entitySpan[i];
                if (coord->entityHasComponent<components::CameraComponent>(entity) && sceneType != SceneType::EDITOR)
                    continue;
				const auto &transform = transformSpan[i];
				const auto &render = renderSpan[i];
				// This needs to be changed, i guess we should go toward a static mesh/material components, way better
				const auto &material = std::dynamic_pointer_cast<components::Renderable3D>(render.renderable)->material;
				if (render.isRendered)
				{
					renderContext.renderer3D.beginScene(camera.viewProjectionMatrix, camera.cameraPosition, material.shader);
					auto shader = renderContext.renderer3D.getShader();
					setupLights(shader, renderContext.sceneLights);
					auto context = std::make_shared<renderer::RendererContext>();
					context->renderer3D = renderContext.renderer3D;
					render.draw(context, transform, static_cast<int>(entity));
					renderContext.renderer3D.endScene();
					if (coord->entityHasComponent<components::SelectedTag>(entity))
					   renderOutline(renderContext, camera, render, transform);
               }
            }

			if (camera.renderTarget != nullptr)
				camera.renderTarget->unbind();
			renderContext.cameras.pop();
		}
		// We have to do this for now to reset the shader stored as a static here, this will change later
		setupLights(nullptr, renderContext.sceneLights);
	}
}
