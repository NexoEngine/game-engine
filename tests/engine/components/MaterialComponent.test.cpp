///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date: Today lol
//  Description: Test file for the MaterialComponent (simplified for testing)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "components/MaterialComponent.hpp"
#include "utils/TestHelpers.hpp"
#include <memory>

using namespace nexo::components;
using namespace nexo::test;

// NOTE: MaterialComponent tests are simplified due to complex Material asset dependencies
// Full Material class requires texture assets and complex initialization
// These tests focus on MaterialComponent structure and AssetRef usage patterns

class MaterialComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        materialComp = MaterialComponent{};
    }
    
    MaterialComponent materialComp;
};

TEST_F(MaterialComponentTest, DefaultConstruction) {
    MaterialComponent comp;
    EXPECT_FALSE(comp.material.isValid());
    EXPECT_EQ(comp.material.lock(), nullptr);
}

TEST_F(MaterialComponentTest, CopyConstruction) {
    MaterialComponent original;
    MaterialComponent copy(original);
    
    EXPECT_FALSE(copy.material.isValid());
    EXPECT_FALSE(original.material.isValid());
}

TEST_F(MaterialComponentTest, MoveConstruction) {
    MaterialComponent original;
    MaterialComponent moved(std::move(original));
    
    EXPECT_FALSE(moved.material.isValid());
}

TEST_F(MaterialComponentTest, CopyAssignment) {
    MaterialComponent first;
    MaterialComponent second;
    
    second = first;
    
    EXPECT_FALSE(second.material.isValid());
    EXPECT_FALSE(first.material.isValid());
}

TEST_F(MaterialComponentTest, MoveAssignment) {
    MaterialComponent first;
    MaterialComponent second;
    
    second = std::move(first);
    
    EXPECT_FALSE(second.material.isValid());
}

TEST_F(MaterialComponentTest, AggregateInitialization) {
    // Default initialization
    MaterialComponent comp1{};
    EXPECT_FALSE(comp1.material.isValid());
}

TEST_F(MaterialComponentTest, MaterialComparison) {
    MaterialComponent comp1;
    MaterialComponent comp2;
    
    // Both null
    EXPECT_EQ(comp1.material.lock(), comp2.material.lock());
}

// Test with ECS integration  
class MaterialComponentIntegrationTest : public EcsTestFixture {
protected:
    void SetUp() override {
        EcsTestFixture::SetUp();
    }
    void TearDown() override {
        EcsTestFixture::TearDown();
    }
};

TEST_F(MaterialComponentIntegrationTest, EcsUsage) {
    auto* coordinator = this->coordinator.get();
    
    // Create entity with material component
    auto entity = coordinator->createEntity();
    
    MaterialComponent matComp;
    coordinator->addComponent(entity, matComp);
    
    // Retrieve and verify
    auto& retrievedComp = coordinator->getComponent<MaterialComponent>(entity);
    EXPECT_FALSE(retrievedComp.material.isValid());
}

// Tests moved out of nexo::components namespace to fix gtest inheritance issues