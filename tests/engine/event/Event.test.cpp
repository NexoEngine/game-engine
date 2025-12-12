//// Event.test.cpp ///////////////////////////////////////////////////////////
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
//  Date:        12/12/2025
//  Description: Test file for the Event base class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/event/Event.hpp"
#include "core/event/Listener.hpp"

namespace nexo::event {

// =============================================================================
// Test Event Types
// =============================================================================

class SimpleEvent : public Event<SimpleEvent> {
public:
    SimpleEvent() = default;
    explicit SimpleEvent(int value) : data(value) {}
    int data = 0;
};

class StringEvent : public Event<StringEvent> {
public:
    explicit StringEvent(std::string msg) : message(std::move(msg)) {}
    std::string message;
};

class ComplexEvent : public Event<ComplexEvent> {
public:
    ComplexEvent(int id, double val, std::string txt)
        : identifier(id), value(val), text(std::move(txt)) {}
    int identifier;
    double value;
    std::string text;
};

class EmptyEvent : public Event<EmptyEvent> {
public:
    EmptyEvent() = default;
};

// =============================================================================
// Test Listeners
// =============================================================================

class SimpleListener : public Listens<SimpleEvent> {
public:
    explicit SimpleListener(const std::string& name = "SimpleListener") : Listens(name) {}

    void handleEvent(SimpleEvent& event) override {
        last_received_data = event.data;
        handle_count++;
    }

    int last_received_data = 0;
    int handle_count = 0;
};

class StringListener : public Listens<StringEvent> {
public:
    explicit StringListener(const std::string& name = "StringListener") : Listens(name) {}

    void handleEvent(StringEvent& event) override {
        last_message = event.message;
        handle_count++;
    }

    std::string last_message;
    int handle_count = 0;
};

class ComplexListener : public Listens<ComplexEvent> {
public:
    explicit ComplexListener(const std::string& name = "ComplexListener") : Listens(name) {}

    void handleEvent(ComplexEvent& event) override {
        last_id = event.identifier;
        last_value = event.value;
        last_text = event.text;
        handle_count++;
    }

    int last_id = 0;
    double last_value = 0.0;
    std::string last_text;
    int handle_count = 0;
};

class MultiEventListener : public Listens<SimpleEvent, StringEvent> {
public:
    explicit MultiEventListener(const std::string& name = "MultiEventListener") : Listens(name) {}

    void handleEvent(SimpleEvent& event) override {
        simple_count++;
        last_simple_data = event.data;
    }

    void handleEvent(StringEvent& event) override {
        string_count++;
        last_string_message = event.message;
    }

    int simple_count = 0;
    int string_count = 0;
    int last_simple_data = 0;
    std::string last_string_message;
};

class ConsumingListener : public Listens<SimpleEvent> {
public:
    explicit ConsumingListener(bool should_consume, const std::string& name = "ConsumingListener")
        : Listens(name), consume_event(should_consume) {}

    void handleEvent(SimpleEvent& event) override {
        handle_count++;
        if (consume_event) {
            event.consumed = true;
        }
    }

    bool consume_event;
    int handle_count = 0;
};

// =============================================================================
// Event Creation and Type Identification Tests
// =============================================================================

TEST(EventTest, SimpleEventCreation) {
    SimpleEvent event(42);
    EXPECT_EQ(event.data, 42);
    EXPECT_FALSE(event.consumed);
}

TEST(EventTest, SimpleEventDefaultCreation) {
    SimpleEvent event;
    EXPECT_EQ(event.data, 0);
    EXPECT_FALSE(event.consumed);
}

TEST(EventTest, StringEventCreation) {
    StringEvent event("Hello, World!");
    EXPECT_EQ(event.message, "Hello, World!");
    EXPECT_FALSE(event.consumed);
}

TEST(EventTest, ComplexEventCreation) {
    ComplexEvent event(123, 45.67, "test");
    EXPECT_EQ(event.identifier, 123);
    EXPECT_DOUBLE_EQ(event.value, 45.67);
    EXPECT_EQ(event.text, "test");
    EXPECT_FALSE(event.consumed);
}

TEST(EventTest, EmptyEventCreation) {
    EmptyEvent event;
    EXPECT_FALSE(event.consumed);
}

TEST(EventTest, EventConsumedFlagManipulation) {
    SimpleEvent event(10);
    EXPECT_FALSE(event.consumed);

    event.consumed = true;
    EXPECT_TRUE(event.consumed);

    event.consumed = false;
    EXPECT_FALSE(event.consumed);
}

// =============================================================================
// Event Triggering and Handling Tests
// =============================================================================

TEST(EventTest, TriggerSimpleEvent) {
    SimpleEvent event(99);
    SimpleListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.last_received_data, 99);
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(EventTest, TriggerStringEvent) {
    StringEvent event("Test Message");
    StringListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.last_message, "Test Message");
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(EventTest, TriggerComplexEvent) {
    ComplexEvent event(5, 3.14, "complex");
    ComplexListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.last_id, 5);
    EXPECT_DOUBLE_EQ(listener.last_value, 3.14);
    EXPECT_EQ(listener.last_text, "complex");
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(EventTest, TriggerMultipleTimes) {
    SimpleEvent event(7);
    SimpleListener listener;

    event.trigger(listener);
    event.trigger(listener);
    event.trigger(listener);

    EXPECT_EQ(listener.handle_count, 3);
    EXPECT_EQ(listener.last_received_data, 7);
}

TEST(EventTest, TriggerWithDifferentListeners) {
    SimpleEvent event(25);
    SimpleListener listener1("Listener1");
    SimpleListener listener2("Listener2");

    event.trigger(listener1);
    event.trigger(listener2);

    EXPECT_EQ(listener1.handle_count, 1);
    EXPECT_EQ(listener1.last_received_data, 25);
    EXPECT_EQ(listener2.handle_count, 1);
    EXPECT_EQ(listener2.last_received_data, 25);
}

// =============================================================================
// Event Data Access Tests
// =============================================================================

TEST(EventTest, ModifyEventDataBeforeTrigger) {
    SimpleEvent event(10);
    event.data = 20;

    SimpleListener listener;
    event.trigger(listener);

    EXPECT_EQ(listener.last_received_data, 20);
}

TEST(EventTest, ModifyEventDataAfterTrigger) {
    SimpleEvent event(15);
    SimpleListener listener;

    event.trigger(listener);
    EXPECT_EQ(listener.last_received_data, 15);

    event.data = 30;
    event.trigger(listener);

    EXPECT_EQ(listener.last_received_data, 30);
    EXPECT_EQ(listener.handle_count, 2);
}

TEST(EventTest, StringEventDataAccess) {
    StringEvent event("Initial");
    StringListener listener;

    event.trigger(listener);
    EXPECT_EQ(listener.last_message, "Initial");

    event.message = "Modified";
    event.trigger(listener);

    EXPECT_EQ(listener.last_message, "Modified");
}

TEST(EventTest, ComplexEventDataAccess) {
    ComplexEvent event(1, 1.1, "one");
    ComplexListener listener;

    event.trigger(listener);
    EXPECT_EQ(listener.last_id, 1);

    event.identifier = 2;
    event.value = 2.2;
    event.text = "two";

    event.trigger(listener);

    EXPECT_EQ(listener.last_id, 2);
    EXPECT_DOUBLE_EQ(listener.last_value, 2.2);
    EXPECT_EQ(listener.last_text, "two");
}

// =============================================================================
// Multiple Handlers for Same Event Tests
// =============================================================================

TEST(EventTest, MultipleListenersForSingleEvent) {
    SimpleEvent event(50);
    SimpleListener listener1("Listener1");
    SimpleListener listener2("Listener2");
    SimpleListener listener3("Listener3");

    event.trigger(listener1);
    event.trigger(listener2);
    event.trigger(listener3);

    EXPECT_EQ(listener1.handle_count, 1);
    EXPECT_EQ(listener2.handle_count, 1);
    EXPECT_EQ(listener3.handle_count, 1);

    EXPECT_EQ(listener1.last_received_data, 50);
    EXPECT_EQ(listener2.last_received_data, 50);
    EXPECT_EQ(listener3.last_received_data, 50);
}

TEST(EventTest, MultiEventListenerHandlingSimpleEvent) {
    SimpleEvent event(77);
    MultiEventListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.simple_count, 1);
    EXPECT_EQ(listener.string_count, 0);
    EXPECT_EQ(listener.last_simple_data, 77);
}

TEST(EventTest, MultiEventListenerHandlingStringEvent) {
    StringEvent event("Multi");
    MultiEventListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.simple_count, 0);
    EXPECT_EQ(listener.string_count, 1);
    EXPECT_EQ(listener.last_string_message, "Multi");
}

TEST(EventTest, MultiEventListenerHandlingBothEvents) {
    SimpleEvent simpleEvent(100);
    StringEvent stringEvent("Both");
    MultiEventListener listener;

    simpleEvent.trigger(listener);
    stringEvent.trigger(listener);

    EXPECT_EQ(listener.simple_count, 1);
    EXPECT_EQ(listener.string_count, 1);
    EXPECT_EQ(listener.last_simple_data, 100);
    EXPECT_EQ(listener.last_string_message, "Both");
}

// =============================================================================
// Event Consumption Tests
// =============================================================================

TEST(EventTest, EventConsumptionFlag) {
    SimpleEvent event(15);
    ConsumingListener listener(true);

    EXPECT_FALSE(event.consumed);

    event.trigger(listener);

    EXPECT_TRUE(event.consumed);
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(EventTest, EventNotConsumed) {
    SimpleEvent event(20);
    ConsumingListener listener(false);

    event.trigger(listener);

    EXPECT_FALSE(event.consumed);
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(EventTest, ConsumedEventCanStillBeTrigger) {
    SimpleEvent event(30);
    ConsumingListener listener1(true, "Consumer");
    SimpleListener listener2("Regular");

    event.trigger(listener1);
    EXPECT_TRUE(event.consumed);

    // Event can still be triggered even if consumed
    event.trigger(listener2);

    EXPECT_EQ(listener2.handle_count, 1);
    EXPECT_EQ(listener2.last_received_data, 30);
}

TEST(EventTest, ResetConsumedFlag) {
    SimpleEvent event(40);
    ConsumingListener listener(true);

    event.trigger(listener);
    EXPECT_TRUE(event.consumed);

    event.consumed = false;
    EXPECT_FALSE(event.consumed);

    event.trigger(listener);
    EXPECT_TRUE(event.consumed);
}

// =============================================================================
// Edge Cases Tests
// =============================================================================

TEST(EventTest, EmptyEventTriggering) {
    EmptyEvent event;

    class EmptyListener : public Listens<EmptyEvent> {
    public:
        explicit EmptyListener(const std::string& name = "EmptyListener") : Listens(name) {}
        void handleEvent(EmptyEvent&) override {
            triggered = true;
        }
        bool triggered = false;
    };

    EmptyListener listener;
    event.trigger(listener);

    EXPECT_TRUE(listener.triggered);
}

TEST(EventTest, EventWithZeroValues) {
    SimpleEvent event(0);
    ComplexEvent complexEvent(0, 0.0, "");

    SimpleListener simpleListener;
    ComplexListener complexListener;

    event.trigger(simpleListener);
    complexEvent.trigger(complexListener);

    EXPECT_EQ(simpleListener.last_received_data, 0);
    EXPECT_EQ(complexListener.last_id, 0);
    EXPECT_DOUBLE_EQ(complexListener.last_value, 0.0);
    EXPECT_EQ(complexListener.last_text, "");
}

TEST(EventTest, EventWithNegativeValues) {
    SimpleEvent event(-42);
    ComplexEvent complexEvent(-10, -3.14, "negative");

    SimpleListener simpleListener;
    ComplexListener complexListener;

    event.trigger(simpleListener);
    complexEvent.trigger(complexListener);

    EXPECT_EQ(simpleListener.last_received_data, -42);
    EXPECT_EQ(complexListener.last_id, -10);
    EXPECT_DOUBLE_EQ(complexListener.last_value, -3.14);
}

TEST(EventTest, EventWithLargeValues) {
    SimpleEvent event(2147483647); // INT_MAX
    ComplexEvent complexEvent(1000000, 1e308, "large");

    SimpleListener simpleListener;
    ComplexListener complexListener;

    event.trigger(simpleListener);
    complexEvent.trigger(complexListener);

    EXPECT_EQ(simpleListener.last_received_data, 2147483647);
    EXPECT_EQ(complexListener.last_id, 1000000);
    EXPECT_DOUBLE_EQ(complexListener.last_value, 1e308);
}

TEST(EventTest, StringEventWithSpecialCharacters) {
    StringEvent event("!@#$%^&*()_+-=[]{}|;':\",./<>?");
    StringListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.last_message, "!@#$%^&*()_+-=[]{}|;':\",./<>?");
}

TEST(EventTest, StringEventWithNewlines) {
    StringEvent event("Line1\nLine2\nLine3");
    StringListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.last_message, "Line1\nLine2\nLine3");
}

TEST(EventTest, StringEventWithUnicode) {
    StringEvent event("Hello 世界 🎮");
    StringListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.last_message, "Hello 世界 🎮");
}

TEST(EventTest, VeryLongString) {
    std::string longString(10000, 'x');
    StringEvent event(longString);
    StringListener listener;

    event.trigger(listener);

    EXPECT_EQ(listener.last_message.size(), 10000);
    EXPECT_EQ(listener.last_message, longString);
}

TEST(EventTest, TriggerAfterConsumedAndReset) {
    SimpleEvent event(100);
    ConsumingListener consumingListener(true, "Consumer");
    SimpleListener regularListener("Regular");

    // First trigger consumes event
    event.trigger(consumingListener);
    EXPECT_TRUE(event.consumed);
    EXPECT_EQ(consumingListener.handle_count, 1);

    // Reset consumed flag
    event.consumed = false;

    // Second trigger should work normally
    event.trigger(regularListener);
    EXPECT_EQ(regularListener.handle_count, 1);
    EXPECT_EQ(regularListener.last_received_data, 100);
}

// =============================================================================
// Type Identification Tests
// =============================================================================

TEST(EventTest, EventTypeIdentification) {
    SimpleEvent simpleEvent;
    StringEvent stringEvent("test");
    ComplexEvent complexEvent(1, 1.0, "test");
    EmptyEvent emptyEvent;

    // Events should have different type_info
    EXPECT_NE(typeid(simpleEvent), typeid(stringEvent));
    EXPECT_NE(typeid(simpleEvent), typeid(complexEvent));
    EXPECT_NE(typeid(stringEvent), typeid(complexEvent));
    EXPECT_NE(typeid(emptyEvent), typeid(simpleEvent));
}

TEST(EventTest, EventDerivedFromIEvent) {
    SimpleEvent simpleEvent;
    StringEvent stringEvent("test");

    IEvent* ievent1 = &simpleEvent;
    IEvent* ievent2 = &stringEvent;

    EXPECT_NE(ievent1, nullptr);
    EXPECT_NE(ievent2, nullptr);

    // Both events should be IEvent instances
    EXPECT_EQ(typeid(*ievent1), typeid(SimpleEvent));
    EXPECT_EQ(typeid(*ievent2), typeid(StringEvent));
}

// =============================================================================
// Listener Name Tests
// =============================================================================

TEST(EventTest, ListenerWithName) {
    SimpleListener listener("TestListener");
    EXPECT_EQ(listener.getListenerName(), "TestListener");
}

TEST(EventTest, ListenerWithEmptyName) {
    SimpleListener listener("");
    EXPECT_EQ(listener.getListenerName(), "");
}

TEST(EventTest, ListenerWithDefaultName) {
    SimpleListener listener;
    EXPECT_EQ(listener.getListenerName(), "SimpleListener");
}

// =============================================================================
// Polymorphic Behavior Tests
// =============================================================================

TEST(EventTest, PolymorphicEventTrigger) {
    SimpleEvent event(55);
    SimpleListener listener;

    IEvent* ievent = &event;
    BaseListener* blistener = &listener;

    ievent->trigger(*blistener);

    EXPECT_EQ(listener.last_received_data, 55);
    EXPECT_EQ(listener.handle_count, 1);
}

TEST(EventTest, PolymorphicConsumedFlag) {
    SimpleEvent event(60);
    ConsumingListener listener(true);

    IEvent* ievent = &event;
    BaseListener* blistener = &listener;

    EXPECT_FALSE(ievent->consumed);
    ievent->trigger(*blistener);
    EXPECT_TRUE(ievent->consumed);
}

}  // namespace nexo::event
