//// Name.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/06/2025
//  Description: Header file for the name component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

namespace nexo::components {
    struct NameComponent {
        std::string name;

        struct Memento {
            std::string name;
        };

        void restore(const Memento& memento)
        {
            name = memento.name;
        }

        [[nodiscard]] Memento save() const
        {
            return {name};
        }
    };
}
