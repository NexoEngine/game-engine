//// EditorScene.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Source for the editor scene document window
//
///////////////////////////////////////////////////////////////////////////////

#include "EditorScene.hpp"

#include <Path.hpp>

#include "ADocumentWindow.hpp"
#include "EntityFactory3D.hpp"
#include "IconsFontAwesome.h"
#include "LightFactory.hpp"
#include "CameraFactory.hpp"
#include "Nexo.hpp"
#include "WindowRegistry.hpp"
#include "components/Camera.hpp"
#include "components/Uuid.hpp"
#include "math/Matrix.hpp"
#include "context/Selector.hpp"
#include "utils/String.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <random>

namespace nexo::editor {

    void EditorScene::setup()
    {
        setupImguizmo();
        setupWindow();
        setupScene();
    }

    void EditorScene::setupScene()
    {
        auto &app = getApp();

        // New handling
        m_sceneId = static_cast<int>(app.getSceneManager().createScene(m_windowName));
        renderer::FramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {
            renderer::FrameBufferTextureFormats::RGBA8, renderer::FrameBufferTextureFormats::RED_INTEGER, renderer::FrameBufferTextureFormats::Depth
        };
        framebufferSpecs.width = static_cast<unsigned int>(m_viewSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(m_viewSize.y);
        const auto renderTarget = renderer::Framebuffer::create(framebufferSpecs);
        m_activeCamera = CameraFactory::createPerspectiveCamera({0.0f, 0.0f, 0.0f}, static_cast<unsigned int>(m_viewSize.x), static_cast<unsigned int>(m_viewSize.y), renderTarget);
        m_cameras.insert(static_cast<ecs::Entity>(m_activeCamera));
        app.getSceneManager().getScene(m_sceneId).addEntity(static_cast<ecs::Entity>(m_activeCamera));
        components::PerspectiveCameraController controller;
        Application::m_coordinator->addComponent<components::PerspectiveCameraController>(static_cast<ecs::Entity>(m_activeCamera), controller);

        m_sceneUuid = app.getSceneManager().getScene(m_sceneId).getUuid();
        if (m_defaultScene)
            loadDefaultEntities();
    }

    void EditorScene::setupImguizmo() const
    {
        ImGuizmo::SetOrthographic(true);
    }

    void EditorScene::loadDefaultEntities() const
    {
        auto &app = getApp();
        scene::Scene &scene = app.getSceneManager().getScene(m_sceneId);
        const ecs::Entity ambientLight = LightFactory::createAmbientLight({0.5f, 0.5f, 0.5f});
        scene.addEntity(ambientLight);
        const ecs::Entity pointLight = LightFactory::createPointLight({1.2f, 5.0f, 0.1f});
        scene.addEntity(pointLight);
        const ecs::Entity directionalLight = LightFactory::createDirectionalLight({0.2f, -1.0f, -0.3f});
        scene.addEntity(directionalLight);
        const ecs::Entity spotLight = LightFactory::createSpotLight({0.0f, 0.5f, -2.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f});
        scene.addEntity(spotLight);
        const ecs::Entity basicCube = EntityFactory3D::createCube({0.0f, -5.0f, -5.0f}, {20.0f, 1.0f, 20.0f},
                                                               {0.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.31f, 1.0f});
        app.getSceneManager().getScene(m_sceneId).addEntity(basicCube);
    }

    void EditorScene::setupWindow()
    {
        constexpr auto size = ImVec2(1280, 720);
        m_viewSize = size;
    }

    void EditorScene::shutdown()
    {
    	// Should probably check if it is necessary to delete the scene here ? (const for now)
    }

    void EditorScene::handleKeyEvents() const
    {
    	// Will be implemeneted later
    }

    void EditorScene::deleteCamera(const ecs::Entity cameraId)
    {
    	if (static_cast<int>(cameraId) == m_activeCamera)
    		m_activeCamera = -1;
    	m_cameras.erase(cameraId);
    	if (!m_cameras.empty())
    		m_activeCamera = *m_cameras.begin();
    }

    void EditorScene::renderToolbar() const
    {
    	// Empty for now, will add it later
    }

    void EditorScene::renderGizmo() const
    {
        const auto &coord = nexo::Application::m_coordinator;
        auto const &selector = Selector::get();
        if (selector.getSelectionType() != SelectionType::ENTITY ||
            selector.getSelectedScene() != m_sceneId)
            return;
        const ecs::Entity entity = selector.getSelectedEntity();
        const auto &transformCameraComponent = coord->getComponent<components::TransformComponent>(m_activeCamera);
        const auto &cameraComponent = coord->getComponent<components::CameraComponent>(m_activeCamera);
        ImGuizmo::SetOrthographic(cameraComponent.type == components::CameraType::ORTHOGRAPHIC);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetID(static_cast<int>(entity));
        ImGuizmo::SetRect(m_viewPosition.x, m_viewPosition.y, m_viewSize.x, m_viewSize.y);
        glm::mat4 viewMatrix = cameraComponent.getViewMatrix(transformCameraComponent);
        glm::mat4 projectionMatrix = cameraComponent.getProjectionMatrix();
        const auto transf = coord->tryGetComponent<components::TransformComponent>(entity);
        if (!transf)
            return;
        const glm::mat4 rotationMat = glm::toMat4(transf->get().quat);
        glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), transf->get().pos) *
                                    rotationMat *
                                    glm::scale(glm::mat4(1.0f), {transf->get().size.x, transf->get().size.y, transf->get().size.z});
        ImGuizmo::Enable(true);
        ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
                             m_currentGizmoOperation,
                             ImGuizmo::MODE::WORLD,
                             glm::value_ptr(transformMatrix));

        glm::vec3 translation(0);
        glm::vec3 scale(0);
        glm::quat quaternion;

        math::decomposeTransformQuat(transformMatrix, translation, quaternion, scale);

        if (ImGuizmo::IsUsing())
        {
            transf->get().pos = translation;
            transf->get().quat = quaternion;
            transf->get().size = scale;
        }
    }

    void EditorScene::renderView()
    {
        const auto viewPortOffset = ImGui::GetCursorPos();
       	auto &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);

        // Resize handling
        if (const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            m_viewSize.x != viewportPanelSize.x || m_viewSize.y != viewportPanelSize.y)
        {
        	cameraComponent.resize(static_cast<unsigned int>(viewportPanelSize.x),
        							static_cast<unsigned int>(viewportPanelSize.y));

            m_viewSize.x = viewportPanelSize.x;
            m_viewSize.y = viewportPanelSize.y;
        }

        // Render framebuffer
        const unsigned int textureId = cameraComponent.m_renderTarget->getColorAttachmentId(0);
        ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)), m_viewSize, ImVec2(0, 1), ImVec2(1, 0));

        const auto windowSize = ImGui::GetWindowSize();
        auto minBounds = ImGui::GetWindowPos();

        minBounds.x += viewPortOffset.x;
        minBounds.y += viewPortOffset.y;

        const ImVec2 maxBounds = {minBounds.x + windowSize.x, minBounds.y + windowSize.y};
        m_viewportBounds[0] = minBounds;
        m_viewportBounds[1] = maxBounds;
    }

    void EditorScene::show()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(480, 270), ImVec2(1920, 1080));
        auto &selector = Selector::get();
        m_windowName = selector.getUiHandle(m_sceneUuid, std::string(ICON_FA_GLOBE) + " " + m_windowName);
        const std::string &sceneWindowName = m_windowName + "###" + std::string(NEXO_WND_USTRID_DEFAULT_SCENE);

        if (ImGui::Begin(sceneWindowName.c_str(), &m_opened, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
        {
            firstDockSetup(NEXO_WND_USTRID_DEFAULT_SCENE);
        	auto &app = getApp();
            m_viewPosition = ImGui::GetCursorScreenPos();

            m_focused = ImGui::IsWindowFocused();
            m_hovered = ImGui::IsWindowHovered();
            app.getSceneManager().getScene(m_sceneId).setActiveStatus(m_focused);
            if (m_focused && selector.getSelectedScene() != m_sceneId)
            {
                selector.setSelectedScene(m_sceneId);
                selector.unselectEntity();
            }

            if (m_activeCamera == -1)
            {
	            // No active camera, render the text at the center of the screen
	            ImVec2 textSize = ImGui::CalcTextSize("No active camera");
	            auto textPos = ImVec2((m_viewSize.x - textSize.x) / 2, (m_viewSize.y - textSize.y) / 2);

	            ImGui::SetCursorScreenPos(textPos);
	            ImGui::Text("No active camera");
            }
            else
            {
	            renderView();
	            renderGizmo();
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorScene::update()
    {
    	auto &selector = Selector::get();
    	//m_windowName = selector.getUiHandle(m_sceneUuid, m_windowName);
        if (!m_opened || m_activeCamera == -1)
            return;
        if (m_focused && m_hovered)
            handleKeyEvents();

        auto const &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(static_cast<ecs::Entity>(m_activeCamera));
        runEngine(m_sceneId, RenderingType::FRAMEBUFFER);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing() && m_focused)
        {
            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_viewportBounds[0].x;
            my -= m_viewportBounds[0].y;

            // Flip the y-coordinate to match opengl texture format (maybe make it modular in some way)
            my = m_viewSize.y - my;

            // Mouse is not inside the viewport
            if (!(mx >= 0 && my >= 0 && mx < m_viewSize.x && my < m_viewSize.y))
                return;

            cameraComponent.m_renderTarget->bind();
            int data = cameraComponent.m_renderTarget->getPixel<int>(1, static_cast<int>(mx), static_cast<int>(my));
            cameraComponent.m_renderTarget->unbind();
            if (data == -1)
            {
            	selector.unselectEntity();
             	return;
            }
            const auto uuid = Application::m_coordinator->tryGetComponent<components::UuidComponent>(data);
            if (uuid)
            {
               	selector.setSelectedEntity(uuid->get().uuid, data);
                selector.setSelectionType(SelectionType::ENTITY);
            }
            selector.setSelectedScene(m_sceneId);
        }
    }

}
