/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** Main3DScene.cpp
*/

#include "imgui.h"
#include "rlImGui.h"

#include "game_engine/editor/DocumentWindows/Main3DScene.hpp"
#include <CameraHandling.hpp>
#include <EntityHandling.hpp>
#include <Math.hpp>

#include <ImGuizmo.h>
#include <rlgl.h>
#include <loguru/loguru.hpp>

#include "game_engine/ecs/components/Physics.hpp"
#include "../../TestBehaviour.hpp"

class input;

engine::editor::Main3DScene::Main3DScene()
    : _camera(engine::createCamera(Vector3{7.0f, 7.0f, 7.0f}, Vector3{0.0f, 0.0f, 0.0f})),
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
        renderView();
        renderToolbar();
        renderGizmo();

        // Draw the rest of the window contents
        _focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
        _prevWindowSize = _currentWindowSize;
        _currentWindowSize = ImGui::GetWindowSize();


    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void engine::editor::Main3DScene::update()
{
    if (!_opened)
        return;
    handleWindowResize();

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
    engine::renderTextureMode(_sceneID, _camera.getCameraID());
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
    ecs::Entity cube = engine::createCube({0, 2, 2}, 4, 4, 4, RED, true);
    _selectedEntity = cube;
    //engine::setRotation(cube, {deg2rad(30), 0, 0});
    //engine::setRotation(cube, {deg2rad(-10), 0, 0});
    auto behave = engine::createBehavior<input>();
    engine::attachBehavior(cube, behave);
    engine::addEntityToScene(cube, _sceneID);
    //ecs::Entity cube2 = engine::createCube({5, 1, 0}, 2, 2, 2);
}

void engine::editor::Main3DScene::handleWindowResize()
{
    if (!isWindowResized())
        return;
    _camera.updateRenderTextureSize(static_cast<int>(_viewSize.x), static_cast<int>(_viewSize.y));
    ImGuizmo::SetRect(_viewPosition.x, _viewPosition.y, _viewSize.x, _viewSize.y);
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



/*Matrix convertToImGuizmo(const Matrix& mat) {
    // Create a swap matrix
    auto mat2 = MatrixTranspose(mat);
    return mat2;

    std::swap(mat2.m1, mat2.m2);
    std::swap(mat2.m4, mat2.m8);
    std::swap(mat2.m5, mat2.m10);
    std::swap(mat2.m6, mat2.m9);
    std::swap(mat2.m7, mat2.m11);




    return mat2;
}*/

inline float16 convertToImGuizmo(const Matrix &mat) {
    // Create a swap matrix

    auto mat2 = float16{
        mat.m2, mat.m1, mat.m0, mat.m3,   // Swap X (m0, m4, m8, m12) with Z (m2, m6, m10, m14)
        mat.m6, mat.m5, mat.m4, mat.m7,   // Keep Y axis the same
        mat.m10, mat.m9, mat.m8, mat.m11, // Swap Z (m2, m6, m10, m14) with X (m0, m4, m8, m12)
        mat.m14, mat.m13, mat.m12, mat.m15 // Translation component remains the same
    };
    return mat2;
}

void engine::editor::Main3DScene::renderGizmo()
{
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetID(_selectedEntity);
    // Assuming you have these functions implemented
    Matrix viewMatrix = _camera.getViewMatrix();
    Matrix projectionMatrix = _camera.getProjectionMatrix(_currentWindowSize.x / _currentWindowSize.y, 0.1f, 1000.0f);

    // Assuming you have a way to get the selected object's transform
    //Transform objectTransform = engine::entity::getTransform(_selectedEntity);

    // Convert transform to matrix
    Matrix objectMatrix = engine::entity::getTransformMatrix(_selectedEntity);

    /*LOG_F(INFO, "objectMatrix: %f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",
          objectMatrix.m0, objectMatrix.m4, objectMatrix.m8, objectMatrix.m12,
            objectMatrix.m1, objectMatrix.m5, objectMatrix.m9, objectMatrix.m13,
            objectMatrix.m2, objectMatrix.m6, objectMatrix.m10, objectMatrix.m14,
            objectMatrix.m3, objectMatrix.m7, objectMatrix.m11, objectMatrix.m15);*/

    // Convert raylib matrices to float arrays for ImGuizmo
    float16 viewMatrixFloats = MatrixToFloatV(viewMatrix);
    float16 projectionMatrixFloats = MatrixToFloatV(projectionMatrix);
    float16 objectMatrixFloats = MatrixToFloatV(objectMatrix);

    // Set ImGuizmo context (window size, etc.)

    // Manipulate the matrix with ImGuizmo
    ImGuizmo::Enable(true);
    if (!ImGuizmo::Manipulate(viewMatrixFloats.v, projectionMatrixFloats.v,
                         ImGuizmo::OPERATION::UNIVERSAL,
                         ImGuizmo::MODE::WORLD,
                         objectMatrixFloats.v)) {
    }
    auto viewManipulateRight = _viewPosition.x + _viewSize.x;
    auto viewManipulateTop = _viewPosition.y;
    ImGuizmo::ViewManipulate(viewMatrixFloats.v, 100, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);


    _camera.setViewMatrix(engine::math::matrixFromFloat16(viewMatrixFloats));



    // Check if the matrix was changed
    if (ImGuizmo::IsUsing())
    {

        /*
        // Update the object's transform with the new matrix
        engine::UpdateSelectedObjectTransform(updatedMatrix);*/
        float translation[3];
        float rotation[3];
        float scale[3];

        ImGuizmo::DecomposeMatrixToComponents(objectMatrixFloats.v, translation, rotation, scale);
        Vector3 pos = {translation[0], translation[1], translation[2]};
        Vector3 rot = {rotation[0], rotation[1], rotation[2]};
        //std::cout << rot.x << " " << rot.y << " " << rot.z << std::endl;
        Vector3 sca = {scale[0], scale[1], scale[2]};

        switch (_lastGizmoOperationOver) {
            case ImGuizmo::OPERATION::TRANSLATE: {
                //LOG_F(INFO, "TRANSLATE");
                auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(_selectedEntity);
                //engine::setScale(_selectedEntity, sca);
                //engine::setRotation(_selectedEntity, rot);
                transform.pos = pos;
                break;
            }
            case ImGuizmo::OPERATION::ROTATE:
                //LOG_F(INFO, "ROTATE");
                engine::setScale(_selectedEntity, sca);
                engine::setRotation(_selectedEntity, rot);
                break;
            default:
                break;
        }


        //engine::entity::setTransform(_selectedEntity, pos, rot, sca);
    } else {
        if (ImGuizmo::IsOver(ImGuizmo::OPERATION::TRANSLATE)) {
            _lastGizmoOperationOver = ImGuizmo::OPERATION::TRANSLATE;
        } else if (ImGuizmo::IsOver(ImGuizmo::OPERATION::ROTATE)) {
            _lastGizmoOperationOver = ImGuizmo::OPERATION::ROTATE;
        } else if (ImGuizmo::IsOver(ImGuizmo::OPERATION::SCALE)) {
            _lastGizmoOperationOver = ImGuizmo::OPERATION::SCALE;
        }
    }
}

void engine::editor::Main3DScene::renderView()
{
    rlImGuiImageRenderTexture(&_camera.getRenderTexture());
}

bool engine::editor::Main3DScene::isWindowResized() const
{
    return _currentWindowSize.x != _prevWindowSize.x || _currentWindowSize.y != _prevWindowSize.y;
}
