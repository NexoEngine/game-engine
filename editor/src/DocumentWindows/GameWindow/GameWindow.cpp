//// GameWindow.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Cardonne
//  Date:        2025-06-24
//  Description: Main implementation file for GameWindow
//
///////////////////////////////////////////////////////////////////////////////

#include "GameWindow.hpp"

namespace nexo::editor {

    void GameWindow::setSceneId(const unsigned int sceneId)
    {
        m_sceneId = sceneId;
    }

    void GameWindow::setSceneUuid(const std::string& sceneUuid)
    {
        m_sceneUuid = sceneUuid;
    }

}
