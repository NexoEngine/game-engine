//// Components.test.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/11/2024
//  Description: Test file for the components ecs classes
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ecs/Components.hpp"

namespace nexo::ecs {
    struct TestComponent {
        int value;
    };

    struct AnotherComponent {
        float data;
    };

    class ComponentManagerTest : public ::testing::Test {
        protected:
            void SetUp() override
            {
                componentManager = std::make_unique<ComponentManager>();
            }

            std::unique_ptr<ComponentManager> componentManager;
    };

    TEST(ComponentArrayTest, InsertAndRetrieveData)
    {
        ComponentArray<TestComponent> array;

        Entity entity1 = 1;
        Entity entity2 = 2;

        array.insertData(entity1, {42});
        array.insertData(entity2, {84});

        EXPECT_EQ(array.getData(entity1).value, 42);
        EXPECT_EQ(array.getData(entity2).value, 84);
    }

    TEST(ComponentArrayTest, RemoveData)
    {
        ComponentArray<TestComponent> array;

        Entity entity = 1;

        array.insertData(entity, {42});
        EXPECT_NO_THROW(array.removeData(entity));

        EXPECT_THROW(static_cast<void>(array.getData(entity)), ComponentNotFound);
    }

    TEST(ComponentArrayTest, HandleEntityDestruction)
    {
        ComponentArray<TestComponent> array;

        Entity entity = 1;

        array.insertData(entity, {42});
        array.entityDestroyed(entity);

        EXPECT_THROW(static_cast<void>(array.getData(entity)), ComponentNotFound);
    }

    TEST(ComponentArrayTest, InsertDuplicateEntity)
    {
        ComponentArray<TestComponent> array;

        Entity entity = 1;

        array.insertData(entity, {42});
        EXPECT_NO_THROW(array.insertData(entity, {100}));

        EXPECT_EQ(array.getData(entity).value, 42); // Original value should remain
    }

    TEST_F(ComponentManagerTest, RegisterAndRetrieveComponentType)
    {
        componentManager->registerComponent<TestComponent>();
        ComponentType type = componentManager->getComponentType<TestComponent>();

        EXPECT_EQ(type, 0); // First registered component type
    }

    TEST_F(ComponentManagerTest, AddAndRetrieveComponent)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;

        componentManager->addComponent(entity, TestComponent{42});
        TestComponent &retrieved = componentManager->getComponent<TestComponent>(entity);

        EXPECT_EQ(retrieved.value, 42);
    }

    TEST_F(ComponentManagerTest, RemoveComponent)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;

        componentManager->addComponent(entity, TestComponent{42});
        EXPECT_NO_THROW(componentManager->removeComponent<TestComponent>(entity));
        EXPECT_THROW(static_cast<void>(componentManager->getComponent<TestComponent>(entity)), ComponentNotFound);
    }

    TEST_F(ComponentManagerTest, TryRemoveComponent)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;

        EXPECT_FALSE(componentManager->tryRemoveComponent<TestComponent>(entity)); // No component yet

        componentManager->addComponent(entity, TestComponent{42});
        EXPECT_TRUE(componentManager->tryRemoveComponent<TestComponent>(entity));
        EXPECT_FALSE(componentManager->tryRemoveComponent<TestComponent>(entity));
    }

    TEST_F(ComponentManagerTest, EntityDestroyedCleansUpComponents)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity1 = 1;
        Entity entity2 = 2;

        componentManager->addComponent(entity1, TestComponent{42});
        componentManager->addComponent(entity2, TestComponent{84});

        componentManager->entityDestroyed(entity1);

        EXPECT_THROW(static_cast<void>(componentManager->getComponent<TestComponent>(entity1)), ComponentNotFound);
        EXPECT_EQ(componentManager->getComponent<TestComponent>(entity2).value, 84);
    }

    TEST_F(ComponentManagerTest, RetrieveUnregisteredComponentType)
    {
        EXPECT_THROW(static_cast<void>(componentManager->getComponentType<TestComponent>()), ComponentNotRegistered);
    }

    TEST_F(ComponentManagerTest, AddComponentWithoutRegistering)
    {
        Entity entity = 1;
        EXPECT_THROW(componentManager->addComponent(entity, TestComponent{42}), ComponentNotRegistered);
    }

    TEST_F(ComponentManagerTest, TryGetComponent)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;

        auto result = componentManager->tryGetComponent<TestComponent>(entity);
        EXPECT_FALSE(result.has_value());

        componentManager->addComponent(entity, TestComponent{42});
        result = componentManager->tryGetComponent<TestComponent>(entity);
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result->get().value, 42);
    }

    TEST(ComponentArrayTest, HandleOverflow)
    {
        ComponentArray<TestComponent> array;

        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
        {
            EXPECT_NO_THROW(array.insertData(entity, {static_cast<int>(entity)}));
        }

        EXPECT_THROW(array.insertData(MAX_ENTITIES, {999}), OutOfRange);
    }

    TEST_F(ComponentManagerTest, RegisterDuplicateComponent)
    {
        componentManager->registerComponent<TestComponent>();
        EXPECT_NO_THROW(componentManager->registerComponent<TestComponent>());
    }
}
