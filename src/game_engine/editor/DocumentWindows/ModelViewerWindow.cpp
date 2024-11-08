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
#include "loguru/loguru.hpp"

namespace engine::editor {
    ModelViewerWindow::ModelViewerWindow()
        : _camera(engine::createCamera(Vector3{7.0f, 7.0f, 7.0f},Vector3{0.0f, 2.0f, 0.0f})),
        _sceneID(engine::createScene()),
        _selectedSceneIdx(0)
    {
        _opened = false;
        _modelLoaded = false;
        _prevWindowSize = ImVec2(0, 0);
    }

    ModelViewerWindow::~ModelViewerWindow()
    {
        if (_modelLoaded)
            engine::destroyEntity(_importedEntity);
    }

    void ModelViewerWindow::setup()
    {
        engine::attachCamera(_sceneID, _camera);
        _camera->updateRenderTextureSize(400, 400);
        _currentWindowSize = ImVec2(400, 400);
    }

    void ModelViewerWindow::shutdown()
    {
    }

    bool ModelViewerWindow::isWindowResized() const
    {
        return _currentWindowSize.x != _prevWindowSize.x || _currentWindowSize.y != _prevWindowSize.y;
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
        ImGui::Begin("Import", &_opened, ImGuiWindowFlags_NoScrollbar);

        showPreviewSection();
        ImGui::SameLine();
        showControlsSection();

        _currentWindowSize = ImGui::GetWindowSize();
        ImGui::End();
    }

    void ModelViewerWindow::showPreviewSection() {
        ImGui::BeginChild("Preview", ImVec2(ImGui::GetContentRegionAvail().x * 0.6f, ImGui::GetContentRegionAvail().y), true, ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY);
        rlImGuiImageRenderTextureFit(&_camera->getRenderTexture(), true);
        ImGui::EndChild();
    }

    void ModelViewerWindow::showControlsSection() {
        ImGui::BeginChild("Controls", ImVec2(0, 0), true, ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY);

        showImportPathButton();
        showSceneComboBox();
        showImportButton();

        ImGui::EndChild();
    }

    void ModelViewerWindow::showImportPathButton() {
        auto path = fileDialogButton("Import path");
        if (!path.empty()) {
            if (_modelLoaded) {
                engine::removeEntityFromScene(_importedEntity, _sceneID);
                engine::destroyEntity(_importedEntity);
            }
            _assetPath = std::string(path);
            _importedEntity = engine::createModel3D(_assetPath.c_str(), {0, 0, 0}, WHITE);
            engine::addEntityToScene(_importedEntity, _sceneID);
            _modelLoaded = true;
        }
        if (_modelLoaded) {
            ImGui::SameLine(); ImGui::Text("%s", _assetPath.c_str());
        }
    }

    void ModelViewerWindow::showSceneComboBox() {
        if (ImGui::BeginCombo("Scene",
            std::to_string(getSceneManager().getSceneIDs().at(0)).c_str(),
            ImGuiComboFlags_HeightRegular | ImGuiComboFlags_PopupAlignLeft))
        {
            for (int n = 0; n < getSceneManager().getSceneIDs().size(); n++)
            {
                const auto current_scene_id = getSceneManager().getSceneIDs().at(n);
                if (current_scene_id == _sceneID)
                    continue;
                const bool is_selected = (_selectedSceneIdx == n);
                if (ImGui::Selectable(std::to_string(current_scene_id).c_str(), is_selected))
                    _selectedSceneIdx = n;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    void ModelViewerWindow::showImportButton() {
        ImGui::NewLine();
        if (ImGui::Button("Import")) {
            engine::removeEntityFromScene(_importedEntity, _sceneID);
            engine::addEntityToScene(_importedEntity, _selectedSceneIdx);
            _assetPath = "";
            _importedEntity = 0;
            _modelLoaded = false;
            _opened = false;
        }
    }


    void ModelViewerWindow::update()
    {
        if (!_opened)
            return;

        if (isWindowResized()) {
            _camera->updateRenderTextureSize(static_cast<int>(_currentWindowSize.x * 0.6f), static_cast<int>(_currentWindowSize.y));
            _prevWindowSize = _currentWindowSize;
        }

        _sceneManagerBridge.deactivateAllScenes();
        engine::activateScene(_sceneID);
        engine::update(_sceneID);
        engine::renderTextureMode(_sceneID, _camera->getCameraID());
    }
}
