//// StaticMesh.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        06/05/2025
//  Description: Header file for the static mesh component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "renderer/Attributes.hpp"
#include "renderer/VertexArray.hpp"

namespace nexo::components {

    struct StaticMeshComponent {
        std::shared_ptr<renderer::NxVertexArray> vao;

        renderer::RequiredAttributes meshAttributes;

        struct Memento {
            std::shared_ptr<renderer::NxVertexArray> vao;
        };

        void restore(const Memento &memento)
        {
            vao = memento.vao;
        }

        [[nodiscard]] Memento save() const
        {
            return {vao};
        }
    };

}
