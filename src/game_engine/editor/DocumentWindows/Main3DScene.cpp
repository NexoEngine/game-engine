/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** Main3DScene.cpp
*/

#include "imgui.h"
#include "rlImGui.h"

#include "game_engine/editor/DocumentWindows/Main3DScene.hpp"
#include "CameraHandling.hpp"
#include "EntityHandling.hpp"
#include "LightHandling.hpp"
#include "Math.hpp"
#include "EventHandling.hpp"
#include "Rendering.hpp"

#include <ImGuizmo.h>
#include <rlgl.h>
#include <loguru/loguru.hpp>
#include <limits>

#include "game_engine/ecs/components/Physics.hpp"
#include "../../TestBehaviour.hpp"

class input;

engine::editor::Main3DScene::Main3DScene()
    : _camera(engine::createMovableCamera(Vector3{7.0f, 7.0f, 7.0f}, Vector3{0.0f, 0.0f, 0.0f})),
    _sceneID(engine::createScene())
{
    setupScene();
}

engine::editor::Main3DScene::~Main3DScene()
{
}

void engine::editor::Main3DScene::setup()
{
    setupWindow();
    setupGridShader();
    engine::enableDebug();
}

void engine::editor::Main3DScene::shutdown()
{

}

void engine::editor::Main3DScene::show()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, 400), ImVec2((float)GetScreenWidth(), (float)GetScreenHeight()));

    if (ImGui::Begin("3D View", &_opened, ImGuiWindowFlags_NoScrollbar))
    {
        _viewPosition = ImGui::GetCursorScreenPos();
        _viewSize = ImGui::GetContentRegionAvail();
        ImVec2 windowPos = ImGui::GetWindowPos();
        Vector2 offset = {windowPos.x, windowPos.y};
        engine::setSceneWindowOffset(_sceneID, offset);
        renderView();
        renderToolbar();
        renderGizmo();
        if (engine::isMouseButtonDown(ecs::components::input::MouseButtons::MouseLeft) && !ImGuizmo::IsUsing())
            rayPicking();

        // Draw the rest of the window contents
        _focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
        _prevWindowSize = _currentWindowSize;
        _currentWindowSize = ImGui::GetWindowSize();


    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void engine::editor::Main3DScene::rayPicking()
{

    ecs::Entity hitEntity = -1;
    float minDistance = std::numeric_limits<float>::max();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 mousePos = ImGui::GetMousePos();
    Vector2 mousePosRelative = {mousePos.x - windowPos.x, mousePos.y - windowPos.y};
    Ray ray = math::castRayFromMouse(mousePosRelative, _camera);

    for (auto entity : _sceneManagerBridge.getSceneEntities(_sceneID)) {
        BoundingBox entityBox = engine::getEntityBoundingBox(entity);
        Matrix transformMatrix = engine::entity::getTransformMatrix(entity);
        float intersectionDistance = 0.0f;
        bool hit = math::rayOBBCollisionFromAABBTransformed(ray.position, ray.direction, entityBox.min, entityBox.max, transformMatrix, intersectionDistance);
        if (hit && intersectionDistance < minDistance) {
            hitEntity = entity;
            minDistance = intersectionDistance;
        }   
    }

    if (hitEntity != -1) {
        _sceneManagerBridge.setSelectedEntity(hitEntity);
    }
}

void engine::editor::Main3DScene::update()
{
    if (!_opened)
        return;
    handleWindowResize();
    handleKeyEvents();

    _selectedEntity = _sceneManagerBridge.getSelectedEntity();

    // limit fps to _targetFPS with clock, frameTime in ms
    auto frameTimeMs = static_cast<long long>(1.0 / _targetFPS * 1000.0);

    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();

    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();


    if (elapsedTime < frameTimeMs)
        return;
    _sceneManagerBridge.deactivateAllScenes();
    engine::activateScene(_sceneID);
    engine::update(_sceneID);
    engine::startRendering(_sceneID, _camera->getCameraID());
    engine::renderGrid(_sceneID, _camera->getCameraID());
    engine::renderAllEntities(_sceneID, _camera->getCameraID());
    engine::endRendering(_sceneID);
    lastTime = currentTime;

}

void engine::editor::Main3DScene::setupWindow()
{
    ImVec2 pos = ImVec2(118, 24);
    ImVec2 size = ImVec2(1389, 804);
    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
    _currentWindowSize = size;
    _viewSize = size;
}

void engine::editor::Main3DScene::setupGridShader()
{
    _gridShader = LoadShader("src/game_engine/ressources/shaders/grid_fading.vs", "src/game_engine/ressources/shaders/grid_fading.fs");
    _matViewLoc = GetShaderLocation(_gridShader, "matView");
    _matProjectionLoc = GetShaderLocation(_gridShader, "matProjection");
}

void engine::editor::Main3DScene::setupScene()
{
    setupCamera();
    loadEntities();
    activateScene(_sceneID);
}

void engine::editor::Main3DScene::setupCamera()
{
    engine::attachCamera(_sceneID, _camera);

    ImGuizmo::SetOrthographic(false);
}

void engine::editor::Main3DScene::loadEntities()
{
    ecs::Entity cube = engine::createCube({0, 0.5, 0}, 2, 2, 2, WHITE, true);
    //ecs::Entity model = engine::createModel3D("src/game_engine/ressources/models/guy.iqm", {0, 0, 0});
    ecs::Entity cube2 = engine::createCube({0, 0, 0}, 10, 1, 10, WHITE, true);
    auto light = engine::createLight(engine::core::POINT, {-2, 1, -2}, {0, 0, 0}, YELLOW);
    auto light2 = engine::createLight(engine::core::POINT, {2, 1, 2}, {0, 0, 0}, RED);
    auto light3 = engine::createLight(engine::core::POINT, {-2, 1, 2}, {0, 0, 0}, GREEN);
    auto light4 = engine::createLight(engine::core::POINT, {2, 1, -2}, {0, 0, 0}, BLUE);
    _selectedEntity = cube;
    engine::addEntityToScene(cube, _sceneID);
    engine::addEntityToScene(cube2, _sceneID);
}

void engine::editor::Main3DScene::handleWindowResize()
{
    if (!isWindowResized())
        return;
    _camera->updateRenderTextureSize(static_cast<int>(_viewSize.x), static_cast<int>(_viewSize.y));
    ImGuizmo::SetRect(_viewPosition.x, _viewPosition.y, _viewSize.x, _viewSize.y);
}



void engine::editor::Main3DScene::handleKeyEvents()
{
    using ecs::components::input::Keys;
    if (engine::isKeyPressed(Keys::KeyG))
    {
        _currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
    }
    else if (engine::isKeyPressed(Keys::KeyR))
    {
        _currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
    }
    else if (engine::isKeyPressed(Keys::KeyE))
    {
        _currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
    }
}

void engine::editor::Main3DScene::renderGrid(void)
{
    Matrix view = GetCameraMatrix(_camera->getCamera());
    Vector2 screenSize = _camera->getRenderTextureSize();
    Matrix projection = MatrixPerspective(_camera->getFov() * DEG2RAD, screenSize.x / screenSize.y, _nearPlane, _farPlane);
    SetShaderValueMatrix(_gridShader, _matViewLoc, view);
    SetShaderValueMatrix(_gridShader, _matProjectionLoc, projection);
    RenderTexture renderTexture = _camera->getRenderTexture();
    BeginTextureMode(renderTexture);
        BeginMode3D(_camera->getCamera());
            rlDisableDepthTest();
            BeginShaderMode(_gridShader);
                rlPushMatrix();
                rlTranslatef(0.0f, 0.0f, 0.0f);
                rlScalef(1000.0f, 1.0f, 1000.0f);

                rlBegin(RL_TRIANGLES);
                    rlVertex3f(-1, 1, -0);
                    rlVertex3f(-1, -1, 0);
                    rlVertex3f(1, -1, 0);
                    rlVertex3f(1, 1, 0);
                    rlVertex3f(-1, 1, 0);
                    rlVertex3f(1, -1, 0);
                rlEnd();
                rlPopMatrix();
            EndShaderMode();
            rlEnableDepthTest();
        EndMode3D();
    EndTextureMode();
}

void engine::editor::Main3DScene::renderToolbar()
{
    ImVec2 buttonSize = ImVec2(40, 40); // Set the desired width and height
    float padding = 0.0f; // Set the desired padding between buttons

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(padding, padding));
    ImGui::SetCursorScreenPos(ImVec2(_viewPosition.x + 10, _viewPosition.y + 10));
    if (ImGui::Button("Orthographic")) {
        // Switch to orthographic camera mode
        //engine::camera::setMode(CameraMode::ORTHOGRAPHIC);
    }

    ImGui::SameLine();
    if (ImGui::Button("Perspective")) {
        // Switch to perspective camera mode
        //engine::camera::setMode(CameraMode::PERSPECTIVE);
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_HAND_POINTER, buttonSize)) {
        // Set cursor to normal mode
        //engine::input::setCursorMode(CursorMode::NORMAL);
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_HAND, buttonSize)) {
        // Set cursor to pan mode
        //engine::input::setCursorMode(CursorMode::PAN);
    }

    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_ARROWS_SPIN, buttonSize)) {
        // Set cursor to rotate mode
        //engine::input::setCursorMode(CursorMode::ROTATE);
    }

    ImGui::SameLine();
    // make the slider not too large
    ImGui::PushItemWidth(100); // Set width for slider
    if (ImGui::DragInt("Target FPS", &_targetFPS, 1, 1, 120)) {
        // Set target FPS
    }
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
}

void engine::editor::Main3DScene::renderGizmo()
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetID(_selectedEntity);
    ImGuizmo::SetRect(_viewPosition.x, _viewPosition.y, _viewSize.x, _viewSize.y);
    Matrix viewMatrix = _camera->getViewMatrix();
    Matrix projectionMatrix = _camera->getProjectionMatrix(_currentWindowSize.x / _currentWindowSize.y, 0.1f, 1000.0f);
    Matrix objectMatrix = engine::entity::getTransformMatrix(_selectedEntity);
    auto &transf = engine::entity::getComponent<ecs::components::physics::transform_t>(_selectedEntity);

    float16 viewMatrixFloats = MatrixToFloatV(viewMatrix);
    float16 projectionMatrixFloats = MatrixToFloatV(projectionMatrix);
    float16 objectMatrixFloats = MatrixToFloatV(objectMatrix);

    ImGuizmo::Enable(true);
    ImGuizmo::Manipulate(viewMatrixFloats.v, projectionMatrixFloats.v,
                         _currentGizmoOperation,
                         ImGuizmo::MODE::LOCAL,
                         objectMatrixFloats.v);

    Vector3 translation = {0};
    Vector3 rotation = {0};
    Vector3 scale = {0};

    math::decomposeTransformMatrixEuler(engine::math::matrixFromFloat16(objectMatrixFloats), translation, rotation, scale);

    if (ImGuizmo::IsUsing())
    {
        transf.pos = translation;
        transf.rotation = rotation;
        transf.scale = scale;
        engine::entity::setTransformMatrix(_selectedEntity, engine::math::matrixFromFloat16(objectMatrixFloats));
    }
}

void engine::editor::Main3DScene::renderView()
{
    rlImGuiImageRenderTexture(&_camera->getRenderTexture());
}

bool engine::editor::Main3DScene::isWindowResized() const
{
    return _currentWindowSize.x != _prevWindowSize.x || _currentWindowSize.y != _prevWindowSize.y;
}
