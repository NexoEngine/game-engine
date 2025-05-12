//// SingletonComponent.test.cpp //////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/03/2025
//  Description: Test file for the singleton component classes
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ecs/SingletonComponent.hpp"

namespace nexo::ecs {

	// Test components
	struct TestComponent {
	    int value;
	    std::string name;

	    TestComponent(int v) : value(v), name("default") {}
	    TestComponent(int v, std::string n) : value(v), name(n) {}

		TestComponent(const TestComponent&) = delete;
		TestComponent& operator=(const TestComponent&) = delete;

	};

	struct ComplexComponent {
	    std::vector<int> data;
	    bool flag;

	    ComplexComponent() : flag(false) {}
	    ComplexComponent(const std::vector<int>& d, bool f) : data(d), flag(f) {}

	    ComplexComponent(const ComplexComponent&) = delete;
		ComplexComponent& operator=(const ComplexComponent&) = delete;
	};

	class SingletonComponentTest : public ::testing::Test {};

	class SingletonComponentManagerTest : public ::testing::Test {
	protected:
	    void SetUp() override {
	        manager = std::make_unique<SingletonComponentManager>();
	    }

	    std::unique_ptr<SingletonComponentManager> manager;
	};

	TEST_F(SingletonComponentTest, ConstructWithSingleArgument) {
	    SingletonComponent<TestComponent> component(42);
	    EXPECT_EQ(component.getInstance().value, 42);
	    EXPECT_EQ(component.getInstance().name, "default");
	}

	TEST_F(SingletonComponentTest, ConstructWithMultipleArguments) {
	    SingletonComponent<TestComponent> component(42, "test");
	    EXPECT_EQ(component.getInstance().value, 42);
	    EXPECT_EQ(component.getInstance().name, "test");
	}

	TEST_F(SingletonComponentTest, GetInstanceReturnsReference) {
	    SingletonComponent<TestComponent> component(42);
	    TestComponent& instance = component.getInstance();

	    // Modify through reference
	    instance.value = 100;
	    instance.name = "modified";

	    // Verify changes
	    EXPECT_EQ(component.getInstance().value, 100);
	    EXPECT_EQ(component.getInstance().name, "modified");
	}

	TEST_F(SingletonComponentTest, ComplexComponentConstruction) {
	    std::vector<int> testData = {1, 2, 3, 4, 5};
	    SingletonComponent<ComplexComponent> component(testData, true);

	    EXPECT_EQ(component.getInstance().data, testData);
	    EXPECT_TRUE(component.getInstance().flag);
	}

	TEST_F(SingletonComponentTest, DefaultConstructible) {
	    SingletonComponent<ComplexComponent> component;
	    EXPECT_TRUE(component.getInstance().data.empty());
	    EXPECT_FALSE(component.getInstance().flag);
	}

	TEST_F(SingletonComponentManagerTest, RegisterAndGetSingletonComponent) {
	    manager->registerSingletonComponent<TestComponent>(42);

	    TestComponent& component = manager->getSingletonComponent<TestComponent>();
	    EXPECT_EQ(component.value, 42);
	    EXPECT_EQ(component.name, "default");
	}

	TEST_F(SingletonComponentManagerTest, RegisterWithMultipleArguments) {
	    manager->registerSingletonComponent<TestComponent>(42, "test");

	    TestComponent& component = manager->getSingletonComponent<TestComponent>();
	    EXPECT_EQ(component.value, 42);
	    EXPECT_EQ(component.name, "test");
	}

	TEST_F(SingletonComponentManagerTest, GetNonexistentComponent) {
	    EXPECT_THROW(manager->getSingletonComponent<TestComponent>(), SingletonComponentNotRegistered);
	}

	TEST_F(SingletonComponentManagerTest, UnregisterComponent) {
	    // Register
	    manager->registerSingletonComponent<TestComponent>(42);
	    EXPECT_NO_THROW(manager->getSingletonComponent<TestComponent>());

	    // Unregister
	    manager->unregisterSingletonComponent<TestComponent>();

	    // Should now throw
	    EXPECT_THROW(manager->getSingletonComponent<TestComponent>(), SingletonComponentNotRegistered);
	}

	TEST_F(SingletonComponentManagerTest, UnregisterNonexistentComponent) {
	    EXPECT_THROW(manager->unregisterSingletonComponent<TestComponent>(), SingletonComponentNotRegistered);
	}

	TEST_F(SingletonComponentManagerTest, RegisterSameComponentTwice) {
	    // First registration
	    manager->registerSingletonComponent<TestComponent>(42);

	    // Second registration should log warning but not throw
	    EXPECT_NO_THROW(manager->registerSingletonComponent<TestComponent>(100));

	    // Should still have the original value
	    TestComponent& component = manager->getSingletonComponent<TestComponent>();
	    EXPECT_EQ(component.value, 42);
	}

	TEST_F(SingletonComponentManagerTest, RegisterMultipleComponentTypes) {
	    manager->registerSingletonComponent<TestComponent>(42);
	    manager->registerSingletonComponent<ComplexComponent>(std::vector<int>{1, 2, 3}, true);

	    // Both should be retrievable
	    TestComponent& comp1 = manager->getSingletonComponent<TestComponent>();
	    ComplexComponent& comp2 = manager->getSingletonComponent<ComplexComponent>();

	    EXPECT_EQ(comp1.value, 42);
	    EXPECT_EQ(comp2.data, std::vector<int>({1, 2, 3}));
	    EXPECT_TRUE(comp2.flag);
	}

	TEST_F(SingletonComponentManagerTest, ModifyComponent) {
	    manager->registerSingletonComponent<TestComponent>(42);

	    // Get and modify
	    TestComponent& component = manager->getSingletonComponent<TestComponent>();
	    component.value = 100;
	    component.name = "modified";

	    // Should reflect changes when retrieved again
	    TestComponent& retrieved = manager->getSingletonComponent<TestComponent>();
	    EXPECT_EQ(retrieved.value, 100);
	    EXPECT_EQ(retrieved.name, "modified");
	}

	TEST_F(SingletonComponentManagerTest, RegisterAfterUnregister) {
	    // Register
	    manager->registerSingletonComponent<TestComponent>(42);

	    // Unregister
	    manager->unregisterSingletonComponent<TestComponent>();

	    // Register again with different value
	    manager->registerSingletonComponent<TestComponent>(100);

	    // Should have new value
	    TestComponent& component = manager->getSingletonComponent<TestComponent>();
	    EXPECT_EQ(component.value, 100);
	}

	TEST_F(SingletonComponentManagerTest, ComplexComponentCycle) {
	    // Register complex component
	    std::vector<int> originalData = {1, 2, 3};
	    manager->registerSingletonComponent<ComplexComponent>(originalData, true);

	    // Get and modify
	    ComplexComponent& comp = manager->getSingletonComponent<ComplexComponent>();
	    comp.data.push_back(4);
	    comp.flag = false;

	    // Verify modifications
	    ComplexComponent& modified = manager->getSingletonComponent<ComplexComponent>();
	    std::vector<int> expectedData = {1, 2, 3, 4};
	    EXPECT_EQ(modified.data, expectedData);
	    EXPECT_FALSE(modified.flag);

	    // Unregister
	    manager->unregisterSingletonComponent<ComplexComponent>();

	    // Register new instance
	    std::vector<int> newData = {5, 6, 7};
	    manager->registerSingletonComponent<ComplexComponent>(newData, true);

	    // Verify new instance
	    ComplexComponent& newComp = manager->getSingletonComponent<ComplexComponent>();
	    EXPECT_EQ(newComp.data, newData);
	    EXPECT_TRUE(newComp.flag);
	}

	TEST_F(SingletonComponentManagerTest, MultipleUnregistrations) {
	    manager->registerSingletonComponent<TestComponent>(42);
	    manager->unregisterSingletonComponent<TestComponent>();

	    // Second unregistration should throw
	    EXPECT_THROW(manager->unregisterSingletonComponent<TestComponent>(), SingletonComponentNotRegistered);
	}

}
