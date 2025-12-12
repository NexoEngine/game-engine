//// Material.test.cpp ////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for Material component
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "components/Render3D.hpp"

namespace nexo::components {

class MaterialTest : public ::testing::Test {};

// =============================================================================
// Default Value Tests
// =============================================================================

TEST_F(MaterialTest, DefaultAlbedoColor) {
    Material mat;
    EXPECT_FLOAT_EQ(mat.albedoColor.r, 1.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.g, 1.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.b, 1.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.a, 1.0f);
}

TEST_F(MaterialTest, DefaultSpecularColor) {
    Material mat;
    EXPECT_FLOAT_EQ(mat.specularColor.r, 1.0f);
    EXPECT_FLOAT_EQ(mat.specularColor.g, 1.0f);
    EXPECT_FLOAT_EQ(mat.specularColor.b, 1.0f);
    EXPECT_FLOAT_EQ(mat.specularColor.a, 1.0f);
}

TEST_F(MaterialTest, DefaultEmissiveColor) {
    Material mat;
    EXPECT_FLOAT_EQ(mat.emissiveColor.r, 0.0f);
    EXPECT_FLOAT_EQ(mat.emissiveColor.g, 0.0f);
    EXPECT_FLOAT_EQ(mat.emissiveColor.b, 0.0f);
}

TEST_F(MaterialTest, DefaultIsOpaque) {
    Material mat;
    EXPECT_TRUE(mat.isOpaque);
}

TEST_F(MaterialTest, DefaultRoughness) {
    Material mat;
    EXPECT_FLOAT_EQ(mat.roughness, 0.0f);
}

TEST_F(MaterialTest, DefaultMetallic) {
    Material mat;
    EXPECT_FLOAT_EQ(mat.metallic, 0.0f);
}

TEST_F(MaterialTest, DefaultOpacity) {
    Material mat;
    EXPECT_FLOAT_EQ(mat.opacity, 1.0f);
}

TEST_F(MaterialTest, DefaultShader) {
    Material mat;
    EXPECT_EQ(mat.shader, "Phong");
}

TEST_F(MaterialTest, DefaultTexturesAreNull) {
    Material mat;
    EXPECT_EQ(mat.albedoTexture, nullptr);
    EXPECT_EQ(mat.normalMap, nullptr);
    EXPECT_EQ(mat.metallicMap, nullptr);
    EXPECT_EQ(mat.roughnessMap, nullptr);
    EXPECT_EQ(mat.emissiveMap, nullptr);
}

// =============================================================================
// Property Modification Tests
// =============================================================================

TEST_F(MaterialTest, ModifyAlbedoColor) {
    Material mat;
    mat.albedoColor = glm::vec4(0.5f, 0.3f, 0.2f, 1.0f);

    EXPECT_FLOAT_EQ(mat.albedoColor.r, 0.5f);
    EXPECT_FLOAT_EQ(mat.albedoColor.g, 0.3f);
    EXPECT_FLOAT_EQ(mat.albedoColor.b, 0.2f);
    EXPECT_FLOAT_EQ(mat.albedoColor.a, 1.0f);
}

TEST_F(MaterialTest, ModifySpecularColor) {
    Material mat;
    mat.specularColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);

    EXPECT_FLOAT_EQ(mat.specularColor.r, 0.8f);
}

TEST_F(MaterialTest, ModifyEmissiveColor) {
    Material mat;
    mat.emissiveColor = glm::vec3(1.0f, 0.5f, 0.0f);  // Orange glow

    EXPECT_FLOAT_EQ(mat.emissiveColor.r, 1.0f);
    EXPECT_FLOAT_EQ(mat.emissiveColor.g, 0.5f);
    EXPECT_FLOAT_EQ(mat.emissiveColor.b, 0.0f);
}

TEST_F(MaterialTest, ModifyRoughness) {
    Material mat;
    mat.roughness = 0.75f;
    EXPECT_FLOAT_EQ(mat.roughness, 0.75f);
}

TEST_F(MaterialTest, ModifyMetallic) {
    Material mat;
    mat.metallic = 1.0f;
    EXPECT_FLOAT_EQ(mat.metallic, 1.0f);
}

TEST_F(MaterialTest, ModifyOpacity) {
    Material mat;
    mat.opacity = 0.5f;
    EXPECT_FLOAT_EQ(mat.opacity, 0.5f);
}

TEST_F(MaterialTest, ModifyIsOpaque) {
    Material mat;
    mat.isOpaque = false;
    EXPECT_FALSE(mat.isOpaque);
}

TEST_F(MaterialTest, ModifyShader) {
    Material mat;
    mat.shader = "PBR";
    EXPECT_EQ(mat.shader, "PBR");
}

// =============================================================================
// Copy Tests
// =============================================================================

TEST_F(MaterialTest, CopyConstruction) {
    Material original;
    original.albedoColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
    original.roughness = 0.5f;
    original.metallic = 0.3f;
    original.shader = "Custom";

    Material copy = original;

    EXPECT_FLOAT_EQ(copy.albedoColor.r, 0.5f);
    EXPECT_FLOAT_EQ(copy.roughness, 0.5f);
    EXPECT_FLOAT_EQ(copy.metallic, 0.3f);
    EXPECT_EQ(copy.shader, "Custom");
}

TEST_F(MaterialTest, CopyAssignment) {
    Material original;
    original.emissiveColor = glm::vec3(1.0f, 0.0f, 0.0f);
    original.opacity = 0.75f;

    Material copy;
    copy = original;

    EXPECT_FLOAT_EQ(copy.emissiveColor.r, 1.0f);
    EXPECT_FLOAT_EQ(copy.opacity, 0.75f);
}

// =============================================================================
// Material Preset Tests
// =============================================================================

TEST_F(MaterialTest, MetallicMaterialSetup) {
    Material metal;
    metal.metallic = 1.0f;
    metal.roughness = 0.2f;
    metal.albedoColor = glm::vec4(0.8f, 0.8f, 0.9f, 1.0f);  // Silver-like

    EXPECT_FLOAT_EQ(metal.metallic, 1.0f);
    EXPECT_FLOAT_EQ(metal.roughness, 0.2f);
}

TEST_F(MaterialTest, RoughMaterialSetup) {
    Material rough;
    rough.metallic = 0.0f;
    rough.roughness = 1.0f;
    rough.albedoColor = glm::vec4(0.6f, 0.4f, 0.3f, 1.0f);  // Rough clay

    EXPECT_FLOAT_EQ(rough.metallic, 0.0f);
    EXPECT_FLOAT_EQ(rough.roughness, 1.0f);
}

TEST_F(MaterialTest, TransparentMaterialSetup) {
    Material glass;
    glass.isOpaque = false;
    glass.opacity = 0.3f;
    glass.albedoColor = glm::vec4(0.9f, 0.9f, 1.0f, 0.3f);

    EXPECT_FALSE(glass.isOpaque);
    EXPECT_FLOAT_EQ(glass.opacity, 0.3f);
}

TEST_F(MaterialTest, EmissiveMaterialSetup) {
    Material glow;
    glow.emissiveColor = glm::vec3(5.0f, 2.0f, 0.0f);  // HDR orange glow

    EXPECT_FLOAT_EQ(glow.emissiveColor.r, 5.0f);
    EXPECT_FLOAT_EQ(glow.emissiveColor.g, 2.0f);
}

// =============================================================================
// Type Traits Tests
// =============================================================================

TEST_F(MaterialTest, IsDefaultConstructible) {
    EXPECT_TRUE(std::is_default_constructible_v<Material>);
}

TEST_F(MaterialTest, IsCopyConstructible) {
    EXPECT_TRUE(std::is_copy_constructible_v<Material>);
}

TEST_F(MaterialTest, IsCopyAssignable) {
    EXPECT_TRUE(std::is_copy_assignable_v<Material>);
}

TEST_F(MaterialTest, IsMoveConstructible) {
    EXPECT_TRUE(std::is_move_constructible_v<Material>);
}

TEST_F(MaterialTest, IsMoveAssignable) {
    EXPECT_TRUE(std::is_move_assignable_v<Material>);
}

}  // namespace nexo::components
