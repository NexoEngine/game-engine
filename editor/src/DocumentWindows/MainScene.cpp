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
#include "EntityFactory2D.hpp"
#include "EntityFactory3D.hpp"
#include "Nexo.hpp"
#include "math/Matrix.hpp"
#include "core/camera/PerspectiveCameraController.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

#include "SceneViewManager.hpp"

namespace nexo::editor {

    MainScene::MainScene(std::string sceneName, const bool defaultScene) : m_sceneName(std::move(sceneName)),
                                                                           m_defaultScene(defaultScene) {}

    MainScene::~MainScene()
    = default;

    void MainScene::setup()
    {
        setupImguizmo();
        setupWindow();
        setupFramebuffer();
        setupScene();
    }

    void MainScene::setupScene()
    {
        auto &app = getApp();
        _sceneID = app.createScene(m_sceneName);
        const scene::LayerId layerId = app.addNewLayer(_sceneID, "Layer 1");
        m_camera = std::make_shared<camera::PerspectiveCameraController>(_viewSize.x / _viewSize.y);
        //m_camera = std::make_shared<camera::OrthographicCameraController>(_viewSize.x / _viewSize.y, true);
        app.attachCamera(_sceneID, m_camera, layerId);
        if (m_defaultScene)
            loadDefaultEntities(layerId);
    }

    void MainScene::setupImguizmo()
    {
        ImGuizmo::SetOrthographic(true);
    }

    float randomColor()
    {
        static std::mt19937 rng(std::random_device{}());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(rng);
    }

    void MainScene::loadDefaultEntities(const scene::LayerId defaultLayerId) const
    {
        auto &app = getApp();
        // const ecs::Entity basicQuad = EntityFactory2D::createQuad({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 45.0f);
        // app.addEntityToScene(basicQuad, _sceneID, static_cast<int>(defaultLayerId));
        // app.setAmbientLightValue(_sceneID, 1.0f);
         const ecs::Entity basicCube = EntityFactory3D::createCube({0.0f, 0.0f, -2.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f});
         app.addEntityToScene(basicCube, _sceneID, static_cast<int>(defaultLayerId));

        // const ecs::Entity gunModel = EntityFactory3D::createModel("./assets/models/9mn/scene.gltf", {0.0f, 0.0f, -2.0f}, {0.01f, 0.01f, 0.01f}, {0.0f, 0.0f, 0.0f});
        // app.addEntityToScene(gunModel, _sceneID, static_cast<int>(defaultLayerId));
        glm::vec3 lightPos1 = {0.0f, 2.0f, 1.0f};
        glm::vec4 color1 = {randomColor(), randomColor(), randomColor(), 1.0f};
        auto pointLight1 = std::make_shared<components::PointLight>(lightPos1, color1, 0.9);
        app.addLightToScene(_sceneID, pointLight1);

        // glm::vec3 lightPos2 = {0.0f, 2.0f, 1.0f};
        // glm::vec4 color2 = {randomColor(), randomColor(), randomColor(), 1.0f};
        // auto pointLight2 = std::make_shared<components::PointLight>(lightPos2, color2);
        // app.addLightToScene(_sceneID, pointLight2);
        //
        // glm::vec3 lightPos3 = {1.0f, 2.0f, 0.0f};
        // glm::vec4 color3 = {randomColor(), randomColor(), randomColor(), 1.0f};
        // auto pointLight3 = std::make_shared<components::PointLight>(lightPos3, color3);
        // app.addLightToScene(_sceneID, pointLight3);

        // glm::vec3 lightPos4 = {-1.0f, 2.0f, 0.0f};
        // glm::vec4 color4 = {1.0f, 1.0f, 1.0f, 1.0f};
        // auto pointLight4 = std::make_shared<components::PointLight>(lightPos4, color4);
        // app.addLightToScene(_sceneID, pointLight4);
        //app.addEntityToScene(basicQuad, _sceneID, static_cast<int>(defaultLayerId));
    }

    void MainScene::setupWindow()
    {
        constexpr auto pos = ImVec2(118, 24);
        constexpr auto size = ImVec2(1280, 720);
        ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        _viewSize = size;
    }

    void MainScene::shutdown()
    {}

    void MainScene::setupFramebuffer()
    {
        renderer::FramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {renderer::FrameBufferTextureFormats::RGBA8, renderer::FrameBufferTextureFormats::Depth};
        framebufferSpecs.width = static_cast<unsigned int>(_viewSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(_viewSize.y);
        m_framebuffer = renderer::Framebuffer::create(framebufferSpecs);
    }

    void MainScene::handleKeyEvents()
    {}

    void MainScene::renderToolbar()
    {
        // static bool chose_primitive = false;
        // ImVec2 buttonSize = ImVec2(40, 40);
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

    void MainScene::renderGizmo() const
    {
        const auto &coord = nexo::Application::m_coordinator;
        if (const auto &viewManager = SceneViewManager::getInstance();
            m_sceneManagerBridge->getSelectionType() != SelectionType::ENTITY ||
            viewManager->getSelectedScene() != static_cast<int>(_sceneID))
            return;
        auto &selectionData = m_sceneManagerBridge->getData();
        if (!std::holds_alternative<EntityProperties>(selectionData))
            return;
        const ecs::Entity entity = std::get<EntityProperties>(selectionData).entity;
        ImGuizmo::SetOrthographic(m_camera->getMode() == camera::CameraMode::ORTHOGRAPHIC);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetID(static_cast<int>(entity));
        ImGuizmo::SetRect(_viewPosition.x, _viewPosition.y, _viewSize.x, _viewSize.y);
        glm::mat4 viewMatrix = m_camera->getViewMatrix();
        glm::mat4 projectionMatrix = m_camera->getProjectionMatrix();
        auto transf = coord->tryGetComponent<components::TransformComponent>(entity);
        if (!transf)
            return;
        glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), transf->get().pos) *
                                    glm::rotate(glm::mat4(1.0f), glm::radians(transf->get().rotation.z),
                                                glm::vec3(0.0f, 0.0f, 1.0f)) *
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
        if (ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            _viewSize.x != viewportPanelSize.x || _viewSize.y != viewportPanelSize.y)
        {
            m_framebuffer->resize(static_cast<unsigned int>(viewportPanelSize.x),
                                  static_cast<unsigned int>(viewportPanelSize.y));
            _viewSize.x = viewportPanelSize.x;
            _viewSize.y = viewportPanelSize.y;
            const auto event = std::make_shared<event::EventWindowResize>(viewportPanelSize.x, viewportPanelSize.y);
            getApp().getEventManager()->emitEvent<event::EventWindowResize>(event);
            m_sceneManagerBridge->setSceneActiveStatus(_sceneID, false);
        }

        // Render framebuffer
        const unsigned int textureId = m_framebuffer->getColorAttachmentId();
        ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)), _viewSize, ImVec2(0, 1), ImVec2(1, 0));
    }

    glm::vec2 MainScene::getMouseWorldPosition() const
    {
        const ImVec2 mousePos = ImGui::GetMousePos();
        const float mouseX = mousePos.x - _viewPosition.x;
        const float mouseY = mousePos.y - _viewPosition.y;

        const glm::vec2 ndc = {
            (mouseX / _viewSize.x) * 2.0f - 1.0f,
            1.0f - (mouseY / _viewSize.y) * 2.0f
        };

        const glm::vec4 worldPos = glm::inverse(m_camera->getViewProjectionMatrix()) * glm::vec4(ndc, 0.0f, 1.0f);
        return {worldPos.x, worldPos.y};
    }

    void MainScene::rayPicking() const
    {
        const ImVec2 mousePos = ImGui::GetMousePos();
        const ImVec2 windowPos = ImGui::GetWindowPos();
        const ImVec2 windowSize = ImGui::GetWindowSize();

        const bool insideWindow = mousePos.x >= windowPos.x && mousePos.x <= (windowPos.x + windowSize.x) &&
                                  mousePos.y >= windowPos.y && mousePos.y <= (windowPos.y + windowSize.y);

        if (!insideWindow)
            return;
        const auto &coord = Application::m_coordinator;
        std::vector<ecs::Entity> sceneEntities = m_sceneManagerBridge->getSceneRenderedEntities(_sceneID);
        const auto mouseWorldPosition = getMouseWorldPosition();
        for (const auto entity: sceneEntities)
        {
            const auto transformComponent = coord->tryGetComponent<components::TransformComponent>(entity);
            const auto renderComponent = coord->tryGetComponent<components::RenderComponent>(entity);

            if (renderComponent && transformComponent)
            {
                if (renderComponent->get().renderable->isClicked(transformComponent->get(), mouseWorldPosition))
                {
                    m_sceneManagerBridge->setSelectedEntity(-1);
                    EntityProperties props{};
                    props.entity = entity;
                    m_sceneManagerBridge->setData(props);
                    m_sceneManagerBridge->setSelectionType(SelectionType::ENTITY);
                    return;
                }
            }
        }
        m_sceneManagerBridge->unselectEntity();
    }

    void MainScene::setHiddenLayerStatus(const bool status) const
    {
        for (const auto hiddenLayer: m_hiddenLayers)
            m_sceneManagerBridge->setLayerRenderStatus(_sceneID, hiddenLayer, status);
    }

    void MainScene::show()
    {
        setHiddenLayerStatus(false);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(480, 270), ImVec2(1920, 1080));

        if (ImGui::Begin(m_sceneName.c_str(), &m_opened, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
        {
            _viewPosition = ImGui::GetCursorScreenPos();

            m_focused = ImGui::IsWindowFocused();
            m_sceneManagerBridge->setSceneActiveStatus(_sceneID, m_focused);
            m_camera->zoomOn = m_focused;
            if (m_focused)
            {
                if (const auto &viewManager = SceneViewManager::getInstance();
                    viewManager->getSelectedScene() != static_cast<int>(_sceneID))
                {
                    viewManager->setSelectedScene(_sceneID);
                    m_sceneManagerBridge->unselectEntity();
                }
            }

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing())
                rayPicking();

            renderView();
            renderGizmo();
        }
        ImGui::End();
        ImGui::PopStyleVar();
        setHiddenLayerStatus(true);
    }

    void MainScene::update()
    {
        setHiddenLayerStatus(false);
        if (!m_opened)
            return;
        handleKeyEvents();

        m_framebuffer->bind();
        runEngine(_sceneID, RenderingType::FRAMEBUFFER);
        m_framebuffer->unbind();
        setHiddenLayerStatus(true);
    }

    void MainScene::addDefaultCameraToLayer(const scene::LayerId id) const
    {
        auto &layerStack = m_sceneManagerBridge->getSceneLayers(_sceneID);
        const std::shared_ptr<layer::Layer> layer = layerStack[id];
        const auto newCamera = std::make_shared<camera::OrthographicCameraController>(_viewSize.x / _viewSize.y, true);
        layer->attachCamera(newCamera);
    }

}
