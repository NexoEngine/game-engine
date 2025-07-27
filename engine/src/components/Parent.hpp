//// Parent.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        06/05/2025
//  Description: Header file for the parent and root component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/Definitions.hpp"
#include "assets/AssetRef.hpp"
#include "assets/Assets/Model/Model.hpp"

namespace nexo::components {

    struct ParentComponent {
        ecs::Entity parent;

        struct Memento {
            ecs::Entity parent;
        };

        void restore(const Memento& memento)
        {
            parent = memento.parent;
        }

        [[nodiscard]] Memento save() const
        {
            return {parent};
        }
    };

    struct RootComponent {
        std::string name = "Root";
        assets::AssetRef<assets::Model> modelRef;
        int childCount = 0;

        struct Memento {
            std::string name;
            assets::AssetRef<assets::Model> model;
            int childCount;
        };

        void restore(const Memento& memento)
        {
            name = memento.name;
            modelRef = memento.model;
            childCount = memento.childCount;
        }

        [[nodiscard]] Memento save() const
        {
            return {name, modelRef, childCount};
        }
    };

}
