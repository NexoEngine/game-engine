//// OnSceneDeletedSystem.hpp /////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        12/11/2024
//  Description: Header file for the system handling the deleting of the scene
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/System.hpp"
#include "core/scene/SceneManager.hpp"

namespace nexo::system {
    class OnSceneDeleted : public ecs::System {
        public:
            void onSceneDelete(scene::SceneId sceneId) const;
    };
}