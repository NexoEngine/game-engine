//// Scene.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/03/2025
//  Description: Source file for the scene class
//
///////////////////////////////////////////////////////////////////////////////

#include "NewScene.hpp"
#include "components/SceneComponents.hpp"

namespace nexo::scene {
	NewScene::NewScene(const std::string &sceneName, const std::shared_ptr<ecs::Coordinator> &coordinator) : m_sceneName(sceneName), m_coordinator(coordinator)
	{

	}

	NewScene::~NewScene()
	{
		for (const ecs::Entity entity : m_entities)
		{
			m_coordinator->destroyEntity(entity);
		}
	}

	void NewScene::addEntity(ecs::Entity entity)
	{
		components::SceneTag tag{m_id, true, true};
		m_coordinator->addComponent<components::SceneTag>(entity, tag);
		m_entities.insert(entity);
	}

	void NewScene::removeEntity(ecs::Entity entity)
	{
		m_coordinator->removeComponent<components::SceneTag>(entity);
		m_entities.erase(entity);
	}

	void NewScene::setActiveStatus(bool active)
	{
		m_active = active;
		for (const ecs::Entity entity : m_entities)
		{
			components::SceneTag &tag = m_coordinator->getComponent<components::SceneTag>(entity);
			tag.isActive = active;
		}
	}

	void NewScene::setRenderStatus(bool rendered)
	{
		m_rendered = rendered;
		for (const ecs::Entity entity : m_entities)
		{
			components::SceneTag &tag = m_coordinator->getComponent<components::SceneTag>(entity);
			tag.isRendered = rendered;
		}
	}

	void NewScene::render()
	{

	}

	void NewScene::update()
	{

	}
}
