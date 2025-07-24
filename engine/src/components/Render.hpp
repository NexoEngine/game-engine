//// Render.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
