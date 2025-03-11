//// SceneManager.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/03/2025
//  Description: Header file for the scene manager class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/Coordinator.hpp"
#include "NewScene.hpp"

namespace nexo::scene {
	class NewSceneManager {
		public:
			NewSceneManager();
			void setCoordinator(const std::shared_ptr<ecs::Coordinator> &coordinator);

			unsigned int createScene(const std::string &name);
			void deleteScene(unsigned int id);
			NewScene &getScene(unsigned int id);

		private:
			std::shared_ptr<ecs::Coordinator> m_coordinator;
			std::unordered_map<unsigned int, NewScene> m_scenes;

	};
}
