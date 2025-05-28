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
#include "components/Model.hpp"

namespace nexo
{
    /**
     * @brief Factory class for creating 3D entities.
     *
     * Provides static methods to create simple 3D entities such as cubes or models,
     * by setting up the required components (TransformComponent, RenderComponent, UuidComponent, etc.).
     */
    class EntityFactory3D
    {
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
        static ecs::Entity createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                      const components::Material& material);

	        static ecs::Entity createModel(assets::AssetRef<assets::Model> modelAsset, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
			static int processModelNode(ecs::Entity parentEntity, const assets::MeshNode& node);

        static ecs::Entity createBillboard(const glm::vec3& pos, const glm::vec3& size, const glm::vec4& color);

        static ecs::Entity createBillboard(const glm::vec3& pos, const glm::vec3& size,
                                           const components::Material& material);

        /**
         * @brief Creates a tetrahedron entity with a specified material.
         *
         * Constructs a tetrahedron at the given position, with the specified size, rotation, and material.
         *
         * @param pos The position of the tetrahedron.
         * @param size The dimensions (width, height, depth) of the tetrahedron.
         * @param rotation The rotation of the tetrahedron (in Euler angles).
         * @param color The color of the tetrahedron's material (default is red).
         * @return ecs::Entity The newly created tetrahedron entity.
         */
        static ecs::Entity createTetrahedron(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                             glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f});

        /**
            * @brief Creates a tetrahedron entity with a specified material.
            *
            * Constructs a tetrahedron at the given position, with the specified size, rotation, and material.
            *
            * @param pos The position of the tetrahedron.
            * @param size The dimensions (width, height, depth) of the tetrahedron.
            * @param rotation The rotation of the tetrahedron (in Euler angles).
            * @param material The material to apply to the tetrahedron.
            * @return ecs::Entity The newly created tetrahedron entity.
            */
        static ecs::Entity createTetrahedron(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                             const components::Material& material);


        /**
         * @brief Creates a pyramid entity with a specified material.
         *
         * Constructs a pyramid at the given position, with the specified size, rotation, and material.
         *
         * @param pos The position of the pyramid.
         * @param size The dimensions (width, height, depth) of the pyramid.
         * @param rotation The rotation of the pyramid (in Euler angles).
         * @param color The color of the pyramid's material (default is red).
         * @return ecs::Entity The newly created pyramid entity.
         */
        static ecs::Entity createPyramid(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                         glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f});

        /**
         * @brief Creates a pyramid entity with a specified material.
         *
         * Constructs a pyramid at the given position, with the specified size, rotation, and material.
         *
         * @param pos The position of the pyramid.
         * @param size The dimensions (width, height, depth) of the pyramid.
         * @param rotation The rotation of the pyramid (in Euler angles).
         * @param material The material to apply to the pyramid.
         * @return ecs::Entity The newly created pyramid entity.
         */
        static ecs::Entity createPyramid(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                         const components::Material& material);

        /**
         * @brief Creates a cylinder entity with a specified material.
         *
         * Constructs a cylinder at the given position, with the specified size, rotation, and material.
         *
         * @param pos The position of the cylinder.
         * @param size The dimensions (width, height, depth) of the cylinder.
         * @param rotation The rotation of the cylinder (in Euler angles).
         * @param color The color of the cylinder's material (default is red).
         * @return ecs::Entity The newly created cylinder entity.
         */
        static ecs::Entity createCylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                          glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f});

        /**
         * @brief Creates a cylinder entity with a specified material.
         *
         * Constructs a cylinder at the given position, with the specified size, rotation, and material.
         *
         * @param pos The position of the cylinder.
         * @param size The dimensions (width, height, depth) of the cylinder.
         * @param rotation The rotation of the cylinder (in Euler angles).
         * @param material The material to apply to the cylinder.
         * @return ecs::Entity The newly created cylinder entity.
         */
        static ecs::Entity createCylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                          const components::Material& material);

        /**
         * @brief Creates a sphere entity with a specified material.
         *
         * Constructs a sphere at the given position, with the specified size, rotation, and material.
         *
         * @param pos The position of the sphere.
         * @param size The dimensions (width, height, depth) of the sphere.
         * @param rotation The rotation of the sphere (in Euler angles).
         * @param color The color of the sphere's material (default is red).
         * @return ecs::Entity The newly created sphere entity.
         */
        static ecs::Entity createSphere(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                        glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f});

        /**
         * @brief Creates a sphere entity with a specified material.
         *
         * Constructs a sphere at the given position, with the specified size, rotation, and material.
         *
         * @param pos The position of the sphere.
         * @param size The dimensions (width, height, depth) of the sphere.
         * @param rotation The rotation of the sphere (in Euler angles).
         * @param material The material to apply to the sphere.
         * @return ecs::Entity The newly created sphere entity.
         */
        static ecs::Entity createSphere(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                        const components::Material& material);
    };
}
