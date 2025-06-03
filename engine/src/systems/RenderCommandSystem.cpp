//// RenderCommandSystem.cpp //////////////////////////////////////////////////
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

#include "RenderCommandSystem.hpp"
#include "Renderer3D.hpp"
#include "renderer/DrawCommand.hpp"
#include "components/Editor.hpp"
#include "components/Light.hpp"
#include "components/Render3D.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/Camera.hpp"
#include "components/StaticMesh.hpp"
#include "components/Transform.hpp"
#include "core/event/Input.hpp"
#include "math/Projection.hpp"
#include "math/Vector.hpp"
#include "renderPasses/Masks.hpp"
#include "Application.hpp"
#include "renderer/ShaderLibrary.hpp"

#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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
    void RenderCommandSystem::setupLights(renderer::DrawCommand &cmd, const components::LightContext& lightContext)
    {
        cmd.uniforms["uAmbientLight"] = lightContext.ambientLight;

        cmd.uniforms["uNumPointLights"] = static_cast<int>(lightContext.pointLightCount);
        cmd.uniforms["uNumSpotLights"] = static_cast<int>(lightContext.spotLightCount);

        const auto &directionalLight = lightContext.dirLight;
        cmd.uniforms["uDirLight.direction"] = directionalLight.direction;
        cmd.uniforms["uDirLight.color"] = glm::vec4(directionalLight.color, 1.0f);

        const auto &pointLightComponentArray = coord->getComponentArray<components::PointLightComponent>();
        const auto &transformComponentArray = coord->getComponentArray<components::TransformComponent>();
        for (unsigned int i = 0; i < lightContext.pointLightCount; ++i)
        {
            const auto &pointLight = pointLightComponentArray->get(lightContext.pointLights[i]);
            const auto &transform = transformComponentArray->get(lightContext.pointLights[i]);
            cmd.uniforms[std::format("uPointLights[{}].position", i)] = transform.pos;
            cmd.uniforms[std::format("uPointLights[{}].color", i)] = glm::vec4(pointLight.color, 1.0f);
            cmd.uniforms[std::format("uPointLights[{}].constant", i)] = pointLight.constant;
            cmd.uniforms[std::format("uPointLights[{}].linear", i)] = pointLight.linear;
            cmd.uniforms[std::format("uPointLights[{}].quadratic", i)] = pointLight.quadratic;
        }

        const auto &spotLightComponentArray = coord->getComponentArray<components::SpotLightComponent>();
        for (unsigned int i = 0; i < lightContext.spotLightCount; ++i)
        {
            const auto &spotLight = spotLightComponentArray->get(lightContext.spotLights[i]);
            const auto &transform = transformComponentArray->get(lightContext.spotLights[i]);
            cmd.uniforms[std::format("uSpotLights[{}].position", i)] = transform.pos;
            cmd.uniforms[std::format("uSpotLights[{}].color", i)] = glm::vec4(spotLight.color, 1.0f);
            cmd.uniforms[std::format("uSpotLights[{}].constant", i)] = spotLight.constant;
            cmd.uniforms[std::format("uSpotLights[{}].linear", i)] = spotLight.linear;
            cmd.uniforms[std::format("uSpotLights[{}].quadratic", i)] = spotLight.quadratic;
            cmd.uniforms[std::format("uSpotLights[{}].direction", i)] = spotLight.direction;
            cmd.uniforms[std::format("uSpotLights[{}].cutOff", i)] = spotLight.cutOff;
            cmd.uniforms[std::format("uSpotLights[{}].outerCutoff", i)] = spotLight.outerCutoff;
        }
    }

    static renderer::DrawCommand createOutlineDrawCommand(const components::CameraContext &camera)
    {
        renderer::DrawCommand cmd;
        cmd.type = renderer::CommandType::FULL_SCREEN;
        cmd.filterMask = 0;
        cmd.filterMask |= renderer::F_OUTLINE_PASS;
        cmd.shader = renderer::ShaderLibrary::getInstance().get("Outline pulse flat");

        cmd.uniforms["uViewProjection"] = camera.viewProjectionMatrix;
        cmd.uniforms["uCamPos"] = camera.cameraPosition;

        cmd.uniforms["uMaskTexture"] = 0;
        cmd.uniforms["uDepthTexture"] = 1;
        cmd.uniforms["uDepthMaskTexture"] = 2;
        cmd.uniforms["uTime"] = static_cast<float>(glfwGetTime());
        const glm::vec2 screenSize = {camera.renderTarget->getSize().x, camera.renderTarget->getSize().y};
        cmd.uniforms["uScreenSize"] = screenSize;
        cmd.uniforms["uOutlineWidth"] = 10.0f;
        return cmd;
    }

    static renderer::DrawCommand createGridDrawCommand(const components::CameraContext &camera, const components::RenderContext &renderContext)
    {
        renderer::DrawCommand cmd;
        cmd.type = renderer::CommandType::FULL_SCREEN;
        cmd.filterMask = 0;
        cmd.filterMask |= renderer::F_GRID_PASS;
        cmd.shader = renderer::ShaderLibrary::getInstance().get("Grid shader");

        cmd.uniforms["uViewProjection"] = camera.viewProjectionMatrix;
        cmd.uniforms["uCamPos"] = camera.cameraPosition;

        const components::RenderContext::GridParams &gridParams = renderContext.gridParams;
        cmd.uniforms["uGridSize"] = gridParams.gridSize;
        cmd.uniforms["uGridCellSize"] = gridParams.cellSize;
        cmd.uniforms["uGridMinPixelsBetweenCells"] = gridParams.minPixelsBetweenCells;
        const glm::vec4 gridColorThin = {0.5f, 0.55f, 0.7f, 0.6f};
        const glm::vec4 gridColorThick = {0.7f, 0.75f, 0.9f, 0.8f};
        cmd.uniforms["uGridColorThin"] = gridColorThin;
        cmd.uniforms["uGridColorThick"] = gridColorThick;


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
                    static_cast<unsigned int>(renderTargetSize.x), static_cast<unsigned int>(renderTargetSize.y)
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
                static_cast<unsigned int>(renderTargetSize.x), static_cast<unsigned int>(renderTargetSize.y)
            );

            if (rayDir.y != 0.0f) {
                float t = -camera.cameraPosition.y / rayDir.y;
                if (t > 0.0f) {
                    mouseWorldPos = camera.cameraPosition + rayDir * t;
                }
            }
        }

        cmd.uniforms["uMouseWorldPos"] = mouseWorldPos;
        cmd.uniforms["uTime"] = static_cast<float>(glfwGetTime());
        return cmd;
    }

    static renderer::DrawCommand createSelectedDrawCommand(
        const components::StaticMeshComponent &mesh,
        const std::shared_ptr<assets::Material> &materialAsset,
        const components::TransformComponent &transform)
    {
        renderer::DrawCommand cmd;
        cmd.vao = mesh.vao;
        bool isOpaque = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->isOpaque : true;
        if (isOpaque)
            cmd.shader = renderer::ShaderLibrary::getInstance().get("Flat color");
        else {
            cmd.shader = renderer::ShaderLibrary::getInstance().get("Albedo unshaded transparent");
            cmd.uniforms["uMaterial.albedoColor"] = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoColor : glm::vec4(0.0f);
            auto albedoTextureAsset = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoTexture.lock() : nullptr;
            auto albedoTexture = albedoTextureAsset && albedoTextureAsset->isLoaded() ? albedoTextureAsset->getData()->texture : nullptr;
            cmd.uniforms["uMaterial.albedoTexIndex"] = renderer::NxRenderer3D::get().getTextureIndex(albedoTexture);
        }
        cmd.uniforms["uMatModel"] = transform.worldMatrix;
        cmd.filterMask = 0;
        cmd.filterMask = renderer::F_OUTLINE_MASK;
        return cmd;
    }

    static renderer::DrawCommand createDrawCommand(
        ecs::Entity entity,
        const std::shared_ptr<renderer::NxShader> &shader,
        const components::StaticMeshComponent &mesh,
        const std::shared_ptr<assets::Material> &materialAsset,
        const components::TransformComponent &transform)
    {
        renderer::DrawCommand cmd;
        cmd.vao = mesh.vao;
        cmd.shader = shader;
        cmd.uniforms["uMatModel"] = transform.worldMatrix;
        cmd.uniforms["uEntityId"] = static_cast<int>(entity);

        cmd.uniforms["uMaterial.albedoColor"] = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoColor : glm::vec4(0.0f);
        auto albedoTextureAsset = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoTexture.lock() : nullptr;
        auto albedoTexture = albedoTextureAsset && albedoTextureAsset->isLoaded() ? albedoTextureAsset->getData()->texture : nullptr;
        cmd.uniforms["uMaterial.albedoTexIndex"] = renderer::NxRenderer3D::get().getTextureIndex(albedoTexture);

        cmd.uniforms["uMaterial.specularColor"] = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->specularColor : glm::vec4(0.0f);
        auto specularTextureAsset = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->metallicMap.lock() : nullptr;
        auto specularTexture = specularTextureAsset && specularTextureAsset->isLoaded() ? specularTextureAsset->getData()->texture : nullptr;
        cmd.uniforms["uMaterial.specularTexIndex"] = renderer::NxRenderer3D::get().getTextureIndex(specularTexture);

        cmd.uniforms["uMaterial.emissiveColor"] = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->emissiveColor : glm::vec3(0.0f);
        auto emissiveTextureAsset = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->metallicMap.lock() : nullptr;
        auto emissiveTexture = emissiveTextureAsset && emissiveTextureAsset->isLoaded() ? emissiveTextureAsset->getData()->texture : nullptr;
        cmd.uniforms["uMaterial.emissiveTexIndex"] = renderer::NxRenderer3D::get().getTextureIndex(emissiveTexture);

        cmd.uniforms["uMaterial.roughness"] = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->roughness : 1.0f;
        auto roughnessTextureAsset = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->roughnessMap.lock() : nullptr;
        auto roughnessTexture = roughnessTextureAsset && roughnessTextureAsset->isLoaded() ? roughnessTextureAsset->getData()->texture : nullptr;
        cmd.uniforms["uMaterial.roughnessTexIndex"] = renderer::NxRenderer3D::get().getTextureIndex(roughnessTexture);

        cmd.filterMask = 0;
        cmd.filterMask |= renderer::F_FORWARD_PASS;
        return cmd;
    }

	void RenderCommandSystem::update()
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
		if (!partition) {
            LOG_ONCE(NEXO_WARN, "Nothing to render in scene {}, skipping", sceneName);
            return;
		}
        Logger::resetOnce(NEXO_LOG_ONCE_KEY("Nothing to render in scene {}, skipping", sceneName));

		const auto transformSpan = get<components::TransformComponent>();
		const auto meshSpan = get<components::StaticMeshComponent>();
		const auto materialSpan = get<components::MaterialComponent>();
		const std::span<const ecs::Entity> entitySpan = m_group->entities();

        std::vector<renderer::DrawCommand> drawCommands;
		for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
		    const ecs::Entity entity = entitySpan[i];
            if (coord->entityHasComponent<components::CameraComponent>(entity) && sceneType != SceneType::EDITOR)
                continue;
            const auto &transform = transformSpan[i];
            const auto &materialAsset = materialSpan[i].material.lock();
            auto shaderStr = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->shader : nullptr;
            const auto &mesh = meshSpan[i];
            auto shader = renderer::ShaderLibrary::getInstance().get(shaderStr);
            if (!shader)
                continue;
            drawCommands.push_back(createDrawCommand(
                entity,
                shader,
                mesh,
                materialAsset,
                transform)
            );

            if (coord->entityHasComponent<components::SelectedTag>(entity))
                drawCommands.push_back(createSelectedDrawCommand(mesh, materialAsset, transform));
		}

		for (auto &camera : renderContext.cameras) {
            for (auto &cmd : drawCommands) {
                cmd.uniforms["uViewProjection"] = camera.viewProjectionMatrix;
                cmd.uniforms["uCamPos"] = camera.cameraPosition;
                setupLights(cmd, renderContext.sceneLights);
            }
            camera.pipeline.addDrawCommands(drawCommands);
            if (sceneType == SceneType::EDITOR && renderContext.gridParams.enabled)
                camera.pipeline.addDrawCommand(createGridDrawCommand(camera, renderContext));
            if (sceneType == SceneType::EDITOR)
                camera.pipeline.addDrawCommand(createOutlineDrawCommand(camera));
		}
	}
}
