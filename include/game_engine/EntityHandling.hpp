/*
** EPITECH PROJECT, 2024
** Nexo
** File description:
** EntityHandling
*/

#pragma once

#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/components/Behaviour.hpp"
#include <GameEngine.hpp>

namespace engine {

    /**
     * @brief Creates an entity without transform or render component
     * (useful for entites that should have a behavior but not physical existence
     * like enemy spawner etc...)
     *
     */
    ecs::Entity createEntity(void);

    std::vector<std::pair<std::type_index, std::any>> getAllComponents(ecs::Entity entity);

    /**
     * @brief Creates a cube entity with specified parameters.
     * @param pos Position of the cube.
     * @param width Width of the cube.
     * @param height Height of the cube.
     * @param length Length of the cube.
     * @param color Color of the cube.
     * @param toggleWire Flag to toggle wireframe mode.
     * @param wireColor Color of the wireframe.
     * @return The created cube entity.
     */
    ecs::Entity createCube(Vector3 pos, float width, float height, float length, Color color = RED, bool toggleWire = false, Color wireColor = BLACK);

    /**
     * @brief Creates a 3D model entity from a file.
     * @param filename Path to the model file.
     * @param pos Position of the model.
     * @param color Color to apply to the model.
     * @return The created model entity.
     */
    ecs::Entity createModel3D(const char *filename, Vector3 pos, Color color = WHITE);

    /**
     * @brief Creates a skybox entity from a file.
     * @param filename Path to the skybox file.
     * @param pos Position of the skybox.
     * @param color Color to apply to the skybox.
     * @return The created skybox entity.
     */
    ecs::Entity createSkybox(const char *filename, Vector3 pos, Color color = WHITE);


    void destroyEntity(ecs::Entity entity);

    /**
     * @brief Creates a behavior component.
     * @tparam T Type of the behavior.
     * @tparam Args Variadic template arguments for the behavior's constructor.
     * @param args Arguments to be forwarded to the behavior's constructor.
     * @return Shared pointer to the created behavior component.
     */
    template<typename T, typename... Args>
    std::shared_ptr<T> createBehavior(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    /**
     * @brief Attaches a behavior to an entity.
     * @param entity The entity to attach the behavior to.
     * @param behaviour Shared pointer to the behavior component.
     */
    void attachBehavior(ecs::Entity entity, std::shared_ptr<ecs::components::behaviour::Behaviour> behaviour);

    /**
     * @brief Rotates by an increment an entity.
     * @param entity The entity to rotate.
     * @param rotation The rotation vector.
     */
    void rotate(ecs::Entity entity, Vector3 rotation);

    /**
    * @brief Set the rotation to an absolute value of an entity.
    * @param entity The entity to rotate.
    * @param rotation The rotation vector.
    */
    void setRotation(ecs::Entity entity, Vector3 rotation);

    /**
     * @brief Scales by an increment an entity.
     * @param entity The entity to scale.
     * @param scale The scale vector.
     */
    void scale(ecs::Entity entity, Vector3 scale);

    /**
    * @brief Set the scale to an absolute value of an entity.
    * @param entity The entity to scale.
    * @param scale The scale vector.
    */
    void setScale(ecs::Entity entity, Vector3 scale);

    /**
     * @brief Sets the animation for an entity.
     * @param entity The entity to set the animation for.
     * @param filename Path to the animation file.
     */
    void setAnimation(ecs::Entity entity, const char *filename);

    /**
     * @brief Adds an entity to a scene
     *
     * @param entity
     * @param sceneID
     */
    void addEntityToScene(ecs::Entity entity, ecs::SceneID sceneID);

    /**
     * @brief Removes an entity from a scene
     *
     * @param entity
     * @param sceneID
     */
    void removeEntityFromScene(ecs::Entity entity, ecs::SceneID sceneID);

    namespace entity {
        /**
         * @brief Get the transform matrix of the entity.
         * @param entity The entity to get the transform matrix from.
         * @return The transform matrix.
         */
        Matrix getTransformMatrix(ecs::Entity entity);

        /**
         * @brief Get the transform of the entity.
         * @param entity The entity to get the transform from.
         * @return The transform.
         */
        Transform getTransform(ecs::Entity entity);

        /**
        * @brief Retrieves a component from an entity.
        * @tparam T Type of the component.
        * @param entity The entity from which the component will be retrieved.
        * @return Reference to the component of type T.
        */
        template<typename T>
        T &getComponent(ecs::Entity entity)
        {
            return Engine::getInstance()->getComponent<T>(entity);
        }

        /**
         * @brief Set the transform of the entity.
         * @param entity The entity to set the transform to.
         * @param transMatrix The transformation matrix.
         */
        void setTransform(ecs::Entity entity, const Vector3 &position,
            const Vector3 &rotation, const Vector3 &scale);
    }
}