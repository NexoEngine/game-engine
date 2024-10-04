/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** ModelViewerWindow.cpp
*/

#include "imgui.h"
#include "rlImGui.h"
#include "game_engine/editor/DocumentWindows/ModelViewerWindow.hpp"

#include <CameraHandling.hpp>
#include <EntityHandling.hpp>

#include <iostream>
#include "tinyfiledialogs.h"

#include <string>

#include "game_engine/GameEngine.hpp"

namespace engine::editor {
    ModelViewerWindow::ModelViewerWindow()
        : _camera(engine::createCamera(Vector3{7.0f, 7.0f, 7.0f}, Vector3{0.0f, 2.0f, 0.0f})),
        _sceneID(engine::createScene())
    {
        _opened = false;
        _modelLoaded = false;
    }

    ModelViewerWindow::~ModelViewerWindow()
    {
        if (_modelLoaded)
            engine::destroyEntity(_importedEntity);
    }

    void ModelViewerWindow::setup()
    {
        engine::attachCamera(_sceneID, _camera);
        _camera.updateRenderTextureSize(400, 400);
    }

    void ModelViewerWindow::shutdown()
    {
    }

    std::string fileDialogButton(const char* label)
    {
        if (ImGui::Button(label))
        {
            const char* filePath = tinyfd_openFileDialog(
                "Open File",
                "",
                0,
                nullptr,
                nullptr,
                0
            );

            if (!filePath) {
                return "";
            }
            return std::string(filePath);
        }
        return "";
    }

   void ModelViewerWindow::show() {


        /*if (!_modelLoaded) {
            const char* filePath = tinyfd_openFileDialog(
                "Open File",
                "",
                0,
                nullptr,
                nullptr,
                0
            );

            if (!filePath) {
                _opened = false;
                return;
            }
            _assetPath = std::string(filePath);
            _importedEntity = engine::createModel3D(_assetPath.c_str(), {0, 0, 0}, WHITE);
            engine::addEntityToScene(_sceneID, _importedEntity);
            _modelLoaded = true;
        }*/
        /*ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_Always);*/
        ImGui::Begin("Import", &_opened, ImGuiWindowFlags_NoScrollbar);

        auto path = fileDialogButton("Import path");
        if (path != "") {
            _assetPath = std::string(path);
            _importedEntity = engine::createModel3D(_assetPath.c_str(), {0, 0, 0}, WHITE);
            engine::addEntityToScene(_sceneID, _importedEntity);
            _modelLoaded = true;
        }


        _focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);


        rlImGuiImageRenderTexture(&_camera.getRenderTexture());

        /*// file input with a button on its right to directly search in windows
        static char buf[128] = "";
        ImGui::InputText("File", buf, IM_ARRAYSIZE(buf));
        ImGui::SameLine();
        if (ImGui::Button("...")) {
            const char* filePath = tinyfd_openFileDialog(
                "Open File",
                "",
                0,
                nullptr,
                nullptr,
                0
            );

            if (!filePath) {
                _opened = false;
                return;
            }
            _assetPath = std::string(filePath);
            _importedEntity = engine::createModel3D(_assetPath.c_str(), {0, 0, 0}, WHITE);
            engine::addEntityToScene(_sceneID, _importedEntity);
            _modelLoaded = true;
        }*/



        ImGui::End();
   }


    void ModelViewerWindow::update()
    {
        if (!_opened)
            return;

        _sceneManagerBridge.deactivateAllScenes();
        engine::activateScene(_sceneID);
        engine::update(_sceneID);
        engine::renderTextureMode(_sceneID, _camera.getCameraID());
        //engine::update(_sceneID);
    }
}
