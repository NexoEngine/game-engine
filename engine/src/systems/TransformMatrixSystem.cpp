//// TransformMatrixSystem.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        01/06/2025
//  Description: Source file for the system responsible of updating the world matrixes
//
///////////////////////////////////////////////////////////////////////////////

#include "TransformMatrixSystem.hpp"
#include "components/Transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <tracy/Tracy.hpp>

namespace nexo::system {
    void TransformMatrixSystem::update()
    {
        ZoneScoped;
        ZoneName("Transform Matrix Update", 22);

        const auto &renderContext = getSingleton<components::RenderContext>();
        if (renderContext.sceneRendered == -1)
            return;

        const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

        {
            ZoneScopedN("Matrix Calculations");
            ZoneValue(static_cast<int64_t>(entities.size()));

            for (const ecs::Entity entity : entities) {
                auto &sceneTag = getComponent<components::SceneTag>(entity);
                if (sceneTag.id != sceneRendered)
                    continue;
                auto &transform = getComponent<components::TransformComponent>(entity);
                transform.localMatrix = createTransformMatrix(transform);
                transform.worldMatrix = transform.localMatrix;
            }
        }
    }

    glm::mat4 TransformMatrixSystem::createTransformMatrix(const components::TransformComponent &transform)
    {
        ZoneScoped;
        ZoneName("Create Transform Matrix", 21);

        return glm::translate(glm::mat4(1.0f), transform.pos) *
               glm::toMat4(transform.quat) *
               glm::scale(glm::mat4(1.0f), transform.size);
    }
}
