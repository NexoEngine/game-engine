//// SceneComponents.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        12/11/2024
//  Description: Header file for the scene components
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <set>

namespace nexo::components {

    struct InActiveScene {
        std::set<unsigned int> sceneIds;
    };

    struct SceneTag {
        unsigned int id{};
        bool isActive = true;
        bool isRendered = true;

        struct Memento {
            unsigned int id;
            bool isActive;
            bool isRendered;
        };

        void restore(const Memento &memento)
        {
            id = memento.id;
            isActive = memento.isActive;
            isRendered = memento.isRendered;
        }

        [[nodiscard]] Memento save() const
        {
            return {id, isActive, isRendered};
        }
    };

}
