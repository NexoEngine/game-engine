//// Timecode.cpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/09/2025
//  Description: Timecode system for timed playback control
//
///////////////////////////////////////////////////////////////////////////////

#include <imgui.h>
#include "EditorScene.hpp"

namespace nexo::editor {

    void EditorScene::handleTimecodeUpdate()
    {
        if (!m_isTimecodeActive || m_timecodeSeconds.empty()) {
            return;
        }

        m_timecodeElapsed += ImGui::GetIO().DeltaTime;

        if (m_currentTimecodeIndex < static_cast<int>(m_timecodeSeconds.size())) {
            float currentDuration = m_timecodeSeconds[m_currentTimecodeIndex];

            if (m_timecodeElapsed >= currentDuration) {
                m_isTimecodeActive = false;
                m_timecodeElapsed  = 0.0f;

                auto &app = getApp();
                app.setGameState(nexo::GameState::EDITOR_MODE);

                m_currentTimecodeIndex = (m_currentTimecodeIndex + 1) % static_cast<int>(m_timecodeSeconds.size());
            }
        }
    }

    void EditorScene::skipVideosToPreviousKeyframe() const
    {
        // app.setGameState(nexo::GameState::PLAY_MODE);
        for (const auto &entity : nexo::Application::m_coordinator->getAllEntitiesWith<components::VideoComponent>()) {
            if (!nexo::Application::m_coordinator->entityHasComponent<components::SceneTag>(entity)) {
                continue;
            }
            const auto &sceneTag = nexo::Application::m_coordinator->getComponent<components::SceneTag>(entity);
            if (sceneTag.id != static_cast<unsigned int>(m_sceneId)) {
                continue;
            }
            auto &videoComponent = nexo::Application::m_coordinator->getComponent<components::VideoComponent>(entity);
            videoComponent.skipToPreviousKeyframe();
        }
    }

    void EditorScene::skipVideosToNextKeyframe() const
    {
        // app.setGameState(nexo::GameState::PLAY_MODE);
        for (const auto &entity : nexo::Application::m_coordinator->getAllEntitiesWith<components::VideoComponent>()) {
            if (!nexo::Application::m_coordinator->entityHasComponent<components::SceneTag>(entity)) {
                continue;
            }
            const auto &sceneTag = nexo::Application::m_coordinator->getComponent<components::SceneTag>(entity);
            if (sceneTag.id != static_cast<unsigned int>(m_sceneId)) {
                continue;
            }
            auto &videoComponent = nexo::Application::m_coordinator->getComponent<components::VideoComponent>(entity);
            videoComponent.skipToNextKeyframe();
        }
    }
} // namespace nexo::editor
