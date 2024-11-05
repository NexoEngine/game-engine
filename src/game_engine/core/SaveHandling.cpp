/*** SaveHandling.cpp *********************************************************
*
*  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
*  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
*  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
*  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
*  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
*
*  Author:      Guillaume Hein
*  Date:        19/10/2024
*  Description: Implementation of save functions for engine class.
*
******************************************************************************/

#include "game_engine/SaveHandling.hpp"

engine::save::json engine::Engine::saveEntityComponents(ecs::Entity entity) const
{
	return _coordinator->saveEntityComponents(entity);
}

void engine::Engine::loadEntityComponents(ecs::Entity entity, const engine::save::json& data)
{
	_coordinator->loadEntityComponents(entity, data);
}

namespace engine {
	engine::save::json engine::saveEntityComponents(ecs::Entity entity)
	{
		return Engine::getInstance()->saveEntityComponents(entity);
	}

	void loadEntityComponents(ecs::Entity entity, const engine::save::json& json)
	{
		Engine::getInstance()->loadEntityComponents(entity, json);
	}

} // namespace engine
