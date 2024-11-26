//// EventManager.test.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/11/2024
//  Description: Test file for the EventManager class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/event/Event.hpp"
#include "core/event/Listener.hpp"
#include "core/scene/Scene.hpp"

namespace nexo::event {
    class TestEvent : public Event<TestEvent> {
        public:
            explicit TestEvent(int data) : data(data) {}
            int data;
    };

    class MockListener : public Listens<TestEvent> {
        public:
            explicit MockListener(const std::string& name = "") : Listens(name) {}

            MOCK_METHOD(void, handleEvent, (TestEvent&), (override));
    };

    class MockScene : public scene::Scene {
        public:
            MOCK_METHOD(void, dispatchEventToLayers, (IEvent&), (override));
    };

    TEST(EventManagerTest, RegisterListener) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);
        // Validate no crash happens
        SUCCEED();
    }

    TEST(EventManagerTest, UnregisterListener) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);
        manager.unregisterListener<TestEvent>(&listener);
        // Validate no crash after unregister
        SUCCEED();
    }

    TEST(EventManagerTest, EmitAndDispatchEventToListener) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(5);
        // Expect the handleEvent function of the listener to be called once
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        MockScene mockScene;
        manager.dispatchEvents(mockScene, false);
    }

    TEST(EventManagerTest, EventConsumption) {
        EventManager manager;
        MockListener listener("MockListener");
        MockListener listener2("MockListener2");

        manager.registerListener<TestEvent>(&listener);
        manager.registerListener<TestEvent>(&listener2);

        auto testEvent = std::make_shared<TestEvent>(5);
        // Expect the first listener to receive the event and consume it
        EXPECT_CALL(listener, handleEvent(::testing::_))
            .WillOnce([](TestEvent& event) { event.consumed = true; });
        // The second listener should not receive anything since the first listener consumed the event
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(0);
        // Same for the scene itself
        MockScene mockScene;
        EXPECT_CALL(mockScene, dispatchEventToLayers(::testing::_)).Times(0);

        manager.emitEvent(testEvent);
        manager.dispatchEvents(mockScene, true);
    }

    TEST(EventManagerTest, DispatchToSceneIfNotConsumed) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(5);
        // The listener should receive the event but not consume it
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        MockScene mockScene;
        // So the scene should also receive it
        EXPECT_CALL(mockScene, dispatchEventToLayers(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents(mockScene, true);
    }

    TEST(EventManagerTest, MultipleListenersSameEvent) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);

        auto testEvent = std::make_shared<TestEvent>(5);
        // Both listeners should receive the event since none of them consumes it
        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(1);

        MockScene mockScene;
        manager.emitEvent(testEvent);
        manager.dispatchEvents(mockScene, false);
    }

    TEST(EventManagerTest, NoListenerForEvent) {
        EventManager manager;
        auto testEvent = std::make_shared<TestEvent>(5);

        // Validate that even with no listener the scene dispatch works
        MockScene mockScene;
        EXPECT_CALL(mockScene, dispatchEventToLayers(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents(mockScene, true);
    }

    class AnotherTestEvent : public Event<AnotherTestEvent> {
        public:
        explicit AnotherTestEvent(std::string message) : message(std::move(message)) {}
        std::string message;
    };

    class AnotherMockListener : public Listens<AnotherTestEvent> {
        public:
        explicit AnotherMockListener(const std::string& name = "") : Listens(name) {}

        MOCK_METHOD(void, handleEvent, (AnotherTestEvent&), (override));
    };

    TEST(EventManagerTest, RegisterListenersForDifferentEventTypes) {
        EventManager manager;
        MockListener listener1("Listener1");
        AnotherMockListener listener2("Listener2");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<AnotherTestEvent>(&listener2);

        auto testEvent = std::make_shared<TestEvent>(42);
        auto anotherEvent = std::make_shared<AnotherTestEvent>("Hello");

        // Each listener should be called once based on their type of event they listen to
        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(1);

        MockScene mockScene;
        manager.emitEvent(testEvent);
        manager.emitEvent(anotherEvent);
        manager.dispatchEvents(mockScene, false);
    }

    TEST(EventManagerTest, UnregisterListenerBeforeDispatch) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);
        manager.unregisterListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(42);
        // The handle event should not be called since we are not listening anymore
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(0);

        MockScene mockScene;
        manager.emitEvent(testEvent);
        manager.dispatchEvents(mockScene, false);
    }

    TEST(EventManagerTest, EmitMultipleEventsQuickly) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        // The handle event function should be called several times if several events have been triggered in one frame
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(3);

        manager.emitEvent(std::make_shared<TestEvent>(1));
        manager.emitEvent(std::make_shared<TestEvent>(2));
        manager.emitEvent(std::make_shared<TestEvent>(3));

        MockScene mockScene;
        manager.dispatchEvents(mockScene, false);
    }

    class MultiEventListener : public Listens<TestEvent, AnotherTestEvent> {
        public:
        explicit MultiEventListener(const std::string& name = "") : Listens(name) {}

        MOCK_METHOD(void, handleEvent, (TestEvent&), (override));
        MOCK_METHOD(void, handleEvent, (AnotherTestEvent&), (override));
    };

    TEST(EventManagerTest, ListenerHandlesMultipleEventTypes) {
        EventManager manager;
        MultiEventListener listener("MultiListener");

        manager.registerListener<TestEvent>(&listener);
        manager.registerListener<AnotherTestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(42);
        auto anotherEvent = std::make_shared<AnotherTestEvent>("Hello");

        // Each handle event should be called once
        EXPECT_CALL(listener, handleEvent(::testing::An<TestEvent&>())).Times(1);
        EXPECT_CALL(listener, handleEvent(::testing::An<AnotherTestEvent&>())).Times(1);

        MockScene mockScene;
        manager.emitEvent(testEvent);
        manager.emitEvent(anotherEvent);
        manager.dispatchEvents(mockScene, false);
    }
}

