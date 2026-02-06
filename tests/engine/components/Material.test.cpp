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
// Boundary Value Tests
// =============================================================================

TEST_F(MaterialTest, RoughnessMinimumBoundary) {
    Material mat;
    mat.roughness = 0.0f;
    EXPECT_FLOAT_EQ(mat.roughness, 0.0f);
}

TEST_F(MaterialTest, RoughnessMaximumBoundary) {
    Material mat;
    mat.roughness = 1.0f;
    EXPECT_FLOAT_EQ(mat.roughness, 1.0f);
}

TEST_F(MaterialTest, RoughnessMidpointValue) {
    Material mat;
    mat.roughness = 0.5f;
    EXPECT_FLOAT_EQ(mat.roughness, 0.5f);
}

TEST_F(MaterialTest, MetallicMinimumBoundary) {
    Material mat;
    mat.metallic = 0.0f;
    EXPECT_FLOAT_EQ(mat.metallic, 0.0f);
}

TEST_F(MaterialTest, MetallicMaximumBoundary) {
    Material mat;
    mat.metallic = 1.0f;
    EXPECT_FLOAT_EQ(mat.metallic, 1.0f);
}

TEST_F(MaterialTest, MetallicMidpointValue) {
    Material mat;
    mat.metallic = 0.5f;
    EXPECT_FLOAT_EQ(mat.metallic, 0.5f);
}

TEST_F(MaterialTest, OpacityMinimumBoundary) {
    Material mat;
    mat.opacity = 0.0f;
    EXPECT_FLOAT_EQ(mat.opacity, 0.0f);
}

TEST_F(MaterialTest, OpacityMaximumBoundary) {
    Material mat;
    mat.opacity = 1.0f;
    EXPECT_FLOAT_EQ(mat.opacity, 1.0f);
}

TEST_F(MaterialTest, OpacityMidpointValue) {
    Material mat;
    mat.opacity = 0.5f;
    EXPECT_FLOAT_EQ(mat.opacity, 0.5f);
}

// =============================================================================
// Edge Case Tests
// =============================================================================

TEST_F(MaterialTest, NegativeRoughnessValue) {
    Material mat;
    mat.roughness = -0.5f;
    EXPECT_FLOAT_EQ(mat.roughness, -0.5f);  // No clamping in struct
}

TEST_F(MaterialTest, RoughnessAboveOne) {
    Material mat;
    mat.roughness = 1.5f;
    EXPECT_FLOAT_EQ(mat.roughness, 1.5f);  // No clamping in struct
}

TEST_F(MaterialTest, NegativeMetallicValue) {
    Material mat;
    mat.metallic = -0.2f;
    EXPECT_FLOAT_EQ(mat.metallic, -0.2f);  // No clamping in struct
}

TEST_F(MaterialTest, MetallicAboveOne) {
    Material mat;
    mat.metallic = 2.0f;
    EXPECT_FLOAT_EQ(mat.metallic, 2.0f);  // No clamping in struct
}

TEST_F(MaterialTest, NegativeOpacityValue) {
    Material mat;
    mat.opacity = -0.1f;
    EXPECT_FLOAT_EQ(mat.opacity, -0.1f);  // No clamping in struct
}

TEST_F(MaterialTest, OpacityAboveOne) {
    Material mat;
    mat.opacity = 1.5f;
    EXPECT_FLOAT_EQ(mat.opacity, 1.5f);  // No clamping in struct
}

TEST_F(MaterialTest, ZeroAlbedoColor) {
    Material mat;
    mat.albedoColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    EXPECT_FLOAT_EQ(mat.albedoColor.r, 0.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.g, 0.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.b, 0.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.a, 0.0f);
}

TEST_F(MaterialTest, MaxAlbedoColor) {
    Material mat;
    mat.albedoColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    EXPECT_FLOAT_EQ(mat.albedoColor.r, 1.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.g, 1.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.b, 1.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.a, 1.0f);
}

TEST_F(MaterialTest, NegativeColorComponents) {
    Material mat;
    mat.albedoColor = glm::vec4(-0.5f, -1.0f, -0.2f, -0.3f);

    EXPECT_FLOAT_EQ(mat.albedoColor.r, -0.5f);
    EXPECT_FLOAT_EQ(mat.albedoColor.g, -1.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.b, -0.2f);
    EXPECT_FLOAT_EQ(mat.albedoColor.a, -0.3f);
}

TEST_F(MaterialTest, HDREmissiveValues) {
    Material mat;
    mat.emissiveColor = glm::vec3(10.0f, 20.0f, 30.0f);

    EXPECT_FLOAT_EQ(mat.emissiveColor.r, 10.0f);
    EXPECT_FLOAT_EQ(mat.emissiveColor.g, 20.0f);
    EXPECT_FLOAT_EQ(mat.emissiveColor.b, 30.0f);
}

TEST_F(MaterialTest, ZeroEmissiveColor) {
    Material mat;
    mat.emissiveColor = glm::vec3(0.0f, 0.0f, 0.0f);

    EXPECT_FLOAT_EQ(mat.emissiveColor.r, 0.0f);
    EXPECT_FLOAT_EQ(mat.emissiveColor.g, 0.0f);
    EXPECT_FLOAT_EQ(mat.emissiveColor.b, 0.0f);
}

// =============================================================================
// Texture Reference Tests
// =============================================================================

TEST_F(MaterialTest, AlbedoTextureRemainsNullAfterAssignment) {
    Material mat;
    EXPECT_EQ(mat.albedoTexture, nullptr);
    mat.albedoTexture = nullptr;
    EXPECT_EQ(mat.albedoTexture, nullptr);
}

TEST_F(MaterialTest, NormalMapRemainsNullAfterAssignment) {
    Material mat;
    EXPECT_EQ(mat.normalMap, nullptr);
    mat.normalMap = nullptr;
    EXPECT_EQ(mat.normalMap, nullptr);
}

TEST_F(MaterialTest, MetallicMapRemainsNullAfterAssignment) {
    Material mat;
    EXPECT_EQ(mat.metallicMap, nullptr);
    mat.metallicMap = nullptr;
    EXPECT_EQ(mat.metallicMap, nullptr);
}

TEST_F(MaterialTest, RoughnessMapRemainsNullAfterAssignment) {
    Material mat;
    EXPECT_EQ(mat.roughnessMap, nullptr);
    mat.roughnessMap = nullptr;
    EXPECT_EQ(mat.roughnessMap, nullptr);
}

TEST_F(MaterialTest, EmissiveMapRemainsNullAfterAssignment) {
    Material mat;
    EXPECT_EQ(mat.emissiveMap, nullptr);
    mat.emissiveMap = nullptr;
    EXPECT_EQ(mat.emissiveMap, nullptr);
}

TEST_F(MaterialTest, AllTexturesNullByDefault) {
    Material mat;
    EXPECT_EQ(mat.albedoTexture, nullptr);
    EXPECT_EQ(mat.normalMap, nullptr);
    EXPECT_EQ(mat.metallicMap, nullptr);
    EXPECT_EQ(mat.roughnessMap, nullptr);
    EXPECT_EQ(mat.emissiveMap, nullptr);
}

// =============================================================================
// Shader Name Tests
// =============================================================================

TEST_F(MaterialTest, EmptyShaderName) {
    Material mat;
    mat.shader = "";
    EXPECT_EQ(mat.shader, "");
    EXPECT_TRUE(mat.shader.empty());
}

TEST_F(MaterialTest, LongShaderName) {
    Material mat;
    mat.shader = "VeryLongCustomShaderNameForTestingPurposes";
    EXPECT_EQ(mat.shader, "VeryLongCustomShaderNameForTestingPurposes");
}

TEST_F(MaterialTest, ShaderNameWithSpecialCharacters) {
    Material mat;
    mat.shader = "Shader_PBR-v2.0";
    EXPECT_EQ(mat.shader, "Shader_PBR-v2.0");
}

TEST_F(MaterialTest, MultipleShaderChanges) {
    Material mat;
    EXPECT_EQ(mat.shader, "Phong");

    mat.shader = "PBR";
    EXPECT_EQ(mat.shader, "PBR");

    mat.shader = "Unlit";
    EXPECT_EQ(mat.shader, "Unlit");

    mat.shader = "Phong";
    EXPECT_EQ(mat.shader, "Phong");
}

// =============================================================================
// Combined Property Tests
// =============================================================================

TEST_F(MaterialTest, FullyMetallicSmooth) {
    Material mat;
    mat.metallic = 1.0f;
    mat.roughness = 0.0f;
    mat.albedoColor = glm::vec4(0.95f, 0.95f, 1.0f, 1.0f);  // Chrome-like

    EXPECT_FLOAT_EQ(mat.metallic, 1.0f);
    EXPECT_FLOAT_EQ(mat.roughness, 0.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.r, 0.95f);
}

TEST_F(MaterialTest, FullyNonMetallicRough) {
    Material mat;
    mat.metallic = 0.0f;
    mat.roughness = 1.0f;
    mat.albedoColor = glm::vec4(0.3f, 0.25f, 0.2f, 1.0f);  // Rough wood

    EXPECT_FLOAT_EQ(mat.metallic, 0.0f);
    EXPECT_FLOAT_EQ(mat.roughness, 1.0f);
    EXPECT_FLOAT_EQ(mat.albedoColor.r, 0.3f);
}

TEST_F(MaterialTest, TransparentWithEmission) {
    Material mat;
    mat.isOpaque = false;
    mat.opacity = 0.6f;
    mat.emissiveColor = glm::vec3(0.5f, 1.0f, 0.5f);  // Green glow

    EXPECT_FALSE(mat.isOpaque);
    EXPECT_FLOAT_EQ(mat.opacity, 0.6f);
    EXPECT_FLOAT_EQ(mat.emissiveColor.g, 1.0f);
}

TEST_F(MaterialTest, ComplexMaterialState) {
    Material mat;
    mat.albedoColor = glm::vec4(0.8f, 0.2f, 0.2f, 1.0f);
    mat.specularColor = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
    mat.emissiveColor = glm::vec3(0.1f, 0.0f, 0.0f);
    mat.roughness = 0.3f;
    mat.metallic = 0.7f;
    mat.opacity = 1.0f;
    mat.isOpaque = true;
    mat.shader = "PBR";

    EXPECT_FLOAT_EQ(mat.albedoColor.r, 0.8f);
    EXPECT_FLOAT_EQ(mat.specularColor.g, 0.9f);
    EXPECT_FLOAT_EQ(mat.emissiveColor.r, 0.1f);
    EXPECT_FLOAT_EQ(mat.roughness, 0.3f);
    EXPECT_FLOAT_EQ(mat.metallic, 0.7f);
    EXPECT_FLOAT_EQ(mat.opacity, 1.0f);
    EXPECT_TRUE(mat.isOpaque);
    EXPECT_EQ(mat.shader, "PBR");
}

// =============================================================================
// Opacity and Transparency Tests
// =============================================================================

TEST_F(MaterialTest, OpacityDoesNotAffectIsOpaque) {
    Material mat;
    mat.opacity = 0.5f;
    EXPECT_TRUE(mat.isOpaque);  // isOpaque is independent
}

TEST_F(MaterialTest, TransparentMaterialWithFullOpacity) {
    Material mat;
    mat.isOpaque = false;
    mat.opacity = 1.0f;

    EXPECT_FALSE(mat.isOpaque);
    EXPECT_FLOAT_EQ(mat.opacity, 1.0f);
}

TEST_F(MaterialTest, OpaqueMaterialWithZeroOpacity) {
    Material mat;
    mat.isOpaque = true;
    mat.opacity = 0.0f;

    EXPECT_TRUE(mat.isOpaque);
    EXPECT_FLOAT_EQ(mat.opacity, 0.0f);
}

// =============================================================================
// Specular Color Tests
// =============================================================================

TEST_F(MaterialTest, ZeroSpecularColor) {
    Material mat;
    mat.specularColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

    EXPECT_FLOAT_EQ(mat.specularColor.r, 0.0f);
    EXPECT_FLOAT_EQ(mat.specularColor.g, 0.0f);
    EXPECT_FLOAT_EQ(mat.specularColor.b, 0.0f);
    EXPECT_FLOAT_EQ(mat.specularColor.a, 0.0f);
}

TEST_F(MaterialTest, PartialSpecularColor) {
    Material mat;
    mat.specularColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

    EXPECT_FLOAT_EQ(mat.specularColor.r, 0.5f);
    EXPECT_FLOAT_EQ(mat.specularColor.g, 0.5f);
    EXPECT_FLOAT_EQ(mat.specularColor.b, 0.5f);
    EXPECT_FLOAT_EQ(mat.specularColor.a, 1.0f);
}

TEST_F(MaterialTest, ColoredSpecular) {
    Material mat;
    mat.specularColor = glm::vec4(1.0f, 0.8f, 0.6f, 1.0f);  // Warm specular

    EXPECT_FLOAT_EQ(mat.specularColor.r, 1.0f);
    EXPECT_FLOAT_EQ(mat.specularColor.g, 0.8f);
    EXPECT_FLOAT_EQ(mat.specularColor.b, 0.6f);
}

// =============================================================================
// Copy and Move Tests (Extended)
// =============================================================================

TEST_F(MaterialTest, CopyPreservesAllTextures) {
    Material original;
    Material copy = original;

    EXPECT_EQ(copy.albedoTexture, nullptr);
    EXPECT_EQ(copy.normalMap, nullptr);
    EXPECT_EQ(copy.metallicMap, nullptr);
    EXPECT_EQ(copy.roughnessMap, nullptr);
    EXPECT_EQ(copy.emissiveMap, nullptr);
}

TEST_F(MaterialTest, CopyPreservesAllColors) {
    Material original;
    original.albedoColor = glm::vec4(0.1f, 0.2f, 0.3f, 0.4f);
    original.specularColor = glm::vec4(0.5f, 0.6f, 0.7f, 0.8f);
    original.emissiveColor = glm::vec3(0.9f, 1.0f, 1.1f);

    Material copy = original;

    EXPECT_FLOAT_EQ(copy.albedoColor.r, 0.1f);
    EXPECT_FLOAT_EQ(copy.specularColor.g, 0.6f);
    EXPECT_FLOAT_EQ(copy.emissiveColor.b, 1.1f);
}

TEST_F(MaterialTest, CopyPreservesAllFloats) {
    Material original;
    original.roughness = 0.123f;
    original.metallic = 0.456f;
    original.opacity = 0.789f;

    Material copy = original;

    EXPECT_FLOAT_EQ(copy.roughness, 0.123f);
    EXPECT_FLOAT_EQ(copy.metallic, 0.456f);
    EXPECT_FLOAT_EQ(copy.opacity, 0.789f);
}

TEST_F(MaterialTest, CopyPreservesOpaqueFlag) {
    Material original;
    original.isOpaque = false;

    Material copy = original;

    EXPECT_FALSE(copy.isOpaque);
}

TEST_F(MaterialTest, IndependentCopiesCanModify) {
    Material original;
    original.roughness = 0.5f;

    Material copy = original;
    copy.roughness = 0.8f;

    EXPECT_FLOAT_EQ(original.roughness, 0.5f);
    EXPECT_FLOAT_EQ(copy.roughness, 0.8f);
}

TEST_F(MaterialTest, AssignmentOverwritesAllFields) {
    Material mat1;
    mat1.roughness = 0.1f;
    mat1.metallic = 0.2f;
    mat1.shader = "Original";

    Material mat2;
    mat2.roughness = 0.9f;
    mat2.metallic = 0.8f;
    mat2.shader = "Modified";

    mat1 = mat2;

    EXPECT_FLOAT_EQ(mat1.roughness, 0.9f);
    EXPECT_FLOAT_EQ(mat1.metallic, 0.8f);
    EXPECT_EQ(mat1.shader, "Modified");
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
