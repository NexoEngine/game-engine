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
#include "Scene.hpp"

namespace nexo::scene {
	using SceneId = unsigned int;

	class SceneManager {
		public:
			SceneManager();
			void setCoordinator(const std::shared_ptr<ecs::Coordinator> &coordinator);

			unsigned int createScene(const std::string &name);
			void deleteScene(unsigned int id);
			Scene &getScene(unsigned int id);

		private:
			std::shared_ptr<ecs::Coordinator> m_coordinator = nullptr;
			std::unordered_map<unsigned int, Scene> m_scenes;

	};
}
