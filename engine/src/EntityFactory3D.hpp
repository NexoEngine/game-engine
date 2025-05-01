//// EntityFactory3D.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Header for the 3D entity factory
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>

#include "assets/Assets/Model/Model.hpp"
#include "components/Components.hpp"

namespace nexo
{

	/**
     * @brief Factory class for creating 3D entities.
     *
     * Provides static methods to create simple 3D entities such as cubes or models,
     * by setting up the required components (TransformComponent, RenderComponent, UuidComponent, etc.).
     */
    class EntityFactory3D {
	    public:
			/**
			* @brief Creates a cube entity with a specified color.
			*
			* Constructs a cube at the given position, with the specified size, rotation, and color.
			*
			* @param pos The position of the cube.
			* @param size The dimensions (width, height, depth) of the cube.
			* @param rotation The rotation of the cube (in Euler angles).
			* @param color The color of the cube's material (default is red).
			* @return ecs::Entity The newly created cube entity.
			*/
	        static ecs::Entity createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
	                                      glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f});

			/**
			* @brief Creates a cube entity with a specified material.
			*
			* Constructs a cube at the given position, with the specified size, rotation, and material.
			*
			* @param pos The position of the cube.
			* @param size The dimensions (width, height, depth) of the cube.
			* @param rotation The rotation of the cube (in Euler angles).
			* @param material The material to apply to the cube.
			* @return ecs::Entity The newly created cube entity.
			*/
	        static ecs::Entity createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, const components::Material &material);

	        static ecs::Entity createModel(assets::AssetRef<assets::Model> modelAsset, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);

			static ecs::Entity createBillboard(const glm::vec3 &pos, const glm::vec3 &size, const glm::vec4 &color);
			static ecs::Entity createBillboard(const glm::vec3 &pos, const glm::vec3 &size, const components::Material &material);
    };
}
