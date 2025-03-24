//// MainScene.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source for the main scene document window
//
///////////////////////////////////////////////////////////////////////////////

#include "MainScene.hpp"

#include <Path.hpp>

#include "EntityFactory3D.hpp"
#include "LightFactory.hpp"
#include "CameraFactory.hpp"
#include "Nexo.hpp"
#include "WindowRegistry.hpp"
#include "components/Camera.hpp"
#include "components/Uuid.hpp"
#include "math/Matrix.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <random>

#include "SceneViewManager.hpp"

namespace nexo::editor {

    MainScene::MainScene(WindowRegistry &windowRegistry, std::string sceneName, const bool defaultScene) : m_sceneName(std::move(sceneName)),
                                                                           m_defaultScene(defaultScene),
                                                                           m_windowRegistry(windowRegistry)
    {
    	windowId = nextWindowId++;
    }

    void MainScene::setup()
    {
        setupImguizmo();
        setupWindow();
        setupScene();
    }

    void MainScene::setupScene()
    {
        auto &app = getApp();

        // New handling
        m_sceneId = static_cast<int>(app.getSceneManager().createScene(m_sceneName));
        renderer::FramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {
            renderer::FrameBufferTextureFormats::RGBA8, renderer::FrameBufferTextureFormats::RED_INTEGER, renderer::FrameBufferTextureFormats::Depth
        };
        framebufferSpecs.width = static_cast<unsigned int>(_viewSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(_viewSize.y);
        const auto renderTarget = renderer::Framebuffer::create(framebufferSpecs);
        m_activeCamera = CameraFactory::createPerspectiveCamera({0.0f, 0.0f, 0.0f}, static_cast<unsigned int>(_viewSize.x), static_cast<unsigned int>(_viewSize.y), renderTarget);
        m_cameras.insert(m_activeCamera);
        app.getSceneManager().getScene(m_sceneId).addEntity(m_activeCamera);
        components::PerspectiveCameraController controller;
        Application::m_coordinator->addComponent<components::PerspectiveCameraController>(m_activeCamera, controller);
        if (m_defaultScene)
            loadDefaultEntities();
    }

    void MainScene::setupImguizmo() const
    {
        ImGuizmo::SetOrthographic(true);
    }

    void MainScene::loadDefaultEntities() const
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

    void MainScene::setupWindow()
    {
        constexpr auto size = ImVec2(1280, 720);
        _viewSize = size;
    }

    void MainScene::shutdown()
    {}

    void MainScene::handleKeyEvents()
    {}

    void MainScene::deleteCamera(const ecs::Entity cameraId)
    {
    	if (cameraId == m_activeCamera)
    		m_activeCamera = -1;
    	m_cameras.erase(cameraId);
    	if (!m_cameras.empty())
    		m_activeCamera = *m_cameras.begin();
    }

    void MainScene::renderToolbar()
    {
        constexpr float padding = 0.0f;

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(padding, padding));
        ImGui::SetCursorScreenPos(ImVec2(_viewPosition.x + 10, _viewPosition.y + 10));
        if (ImGui::Button("Orthographic"))
        {
            // Switch to orthographic camera mode
            //engine::camera::setMode(CameraMode::ORTHOGRAPHIC);
        }

        ImGui::SameLine();
        if (ImGui::Button("Perspective"))
        {
            // Switch to perspective camera mode
            //engine::camera::setMode(CameraMode::PERSPECTIVE);
        }

        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_HAND_POINTER, buttonSize))
        // {
        //     // Set cursor to normal mode
        //     //engine::input::setCursorMode(CursorMode::NORMAL);
        // }
        //
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_HAND, buttonSize))
        // {
        //     // Set cursor to pan mode
        //     //engine::input::setCursorMode(CursorMode::PAN);
        // }
        //
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_ARROWS_SPIN, buttonSize))
        // {
        //     // Set cursor to rotate mode
        //     //engine::input::setCursorMode(CursorMode::ROTATE);
        // }
        //
        // ImGui::SameLine();
        // if (ImGui::Button(ICON_FA_CUBE, buttonSize))
        // {
        //     ImGui::OpenPopup("add_primitive");
        // }

        ImGui::SameLine();
        if (ImGui::BeginPopup("add_primitive"))
        {
            // const char *primitives_names[] = {
            //     "  Cube", "  Plan", "  Sphere",\
            //     "  Cylinder", "  Cone", "  Polygon", "  Torus", "  Knot",\
            //     "  Hemisphere"
            // };
            // PrimitiveFunction add_primitive_fct[] = {
            //     &Main3DScene::add_cube,
            //     &Main3DScene::add_plan,
            //     &Main3DScene::add_sphere,
            //     &Main3DScene::add_cylinder,
            //     &Main3DScene::add_cone,
            //     &Main3DScene::add_polygon,
            //     &Main3DScene::add_torus,
            //     &Main3DScene::add_knot,
            //     &Main3DScene::add_hemisphere
            // };
            ImGui::SeparatorText(" Add primitive ");
            // for (int i = 0; i < IM_ARRAYSIZE(primitives_names); i++)
            // {
            //     if (ImGui::Selectable(primitives_names[i], &chose_primitive))
            //     {
            //         (this->*add_primitive_fct[i])();
            //     }
            // }

            ImGui::EndPopup();
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(100);
        if (ImGui::DragInt("Target FPS", &_targetFPS, 1, 1, 120))
        {
            // Set target FPS
        }
        ImGui::PopItemWidth();
        ImGui::PopStyleVar();
    }

    void MainScene::renderGizmo()
    {
        const auto &coord = nexo::Application::m_coordinator;
        auto const &selector = Selector::get();
        const auto &sceneViewManager = m_windowRegistry.getWindow<SceneViewManager>();
        if (selector.getSelectionType() != SelectionType::ENTITY ||
            sceneViewManager->getSelectedScene() != m_sceneId)
            return;
        const ecs::Entity entity = selector.getSelectedEntity();
        const auto &transformCameraComponent = coord->getComponent<components::TransformComponent>(m_activeCamera);
        const auto &cameraComponent = coord->getComponent<components::CameraComponent>(m_activeCamera);
        ImGuizmo::SetOrthographic(cameraComponent.type == components::CameraType::ORTHOGRAPHIC);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetID(static_cast<int>(entity));
        ImGuizmo::SetRect(_viewPosition.x, _viewPosition.y, _viewSize.x, _viewSize.y);
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
                             _currentGizmoOperation,
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

    void MainScene::renderView()
    {
        const auto viewPortOffset = ImGui::GetCursorPos();
       	auto &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);

        // Resize handling
        if (const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            _viewSize.x != viewportPanelSize.x || _viewSize.y != viewportPanelSize.y)
        {
        	cameraComponent.resize(static_cast<unsigned int>(viewportPanelSize.x),
        							static_cast<unsigned int>(viewportPanelSize.y));

            _viewSize.x = viewportPanelSize.x;
            _viewSize.y = viewportPanelSize.y;
        }

        // Render framebuffer
        const unsigned int textureId = cameraComponent.m_renderTarget->getColorAttachmentId(0);
        ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)), _viewSize, ImVec2(0, 1), ImVec2(1, 0));

        const auto windowSize = ImGui::GetWindowSize();
        auto minBounds = ImGui::GetWindowPos();

        minBounds.x += viewPortOffset.x;
        minBounds.y += viewPortOffset.y;

        const ImVec2 maxBounds = {minBounds.x + windowSize.x, minBounds.y + windowSize.y};
        m_viewportBounds[0] = minBounds;
        m_viewportBounds[1] = maxBounds;
    }

    void MainScene::show()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(480, 270), ImVec2(1920, 1080));

        if (ImGui::Begin(m_sceneName.c_str(), &m_opened, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
        {
        	auto &app = getApp();
            _viewPosition = ImGui::GetCursorScreenPos();

            m_focused = ImGui::IsWindowFocused();
            app.getSceneManager().getScene(m_sceneId).setActiveStatus(m_focused);
            if (m_focused)
            {
                if (auto viewManager = m_windowRegistry.getWindow<SceneViewManager>();
                    viewManager->getSelectedScene() != m_sceneId)
                {
                	auto &selector = Selector::get();
                    viewManager->setSelectedScene(m_sceneId);
                    selector.unselectEntity();
                }
            }

            renderView();
            renderGizmo();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void MainScene::update()
    {
        if (!m_opened)
            return;
        handleKeyEvents();

        auto const &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);
        runEngine(m_sceneId, RenderingType::FRAMEBUFFER);
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing() && m_focused)
        {
            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_viewportBounds[0].x;
            my -= m_viewportBounds[0].y;

            // Flip the y-coordinate to match opengl texture format (maybe make it modular in some way)
            my = _viewSize.y - my;

            if (mx >= 0 && my >= 0 && mx < _viewSize.x && my < _viewSize.y)
            {
            	cameraComponent.m_renderTarget->bind();
                int data = cameraComponent.m_renderTarget->getPixel<int>(1, static_cast<int>(mx), static_cast<int>(my));
                cameraComponent.m_renderTarget->unbind();
                auto &selector = Selector::get();
                if (data != -1)
                {
                	std::cout << "Clicked on entity with ID: " << data << std::endl;
                    auto viewManager = m_windowRegistry.getWindow<SceneViewManager>();
                    const auto uuid = Application::m_coordinator->tryGetComponent<components::UuidComponent>(data);
                    if (uuid)
                    {
	                   	selector.setSelectedEntity(uuid->get().uuid, data);
	                    selector.setSelectionType(SelectionType::ENTITY);
                    }
                    viewManager->setSelectedScene(m_sceneId);
                }
                else
                {
                	selector.unselectEntity();
                }
            }
        }
    }

}
