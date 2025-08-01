//// RenderBillboardSystem.cpp ////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        11/05/2025
//  Description: Source file for the billboard rendering system
//
///////////////////////////////////////////////////////////////////////////////

#include "RenderBillboardSystem.hpp"
#include "components/BillboardMesh.hpp"
#include "renderPasses/Masks.hpp"
#include "Application.hpp"
#include "renderer/ShaderLibrary.hpp"
#include "renderer/Renderer3D.hpp"
#include "components/Editor.hpp"

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
    void RenderBillboardSystem::setupLights(renderer::DrawCommand &cmd, const components::LightContext& lightContext)
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

    static glm::mat4 createBillboardTransformMatrix(
        const glm::vec3 &cameraPosition,
        const components::TransformComponent &transform,
        const glm::vec3& cameraUp = glm::vec3(0.0f, 1.0f, 0.0f),
        bool constrainToY = false
    ) {
        glm::vec3 look = glm::normalize(cameraPosition - transform.pos);

        if (constrainToY) {
            look.y = 0.0f;
            look = glm::normalize(look);

            glm::vec3 right = glm::normalize(glm::cross(cameraUp, look));
            glm::vec3 up = glm::cross(look, right);

            return {glm::vec4(right, 0.0f),
                    glm::vec4(up, 0.0f),
                    glm::vec4(-look, 0.0f), // Negative look preserves winding
                    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
                };
        }
        const glm::vec3 right = glm::normalize(glm::cross(cameraUp, look));
        const glm::vec3 up = glm::cross(look, right);

        return {glm::vec4(right, 0.0f),
                glm::vec4(up, 0.0f),
                glm::vec4(-look, 0.0f), // Negative look preserves winding
                glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
            };
    }

    static renderer::DrawCommand createSelectedDrawCommand(
        const glm::vec3 &cameraPosition,
        const components::BillboardComponent &mesh,
        const std::shared_ptr<assets::Material> &materialAsset,
        const components::TransformComponent &transform)
    {
        renderer::DrawCommand cmd;
        cmd.vao = mesh.vao;
        const bool isOpaque = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->isOpaque : true;
        if (isOpaque)
            cmd.shader = renderer::ShaderLibrary::getInstance().get("Flat color");
        else {
            cmd.shader = renderer::ShaderLibrary::getInstance().get("Albedo unshaded transparent");
            cmd.uniforms["uMaterial.albedoColor"] = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoColor : glm::vec4(0.0f);
            const auto albedoTextureAsset = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoTexture.lock() : nullptr;
            const auto albedoTexture = albedoTextureAsset && albedoTextureAsset->isLoaded() ? albedoTextureAsset->getData()->texture : nullptr;
            cmd.uniforms["uMaterial.albedoTexIndex"] = renderer::NxRenderer3D::get().getTextureIndex(albedoTexture);
        }
        const glm::mat4 &billboardRotation = createBillboardTransformMatrix(cameraPosition, transform);
        cmd.uniforms["uMatModel"]= glm::translate(glm::mat4(1.0f), transform.pos) *
                                    billboardRotation *
                                    glm::scale(glm::mat4(1.0f), glm::vec3(transform.size.x, transform.size.y, 1.0f));
        cmd.filterMask = 0;
        cmd.filterMask = renderer::F_OUTLINE_MASK;
        return cmd;
    }

    static renderer::DrawCommand createDrawCommand(
        const ecs::Entity entity,
        const glm::vec3 &cameraPosition,
        const std::shared_ptr<renderer::NxShader> &shader,
        const components::BillboardComponent &billboard,
        const std::shared_ptr<assets::Material> &materialAsset,
        const components::TransformComponent &transform)
    {
        renderer::DrawCommand cmd;
        cmd.vao = billboard.vao;
        cmd.shader = shader;
        const glm::mat4 &billboardRotation = createBillboardTransformMatrix(cameraPosition, transform);
        cmd.uniforms["uMatModel"]= glm::translate(glm::mat4(1.0f), transform.pos) *
                                    billboardRotation *
                                    glm::scale(glm::mat4(1.0f), glm::vec3(transform.size.x, transform.size.y, 1.0f));
        cmd.uniforms["uEntityId"] = static_cast<int>(entity);

        cmd.uniforms["uMaterial.albedoColor"] = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoColor : glm::vec4(0.0f);
        const auto albedoTextureAsset = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->albedoTexture.lock() : nullptr;
        const auto albedoTexture = albedoTextureAsset && albedoTextureAsset->isLoaded() ? albedoTextureAsset->getData()->texture : nullptr;
        cmd.uniforms["uMaterial.albedoTexIndex"] = renderer::NxRenderer3D::get().getTextureIndex(albedoTexture);

        cmd.uniforms["uMaterial.specularColor"] = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->specularColor : glm::vec4(0.0f);
        const auto specularTextureAsset = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->metallicMap.lock() : nullptr;
        const auto specularTexture = specularTextureAsset && specularTextureAsset->isLoaded() ? specularTextureAsset->getData()->texture : nullptr;
        cmd.uniforms["uMaterial.specularTexIndex"] = renderer::NxRenderer3D::get().getTextureIndex(specularTexture);

        cmd.uniforms["uMaterial.emissiveColor"] = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->emissiveColor : glm::vec3(0.0f);
        const auto emissiveTextureAsset = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->emissiveMap.lock() : nullptr;
        const auto emissiveTexture = emissiveTextureAsset && emissiveTextureAsset->isLoaded() ? emissiveTextureAsset->getData()->texture : nullptr;
        cmd.uniforms["uMaterial.emissiveTexIndex"] = renderer::NxRenderer3D::get().getTextureIndex(emissiveTexture);

        cmd.uniforms["uMaterial.roughness"] = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->roughness : 1.0f;
        const auto roughnessTextureAsset = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->roughnessMap.lock() : nullptr;
        const auto roughnessTexture = roughnessTextureAsset && roughnessTextureAsset->isLoaded() ? roughnessTextureAsset->getData()->texture : nullptr;
        cmd.uniforms["uMaterial.roughnessTexIndex"] = renderer::NxRenderer3D::get().getTextureIndex(roughnessTexture);

        cmd.filterMask = 0;
        cmd.filterMask |= renderer::F_FORWARD_PASS;
        return cmd;
    }

	void RenderBillboardSystem::update()
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

		const auto transformComponentArray = get<components::TransformComponent>();
		const auto billboardSpan = get<components::BillboardComponent>();
		const auto materialComponentArray = get<components::MaterialComponent>();
		const std::span<const ecs::Entity> entitySpan = m_group->entities();

		for (auto &camera : renderContext.cameras) {
            std::vector<renderer::DrawCommand> drawCommands;
            for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
                const ecs::Entity entity = entitySpan[i];
                if (coord->entityHasComponent<components::CameraComponent>(entity) && sceneType != SceneType::EDITOR)
                    continue;
                const auto &transform = transformComponentArray->get(entitySpan[i]);
                const auto &materialAsset = materialComponentArray->get(entitySpan[i]).material.lock();
                const auto &billboard = billboardSpan[i];
                auto shaderStr = materialAsset && materialAsset->isLoaded() ? materialAsset->getData()->shader : "";
                auto shader = renderer::ShaderLibrary::getInstance().get(shaderStr);
                auto cmd = createDrawCommand(
                    entity,
                    camera.cameraPosition,
                    shader,
                    billboard,
                    materialAsset,
                    transform
                );
                cmd.uniforms["uViewProjection"] = camera.viewProjectionMatrix;
                cmd.uniforms["uCamPos"] = camera.cameraPosition;
                setupLights(cmd, renderContext.sceneLights);
                drawCommands.push_back(cmd);

                if (coord->entityHasComponent<components::SelectedTag>(entity)) {
                    auto selectedCmd = createSelectedDrawCommand(camera.cameraPosition, billboard, materialAsset, transform);
                    selectedCmd.uniforms["uViewProjection"] = camera.viewProjectionMatrix;
                    selectedCmd.uniforms["uCamPos"] = camera.cameraPosition;
                    setupLights(selectedCmd, renderContext.sceneLights);
                    drawCommands.push_back(selectedCmd);
                }
            }
            camera.pipeline.addDrawCommands(drawCommands);
		}
	}
}
