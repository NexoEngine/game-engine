//// Render.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/11/2024
//  Description: Header file for the render component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

namespace nexo::components
{
    enum class PrimitiveType
    {
        UNKNOWN,
        CUBE,
        MESH,
        BILLBOARD,
        _COUNT
    };

    struct RenderComponent {
        bool isRendered = true;
        PrimitiveType type = PrimitiveType::MESH;

        RenderComponent() = default;

        struct Memento {
            bool isRendered;
            PrimitiveType type;
        };

        void restore(const Memento &memento)
        {
            isRendered = memento.isRendered;
            type = memento.type;
        }

        [[nodiscard]] Memento save() const
        {
            return {
                isRendered,
                type
            };
        }
    };
}
