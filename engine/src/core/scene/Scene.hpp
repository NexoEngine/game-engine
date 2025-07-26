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
#include "components/Model.hpp"
#include <set>

namespace nexo::scene {
	inline unsigned int nextSceneId = 0;

	/**
	* @class Scene
	* @brief Represents a scene in the engine.
	*
	* A Scene encapsulates a collection of entities, a unique identifier, a name,
	* and active/rendered states. It manages its entities via an ECS Coordinator.
	*
	* Responsibilities:
	* - Adding and removing entities.
	* - Managing active and rendered states for the scene and its entities.
	* - Providing access to scene metadata (name, ID, UUID).
	*/
	class Scene {
		public:
			/**
			* @brief Constructs a new Scene.
			*
			* Initializes the scene with the specified name and ECS coordinator.
			* Optionally marks the scene as editor-only.
			*
			* @param sceneName The name of the scene.
			* @param coordinator Shared pointer to the ECS Coordinator managing entities/components.
			* @param editorOnly If true, the scene is marked as editor-only.
			*/
			Scene(std::string sceneName, const std::shared_ptr<ecs::Coordinator>& coordinator, bool editorOnly = false);
			~Scene();

			/**
			* @brief Adds an entity to the scene.
			*
			* Attaches a SceneTag component (with the scene ID and default active/rendered state)
			* to the entity and stores it in the scene's entity set.
			*
			* @param entity The entity identifier to add.
			*/
			void addEntity(ecs::Entity entity);
			void addChildEntityToScene(ecs::Entity entity);

			/**
             * @brief Removes an entity from the scene.
             *
             * Detaches the SceneTag component from the entity and removes it from the scene's entity set.
             *
             * @param entity The entity identifier to remove.
             */
			void removeEntity(ecs::Entity entity);

			/**
             * @brief Sets the active status for the scene.
             *
             * Updates the scene's active state and propagates the change to all entities by updating
             * their SceneTag components.
             *
             * @param active True to mark the scene as active, false to deactivate.
             */
			void setActiveStatus(bool active);
			[[nodiscard]] bool isActive() const { return m_active; }

			/**
             * @brief Sets the rendered status for the scene.
             *
             * Updates the scene's rendered state and propagates the change to all entities by updating
             * their SceneTag components.
             *
             * @param rendered True to mark the scene as rendered, false to not render.
             */
			void setRenderStatus(bool rendered);
			[[nodiscard]] bool isRendered() const { return m_rendered; }

			[[nodiscard]] const std::string& getName() const {return m_sceneName;};
			void setName(const std::string_view newName) { m_sceneName = newName; }
			[[nodiscard]] unsigned int getId() const {return m_id;};
			[[nodiscard]] const std::string &getUuid() const {return m_uuid;}
			[[nodiscard]] const std::set<ecs::Entity> &getEntities() const {return m_entities;};
		private:
			unsigned int m_id = nextSceneId++;
			std::string m_sceneName;
			std::string m_uuid;
			std::set<ecs::Entity> m_entities;
			std::shared_ptr<ecs::Coordinator> m_coordinator;

			bool m_active = true;
			bool m_rendered = true;
			bool isEditor = false;
	};
}
