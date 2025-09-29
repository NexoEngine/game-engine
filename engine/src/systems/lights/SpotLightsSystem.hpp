//// SpotLightsSystem.hpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Header file for the spot lights system
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GroupSystem.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"

namespace nexo::system {

    /**
     * @brief System responsible for updating spot lights in the scene.
     *
     * This system iterates over all spot light entities in the active scene and updates
     * the RenderContext's sceneLights collection with their spot light components.
     *
     * @note Component Access Rights:
     *  - READ access to components::SpotLightComponent (owned)
     *  - READ access to components::SceneTag (non-owned)
     *  - WRITE access to components::RenderContext (singleton)
     *
     * @note The system uses scene partitioning to only process spot light entities
     * belonging to the currently active scene (identified by RenderContext.sceneRendered).
     *
     * @throws TooManySpotLightsException if the count of spot light entities exceeds MAX_SPOT_LIGHTS.
     */
    class SpotLightsSystem final : public ecs::GroupSystem<ecs::Owned<ecs::Read<components::SpotLightComponent>>,
                                                           ecs::NonOwned<ecs::Read<components::SceneTag>>,
                                                           ecs::WriteSingleton<components::RenderContext>> {
       public:
        /**
         * @brief Updates the RenderContext with spotlight from the active scene.
         * This method retrieves the currently active scene from the RenderContext singleton.
         * It then partitions entities by their SceneTag to find those belonging to the active scene.
         * If spotlight entities are found, their components are collected and used to update
         * the RenderContext's sceneLights collection. If the number of spotlight exceeds
         * MAX_SPOT_LIGHTS, a TooManySpotLightsException is thrown.
         * If no spotlight are found for the active scene, a warning is logged.
         */
        void update();
    };
} // namespace nexo::system
