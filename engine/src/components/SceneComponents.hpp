//// SceneComponents.hpp //////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        12/11/2024
//  Description: Header file for the scene components
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

namespace nexo::components {

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
