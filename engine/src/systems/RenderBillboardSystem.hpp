//// RenderBillboardSystem.hpp ////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/05/2025
//  Description: Header file for rendering the billboards
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "components/BillboardMesh.hpp"
#include "components/MaterialComponent.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/Transform.hpp"
#include "ecs/GroupSystem.hpp"

namespace nexo::system {
    class RenderBillboardSystem final
        : public ecs::GroupSystem<
              ecs::Owned<ecs::Read<components::BillboardComponent>>,
              ecs::NonOwned<ecs::Read<components::TransformComponent>, ecs::Read<components::MaterialComponent>,
                            ecs::Read<components::SceneTag>>,
              ecs::WriteSingleton<components::RenderContext>> {
       public:
        /** @brief Updates the rendering of billboards in the active scene.
         * This method retrieves the currently active scene from the RenderContext singleton.
         * It then partitions entities by their SceneTag to find those belonging to the active scene.
         * For each billboard entity found, it creates draw commands using the entity's
         * BillboardComponent, MaterialComponent, and TransformComponent.
         * The draw commands are then added to the camera pipelines in the RenderContext.
         * If no billboard entities are found for the active scene, a warning is logged.
         */
        void update();

       private:
        /** @brief Sets up the lighting uniforms in the given shader.
         *
         * This static helper function binds the provided shader and sets uniforms for ambient, directional,
         * point, and spotlights based on the current lightContext data. After updating the uniforms, the shader is
         * unbound.
         * @param cmd The draw command containing the shader to set up.
         * @param lightContext The light context containing lighting information for the scene.
         * @note The light context must contain valid values for:
         *  - ambientLight
         *  - directionalLights (and directionalLightCount)
         *  - pointLights (and pointLightCount)
         *  - spotLights (and spotLightCount)
         */
        static void setupLights(renderer::DrawCommand& cmd, const components::LightContext& lightContext);
    };
} // namespace nexo::system
