//// GameWindow.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Cardonne
//  Date:        2025-06-24
//  Description: Main implementation file for GameWindow
//
///////////////////////////////////////////////////////////////////////////////

#include "GameWindow.hpp"

namespace nexo::editor {

    void GameWindow::setSceneId(unsigned int sceneId)
    {
        m_sceneId = sceneId;
    }

    void GameWindow::setSceneUuid(const std::string& sceneUuid)
    {
        m_sceneUuid = sceneUuid;
    }

    void GameWindow::setParentEditorScene(EditorScene* parentScene)
    {
        m_parentEditorScene = parentScene;
    }

}
