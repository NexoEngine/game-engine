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

#include "EditorScene.hpp"
#include <imgui.h>

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
                m_timecodeElapsed = 0.0f;
                
                auto& app = getApp();
                app.setGameState(nexo::GameState::EDITOR_MODE);
                
                m_currentTimecodeIndex = (m_currentTimecodeIndex + 1) % m_timecodeSeconds.size();
            }
        }
    }

    void EditorScene::skipVideosToPreviousKeyframe()
    {
        auto& app = getApp();
        //app.setGameState(nexo::GameState::PLAY_MODE);
        for (auto &entity : app.m_coordinator->getAllEntitiesWith<components::VideoComponent>()) {
            if (!app.m_coordinator->entityHasComponent<components::SceneTag>(entity)) {
                continue;
            }
            const auto &sceneTag = app.m_coordinator->getComponent<components::SceneTag>(entity);
            if (sceneTag.id != static_cast<unsigned int>(m_sceneId)) {
                continue;
            }
            auto &videoComponent = app.m_coordinator->getComponent<components::VideoComponent>(entity);
            videoComponent.skipToPreviousKeyframe();
        }
    }

    void EditorScene::skipVideosToNextKeyframe()
    {
        // if (m_timecodeSeconds.empty() || m_isTimecodeActive) {
        //     return;
        // }
        //
        // m_isTimecodeActive = true;
        // m_timecodeElapsed = 0.0f;
        
        auto& app = getApp();
        //app.setGameState(nexo::GameState::PLAY_MODE);
        for (auto &entity : app.m_coordinator->getAllEntitiesWith<components::VideoComponent>()) {
            if (!app.m_coordinator->entityHasComponent<components::SceneTag>(entity)) {
                continue;
            }
            const auto &sceneTag = app.m_coordinator->getComponent<components::SceneTag>(entity);
            if (sceneTag.id != static_cast<unsigned int>(m_sceneId)) {
                continue;
            }
            auto &videoComponent = app.m_coordinator->getComponent<components::VideoComponent>(entity);
            videoComponent.skipToNextKeyframe();
        }
    }
}
