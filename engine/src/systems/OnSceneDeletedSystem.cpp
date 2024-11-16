//// OnSceneDeletedSystem.cpp /////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        12/11/2024
//  Description: Source file for the system handling the deleting of the scene
//
///////////////////////////////////////////////////////////////////////////////

#include "OnSceneDeletedSystem.hpp"
#include "components/SceneComponents.hpp"

namespace nexo::system {
    void OnSceneDeleted::onSceneDelete(const scene::SceneId sceneId) const
    {
        std::queue<ecs::Entity> inactiveEntities;
        for (const auto entity : entities)
        {
            auto &sceneComponent = coord->getComponent<components::InActiveScene>(entity);
            if (sceneComponent.sceneIds.erase(sceneId) != 0)
                if (sceneComponent.sceneIds.empty())
                    inactiveEntities.push(entity);
        }

        while (!inactiveEntities.empty())
        {
            auto entity = inactiveEntities.front();
            coord->removeComponent<components::InActiveScene>(entity);
            inactiveEntities.pop();
        }
    }

}