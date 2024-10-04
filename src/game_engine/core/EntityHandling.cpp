/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** EntityHandling
*/

#include "game_engine/GameEngine.hpp"
#include "game_engine/EntityHandling.hpp"

namespace engine {

    ecs::Entity createEntity(void)
    {
        return Engine::getInstance()->addInvisibleEntity();
    }

    std::vector<std::pair<std::type_index, std::any>> getAllComponents(ecs::Entity entity)
    {
        return Engine::getInstance()->getAllComponents(entity);
    }

    ecs::Entity createCube(
        Vector3 pos,
        float width,
        float height,
        float length,
        Color color,
        bool toggleWire,
        Color wireColor)
    {
        auto cube = std::make_shared<ecs::components::Cube>(width, height, length, toggleWire, color, wireColor);
        ecs::components::physics::transform_t transf = {{0}, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::CUBE, true, cube};
        ecs::components::physics::collider_t collider = {
            ecs::components::ShapeType::CUBE,
            ecs::components::physics::CollisionType::COLLIDE,
            cube,
            GetModelBoundingBox(cube->getModel()),
            {0},
            MatrixIdentity(),
            MatrixIdentity(),
            MatrixIdentity()};
        Matrix matTranslate = MatrixTranslate(pos.x, pos.y, pos.z);
        render.data->getModel().transform = matTranslate;
        collider.matTranslate = MatrixMultiply(collider.matTranslate, matTranslate);
        ecs::components::health::health_t health = {0};
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        Engine::getInstance()->addComponent<ecs::components::physics::collider_t>(entity, collider);
        Engine::getInstance()->addComponent<ecs::components::physics::rigidBody_t>(entity, body);
        Engine::getInstance()->addComponent<ecs::components::health::health_t>(entity, health);
        //Engine::getInstance()->addComponent<ecs::components::metadata::metadata_t>(entity, {server::entities::EntityType::ENTITY_UNDEFINED});
        return entity;
    }

    ecs::Entity createModel3D(const char *filename, Vector3 pos, Color color)
    {
        auto model = std::make_shared<ecs::components::Model3D>(filename, color);
        ecs::components::physics::transform_t transf = {pos, {0}, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::MODEL, true, model};
        ecs::components::health::health_t health = {0};
        ecs::components::physics::collider_t collider = {ecs::components::ShapeType::MODEL, ecs::components::physics::CollisionType::COLLIDE, model};
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        Engine::getInstance()->addComponent<ecs::components::physics::collider_t>(entity, collider);
        Engine::getInstance()->addComponent<ecs::components::physics::rigidBody_t>(entity, body);
        Engine::getInstance()->addComponent<ecs::components::health::health_t>(entity, health);
        Engine::getInstance()->addComponent<ecs::components::direction::direction_t>(entity, {0, 0, 0});
        //Engine::getInstance()->addComponent<ecs::components::metadata::metadata_t>(entity, {server::entities::EntityType::ENTITY_UNDEFINED});
        return entity;
    }

    ecs::Entity createSkybox(const char *filename, Vector3 pos, Color color)
    {
        auto model = std::make_shared<ecs::components::Skybox>(filename);
        ecs::components::physics::transform_t transf = {pos, {0}, {1,1,1}};
        ecs::components::render::render_t render = {ecs::components::ShapeType::MODEL, true, model};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::physics::collider_t collider = {ecs::components::ShapeType::MODEL, ecs::components::physics::CollisionType::NON_COLLIDE, model};
        ecs::Entity entity = Engine::getInstance()->addEntity(transf, render);
        Engine::getInstance()->addComponent<ecs::components::physics::collider_t>(entity, collider);
        Engine::getInstance()->addComponent<ecs::components::physics::rigidBody_t>(entity, body);
        //Engine::getInstance()->addComponent<ecs::components::metadata::metadata_t>(entity, {server::entities::EntityType::ENTITY_UNDEFINED});

        return entity;
    }

    void Engine::destroyEntity(ecs::Entity entity)
    {
        _entitiesToDestroy.push(entity);
    }

    void attachBehavior(
        ecs::Entity entity,
        std::shared_ptr<ecs::components::behaviour::Behaviour> behaviour)
    {
        Engine::getInstance()->addComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>(entity, behaviour);
        behaviour->setEntity(entity);
    }

    void rotate(ecs::Entity entity, Vector3 rotation)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);
        rotation.x = rotation.x * DEG2RAD;
        rotation.y = rotation.y * DEG2RAD;
        rotation.z = rotation.z * DEG2RAD;
        transform.rotation = Vector3Add(transform.rotation, rotation);
        Matrix matTemp = MatrixRotateXYZ(rotation);
        render.data->getModel().transform = MatrixMultiply(render.data->getModel().transform, matTemp);
        collider.matRotate = MatrixMultiply(collider.matRotate, matTemp);
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
    }

    void setRotation(ecs::Entity entity, Vector3 rotation)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);
        rotation.x = rotation.x * DEG2RAD;
        rotation.y = rotation.y * DEG2RAD;
        rotation.z = rotation.z * DEG2RAD;
        transform.rotation = rotation;
        Matrix matTemp = MatrixRotateXYZ(transform.rotation);
        render.data->getModel().transform = MatrixMultiply(render.data->getModel().transform, matTemp);
        collider.matRotate = MatrixMultiply(collider.matRotate, matTemp);
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
    }

    void scale(ecs::Entity entity, Vector3 scale)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);
        transform.scale = Vector3Add(transform.scale, scale);
        Matrix matTemp = MatrixScale(scale.x, scale.y, scale.z);
        render.data->getModel().transform = MatrixMultiply(render.data->getModel().transform, matTemp);
        collider.matScale = MatrixMultiply(collider.matScale, matTemp);
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
    }

    void setScale(ecs::Entity entity, Vector3 scale)
    {
        auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);

        // Set the scale of the transform directly to the new scale value
        transform.scale = scale;

        // Create a scaling matrix with the new scale values
        Matrix matTemp = MatrixScale(scale.x, scale.y, scale.z);

        // Apply the scaling matrix to the transform of the render data and the collider
        render.data->getModel().transform = matTemp;
        collider.matScale = matTemp;

        // Update the collider's global vertices
        ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
    }

    void setAnimation(ecs::Entity entity, const char *filename)
    {
        ecs::components::animations::animation_t anim;
        auto &draw = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        anim.animFrameCounter = 0;
        anim.model = draw.data->getModel();
        anim.animsCount = 0;
        anim.anims = LoadModelAnimations(filename, &anim.animsCount);
        Engine::getInstance()->addComponent<ecs::components::animations::animation_t>(entity, anim);
    }

    void addEntityToScene(ecs::Entity entity, ecs::SceneID sceneID)
    {
        Engine::getInstance()->addEntityToScene(entity, sceneID);
    }

    void removeEntityFromScene(ecs::Entity entity, ecs::SceneID sceneID)
    {
        Engine::getInstance()->removeEntityFromScene(entity, sceneID);
    }

    Matrix transformToMatrix(const ecs::components::physics::transform_t &transform) 
    {
        Matrix matRotation = MatrixRotateXYZ(transform.rotation);
        Matrix matScale = MatrixScale(transform.scale.x, transform.scale.y, transform.scale.z);
        Matrix matTranslation = MatrixTranslate(transform.pos.x, transform.pos.y, transform.pos.z);

        Matrix transformMatrix = MatrixMultiply(matScale, matRotation);
        transformMatrix = MatrixMultiply(transformMatrix, matTranslation);

        return transformMatrix;
    }

    Matrix entity::getTransformMatrix(ecs::Entity entity)
    {
        const auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);

        return render.data->getModel().transform;
    }

    Transform entity::getTransform(ecs::Entity entity)
    {
        const auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);

        return Transform {
            .translation = transform.pos,
            .rotation = {transform.rotation.x, transform.rotation.y, transform.rotation.z},
            .scale = transform.scale
        };
    }

    void entity::setTransformMatrix(ecs::Entity entity, Matrix transform)
    {
        auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        render.data->getModel().transform = transform;
    }
}