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

    // ===== Extended Tests for Event Listener System =====

    // Multiple listeners for same event type tests
    TEST(EventManagerExtendedTest, ThreeListenersForSameEventAllReceive) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");
        MockListener listener3("Listener3");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);
        manager.registerListener<TestEvent>(&listener3);

        auto testEvent = std::make_shared<TestEvent>(100);

        // All three listeners should receive the event
        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener3, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, FiveListenersForSameEventSequentialExecution) {
        EventManager manager;
        MockListener l1("L1"), l2("L2"), l3("L3"), l4("L4"), l5("L5");

        manager.registerListener<TestEvent>(&l1);
        manager.registerListener<TestEvent>(&l2);
        manager.registerListener<TestEvent>(&l3);
        manager.registerListener<TestEvent>(&l4);
        manager.registerListener<TestEvent>(&l5);

        auto testEvent = std::make_shared<TestEvent>(50);

        // All five listeners should execute in order
        testing::InSequence sequence;
        EXPECT_CALL(l1, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(l2, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(l3, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(l4, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(l5, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, MultipleListenersWithDifferentEventTypes) {
        EventManager manager;
        MockListener testListener1("TestListener1");
        MockListener testListener2("TestListener2");
        AnotherMockListener anotherListener1("AnotherListener1");
        AnotherMockListener anotherListener2("AnotherListener2");

        manager.registerListener<TestEvent>(&testListener1);
        manager.registerListener<TestEvent>(&testListener2);
        manager.registerListener<AnotherTestEvent>(&anotherListener1);
        manager.registerListener<AnotherTestEvent>(&anotherListener2);

        auto testEvent = std::make_shared<TestEvent>(42);
        auto anotherEvent = std::make_shared<AnotherTestEvent>("Test");

        // Each event type's listeners should only receive their event type
        EXPECT_CALL(testListener1, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(testListener2, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(anotherListener1, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(anotherListener2, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.emitEvent(anotherEvent);
        manager.dispatchEvents();
    }

    // Listener registration and unregistration tests
    TEST(EventManagerExtendedTest, RegisterListenerMultipleTimesForDifferentEvents) {
        EventManager manager;

        // Create a listener that can handle multiple event types
        class MultiListener : public Listens<TestEvent, AnotherTestEvent> {
        public:
            explicit MultiListener(const std::string& name = "") : Listens(name) {}
            MOCK_METHOD(void, handleEvent, (TestEvent&), (override));
            MOCK_METHOD(void, handleEvent, (AnotherTestEvent&), (override));
        };

        MultiListener listener("MultiListener");

        manager.registerListener<TestEvent>(&listener);
        manager.registerListener<AnotherTestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(10);
        auto anotherEvent = std::make_shared<AnotherTestEvent>("Multi");

        EXPECT_CALL(listener, handleEvent(testing::A<TestEvent&>())).Times(1);
        EXPECT_CALL(listener, handleEvent(testing::A<AnotherTestEvent&>())).Times(1);

        manager.emitEvent(testEvent);
        manager.emitEvent(anotherEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, UnregisterSpecificListenerFromMultiple) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");
        MockListener listener3("Listener3");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);
        manager.registerListener<TestEvent>(&listener3);

        // Unregister the middle listener
        manager.unregisterListener<TestEvent>(&listener2);

        auto testEvent = std::make_shared<TestEvent>(25);

        // Only listener1 and listener3 should receive the event
        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(0);
        EXPECT_CALL(listener3, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, UnregisterFirstListenerFromMultiple) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");
        MockListener listener3("Listener3");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);
        manager.registerListener<TestEvent>(&listener3);

        manager.unregisterListener<TestEvent>(&listener1);

        auto testEvent = std::make_shared<TestEvent>(30);

        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(0);
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener3, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, UnregisterLastListenerFromMultiple) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");
        MockListener listener3("Listener3");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);
        manager.registerListener<TestEvent>(&listener3);

        manager.unregisterListener<TestEvent>(&listener3);

        auto testEvent = std::make_shared<TestEvent>(35);

        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener3, handleEvent(::testing::_)).Times(0);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, ReregisterListenerAfterUnregister) {
        EventManager manager;
        MockListener listener("Listener");

        manager.registerListener<TestEvent>(&listener);
        manager.unregisterListener<TestEvent>(&listener);
        manager.registerListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(40);

        // Listener should receive the event since it was re-registered
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    // Event emission with shared_ptr tests
    TEST(EventManagerExtendedTest, EmitEventWithSharedPtr) {
        EventManager manager;
        MockListener listener("Listener");

        manager.registerListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(100);

        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, EmitEventWithInlineConstruction) {
        EventManager manager;
        MockListener listener("Listener");

        manager.registerListener<TestEvent>(&listener);

        // Emit event using template variadic constructor
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);

        manager.emitEvent<TestEvent>(200);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, EmitMultipleSharedPtrEvents) {
        EventManager manager;
        MockListener listener("Listener");

        manager.registerListener<TestEvent>(&listener);

        auto event1 = std::make_shared<TestEvent>(1);
        auto event2 = std::make_shared<TestEvent>(2);
        auto event3 = std::make_shared<TestEvent>(3);

        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(3);

        manager.emitEvent(event1);
        manager.emitEvent(event2);
        manager.emitEvent(event3);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, EmitMixedSharedPtrAndInlineEvents) {
        EventManager manager;
        MockListener listener("Listener");

        manager.registerListener<TestEvent>(&listener);

        auto event1 = std::make_shared<TestEvent>(10);

        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(3);

        manager.emitEvent(event1);
        manager.emitEvent<TestEvent>(20);
        manager.emitEvent(std::make_shared<TestEvent>(30));
        manager.dispatchEvents();
    }

    // Event dispatching and queue processing tests
    TEST(EventManagerExtendedTest, DispatchProcessesEventsInFIFOOrder) {
        EventManager manager;

        class OrderTrackingListener : public Listens<TestEvent> {
        public:
            explicit OrderTrackingListener(const std::string& name = "") : Listens(name) {}
            void handleEvent(TestEvent& event) override {
                received_order.push_back(event.data);
            }
            std::vector<int> received_order;
        };

        OrderTrackingListener listener("OrderListener");
        manager.registerListener<TestEvent>(&listener);

        manager.emitEvent<TestEvent>(1);
        manager.emitEvent<TestEvent>(2);
        manager.emitEvent<TestEvent>(3);
        manager.emitEvent<TestEvent>(4);
        manager.emitEvent<TestEvent>(5);

        manager.dispatchEvents();

        ASSERT_EQ(listener.received_order.size(), 5);
        EXPECT_EQ(listener.received_order[0], 1);
        EXPECT_EQ(listener.received_order[1], 2);
        EXPECT_EQ(listener.received_order[2], 3);
        EXPECT_EQ(listener.received_order[3], 4);
        EXPECT_EQ(listener.received_order[4], 5);
    }

    TEST(EventManagerExtendedTest, MultipleDispatchCallsProcessQueue) {
        EventManager manager;
        MockListener listener("Listener");

        manager.registerListener<TestEvent>(&listener);

        manager.emitEvent<TestEvent>(1);

        // First dispatch
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        manager.dispatchEvents();

        // Second dispatch (event was requeued)
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        manager.dispatchEvents();

        // Third dispatch
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, EmitDuringDispatchDoesNotCauseImmediateDispatch) {
        EventManager manager;

        class EmittingListener : public Listens<TestEvent> {
        public:
            explicit EmittingListener(EventManager* mgr, const std::string& name = "")
                : Listens(name), manager(mgr), call_count(0) {}

            void handleEvent(TestEvent& event) override {
                call_count++;
                // Emit another event during handling
                if (call_count == 1) {
                    manager->emitEvent<TestEvent>(999);
                }
            }

            EventManager* manager;
            int call_count;
        };

        EmittingListener listener(&manager, "EmittingListener");
        manager.registerListener<TestEvent>(&listener);

        manager.emitEvent<TestEvent>(1);
        manager.dispatchEvents();

        // First dispatch should process the initial event and requeue it,
        // plus add the new event emitted during handling
        // The listener was called once during first dispatch
        EXPECT_EQ(listener.call_count, 1);

        // Second dispatch should process both requeued events
        manager.dispatchEvents();
        EXPECT_GE(listener.call_count, 2);
    }

    // Event consumption flag handling tests
    TEST(EventManagerExtendedTest, FirstListenerConsumesEventStopsChain) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");
        MockListener listener3("Listener3");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);
        manager.registerListener<TestEvent>(&listener3);

        auto testEvent = std::make_shared<TestEvent>(100);

        // First listener consumes the event
        EXPECT_CALL(listener1, handleEvent(::testing::_))
            .WillOnce([](TestEvent& event) { event.consumed = true; });
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(0);
        EXPECT_CALL(listener3, handleEvent(::testing::_)).Times(0);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, MiddleListenerConsumesEvent) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");
        MockListener listener3("Listener3");
        MockListener listener4("Listener4");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);
        manager.registerListener<TestEvent>(&listener3);
        manager.registerListener<TestEvent>(&listener4);

        auto testEvent = std::make_shared<TestEvent>(50);

        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(1);
        // Third listener consumes
        EXPECT_CALL(listener3, handleEvent(::testing::_))
            .WillOnce([](TestEvent& event) { event.consumed = true; });
        EXPECT_CALL(listener4, handleEvent(::testing::_)).Times(0);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, NoListenerConsumesEventAllReceive) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");
        MockListener listener3("Listener3");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);
        manager.registerListener<TestEvent>(&listener3);

        auto testEvent = std::make_shared<TestEvent>(75);

        // None consume, all should receive
        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(1);
        EXPECT_CALL(listener3, handleEvent(::testing::_)).Times(1);

        manager.emitEvent(testEvent);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, ConsumedEventIsRequeuedButNotReconsumed) {
        EventManager manager;

        class ConsumptionTracker : public Listens<TestEvent> {
        public:
            explicit ConsumptionTracker(const std::string& name = "")
                : Listens(name), consume_on_first_call(true), call_count(0) {}

            void handleEvent(TestEvent& event) override {
                call_count++;
                if (call_count == 1 && consume_on_first_call) {
                    event.consumed = true;
                }
            }

            bool consume_on_first_call;
            int call_count;
        };

        ConsumptionTracker listener("Tracker");
        manager.registerListener<TestEvent>(&listener);

        auto testEvent = std::make_shared<TestEvent>(25);

        manager.emitEvent(testEvent);

        // First dispatch: event consumed
        manager.dispatchEvents();
        EXPECT_EQ(listener.call_count, 1);

        // Second dispatch: event requeued and processed again
        manager.dispatchEvents();
        EXPECT_EQ(listener.call_count, 2);
    }

    // Listener chain execution order tests
    TEST(EventManagerExtendedTest, ListenerChainExecutesInRegistrationOrder) {
        EventManager manager;

        class OrderedListener : public Listens<TestEvent> {
        public:
            explicit OrderedListener(int id, std::vector<int>* order, const std::string& name = "")
                : Listens(name), listener_id(id), execution_order(order) {}

            void handleEvent(TestEvent&) override {
                execution_order->push_back(listener_id);
            }

            int listener_id;
            std::vector<int>* execution_order;
        };

        std::vector<int> order;
        OrderedListener l1(1, &order, "L1");
        OrderedListener l2(2, &order, "L2");
        OrderedListener l3(3, &order, "L3");
        OrderedListener l4(4, &order, "L4");
        OrderedListener l5(5, &order, "L5");

        manager.registerListener<TestEvent>(&l1);
        manager.registerListener<TestEvent>(&l2);
        manager.registerListener<TestEvent>(&l3);
        manager.registerListener<TestEvent>(&l4);
        manager.registerListener<TestEvent>(&l5);

        manager.emitEvent<TestEvent>(1);
        manager.dispatchEvents();

        ASSERT_EQ(order.size(), 5);
        EXPECT_EQ(order[0], 1);
        EXPECT_EQ(order[1], 2);
        EXPECT_EQ(order[2], 3);
        EXPECT_EQ(order[3], 4);
        EXPECT_EQ(order[4], 5);
    }

    TEST(EventManagerExtendedTest, ListenerChainReverseRegistrationOrder) {
        EventManager manager;

        class OrderedListener : public Listens<TestEvent> {
        public:
            explicit OrderedListener(int id, std::vector<int>* order, const std::string& name = "")
                : Listens(name), listener_id(id), execution_order(order) {}

            void handleEvent(TestEvent&) override {
                execution_order->push_back(listener_id);
            }

            int listener_id;
            std::vector<int>* execution_order;
        };

        std::vector<int> order;
        OrderedListener l1(1, &order, "L1");
        OrderedListener l2(2, &order, "L2");
        OrderedListener l3(3, &order, "L3");

        // Register in reverse order
        manager.registerListener<TestEvent>(&l3);
        manager.registerListener<TestEvent>(&l2);
        manager.registerListener<TestEvent>(&l1);

        manager.emitEvent<TestEvent>(1);
        manager.dispatchEvents();

        // Should execute in registration order: 3, 2, 1
        ASSERT_EQ(order.size(), 3);
        EXPECT_EQ(order[0], 3);
        EXPECT_EQ(order[1], 2);
        EXPECT_EQ(order[2], 1);
    }

    TEST(EventManagerExtendedTest, ConsumptionStopsChainMidExecution) {
        EventManager manager;

        class OrderedConsumer : public Listens<TestEvent> {
        public:
            explicit OrderedConsumer(int id, bool consume, std::vector<int>* order, const std::string& name = "")
                : Listens(name), listener_id(id), should_consume(consume), execution_order(order) {}

            void handleEvent(TestEvent& event) override {
                execution_order->push_back(listener_id);
                if (should_consume) {
                    event.consumed = true;
                }
            }

            int listener_id;
            bool should_consume;
            std::vector<int>* execution_order;
        };

        std::vector<int> order;
        OrderedConsumer l1(1, false, &order, "L1");
        OrderedConsumer l2(2, false, &order, "L2");
        OrderedConsumer l3(3, true, &order, "L3");  // Consumes
        OrderedConsumer l4(4, false, &order, "L4");
        OrderedConsumer l5(5, false, &order, "L5");

        manager.registerListener<TestEvent>(&l1);
        manager.registerListener<TestEvent>(&l2);
        manager.registerListener<TestEvent>(&l3);
        manager.registerListener<TestEvent>(&l4);
        manager.registerListener<TestEvent>(&l5);

        manager.emitEvent<TestEvent>(1);
        manager.dispatchEvents();

        // Only 1, 2, 3 should execute
        ASSERT_EQ(order.size(), 3);
        EXPECT_EQ(order[0], 1);
        EXPECT_EQ(order[1], 2);
        EXPECT_EQ(order[2], 3);
    }

    // Clear events functionality tests
    TEST(EventManagerExtendedTest, ClearEventsRemovesSingleEvent) {
        EventManager manager;
        MockListener listener("Listener");

        manager.registerListener<TestEvent>(&listener);

        manager.emitEvent<TestEvent>(1);
        manager.clearEvents();

        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(0);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, ClearEventsRemovesMultipleEvents) {
        EventManager manager;
        MockListener listener("Listener");

        manager.registerListener<TestEvent>(&listener);

        manager.emitEvent<TestEvent>(1);
        manager.emitEvent<TestEvent>(2);
        manager.emitEvent<TestEvent>(3);
        manager.emitEvent<TestEvent>(4);
        manager.emitEvent<TestEvent>(5);

        manager.clearEvents();

        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(0);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, ClearEventsBetweenDispatches) {
        EventManager manager;
        MockListener listener("Listener");

        manager.registerListener<TestEvent>(&listener);

        manager.emitEvent<TestEvent>(1);

        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        manager.dispatchEvents();

        // Clear the requeued event
        manager.clearEvents();

        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(0);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, ClearEventsWithMixedEventTypes) {
        EventManager manager;
        MockListener testListener("TestListener");
        AnotherMockListener anotherListener("AnotherListener");

        manager.registerListener<TestEvent>(&testListener);
        manager.registerListener<AnotherTestEvent>(&anotherListener);

        manager.emitEvent<TestEvent>(1);
        manager.emitEvent<AnotherTestEvent>("test");
        manager.emitEvent<TestEvent>(2);

        manager.clearEvents();

        EXPECT_CALL(testListener, handleEvent(::testing::_)).Times(0);
        EXPECT_CALL(anotherListener, handleEvent(::testing::_)).Times(0);
        manager.dispatchEvents();
    }

    // Edge cases: unregister non-existent, emit with no listeners
    TEST(EventManagerExtendedTest, UnregisterNonExistentListenerDoesNotCrash) {
        EventManager manager;
        MockListener listener("Listener");

        // Try to unregister without registering first
        manager.unregisterListener<TestEvent>(&listener);
        SUCCEED();
    }

    TEST(EventManagerExtendedTest, UnregisterFromWrongEventType) {
        EventManager manager;
        MockListener listener("Listener");

        manager.registerListener<TestEvent>(&listener);

        // Try to unregister from a different event type
        manager.unregisterListener<AnotherTestEvent>(reinterpret_cast<BaseListener*>(&listener));
        SUCCEED();

        // Original registration should still work
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        manager.emitEvent<TestEvent>(1);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, EmitEventWithNoRegisteredListeners) {
        EventManager manager;

        // Emit multiple events without any listeners
        manager.emitEvent<TestEvent>(1);
        manager.emitEvent<TestEvent>(2);
        manager.emitEvent<TestEvent>(3);

        // Should not crash
        manager.dispatchEvents();
        SUCCEED();
    }

    TEST(EventManagerExtendedTest, EmitMultipleEventTypesWithNoListeners) {
        EventManager manager;

        manager.emitEvent<TestEvent>(1);
        manager.emitEvent<AnotherTestEvent>("test");
        manager.emitEvent<TestEvent>(2);
        manager.emitEvent<AnotherTestEvent>("test2");

        // Should not crash
        manager.dispatchEvents();
        SUCCEED();
    }

    TEST(EventManagerExtendedTest, UnregisterAllListenersThenEmit) {
        EventManager manager;
        MockListener listener1("Listener1");
        MockListener listener2("Listener2");

        manager.registerListener<TestEvent>(&listener1);
        manager.registerListener<TestEvent>(&listener2);

        manager.unregisterListener<TestEvent>(&listener1);
        manager.unregisterListener<TestEvent>(&listener2);

        EXPECT_CALL(listener1, handleEvent(::testing::_)).Times(0);
        EXPECT_CALL(listener2, handleEvent(::testing::_)).Times(0);

        manager.emitEvent<TestEvent>(1);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, RegisterUnregisterRegisterSameListener) {
        EventManager manager;
        MockListener listener("Listener");

        manager.registerListener<TestEvent>(&listener);
        manager.unregisterListener<TestEvent>(&listener);
        manager.registerListener<TestEvent>(&listener);
        manager.unregisterListener<TestEvent>(&listener);
        manager.registerListener<TestEvent>(&listener);

        // Final state: listener should be registered
        EXPECT_CALL(listener, handleEvent(::testing::_)).Times(1);
        manager.emitEvent<TestEvent>(1);
        manager.dispatchEvents();
    }

    TEST(EventManagerExtendedTest, EmitHundredEventsProcessedCorrectly) {
        EventManager manager;

        class CountingListener : public Listens<TestEvent> {
        public:
            explicit CountingListener(const std::string& name = "") : Listens(name), count(0) {}
            void handleEvent(TestEvent&) override { count++; }
            int count;
        };

        CountingListener listener("Counter");
        manager.registerListener<TestEvent>(&listener);

        // Emit 100 events
        for (int i = 0; i < 100; i++) {
            manager.emitEvent<TestEvent>(i);
        }

        manager.dispatchEvents();
        EXPECT_EQ(listener.count, 100);
    }

    TEST(EventManagerExtendedTest, StressTestMultipleListenersAndEvents) {
        EventManager manager;

        class CountingListener : public Listens<TestEvent> {
        public:
            explicit CountingListener(const std::string& name = "") : Listens(name), count(0) {}
            void handleEvent(TestEvent&) override { count++; }
            int count;
        };

        CountingListener l1("L1"), l2("L2"), l3("L3"), l4("L4"), l5("L5");

        manager.registerListener<TestEvent>(&l1);
        manager.registerListener<TestEvent>(&l2);
        manager.registerListener<TestEvent>(&l3);
        manager.registerListener<TestEvent>(&l4);
        manager.registerListener<TestEvent>(&l5);

        // Emit 20 events
        for (int i = 0; i < 20; i++) {
            manager.emitEvent<TestEvent>(i);
        }

        manager.dispatchEvents();

        // Each listener should have received all 20 events
        EXPECT_EQ(l1.count, 20);
        EXPECT_EQ(l2.count, 20);
        EXPECT_EQ(l3.count, 20);
        EXPECT_EQ(l4.count, 20);
        EXPECT_EQ(l5.count, 20);
    }
}
