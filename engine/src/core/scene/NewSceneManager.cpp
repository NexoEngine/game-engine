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

#include "NewSceneManager.hpp"
#include <cmath>

namespace nexo::scene {
	NewSceneManager::NewSceneManager() {}

	void NewSceneManager::setCoordinator(const std::shared_ptr<ecs::Coordinator> &coordinator)
	{
		m_coordinator = coordinator;
	}

	unsigned int NewSceneManager::createScene(const std::string &name)
	{
		NewScene newScene = NewScene(name, m_coordinator);
		m_scenes.emplace(newScene.getId(), newScene);

		return newScene.getId();
	}

	void NewSceneManager::deleteScene(unsigned int id)
	{
		m_scenes.erase(id);
	}

	NewScene &NewSceneManager::getScene(unsigned int id)
	{
		return m_scenes.at(id);
	}

}
