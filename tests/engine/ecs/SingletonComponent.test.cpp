//// SingletonComponent.test.cpp ///////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for SingletonComponent and SingletonComponentManager
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ecs/SingletonComponent.hpp"
#include "ecs/ECSExceptions.hpp"

namespace nexo::ecs {

// =============================================================================
// Test Singleton Types
// =============================================================================

// Non-copyable singleton for testing
class TestSingleton {
public:
    TestSingleton() = default;
    explicit TestSingleton(int val) : value(val) {}
    TestSingleton(int val, float f) : value(val), floatVal(f) {}

    // Delete copy to satisfy static_assert
    TestSingleton(const TestSingleton&) = delete;
    TestSingleton& operator=(const TestSingleton&) = delete;

    // Allow move
    TestSingleton(TestSingleton&&) = default;
    TestSingleton& operator=(TestSingleton&&) = default;

    int value = 0;
    float floatVal = 0.0f;
};

class AnotherSingleton {
public:
    AnotherSingleton() = default;
    explicit AnotherSingleton(const std::string& s) : name(s) {}

    AnotherSingleton(const AnotherSingleton&) = delete;
    AnotherSingleton& operator=(const AnotherSingleton&) = delete;
    AnotherSingleton(AnotherSingleton&&) = default;
    AnotherSingleton& operator=(AnotherSingleton&&) = default;

    std::string name = "default";
};

// =============================================================================
// SingletonComponent Tests
// =============================================================================

class SingletonComponentTest : public ::testing::Test {};

TEST_F(SingletonComponentTest, DefaultConstruction) {
    SingletonComponent<TestSingleton> comp;
    EXPECT_EQ(comp.getInstance().value, 0);
}

TEST_F(SingletonComponentTest, ConstructionWithArgs) {
    SingletonComponent<TestSingleton> comp(42);
    EXPECT_EQ(comp.getInstance().value, 42);
}

TEST_F(SingletonComponentTest, ConstructionWithMultipleArgs) {
    SingletonComponent<TestSingleton> comp(42, 3.14f);
    EXPECT_EQ(comp.getInstance().value, 42);
    EXPECT_FLOAT_EQ(comp.getInstance().floatVal, 3.14f);
}

TEST_F(SingletonComponentTest, GetInstanceReturnsReference) {
    SingletonComponent<TestSingleton> comp(10);
    comp.getInstance().value = 20;
    EXPECT_EQ(comp.getInstance().value, 20);
}

TEST_F(SingletonComponentTest, MoveConstruction) {
    SingletonComponent<TestSingleton> comp1(42);
    SingletonComponent<TestSingleton> comp2(std::move(comp1));
    EXPECT_EQ(comp2.getInstance().value, 42);
}

TEST_F(SingletonComponentTest, MoveAssignment) {
    SingletonComponent<TestSingleton> comp1(42);
    SingletonComponent<TestSingleton> comp2;
    comp2 = std::move(comp1);
    EXPECT_EQ(comp2.getInstance().value, 42);
}

// =============================================================================
// SingletonComponentManager Basic Tests
// =============================================================================

class SingletonComponentManagerTest : public ::testing::Test {
protected:
    SingletonComponentManager manager;
};

TEST_F(SingletonComponentManagerTest, RegisterAndGetSingleton) {
    manager.registerSingletonComponent<TestSingleton>(42);
    auto& singleton = manager.getSingletonComponent<TestSingleton>();
    EXPECT_EQ(singleton.value, 42);
}

TEST_F(SingletonComponentManagerTest, RegisterWithDefaultConstruction) {
    manager.registerSingletonComponent<TestSingleton>();
    auto& singleton = manager.getSingletonComponent<TestSingleton>();
    EXPECT_EQ(singleton.value, 0);
}

TEST_F(SingletonComponentManagerTest, RegisterWithMultipleArgs) {
    manager.registerSingletonComponent<TestSingleton>(100, 2.5f);
    auto& singleton = manager.getSingletonComponent<TestSingleton>();
    EXPECT_EQ(singleton.value, 100);
    EXPECT_FLOAT_EQ(singleton.floatVal, 2.5f);
}

TEST_F(SingletonComponentManagerTest, GetSingletonReturnsModifiableReference) {
    manager.registerSingletonComponent<TestSingleton>(0);
    manager.getSingletonComponent<TestSingleton>().value = 999;
    EXPECT_EQ(manager.getSingletonComponent<TestSingleton>().value, 999);
}

TEST_F(SingletonComponentManagerTest, MultipleSingletonTypes) {
    manager.registerSingletonComponent<TestSingleton>(42);
    manager.registerSingletonComponent<AnotherSingleton>("hello");

    EXPECT_EQ(manager.getSingletonComponent<TestSingleton>().value, 42);
    EXPECT_EQ(manager.getSingletonComponent<AnotherSingleton>().name, "hello");
}

TEST_F(SingletonComponentManagerTest, SingletonsAreIndependent) {
    manager.registerSingletonComponent<TestSingleton>(1);
    manager.registerSingletonComponent<AnotherSingleton>("test");

    manager.getSingletonComponent<TestSingleton>().value = 100;

    EXPECT_EQ(manager.getSingletonComponent<TestSingleton>().value, 100);
    EXPECT_EQ(manager.getSingletonComponent<AnotherSingleton>().name, "test");
}

// =============================================================================
// SingletonComponentManager Unregister Tests
// =============================================================================

class SingletonComponentManagerUnregisterTest : public ::testing::Test {
protected:
    SingletonComponentManager manager;
};

TEST_F(SingletonComponentManagerUnregisterTest, UnregisterRemovesSingleton) {
    manager.registerSingletonComponent<TestSingleton>(42);
    manager.unregisterSingletonComponent<TestSingleton>();

    EXPECT_THROW(manager.getSingletonComponent<TestSingleton>(), SingletonComponentNotRegistered);
}

TEST_F(SingletonComponentManagerUnregisterTest, UnregisterOnlyAffectsTarget) {
    manager.registerSingletonComponent<TestSingleton>(42);
    manager.registerSingletonComponent<AnotherSingleton>("keep");

    manager.unregisterSingletonComponent<TestSingleton>();

    EXPECT_THROW(manager.getSingletonComponent<TestSingleton>(), SingletonComponentNotRegistered);
    EXPECT_EQ(manager.getSingletonComponent<AnotherSingleton>().name, "keep");
}

TEST_F(SingletonComponentManagerUnregisterTest, UnregisterNonExistentThrows) {
    EXPECT_THROW(manager.unregisterSingletonComponent<TestSingleton>(), SingletonComponentNotRegistered);
}

// =============================================================================
// SingletonComponentManager Exception Tests
// =============================================================================

class SingletonComponentManagerExceptionTest : public ::testing::Test {
protected:
    SingletonComponentManager manager;
};

TEST_F(SingletonComponentManagerExceptionTest, GetNonRegisteredThrows) {
    EXPECT_THROW(manager.getSingletonComponent<TestSingleton>(), SingletonComponentNotRegistered);
}

TEST_F(SingletonComponentManagerExceptionTest, GetRawNonRegisteredThrows) {
    EXPECT_THROW(manager.getRawSingletonComponent<TestSingleton>(), SingletonComponentNotRegistered);
}

TEST_F(SingletonComponentManagerExceptionTest, RegisterTwiceIsNoOp) {
    manager.registerSingletonComponent<TestSingleton>(42);
    manager.registerSingletonComponent<TestSingleton>(100); // Should be ignored with warning

    // Original value should be preserved
    EXPECT_EQ(manager.getSingletonComponent<TestSingleton>().value, 42);
}

// =============================================================================
// SingletonComponentManager Raw Access Tests
// =============================================================================

class SingletonComponentManagerRawTest : public ::testing::Test {
protected:
    SingletonComponentManager manager;
};

TEST_F(SingletonComponentManagerRawTest, GetRawReturnsValidPointer) {
    manager.registerSingletonComponent<TestSingleton>(42);
    auto raw = manager.getRawSingletonComponent<TestSingleton>();

    EXPECT_NE(raw, nullptr);
}

TEST_F(SingletonComponentManagerRawTest, RawPointerCanBeCast) {
    manager.registerSingletonComponent<TestSingleton>(42);
    auto raw = manager.getRawSingletonComponent<TestSingleton>();

    auto* typed = dynamic_cast<SingletonComponent<TestSingleton>*>(raw.get());
    ASSERT_NE(typed, nullptr);
    EXPECT_EQ(typed->getInstance().value, 42);
}

// =============================================================================
// ISingletonComponent Interface Tests
// =============================================================================

class ISingletonComponentTest : public ::testing::Test {};

TEST_F(ISingletonComponentTest, PolymorphicDestruction) {
    std::unique_ptr<ISingletonComponent> base =
        std::make_unique<SingletonComponent<TestSingleton>>(42);

    // Should not leak or crash
    base.reset();
    SUCCEED();
}

TEST_F(ISingletonComponentTest, CanStoreInContainer) {
    std::vector<std::shared_ptr<ISingletonComponent>> components;

    components.push_back(std::make_shared<SingletonComponent<TestSingleton>>(1));
    components.push_back(std::make_shared<SingletonComponent<AnotherSingleton>>("test"));

    EXPECT_EQ(components.size(), 2u);
}

}  // namespace nexo::ecs
