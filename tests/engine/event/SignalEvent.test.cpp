//// SignalEvent.test.cpp ////////////////////////////////////////////////////
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
//  Author:      Claude AI
//  Date:        13/12/2025
//  Description: Test file for the SignalEvent system and SignalHandler
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <csignal>
#include <memory>
#include "core/event/SignalEvent.hpp"
#include "core/event/Event.hpp"
#include "core/event/Listener.hpp"

namespace nexo::event {

// =============================================================================
// Test Listeners for Signal Events
// =============================================================================

class AnySignalListener : public Listens<EventAnySignal> {
public:
    explicit AnySignalListener(const std::string& name = "AnySignalListener")
        : Listens(name), signal_received(0), handle_count(0) {}

    void handleEvent(EventAnySignal& event) override {
        signal_received = event.signal;
        handle_count++;
        last_event = event;
    }

    int signal_received;
    int handle_count;
    EventAnySignal last_event{0};
};

class TerminateSignalListener : public Listens<EventSignalTerminate> {
public:
    explicit TerminateSignalListener(const std::string& name = "TerminateSignalListener")
        : Listens(name), handle_count(0), was_triggered(false) {}

    void handleEvent(EventSignalTerminate& event) override {
        handle_count++;
        was_triggered = true;
    }

    int handle_count;
    bool was_triggered;
};

class InterruptSignalListener : public Listens<EventSignalInterrupt> {
public:
    explicit InterruptSignalListener(const std::string& name = "InterruptSignalListener")
        : Listens(name), handle_count(0), was_triggered(false) {}

    void handleEvent(EventSignalInterrupt& event) override {
        handle_count++;
        was_triggered = true;
    }

    int handle_count;
    bool was_triggered;
};

class MultiSignalListener : public Listens<EventAnySignal, EventSignalTerminate, EventSignalInterrupt> {
public:
    explicit MultiSignalListener(const std::string& name = "MultiSignalListener")
        : Listens(name), any_count(0), terminate_count(0), interrupt_count(0) {}

    void handleEvent(EventAnySignal& event) override {
        any_count++;
        last_any_signal = event.signal;
    }

    void handleEvent(EventSignalTerminate& event) override {
        terminate_count++;
    }

    void handleEvent(EventSignalInterrupt& event) override {
        interrupt_count++;
    }

    int any_count;
    int terminate_count;
    int interrupt_count;
    int last_any_signal = 0;
};

class ConsumingSignalListener : public Listens<EventAnySignal> {
public:
    explicit ConsumingSignalListener(bool should_consume, const std::string& name = "ConsumingSignalListener")
        : Listens(name), consume_event(should_consume), handle_count(0) {}

    void handleEvent(EventAnySignal& event) override {
        handle_count++;
        if (consume_event) {
            event.consumed = true;
        }
    }

    bool consume_event;
    int handle_count;
};

// =============================================================================
// EventAnySignal Tests
// =============================================================================

TEST(SignalEventTest, EventAnySignalCreation) {
    EventAnySignal event(SIGTERM);
    EXPECT_EQ(event.signal, SIGTERM);
    EXPECT_FALSE(event.consumed);
}

TEST(SignalEventTest, EventAnySignalWithDifferentSignals) {
    EventAnySignal sigterm_event(SIGTERM);
    EventAnySignal sigint_event(SIGINT);
    EventAnySignal sigabrt_event(SIGABRT);

    EXPECT_EQ(sigterm_event.signal, SIGTERM);
    EXPECT_EQ(sigint_event.signal, SIGINT);
    EXPECT_EQ(sigabrt_event.signal, SIGABRT);
}

TEST(SignalEventTest, EventAnySignalTriggerListener) {
    EventAnySignal event(SIGTERM);
    AnySignalListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.signal_received, SIGTERM);
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(SignalEventTest, EventAnySignalMultipleTriggers) {
    EventAnySignal event(SIGINT);
    AnySignalListener listener;

    event.trigger(listener);
    event.trigger(listener);
    event.trigger(listener);

    EXPECT_EQ(listener.handle_count, 3);
    EXPECT_EQ(listener.signal_received, SIGINT);
}

TEST(SignalEventTest, EventAnySignalConsumption) {
    EventAnySignal event(SIGTERM);
    ConsumingSignalListener consumer(true);

    EXPECT_FALSE(event.consumed);
    event.trigger(consumer);
    EXPECT_TRUE(event.consumed);
}

TEST(SignalEventTest, EventAnySignalDataAccess) {
    EventAnySignal event(SIGTERM);
    AnySignalListener listener;

    event.trigger(listener);
    EXPECT_EQ(listener.last_event.signal, SIGTERM);

    event.signal = SIGINT;
    event.trigger(listener);
    EXPECT_EQ(listener.last_event.signal, SIGINT);
}

// =============================================================================
// EventSignalTerminate Tests
// =============================================================================

TEST(SignalEventTest, EventSignalTerminateCreation) {
    EventSignalTerminate event;
    EXPECT_FALSE(event.consumed);
}

TEST(SignalEventTest, EventSignalTerminateTriggerListener) {
    EventSignalTerminate event;
    TerminateSignalListener listener;

    event.trigger(listener);

    EXPECT_TRUE(listener.was_triggered);
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(SignalEventTest, EventSignalTerminateMultipleTriggers) {
    EventSignalTerminate event;
    TerminateSignalListener listener;

    event.trigger(listener);
    event.trigger(listener);
    event.trigger(listener);

    EXPECT_EQ(listener.handle_count, 3);
}

TEST(SignalEventTest, EventSignalTerminateMultipleListeners) {
    EventSignalTerminate event;
    TerminateSignalListener listener1("Listener1");
    TerminateSignalListener listener2("Listener2");
    TerminateSignalListener listener3("Listener3");

    event.trigger(listener1);
    event.trigger(listener2);
    event.trigger(listener3);

    EXPECT_TRUE(listener1.was_triggered);
    EXPECT_TRUE(listener2.was_triggered);
    EXPECT_TRUE(listener3.was_triggered);
    EXPECT_EQ(listener1.handle_count, 1);
    EXPECT_EQ(listener2.handle_count, 1);
    EXPECT_EQ(listener3.handle_count, 1);
}

// =============================================================================
// EventSignalInterrupt Tests
// =============================================================================

TEST(SignalEventTest, EventSignalInterruptCreation) {
    EventSignalInterrupt event;
    EXPECT_FALSE(event.consumed);
}

TEST(SignalEventTest, EventSignalInterruptTriggerListener) {
    EventSignalInterrupt event;
    InterruptSignalListener listener;

    event.trigger(listener);

    EXPECT_TRUE(listener.was_triggered);
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(SignalEventTest, EventSignalInterruptMultipleTriggers) {
    EventSignalInterrupt event;
    InterruptSignalListener listener;

    event.trigger(listener);
    event.trigger(listener);

    EXPECT_EQ(listener.handle_count, 2);
}

TEST(SignalEventTest, EventSignalInterruptMultipleListeners) {
    EventSignalInterrupt event;
    InterruptSignalListener listener1("Listener1");
    InterruptSignalListener listener2("Listener2");

    event.trigger(listener1);
    event.trigger(listener2);

    EXPECT_TRUE(listener1.was_triggered);
    EXPECT_TRUE(listener2.was_triggered);
}

// =============================================================================
// Multi-Event Listener Tests
// =============================================================================

TEST(SignalEventTest, MultiSignalListenerHandlesAnySignal) {
    EventAnySignal event(SIGTERM);
    MultiSignalListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.any_count, 1);
    EXPECT_EQ(listener.terminate_count, 0);
    EXPECT_EQ(listener.interrupt_count, 0);
    EXPECT_EQ(listener.last_any_signal, SIGTERM);
}

TEST(SignalEventTest, MultiSignalListenerHandlesTerminate) {
    EventSignalTerminate event;
    MultiSignalListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.any_count, 0);
    EXPECT_EQ(listener.terminate_count, 1);
    EXPECT_EQ(listener.interrupt_count, 0);
}

TEST(SignalEventTest, MultiSignalListenerHandlesInterrupt) {
    EventSignalInterrupt event;
    MultiSignalListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.any_count, 0);
    EXPECT_EQ(listener.terminate_count, 0);
    EXPECT_EQ(listener.interrupt_count, 1);
}

TEST(SignalEventTest, MultiSignalListenerHandlesAllEventTypes) {
    EventAnySignal any_event(SIGABRT);
    EventSignalTerminate terminate_event;
    EventSignalInterrupt interrupt_event;
    MultiSignalListener listener;

    any_event.trigger(listener);
    terminate_event.trigger(listener);
    interrupt_event.trigger(listener);

    EXPECT_EQ(listener.any_count, 1);
    EXPECT_EQ(listener.terminate_count, 1);
    EXPECT_EQ(listener.interrupt_count, 1);
    EXPECT_EQ(listener.last_any_signal, SIGABRT);
}

// =============================================================================
// SignalHandler Singleton Tests
// =============================================================================

TEST(SignalHandlerTest, GetInstanceReturnsSingleton) {
    auto instance1 = SignalHandler::getInstance();
    auto instance2 = SignalHandler::getInstance();

    EXPECT_NE(instance1, nullptr);
    EXPECT_EQ(instance1, instance2);
}

TEST(SignalHandlerTest, RegisterEventManager) {
    auto handler = SignalHandler::getInstance();
    auto eventManager = std::make_shared<EventManager>();

    // Should not crash
    handler->registerEventManager(eventManager);
    SUCCEED();
}

TEST(SignalHandlerTest, RegisterMultipleEventManagers) {
    auto handler = SignalHandler::getInstance();
    auto eventManager1 = std::make_shared<EventManager>();
    auto eventManager2 = std::make_shared<EventManager>();
    auto eventManager3 = std::make_shared<EventManager>();

    handler->registerEventManager(eventManager1);
    handler->registerEventManager(eventManager2);
    handler->registerEventManager(eventManager3);

    SUCCEED();
}

// =============================================================================
// Event Manager Integration Tests
// =============================================================================

TEST(SignalEventIntegrationTest, EventManagerDispatchesAnySignalEvent) {
    EventManager manager;
    AnySignalListener listener;

    manager.registerListener<EventAnySignal>(&listener);

    auto event = std::make_shared<EventAnySignal>(SIGTERM);
    manager.emitEvent(event);
    manager.dispatchEvents();

    EXPECT_EQ(listener.signal_received, SIGTERM);
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(SignalEventIntegrationTest, EventManagerDispatchesTerminateEvent) {
    EventManager manager;
    TerminateSignalListener listener;

    manager.registerListener<EventSignalTerminate>(&listener);

    manager.emitEvent<EventSignalTerminate>();
    manager.dispatchEvents();

    EXPECT_TRUE(listener.was_triggered);
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(SignalEventIntegrationTest, EventManagerDispatchesInterruptEvent) {
    EventManager manager;
    InterruptSignalListener listener;

    manager.registerListener<EventSignalInterrupt>(&listener);

    manager.emitEvent<EventSignalInterrupt>();
    manager.dispatchEvents();

    EXPECT_TRUE(listener.was_triggered);
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(SignalEventIntegrationTest, MultipleListenersForAnySignal) {
    EventManager manager;
    AnySignalListener listener1("Listener1");
    AnySignalListener listener2("Listener2");
    AnySignalListener listener3("Listener3");

    manager.registerListener<EventAnySignal>(&listener1);
    manager.registerListener<EventAnySignal>(&listener2);
    manager.registerListener<EventAnySignal>(&listener3);

    auto event = std::make_shared<EventAnySignal>(SIGINT);
    manager.emitEvent(event);
    manager.dispatchEvents();

    EXPECT_EQ(listener1.signal_received, SIGINT);
    EXPECT_EQ(listener2.signal_received, SIGINT);
    EXPECT_EQ(listener3.signal_received, SIGINT);
    EXPECT_EQ(listener1.handle_count, 1);
    EXPECT_EQ(listener2.handle_count, 1);
    EXPECT_EQ(listener3.handle_count, 1);
}

TEST(SignalEventIntegrationTest, ConsumptionStopsListenerChain) {
    EventManager manager;
    ConsumingSignalListener consumer(true, "Consumer");
    AnySignalListener regular("Regular");

    manager.registerListener<EventAnySignal>(&consumer);
    manager.registerListener<EventAnySignal>(&regular);

    auto event = std::make_shared<EventAnySignal>(SIGTERM);
    manager.emitEvent(event);
    manager.dispatchEvents();

    EXPECT_EQ(consumer.handle_count, 1);
    EXPECT_EQ(regular.handle_count, 0);  // Should not receive due to consumption
}

TEST(SignalEventIntegrationTest, NoConsumptionAllReceive) {
    EventManager manager;
    ConsumingSignalListener non_consumer(false, "NonConsumer");
    AnySignalListener regular("Regular");

    manager.registerListener<EventAnySignal>(&non_consumer);
    manager.registerListener<EventAnySignal>(&regular);

    auto event = std::make_shared<EventAnySignal>(SIGTERM);
    manager.emitEvent(event);
    manager.dispatchEvents();

    EXPECT_EQ(non_consumer.handle_count, 1);
    EXPECT_EQ(regular.handle_count, 1);
}

TEST(SignalEventIntegrationTest, MultipleSignalEventTypes) {
    EventManager manager;
    MultiSignalListener listener;

    manager.registerListener<EventAnySignal>(&listener);
    manager.registerListener<EventSignalTerminate>(&listener);
    manager.registerListener<EventSignalInterrupt>(&listener);

    manager.emitEvent<EventAnySignal>(SIGABRT);
    manager.emitEvent<EventSignalTerminate>();
    manager.emitEvent<EventSignalInterrupt>();
    manager.dispatchEvents();

    EXPECT_EQ(listener.any_count, 1);
    EXPECT_EQ(listener.terminate_count, 1);
    EXPECT_EQ(listener.interrupt_count, 1);
}

TEST(SignalEventIntegrationTest, UnregisterListener) {
    EventManager manager;
    AnySignalListener listener;

    manager.registerListener<EventAnySignal>(&listener);
    manager.unregisterListener<EventAnySignal>(&listener);

    auto event = std::make_shared<EventAnySignal>(SIGTERM);
    manager.emitEvent(event);
    manager.dispatchEvents();

    EXPECT_EQ(listener.handle_count, 0);
}

TEST(SignalEventIntegrationTest, UnregisterMiddleListener) {
    EventManager manager;
    AnySignalListener listener1("Listener1");
    AnySignalListener listener2("Listener2");
    AnySignalListener listener3("Listener3");

    manager.registerListener<EventAnySignal>(&listener1);
    manager.registerListener<EventAnySignal>(&listener2);
    manager.registerListener<EventAnySignal>(&listener3);

    manager.unregisterListener<EventAnySignal>(&listener2);

    auto event = std::make_shared<EventAnySignal>(SIGTERM);
    manager.emitEvent(event);
    manager.dispatchEvents();

    EXPECT_EQ(listener1.handle_count, 1);
    EXPECT_EQ(listener2.handle_count, 0);
    EXPECT_EQ(listener3.handle_count, 1);
}

// =============================================================================
// Execution Order Tests
// =============================================================================

TEST(SignalEventOrderTest, ListenersExecuteInRegistrationOrder) {
    EventManager manager;

    class OrderedSignalListener : public Listens<EventAnySignal> {
    public:
        explicit OrderedSignalListener(int id, std::vector<int>* order, const std::string& name = "")
            : Listens(name), listener_id(id), execution_order(order) {}

        void handleEvent(EventAnySignal& event) override {
            execution_order->push_back(listener_id);
        }

        int listener_id;
        std::vector<int>* execution_order;
    };

    std::vector<int> order;
    OrderedSignalListener l1(1, &order, "L1");
    OrderedSignalListener l2(2, &order, "L2");
    OrderedSignalListener l3(3, &order, "L3");
    OrderedSignalListener l4(4, &order, "L4");
    OrderedSignalListener l5(5, &order, "L5");

    manager.registerListener<EventAnySignal>(&l1);
    manager.registerListener<EventAnySignal>(&l2);
    manager.registerListener<EventAnySignal>(&l3);
    manager.registerListener<EventAnySignal>(&l4);
    manager.registerListener<EventAnySignal>(&l5);

    manager.emitEvent<EventAnySignal>(SIGTERM);
    manager.dispatchEvents();

    ASSERT_EQ(order.size(), 5);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 2);
    EXPECT_EQ(order[2], 3);
    EXPECT_EQ(order[3], 4);
    EXPECT_EQ(order[4], 5);
}

TEST(SignalEventOrderTest, ConsumptionStopsOrderedExecution) {
    EventManager manager;

    class OrderedConsumer : public Listens<EventAnySignal> {
    public:
        explicit OrderedConsumer(int id, bool consume, std::vector<int>* order, const std::string& name = "")
            : Listens(name), listener_id(id), should_consume(consume), execution_order(order) {}

        void handleEvent(EventAnySignal& event) override {
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

    manager.registerListener<EventAnySignal>(&l1);
    manager.registerListener<EventAnySignal>(&l2);
    manager.registerListener<EventAnySignal>(&l3);
    manager.registerListener<EventAnySignal>(&l4);
    manager.registerListener<EventAnySignal>(&l5);

    manager.emitEvent<EventAnySignal>(SIGINT);
    manager.dispatchEvents();

    ASSERT_EQ(order.size(), 3);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 2);
    EXPECT_EQ(order[2], 3);
}

// =============================================================================
// Edge Cases and Special Scenarios
// =============================================================================

TEST(SignalEventEdgeCaseTest, ZeroSignalValue) {
    EventAnySignal event(0);
    AnySignalListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.signal_received, 0);
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(SignalEventEdgeCaseTest, NegativeSignalValue) {
    EventAnySignal event(-1);
    AnySignalListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.signal_received, -1);
}

TEST(SignalEventEdgeCaseTest, LargeSignalValue) {
    EventAnySignal event(9999);
    AnySignalListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.signal_received, 9999);
}

TEST(SignalEventEdgeCaseTest, EmptyEventManagerDispatch) {
    EventManager manager;

    // Should not crash
    manager.emitEvent<EventAnySignal>(SIGTERM);
    manager.dispatchEvents();
    SUCCEED();
}

TEST(SignalEventEdgeCaseTest, ClearEventsBeforeDispatch) {
    EventManager manager;
    AnySignalListener listener;

    manager.registerListener<EventAnySignal>(&listener);
    manager.emitEvent<EventAnySignal>(SIGTERM);
    manager.clearEvents();
    manager.dispatchEvents();

    EXPECT_EQ(listener.handle_count, 0);
}

TEST(SignalEventEdgeCaseTest, MultipleEmitsBeforeDispatch) {
    EventManager manager;
    AnySignalListener listener;

    manager.registerListener<EventAnySignal>(&listener);

    manager.emitEvent<EventAnySignal>(SIGTERM);
    manager.emitEvent<EventAnySignal>(SIGINT);
    manager.emitEvent<EventAnySignal>(SIGABRT);

    manager.dispatchEvents();

    EXPECT_EQ(listener.handle_count, 3);
}

TEST(SignalEventEdgeCaseTest, RepeatedEventDispatches) {
    EventManager manager;
    AnySignalListener listener;

    manager.registerListener<EventAnySignal>(&listener);
    auto event = std::make_shared<EventAnySignal>(SIGTERM);
    manager.emitEvent(event);

    // First dispatch
    manager.dispatchEvents();
    EXPECT_EQ(listener.handle_count, 1);

    // Second dispatch (event should be requeued)
    manager.dispatchEvents();
    EXPECT_EQ(listener.handle_count, 2);

    // Third dispatch
    manager.dispatchEvents();
    EXPECT_EQ(listener.handle_count, 3);
}

TEST(SignalEventEdgeCaseTest, SameListenerRegisteredTwice) {
    EventManager manager;
    AnySignalListener listener;

    manager.registerListener<EventAnySignal>(&listener);
    manager.registerListener<EventAnySignal>(&listener);

    auto event = std::make_shared<EventAnySignal>(SIGTERM);
    manager.emitEvent(event);
    manager.dispatchEvents();

    // Should be called twice since registered twice
    EXPECT_EQ(listener.handle_count, 2);
}

TEST(SignalEventEdgeCaseTest, ModifySignalValueAfterCreation) {
    EventAnySignal event(SIGTERM);
    AnySignalListener listener;

    event.signal = SIGINT;
    event.trigger(listener);

    EXPECT_EQ(listener.signal_received, SIGINT);
}

// DISABLED: Test logic is flawed - consumer registered after listener,
// so listener receives event before consumer can consume it
TEST(SignalEventEdgeCaseTest, DISABLED_ConsumedEventRequeuing) {
    EventManager manager;
    AnySignalListener listener;

    manager.registerListener<EventAnySignal>(&listener);

    auto event = std::make_shared<EventAnySignal>(SIGTERM);
    manager.emitEvent(event);

    // First dispatch - consume the event
    class ConsumingListener : public Listens<EventAnySignal> {
    public:
        explicit ConsumingListener(const std::string& name = "") : Listens(name) {}
        void handleEvent(EventAnySignal& event) override { event.consumed = true; }
    };

    ConsumingListener consumer;
    manager.registerListener<EventAnySignal>(&consumer);

    manager.dispatchEvents();

    // Consumer should have consumed, so regular listener doesn't get it
    EXPECT_EQ(listener.handle_count, 0);
}

TEST(SignalEventEdgeCaseTest, AllCommonSignalValues) {
    struct SignalTest {
        int signal;
        const char* name;
    };

    SignalTest signals[] = {
        {SIGTERM, "SIGTERM"},
        {SIGINT, "SIGINT"},
        {SIGABRT, "SIGABRT"},
        {SIGFPE, "SIGFPE"},
        {SIGILL, "SIGILL"},
        {SIGSEGV, "SIGSEGV"}
    };

    for (const auto& sig : signals) {
        EventAnySignal event(sig.signal);
        AnySignalListener listener(sig.name);

        event.trigger(listener);

        EXPECT_EQ(listener.signal_received, sig.signal);
        EXPECT_EQ(listener.handle_count, 1);
    }
}

TEST(SignalEventEdgeCaseTest, MixedSignalEventTypes) {
    EventManager manager;
    AnySignalListener any_listener;
    TerminateSignalListener term_listener;
    InterruptSignalListener int_listener;

    manager.registerListener<EventAnySignal>(&any_listener);
    manager.registerListener<EventSignalTerminate>(&term_listener);
    manager.registerListener<EventSignalInterrupt>(&int_listener);

    // Emit in mixed order
    manager.emitEvent<EventSignalInterrupt>();
    manager.emitEvent<EventAnySignal>(SIGABRT);
    manager.emitEvent<EventSignalTerminate>();
    manager.emitEvent<EventAnySignal>(SIGFPE);

    manager.dispatchEvents();

    EXPECT_EQ(any_listener.handle_count, 2);
    EXPECT_EQ(term_listener.handle_count, 1);
    EXPECT_EQ(int_listener.handle_count, 1);
}

TEST(SignalEventEdgeCaseTest, UnregisterAndReregister) {
    EventManager manager;
    AnySignalListener listener;

    manager.registerListener<EventAnySignal>(&listener);
    manager.unregisterListener<EventAnySignal>(&listener);
    manager.registerListener<EventAnySignal>(&listener);

    manager.emitEvent<EventAnySignal>(SIGTERM);
    manager.dispatchEvents();

    EXPECT_EQ(listener.handle_count, 1);
}

}  // namespace nexo::event
