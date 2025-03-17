//// Scene.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/03/2025
//  Description: Header file for the scene class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/Coordinator.hpp"
#include <set>

namespace nexo::scene {
	inline unsigned int nextSceneId = 0;
	class Scene {
		public:
			Scene(const std::string &sceneName, const std::shared_ptr<ecs::Coordinator>& coordinator);
			~Scene();

			void addEntity(ecs::Entity entity);
			void removeEntity(ecs::Entity entity);

			void setActiveStatus(bool active);
			bool isActive() const { return m_active; }
			void setRenderStatus(bool rendered);
			bool isRendered() const { return m_rendered; }

			const std::string& getName() const {return m_sceneName;};
			void setName(const std::string& newName) { m_sceneName = newName; }
			unsigned int getId() const {return m_id;};
			const std::string &getUuid() const {return m_uuid;}
		private:
			unsigned int m_id = nextSceneId++;
			std::string m_sceneName;
			std::string m_uuid;
			std::set<ecs::Entity> m_entities;
			std::shared_ptr<ecs::Coordinator> m_coordinator;

			bool m_active = true;
			bool m_rendered = true;
	};
}
