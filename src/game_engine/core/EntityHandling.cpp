/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** EntityHandling
*/

#include "game_engine/GameEngine.hpp"
#include "game_engine/EntityHandling.hpp"
#include "game_engine/Math.hpp"

namespace engine {

    ecs::Entity Engine::addInvisibleEntity(void)
    {
        return _coordinator->createEntity();
    }

    ecs::Entity Engine::addEntity(ecs::components::physics::transform_t transf, ecs::components::render::render_t render)
    {
        ecs::Entity entity = _coordinator->createEntity();
        _coordinator->addComponent<ecs::components::physics::transform_t>(entity, transf);
        _coordinator->addComponent<ecs::components::render::render_t>(entity, render);
        return entity;
    }

    ecs::Entity createEntity(void)
    {
        return Engine::getInstance()->addInvisibleEntity();
    }

    std::vector<std::pair<std::type_index, std::any>> Engine::getAllComponents(ecs::Entity entity)
    {
        return _coordinator->getAllComponents(entity);
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

    ecs::Entity createSphere(
        Vector3 pos,
        float radius,
        Color color,
        bool toggleWire,
        Color wireColor)
    {
        auto sphere = std::make_shared<ecs::components::Sphere>(radius, toggleWire, color, wireColor);
        ecs::components::physics::transform_t transf = {{0}, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::SPHERE, true, sphere};
        ecs::components::physics::collider_t collider = {
            ecs::components::ShapeType::SPHERE,
            ecs::components::physics::CollisionType::COLLIDE,
            sphere,
            GetModelBoundingBox(sphere->getModel()),
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
        return entity;
    }

    ecs::Entity createPlane(
        Vector3 pos,
        float width,
        float length,
        Color color,
        bool toggleWire,
        Color wireColor)
    {
        auto plane = std::make_shared<ecs::components::Plane>(width, length, toggleWire, color, wireColor);
        ecs::components::physics::transform_t transf = {{0}, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::PLANE, true, plane};
        ecs::components::physics::collider_t collider = {
            ecs::components::ShapeType::PLANE,
            ecs::components::physics::CollisionType::COLLIDE,
            plane,
            GetModelBoundingBox(plane->getModel()),
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
        return entity;
    }

    ecs::Entity createCylinder(
        Vector3 pos,
        float radius,
        float height,
        Color color,
        bool toggleWire,
        Color wireColor)
    {
        auto cylinder = std::make_shared<ecs::components::Cylinder>(radius, height, toggleWire, color, wireColor);
        ecs::components::physics::transform_t transf = {{0}, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::CYLINDER, true, cylinder};
        ecs::components::physics::collider_t collider = {
            ecs::components::ShapeType::CYLINDER,
            ecs::components::physics::CollisionType::COLLIDE,
            cylinder,
            GetModelBoundingBox(cylinder->getModel()),
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
        return entity;
    }

    ecs::Entity createPolygon(
    Vector3 pos,
    int sides,
    float radius,
    float height,
    Color color,
    bool toggleWire,
    Color wireColor)
    {
        auto polygon = std::make_shared<ecs::components::Polygon>(sides, radius, height, toggleWire, color, wireColor);
        ecs::components::physics::transform_t transf = {{0}, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::POLYGON, true, polygon};
        ecs::components::physics::collider_t collider = {
            ecs::components::ShapeType::POLYGON,
            ecs::components::physics::CollisionType::COLLIDE,
            polygon,
            GetModelBoundingBox(polygon->getModel()),
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
        return entity;
    }

    ecs::Entity createHemisphere(
    Vector3 pos,
    float radius,
    int rings,
    int slices,
    Color color,
    bool toggleWire,
    Color wireColor)
    {
        auto hemisphere = std::make_shared<ecs::components::Hemisphere>(radius, rings, slices, toggleWire, color, wireColor);
        ecs::components::physics::transform_t transf = {{0}, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::HEMISPHERE, true, hemisphere};
        ecs::components::physics::collider_t collider = {
            ecs::components::ShapeType::HEMISPHERE,
            ecs::components::physics::CollisionType::COLLIDE,
            hemisphere,
            GetModelBoundingBox(hemisphere->getModel()),
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
        return entity;
    }

    ecs::Entity createCone(
    Vector3 pos,
    float radius,
    float height,
    int slices,
    Color color,
    bool toggleWire,
    Color wireColor)
    {
        auto cone = std::make_shared<ecs::components::Cone>(radius, height, slices, toggleWire, color, wireColor);
        ecs::components::physics::transform_t transf = {{0}, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::CONE, true, cone};
        ecs::components::physics::collider_t collider = {
            ecs::components::ShapeType::CONE,
            ecs::components::physics::CollisionType::COLLIDE,
            cone,
            GetModelBoundingBox(cone->getModel()),
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
        return entity;
    }

    ecs::Entity createTorus(
    Vector3 pos,
    float radius,
    float size,
    int radSeg,
    int sides,
    Color color,
    bool toggleWire,
    Color wireColor)
    {
        auto torus = std::make_shared<ecs::components::Torus>(radius, size, radSeg, sides, toggleWire, color, wireColor);
        ecs::components::physics::transform_t transf = {{0}, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::TORUS, true, torus};
        ecs::components::physics::collider_t collider = {
            ecs::components::ShapeType::TORUS,
            ecs::components::physics::CollisionType::COLLIDE,
            torus,
            GetModelBoundingBox(torus->getModel()),
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
        return entity;
    }

    ecs::Entity createKnot(
    Vector3 pos,
    float radius,
    float size,
    int radSeg,
    int sides,
    Color color,
    bool toggleWire,
    Color wireColor)
    {
        auto knot = std::make_shared<ecs::components::Knot>(radius, size, radSeg, sides, toggleWire, color, wireColor);
        ecs::components::physics::transform_t transf = {{0}, {0}};
        double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
        ecs::components::physics::rigidBody_t body = {0.0, {0}, {0}, now};
        ecs::components::render::render_t render = {ecs::components::ShapeType::KNOT, true, knot};
        ecs::components::physics::collider_t collider = {
            ecs::components::ShapeType::KNOT,
            ecs::components::physics::CollisionType::COLLIDE,
            knot,
            GetModelBoundingBox(knot->getModel()),
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

    BoundingBox getEntityBoundingBox(ecs::Entity entity)
    {
        auto &renderComponent = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        return renderComponent.data->getBoundingBox();
    }

    void destroyEntity(ecs::Entity entity)
    {
        Engine::getInstance()->destroyEntity(entity);
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
        // auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        // auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        // auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);
        // rotation.x = rotation.x * DEG2RAD;
        // rotation.y = rotation.y * DEG2RAD;
        // rotation.z = rotation.z * DEG2RAD;
        // transform.rotation = Vector3Add(transform.rotation, rotation);
        // Matrix matTemp = MatrixRotateXYZ(rotation);
        // render.data->getModel().transform = MatrixMultiply(render.data->getModel().transform, matTemp);
        // collider.matRotate = MatrixMultiply(collider.matRotate, matTemp);
        // ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
    }

    void setRotation(ecs::Entity entity, Vector3 rotation)
    {
        // auto &transform = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        // auto &render = Engine::getInstance()->getComponent<ecs::components::render::render_t>(entity);
        // auto &collider = Engine::getInstance()->getComponent<ecs::components::physics::collider_t>(entity);
        // rotation.x = rotation.x * DEG2RAD;
        // rotation.y = rotation.y * DEG2RAD;
        // rotation.z = rotation.z * DEG2RAD;
        // transform.rotation = rotation;
        // Matrix matTemp = MatrixRotateXYZ(transform.rotation);
        // render.data->getModel().transform = MatrixMultiply(render.data->getModel().transform, matTemp);
        // collider.matRotate = MatrixMultiply(collider.matRotate, matTemp);
        // ecs::system::CollisionResponse::updateColliderGlobalVerts(collider);
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

    void decomposeTransformMatrix(Matrix mat, Vector3 &translation, Quaternion &rotation, Vector3 &scale) 
    {
        translation = { mat.m12, mat.m13, mat.m14 };

        scale.x = Vector3Length({ mat.m0, mat.m1, mat.m2 });
        scale.y = Vector3Length({ mat.m4, mat.m5, mat.m6 });
        scale.z = Vector3Length({ mat.m8, mat.m9, mat.m10 });

        Matrix rotationMatrix = mat;

        rotationMatrix.m0 /= scale.x;
        rotationMatrix.m1 /= scale.x;
        rotationMatrix.m2 /= scale.x;

        rotationMatrix.m4 /= scale.y;
        rotationMatrix.m5 /= scale.y;
        rotationMatrix.m6 /= scale.y;

        rotationMatrix.m8 /= scale.z;
        rotationMatrix.m9 /= scale.z;
        rotationMatrix.m10 /= scale.z;

        rotation = QuaternionFromMatrix(rotationMatrix);
    }

    void entity::updateEntityTransformMatrix(ecs::Entity entity, bool inDeg)
    {
        auto &transf = Engine::getInstance()->getComponent<ecs::components::physics::transform_t>(entity);
        Matrix transformMatrix = math::createTransformMatrixEuler(transf.pos, transf.rotation, transf.scale, inDeg);
        setTransformMatrix(entity, transformMatrix);
    }
}