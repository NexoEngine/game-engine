/*** SaveHandling.hpp **********************************************************
*
*  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
*  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
*  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
*  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
*  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
*
*  Author:      Guillaume Hein
*  Date:        05/11/2024
*  Description: File to implement save handling in engine.
*
******************************************************************************/

#pragma once

#include "game_engine/GameEngine.hpp"

namespace engine {
	engine::save::json saveEntityComponents(ecs::Entity entity);
	void loadEntityComponents(ecs::Entity entity, const engine::save::json& json);
} // namespace engine
