//// TransformProperty.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/03/2025
//  Description: Header file for the transform property inspector
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AEntityProperty.hpp"

namespace nexo::editor {
    class TransformProperty : public nexo::editor::AEntityProperty {
        public:
        	using AEntityProperty::AEntityProperty;
            bool show(ecs::Entity entity);
    };
}
