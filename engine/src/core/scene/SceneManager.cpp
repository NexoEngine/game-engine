//// SceneManager.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/03/2025
//  Description: Source file for the scene manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.hpp"
#include <cmath>

namespace nexo::scene {
	SceneManager::SceneManager() {}

	void SceneManager::setCoordinator(const std::shared_ptr<ecs::Coordinator> &coordinator)
	{
		m_coordinator = coordinator;
	}

	unsigned int SceneManager::createScene(const std::string &name)
	{
		Scene newScene = Scene(name, m_coordinator);
		m_scenes.emplace(newScene.getId(), newScene);

		return newScene.getId();
	}

	void SceneManager::deleteScene(unsigned int id)
	{
		m_scenes.erase(id);
	}

	Scene &SceneManager::getScene(unsigned int id)
	{
		return m_scenes.at(id);
	}

}
