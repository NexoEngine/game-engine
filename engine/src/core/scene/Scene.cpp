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

#include "Scene.hpp"
#include "components/SceneComponents.hpp"
#include "components/Uuid.hpp"

namespace nexo::scene {
	Scene::Scene(const std::string &sceneName, const std::shared_ptr<ecs::Coordinator> &coordinator) : m_sceneName(sceneName), m_coordinator(coordinator)
	{
		m_uuid = components::genUuid();
	}

	Scene::~Scene()
	{
		for (const ecs::Entity entity : m_entities)
		{
			m_coordinator->destroyEntity(entity);
		}
	}

	void Scene::addEntity(ecs::Entity entity)
	{
		components::SceneTag tag{m_id, true, true};
		m_coordinator->addComponent<components::SceneTag>(entity, tag);
		m_entities.insert(entity);
	}

	void Scene::removeEntity(ecs::Entity entity)
	{
		m_coordinator->removeComponent<components::SceneTag>(entity);
		m_entities.erase(entity);
	}

	void Scene::setActiveStatus(bool active)
	{
		m_active = active;
		for (const ecs::Entity entity : m_entities)
		{
			auto tag = m_coordinator->tryGetComponent<components::SceneTag>(entity);
			if (tag)
				tag->get().isActive = active;
		}
	}

	void Scene::setRenderStatus(bool rendered)
	{
		m_rendered = rendered;
		for (const ecs::Entity entity : m_entities)
		{
			auto tag = m_coordinator->tryGetComponent<components::SceneTag>(entity);
			if (tag)
				tag->get().isRendered = rendered;
		}
	}
}
