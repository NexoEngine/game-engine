//// RenderCommandSystem.hpp //////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/05/2025
//  Description: Header file for the render command system
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Access.hpp"
#include "DrawCommand.hpp"
#include "GroupSystem.hpp"
#include "VertexArray.hpp"
#include "assets/Asset.hpp"
#include "components/MaterialComponent.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/StaticMesh.hpp"
#include "components/Transform.hpp"

namespace nexo::system {

    struct RenderItem {
        ecs::Entity entity;
        std::shared_ptr<renderer::NxShader> shader;
        std::shared_ptr<renderer::NxVertexArray> mesh;
        int materialIndex;
        uint32_t filterMask;
        bool isTransparent;

        uint32_t instanceIndex;

        glm::mat4 modelMatrix;
    };

    struct RenderBatch {
        std::shared_ptr<renderer::NxShader> shader;
        std::shared_ptr<renderer::NxVertexArray> mesh;
        int materialIndex;
        uint32_t filterMask;

        uint32_t instanceOffset;
        uint32_t instanceCount;
    };

    /**
     * @brief System responsible for rendering the scene.
     *
     * The RenderSystem iterates over the active cameras stored in the RenderContext singleton,
     * sets up lighting uniforms using the sceneLights data, and then renders entities that have
     * a valid RenderComponent. The system binds each camera's render target, clears the buffers,
     * and then draws each renderable entity.
     *
     * @note Component Access Rights:
     *  - READ access to components::TransformComponent (owned)
     *  - READ access to components::RenderComponent (owned)
     *  - READ access to components::SceneTag (non-owned)
     *  - WRITE access to components::RenderContext (singleton)
     *
     * @note The system uses scene partitioning to only render entities belonging to the
     * currently active scene (identified by RenderContext.sceneRendered).
     */
    class RenderCommandSystem final
        : public ecs::GroupSystem<
              ecs::Owned<ecs::Read<components::TransformComponent>, ecs::Read<components::StaticMeshComponent>,
                         ecs::Read<components::MaterialComponent>>,
              ecs::NonOwned<ecs::Read<components::SceneTag>>, ecs::WriteSingleton<components::RenderContext>> {
       public:
        /** @brief Updates the rendering of the active scene.
         * This method retrieves the currently active scene from the RenderContext singleton.
         * It then partitions entities by their SceneTag to find those belonging to the active scene.
         * For each renderable entity found, it creates draw commands using the entity's
         * TransformComponent, StaticMeshComponent, and MaterialComponent.
         * The draw commands are then added to the camera pipelines in the RenderContext.
         * If no renderable entities are found for the active scene, a warning is logged.
         */
        void update();
    };
} // namespace nexo::system
