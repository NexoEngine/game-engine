//// EventManager.test.cpp ////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
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
        manager.dispatchEvents();
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

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerTest, DispatchToSceneIfNotConsumed) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(5);
        // The listener should receive the event but not consume it
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
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

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
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

        manager.emitEvent(testEvent);
        manager.emitEvent(anotherEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerTest, UnregisterListenerBeforeDispatch) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);
        manager.unregisterListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(42);
        // The handle event should not be called since we are not listening anymore
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(0);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
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

        manager.dispatchEvents();
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

        manager.emitEvent(testEvent);
        manager.emitEvent(anotherEvent);
        manager.dispatchEvents();
    }

    // ===== Edge Case Tests =====

    TEST(EventManagerEdgeCaseTest, ClearEventsRemovesAllQueuedEvents) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        // Emit multiple events
        manager.emitEvent(std::make_shared<TestEvent>(1));
        manager.emitEvent(std::make_shared<TestEvent>(2));
        manager.emitEvent(std::make_shared<TestEvent>(3));

        // Clear all events before dispatch
        manager.clearEvents();

        // No events should be dispatched since the queue was cleared
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(0);

        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, DispatchEventsWithEmptyQueue) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        // No events should be called since queue is empty
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(0);

        // Should not crash when dispatching with an empty queue
        manager.dispatchEvents();
        SUCCEED();
    }

    TEST(EventManagerEdgeCaseTest, DispatchEventsRequeuesBehavior) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        // Emit one event
        auto testEvent = std::make_shared<TestEvent>(42);
        manager.emitEvent(testEvent);

        // First dispatch should call the listener once
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        manager.dispatchEvents();

        // After dispatch, event should be requeued (based on line 40 in Event.cpp)
        // Second dispatch should call the listener again
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, DispatchEventsMultipleTimesRequeuingBehavior) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        // Emit multiple events
        manager.emitEvent(std::make_shared<TestEvent>(1));
        manager.emitEvent(std::make_shared<TestEvent>(2));

        // First dispatch: both events should be processed
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(2);
        manager.dispatchEvents();

        // Events are requeued, so second dispatch should process them again
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(2);
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, UnregisterFromNonExistentEventType) {
        EventManager manager;
        MockListener listener("MockListener");

        // Try to unregister a listener from an event type that has no registered listeners
        // Should not crash and should handle gracefully
        manager.unregisterListener<TestEvent>(&listener);
        SUCCEED();
    }

    TEST(EventManagerEdgeCaseTest, UnregisterListenerNotInList) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");

        // Register only listener1
        manager.registerListener<TestEvent>(&listener1);

        // Try to unregister listener2 which was never registered
        // Should not crash and should handle gracefully
        manager.unregisterListener<TestEvent>(&listener2);
        SUCCEED();
    }

    TEST(EventManagerEdgeCaseTest, LastListenerRemovalCleanup) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);
        manager.unregisterListener<TestEvent>(&listener);

        // After removing the last listener, the event type entry should be removed
        // Emitting an event should not crash even though no listeners exist
        auto testEvent = std::make_shared<TestEvent>(42);
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(0);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
        SUCCEED();
    }

    TEST(EventManagerEdgeCaseTest, MultipleListenersRemovalCleanup) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");
        MockListener listener3("Listener3");

        // Register three listeners
        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);
        manager.registerListener<TestEvent>(&listener3);

        // Remove all listeners one by one
        manager.unregisterListener<TestEvent>(&listener1);
        manager.unregisterListener<TestEvent>(&listener2);
        manager.unregisterListener<TestEvent>(&listener3);

        // No events should be dispatched
        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(0);
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(0);
        EXPECT_CALL(listener3, handleEvent(::testing::_)).Times(0);

        manager.emitEvent(std::make_shared<TestEvent>(42));
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, ListenerRegistrationOrder) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");
        MockListener listener3("Listener3");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);
        manager.registerListener<TestEvent>(&listener3);

        auto testEvent = std::make_shared<TestEvent>(42);

        // Listeners should be called in the order they were registered
        testing::InSequence sequence;
        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener3, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, SameListenerRegisteredTwiceForSameEvent) {
        EventManager manager;
        MockListener listener("MockListener");

        // Register the same listener twice
        manager.registerListener<TestEvent>(&listener);
        manager.registerListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(42);

        // The listener should be called twice since it's registered twice
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(2);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, UnregisterOnlyOneInstanceOfDuplicateListener) {
        EventManager manager;
        MockListener listener("MockListener");

        // Register the same listener twice
        manager.registerListener<TestEvent>(&listener);
        manager.registerListener<TestEvent>(&listener);

        // Unregister once (should remove only the first occurrence)
        manager.unregisterListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(42);

        // The listener should still be called once since one registration remains
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, ClearEventsAfterPartialDispatch) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        // Emit multiple events
        manager.emitEvent(std::make_shared<TestEvent>(1));
        manager.emitEvent(std::make_shared<TestEvent>(2));
        manager.emitEvent(std::make_shared<TestEvent>(3));

        // First dispatch processes all events and requeues them
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(3);
        manager.dispatchEvents();

        // Clear the requeued events
        manager.clearEvents();

        // No events should be dispatched since queue was cleared
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(0);
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, EmitEventsAfterClear) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        // Emit and clear
        manager.emitEvent(std::make_shared<TestEvent>(1));
        manager.clearEvents();

        // Emit new events after clearing
        manager.emitEvent(std::make_shared<TestEvent>(2));
        manager.emitEvent(std::make_shared<TestEvent>(3));

        // Only the two new events should be dispatched
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(2);
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, ConsumedEventStopsListenerChain) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");
        MockListener listener3("Listener3");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);
        manager.registerListener<TestEvent>(&listener3);

        auto testEvent = std::make_shared<TestEvent>(42);

        // First listener receives the event but doesn't consume
        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(1);
        // Second listener consumes the event
        EXPECT_CALL(listener2, handleEvent(::testing::_))
            .WillOnce([](TestEvent& event) { event.consumed = true; });
        // Third listener should not receive the event
        EXPECT_CALL(listener3, handleEvent(::testing::_)).Times(0);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, EventConsumptionDoesNotAffectRequeuing) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(42);

        // First dispatch: consume the event
        EXPECT_CALL(listener, handleEvent(::testing::_))
            .WillOnce([](TestEvent& event) { event.consumed = true; });
        manager.emitEvent(testEvent);
        manager.dispatchEvents();

        // Event is requeued even if consumed, but consumed flag should be reset
        // Second dispatch should process the event again
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, MultipleClearsCalls) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        // Multiple clears should not crash
        manager.clearEvents();
        manager.clearEvents();
        manager.clearEvents();

        // Emit and dispatch should work normally
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        manager.emitEvent(std::make_shared<TestEvent>(42));
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, DispatchWithNoRegisteredListeners) {
        EventManager manager;

        // Emit events without any registered listeners
        manager.emitEvent(std::make_shared<TestEvent>(1));
        manager.emitEvent(std::make_shared<TestEvent>(2));

        // Should not crash when dispatching events with no listeners
        manager.dispatchEvents();
        SUCCEED();
    }

    TEST(EventManagerEdgeCaseTest, UnregisterListenerDuringMultipleEventTypes) {
        EventManager manager;
        MultiEventListener listener("MultiListener");

        // Register for both event types
        manager.registerListener<TestEvent>(&listener);
        manager.registerListener<AnotherTestEvent>(&listener);

        // Unregister from only one event type
        manager.unregisterListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(42);
        auto anotherEvent = std::make_shared<AnotherTestEvent>("Hello");

        // TestEvent handler should not be called
        EXPECT_CALL(listener, handleEvent(::testing::An<TestEvent&>())).Times(0);
        // AnotherTestEvent handler should still be called
        EXPECT_CALL(listener, handleEvent(::testing::An<AnotherTestEvent&>())).Times(1);

        manager.emitEvent(testEvent);
        manager.emitEvent(anotherEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerEdgeCaseTest, ClearEventsDoesNotAffectListeners) {
        EventManager manager;
        MockListener listener("MockListener");

        manager.registerListener<TestEvent>(&listener);

        // Emit and clear events
        manager.emitEvent(std::make_shared<TestEvent>(1));
        manager.clearEvents();

        // Listeners should still be registered and functional
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        manager.emitEvent(std::make_shared<TestEvent>(2));
        manager.dispatchEvents();
    }
}
