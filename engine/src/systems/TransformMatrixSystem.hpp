//// TransformMatrixSystem ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        01/06/2025
//  Description: System responsible of updating the world matrix from the transfom component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/QuerySystem.hpp"
#include "components/Transform.hpp"
#include "components/SceneComponents.hpp"
#include "components/RenderContext.hpp"
namespace nexo::system {
    class TransformMatrixSystem final : public ecs::QuerySystem<
        ecs::Write<components::TransformComponent>,
        ecs::Read<components::SceneTag>,
        ecs::WriteSingleton<components::RenderContext>> {
			public:
               void update();
           private:
               static glm::mat4 createTransformMatrix(const components::TransformComponent &transform);
	};
}
