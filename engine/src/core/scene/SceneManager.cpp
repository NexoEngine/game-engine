//// SceneManager.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/03/2025
//  Description: Source file for the scene manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.hpp"
#include "Exception.hpp"
#include "core/exceptions/Exceptions.hpp"
#include <cmath>

namespace nexo::scene {
	SceneManager::SceneManager() = default;

	void SceneManager::setCoordinator(const std::shared_ptr<ecs::Coordinator> &coordinator)
	{
		m_coordinator = coordinator;
	}

	unsigned int SceneManager::createScene(const std::string &name)
	{
		if (!m_coordinator)
			THROW_EXCEPTION(core::SceneManagerLifecycleException, "Coordinator is not set");
		Scene newScene = {name, m_coordinator};
		m_scenes.try_emplace(newScene.getId(), newScene);

		return newScene.getId();
	}

	unsigned int SceneManager::createEditorScene(const std::string &name)
	{
		if (!m_coordinator)
			THROW_EXCEPTION(core::SceneManagerLifecycleException, "Coordinator is not set");
		Scene newScene = {name, m_coordinator, true};
		m_scenes.try_emplace(newScene.getId(), newScene);

		return newScene.getId();
	}

	void SceneManager::deleteScene(const unsigned int id)
	{
		m_scenes.erase(id);
	}

	Scene &SceneManager::getScene(const unsigned int id)
	{
		return m_scenes.at(id);
	}

}
