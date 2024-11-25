//// Layer.test.cpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/11/2024
//  Description: Test file for the layers
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/layer/Layer.hpp"
#include "core/camera/Camera.hpp"
#include "ecs/Entity.hpp"
#include "components/Components.hpp"
#include "core/event/Event.hpp"

// Mock definitions for dependencies
namespace nexo::camera {
    class MockCamera : public Camera {
    public:
        MOCK_METHOD(void, onUpdate, (Timestep), ());
        MOCK_METHOD(CameraMode, getMode, (), (const, override));
        MOCK_METHOD(const glm::mat4&, getViewProjectionMatrix, (), (const, override));
        MOCK_METHOD(const glm::vec3&, getPosition, (), (const, override));
    };
}

namespace nexo::event {
    class MockEvent : public IEvent {
    public:
        MOCK_METHOD(void, trigger, (BaseListener&), (override));
    };
}

namespace nexo::layer {

    class LayerTest : public ::testing::Test {
    protected:
        std::shared_ptr<Layer> layer;

        void SetUp() override {
            layer = std::make_shared<Layer>(1, "TestLayer");
            auto &app = nexo::Application::getInstance();
            nexo::init();

        }
    };

    TEST_F(LayerTest, AttachAndDetachCamera) {
        auto camera = std::make_shared<camera::MockCamera>();

        layer->attachCamera(camera);
        EXPECT_EQ(layer->getCamera(), camera);
        EXPECT_TRUE(layer->isRendered);

        layer->detachCamera();
        EXPECT_EQ(layer->getCamera(), nullptr);
        EXPECT_FALSE(layer->isRendered);
    }

    TEST_F(LayerTest, AddAndRemoveEntity) {
        ecs::Entity entity1 = 1;
        ecs::Entity entity2 = 2;
        auto &app = getApp();

        app.m_coordinator->addComponent(entity1, components::TransformComponent{});
        app.m_coordinator->addComponent(entity1, components::RenderComponent{});
        app.m_coordinator->addComponent(entity2, components::TransformComponent{});

        layer->addEntity(entity1);
        // Validate the layer holds the first
        EXPECT_TRUE(layer->getEntities().contains(entity1));

        // Adding entity without required components
        layer->addEntity(entity2);
        // Validate that the entity should not be added since it does not have render + transform components
        EXPECT_FALSE(layer->getEntities().contains(entity2));

        layer->removeEntity(entity1);
        // Validate that the entity has been removed
        EXPECT_FALSE(layer->getEntities().contains(entity1));
    }

    TEST_F(LayerTest, EntityDestroyed) {
        ecs::Entity entity = 1;

        auto &app = getApp();
        app.m_coordinator->addComponent(entity, components::TransformComponent{});
        app.m_coordinator->addComponent(entity, components::RenderComponent{});

        layer->addEntity(entity);
        EXPECT_TRUE(layer->getEntities().contains(entity));

        layer->entityDestroyed(entity);
        EXPECT_FALSE(layer->getEntities().contains(entity));
    }

    TEST_F(LayerTest, HandleEvent) {
        auto mockEvent = std::make_shared<event::MockEvent>();
        bool callbackTriggered = false;

        layer->registerCallbackEventFunction<event::MockEvent>([&](const event::IEvent&) {
            callbackTriggered = true;
        });

        layer->handleEvent(*mockEvent);
        // Validate that the callback has been called
        EXPECT_TRUE(callbackTriggered);
    }

    TEST_F(LayerTest, UpdateWithCamera) {
        auto mockCamera = std::make_shared<camera::MockCamera>();
        Timestep timestep{1.0f};

        layer->attachCamera(mockCamera);

        EXPECT_CALL(*mockCamera, onUpdate(timestep)).Times(1);

        layer->onUpdate(timestep);
    }

    TEST_F(LayerTest, UpdateWithoutCamera) {
        Timestep timestep{1.0f};
        EXPECT_NO_THROW(layer->onUpdate(timestep));
    }
}
