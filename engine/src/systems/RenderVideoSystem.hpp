//// RenderVideoSystem.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        03/09/2025
//  Description: Header file for the Video System functions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "components/MaterialComponent.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/Video.hpp"
#include "ecs/GroupSystem.hpp"

namespace nexo::system {
    class RenderVideoSystem final
        : public ecs::GroupSystem<
              ecs::Owned<ecs::Write<components::VideoComponent>>,
              ecs::NonOwned<ecs::Write<components::MaterialComponent>, ecs::Read<components::SceneTag>>,
              ecs::WriteSingleton<components::RenderContext>> {
       public:
        static int updateVideoComponent(components::VideoComponent& videoComponent, ecs::Entity entity);
        void update();
        void reset();
    };
} // namespace nexo::system
