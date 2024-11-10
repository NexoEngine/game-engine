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
#include "Nexo.hpp"

namespace nexo::editor {

    class ExampleLayer final : LAYER_LISTENS_TO(
                ExampleLayer,
                event::EventKey,
                event::EventMouseClick
            ) {
        public:
            ExampleLayer() : Layer("EXAMPLE"),
                             m_camera(1920.0f / 1080.0f, true)
            {
                // Texture init
                m_texture = renderer::Texture2D::create("../assets/textures/Checkerboard.png");
                m_nexoLogo = renderer::Texture2D::create("../assets/textures/logo_nexo.png");
            };

            void onUpdate(const core::Timestep timeStep) override
            {
                m_camera.onUpdate(timeStep);
                m_rotation += timeStep * 50.0f;
            }

            void onRender() override
            {
                renderer::Renderer2D::beginScene(m_camera.getCamera());
                //renderer::Renderer2D::drawQuad({1.0f, 0.0f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
                renderer::Renderer2D::drawQuad({-2.0f, 0.0f}, {1.0f, 1.0f}, m_rotation, m_texture);
                renderer::Renderer2D::drawQuad({0.0f, 0.0f, 0.1f}, {1.0f, 1.0f}, m_nexoLogo);
                renderer::Renderer2D::endScene();
            }

            void onAttach() override
            {}

            void onDetach() override
            {}

            void handleEvent(event::EventKey &event) override
            {
                std::cout << "[Layer: " << m_debugName << "]" << event << std::endl;
            }

            void handleEvent(event::EventMouseClick &event) override
            {
                std::cout << "[Layer: " << m_debugName << "]" << event << std::endl;
            }

        private:
            renderer::ShaderLibrary m_shaderLibrary;

            std::shared_ptr<renderer::Texture2D> m_texture;
            std::shared_ptr<renderer::Texture2D> m_nexoLogo;

            camera::OrthographicCameraController m_camera;

            float m_rotation = 0.0f;

            struct ProfileResult {
                std::string name;
                float time;
            };

            std::vector<ProfileResult> m_profileResults;

    };

    MainScene::MainScene() : ADocumentWindow(), _sceneID(getApp().createScene("Default editor scene"))
    {
        auto &app = getApp();
        auto &sceneManager = app.getSceneManager();
        auto &scene = sceneManager.getScene(_sceneID);
        const auto layer = std::make_shared<ExampleLayer>();
        sceneManager.addLayer(_sceneID, layer);
        app.addScene(scene);
        app.switchScene(scene);
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
        ImGuizmo::SetOrthographic(false);
    }

    void MainScene::loadEntities()
    {}

    void MainScene::setupWindow()
    {
        ImVec2 pos = ImVec2(118, 24);
        ImVec2 size = ImVec2(1389, 804);
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
            //_viewSize = ImGui::GetContentRegionAvail();
            ImVec2 windowPos = ImGui::GetWindowPos();
            // Vector2 offset = {windowPos.x, windowPos.y};
            // engine::setSceneWindowOffset(_sceneID, offset);
            renderView();
            //renderToolbar();
            //renderGizmo();
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
        //handleWindowResize();
        handleKeyEvents();

        _selectedEntity = m_sceneManagerBridge.getSelectedEntity();

        // limit fps to _targetFPS with clock, frameTime in ms
        // auto frameTimeMs = static_cast<long long>(1.0 / _targetFPS * 1000.0);
        //
        // static auto lastTime = std::chrono::high_resolution_clock::now();
        // auto currentTime = std::chrono::high_resolution_clock::now();
        //
        // auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
        //
        // if (elapsedTime < frameTimeMs)
        //     return;
        m_framebuffer->bind();
        getApp().run();
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

    bool MainScene::isWindowResized() const
    {
        return _currentWindowSize.x != _prevWindowSize.x || _currentWindowSize.y != _prevWindowSize.y;
    }

    void MainScene::handleWindowResize()
    {
        if (!isWindowResized())
            return;
        //_camera->updateRenderTextureSize(static_cast<int>(_viewSize.x), static_cast<int>(_viewSize.y));
        //ImGuizmo::SetRect(_viewPosition.x, _viewPosition.y, _viewSize.x, _viewSize.y);
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
        // ImGuizmo::SetOrthographic(false);
        // ImGuizmo::SetDrawlist();
        // ImGuizmo::SetID(_selectedEntity);
        // ImGuizmo::SetRect(_viewPosition.x, _viewPosition.y, _viewSize.x, _viewSize.y);
        // Matrix viewMatrix = _camera->getViewMatrix();
        // Matrix projectionMatrix = _camera->getProjectionMatrix(_currentWindowSize.x / _currentWindowSize.y, 0.1f,
        //                                                        1000.0f);
        // Matrix objectMatrix = engine::entity::getTransformMatrix(_selectedEntity);
        // auto &transf = engine::entity::getComponent<ecs::components::physics::transform_t>(_selectedEntity);
        //
        // float16 viewMatrixFloats = MatrixToFloatV(viewMatrix);
        // float16 projectionMatrixFloats = MatrixToFloatV(projectionMatrix);
        // float16 objectMatrixFloats = MatrixToFloatV(objectMatrix);
        //
        // ImGuizmo::Enable(true);
        // ImGuizmo::Manipulate(viewMatrixFloats.v, projectionMatrixFloats.v,
        //                      _currentGizmoOperation,
        //                      ImGuizmo::MODE::LOCAL,
        //                      objectMatrixFloats.v);
        //
        // Vector3 translation = {0};
        // Vector3 rotation = {0};
        // Vector3 scale = {0};
        //
        // math::decomposeTransformMatrixEuler(engine::math::matrixFromFloat16(objectMatrixFloats), translation, rotation,
        //                                     scale);
        //
        // if (ImGuizmo::IsUsing())
        // {
        //     transf.pos = translation;
        //     transf.rotation = rotation;
        //     transf.scale = scale;
        //     engine::entity::setTransformMatrix(_selectedEntity, engine::math::matrixFromFloat16(objectMatrixFloats));
        // }
    }

    void MainScene::renderView()
    {
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        if (_viewSize.x != viewportPanelSize.x || _viewSize.y != viewportPanelSize.y)
        {
            std::cout << _viewSize.x << " " << viewportPanelSize.x << std::endl;
            std::cout << _viewSize.y << " " << viewportPanelSize.y << std::endl;
            m_framebuffer->resize(viewportPanelSize.x, viewportPanelSize.y);
            _viewSize.x = viewportPanelSize.x;
            _viewSize.y = viewportPanelSize.y;
            const auto event = std::make_shared<event::EventWindowResize>(viewportPanelSize.x, viewportPanelSize.y);
            getApp().getEventManager()->emitEvent<event::EventWindowResize>(event);
        }
        unsigned int textureId = m_framebuffer->getColorAttachmentId();
        ImGui::Image((void *)textureId, _viewSize, ImVec2(0, 1), ImVec2(1, 0));

        //ImGui::End();
        //rlImGuiImageRenderTexture(&_camera->getRenderTexture());
    }

    void MainScene::rayPicking()
    {}
}
