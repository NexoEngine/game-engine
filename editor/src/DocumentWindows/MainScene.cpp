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

#include <EntityFactory2D.hpp>

#include "Nexo.hpp"
#include "math/Matrix.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace nexo::editor {

    MainScene::MainScene()
    {
        renderer::FramebufferSpecs framebufferSpecs;
        framebufferSpecs.width = 1280;
        framebufferSpecs.height = 720;
        m_framebuffer = renderer::Framebuffer::create(framebufferSpecs);
    }

    MainScene::~MainScene()
    {}

    void MainScene::setup()
    {
        setupCamera();
        loadEntities();
        setupWindow();
        setupGridShader();
    }

    void MainScene::setupCamera()
    {
        ImGuizmo::SetOrthographic(true);
    }

    void MainScene::loadEntities()
    {
        auto &app = getApp();
        _sceneID = app.createScene("Default editor scene");
        app.addNewLayer(_sceneID, "Default layer editor");
        ecs::Entity basicQuad = EntityFactory2D::createQuad({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 45.0f);
        _selectedEntity = basicQuad;
        m_camera = std::make_shared<camera::OrthographicCameraController>(1280.0f / 720.0f, true);
        app.attachCamera(_sceneID, m_camera, "Default layer editor");
        app.addEntityToScene(basicQuad, _sceneID, "Default layer editor");}

    void MainScene::setupWindow()
    {
        ImVec2 pos = ImVec2(118, 24);
        ImVec2 size = ImVec2(1280, 720);
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        _currentWindowSize = size;
        _viewSize = size;
    }

    void MainScene::setupGridShader()
    {}

    void MainScene::shutdown()
    {}

    void MainScene::show()
    {
        float width = static_cast<float>(m_framebuffer->getSpecs().width);
        float height = static_cast<float>(m_framebuffer->getSpecs().height);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        //ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400),
                                            //ImVec2(width, height));

        if (ImGui::Begin("3D View", &m_opened, ImGuiWindowFlags_NoScrollbar))
        {
            _viewPosition = ImGui::GetCursorScreenPos();
            ImVec2 windowPos = ImGui::GetWindowPos();
            // Vector2 offset = {windowPos.x, windowPos.y};
            // engine::setSceneWindowOffset(_sceneID, offset);
            renderView();
            //renderToolbar();
            renderGizmo();
            // if (engine::isMouseButtonDown(ecs::components::input::MouseButtons::MouseLeft) && !ImGuizmo::IsUsing())
            //     rayPicking();

            // Draw the rest of the window contents
            m_focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
            _prevWindowSize = _currentWindowSize;
            _currentWindowSize = ImGui::GetWindowSize();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void MainScene::update()
    {
        if (!m_opened)
            return;
        handleKeyEvents();

        _selectedEntity = m_sceneManagerBridge.getSelectedEntity();

        m_framebuffer->bind();
        runEngine(true);
        m_framebuffer->unbind();
        //m_sceneManagerBridge.deactivateAllScenes();
        //engine::activateScene(_sceneID);
        //engine::update(_sceneID);
        // engine::startRendering(_sceneID, _camera->getCameraID());
        // engine::renderGrid(_sceneID, _camera->getCameraID());
        // engine::renderAllEntities(_sceneID, _camera->getCameraID());
        // engine::endRendering(_sceneID);
        //lastTime = currentTime;
    }

    void MainScene::handleKeyEvents()
    {}

    void MainScene::renderGrid()
    {}

    void MainScene::renderToolbar()
    {
        static bool chose_primitive = false;
        ImVec2 buttonSize = ImVec2(40, 40);
        float padding = 0.0f;

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
            const char *primitives_names[] = {
                "  Cube", "  Plan", "  Sphere",\
                "  Cylinder", "  Cone", "  Polygon", "  Torus", "  Knot",\
                "  Hemisphere"
            };
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
        auto coord = nexo::Application::m_coordinator;
        ImGuizmo::SetOrthographic(true);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetID(_selectedEntity);
        ImGuizmo::SetRect(_viewPosition.x, _viewPosition.y, _viewSize.x, _viewSize.y);
        glm::mat4 viewMatrix = m_camera->getViewMatrix();
        glm::mat4 projectionMatrix = m_camera->getProjectionMatrix();
        auto transf = coord->tryGetComponent<components::TransformComponent>(_selectedEntity);
        if (!transf)
            return;
        glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), transf->get().pos) *
                                    glm::rotate(glm::mat4(1.0f), glm::radians(transf->get().rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
                                    glm::scale(glm::mat4(1.0f), {transf->get().size.x, transf->get().size.y, 1.0f});
        ImGuizmo::Enable(true);
        ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
                              _currentGizmoOperation,
                              ImGuizmo::MODE::LOCAL,
                              glm::value_ptr(transformMatrix));

        glm::vec3 translation(0);
        glm::vec3 rotation(0);
        glm::vec3 scale(0);

        math::decomposeTransformEuler(transformMatrix, translation, rotation, scale);

        if (ImGuizmo::IsUsing())
        {
            transf->get().pos = translation;
            transf->get().rotation = glm::degrees(rotation);
            transf->get().size = scale;
        }
    }

    void MainScene::renderView()
    {
        // Resize handling
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (_viewSize.x != viewportPanelSize.x || _viewSize.y != viewportPanelSize.y)
        {
            m_framebuffer->resize(viewportPanelSize.x, viewportPanelSize.y);
            _viewSize.x = viewportPanelSize.x;
            _viewSize.y = viewportPanelSize.y;
            const auto event = std::make_shared<event::EventWindowResize>(viewportPanelSize.x, viewportPanelSize.y);
            getApp().getEventManager()->emitEvent<event::EventWindowResize>(event);
        }

        // Render framebuffer
        unsigned int textureId = m_framebuffer->getColorAttachmentId();
        ImGui::Image((void *)textureId, _viewSize, ImVec2(0, 1), ImVec2(1, 0));
    }

    void MainScene::rayPicking()
    {}
}
