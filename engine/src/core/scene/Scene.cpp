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

#include <utility>
#include "components/SceneComponents.hpp"
#include "components/Uuid.hpp"

namespace nexo::scene {
	Scene::Scene(std::string sceneName, const std::shared_ptr<ecs::Coordinator> &coordinator, const bool editorOnly) : m_sceneName(std::move(sceneName)), m_coordinator(coordinator), isEditor(editorOnly)
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

	void Scene::addEntity(const ecs::Entity entity)
    {
        const components::SceneTag tag{m_id, true, true};
        m_coordinator->addComponent<components::SceneTag>(entity, tag);
        m_entities.insert(entity);

        auto modelComponent = m_coordinator->tryGetComponent<components::ModelComponent>(entity);
        if (modelComponent)
            addModelChildEntities(modelComponent->get());
    }

    void Scene::addModelChildEntities(const components::ModelComponent& modelComponent)
    {
        for (const auto& childIndex : modelComponent.children) {
            const components::SceneTag childTag{m_id, true, true};
            m_coordinator->addComponent<components::SceneTag>(childIndex.child, childTag);
            m_entities.insert(childIndex.child);

            // Recursively add any nested children
            if (!childIndex.children.empty()) {
                components::ModelComponent tempModelComponent;
                tempModelComponent.children = childIndex.children;
                addModelChildEntities(tempModelComponent);
            }
        }
    }

	void Scene::removeEntity(const ecs::Entity entity)
	{
		m_coordinator->removeComponent<components::SceneTag>(entity);
		m_entities.erase(entity);
	}

	void Scene::setActiveStatus(const bool active)
	{
		m_active = active;
		for (const ecs::Entity entity : m_entities)
		{
			auto tag = m_coordinator->tryGetComponent<components::SceneTag>(entity);
			if (tag)
				tag->get().isActive = active;
		}
	}

	void Scene::setRenderStatus(const bool rendered)
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
