//// StaticMesh.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
    };

}
