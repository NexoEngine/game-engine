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
//  Description: Test file for the Render3D components (simplified for testing)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "components/Render3D.hpp"
#include <glm/gtc/epsilon.hpp>

// NOTE: Using simplified Render3D tests due to complex Texture asset dependencies
// Full Texture class requires OpenGL context and complex initialization
// These tests focus on Material structure and AssetRef usage patterns

namespace nexo::components {

class Render3DTest : public ::testing::Test {
protected:
    void SetUp() override {
        material = Material{};
    }
    
    Material material;
};

TEST_F(Render3DTest, DefaultConstruction) {
    Material defaultMaterial;
    
    // Default color values
    EXPECT_FLOAT_EQ(defaultMaterial.albedoColor.r, 1.0f);
    EXPECT_FLOAT_EQ(defaultMaterial.albedoColor.g, 1.0f);
    EXPECT_FLOAT_EQ(defaultMaterial.albedoColor.b, 1.0f);
    EXPECT_FLOAT_EQ(defaultMaterial.albedoColor.a, 1.0f);
    
    // Default PBR values
    EXPECT_FLOAT_EQ(defaultMaterial.metallic, 0.0f);
    EXPECT_FLOAT_EQ(defaultMaterial.roughness, 0.0f);
    EXPECT_FLOAT_EQ(defaultMaterial.opacity, 1.0f);
    EXPECT_TRUE(defaultMaterial.isOpaque);
    
    // Default specular color
    EXPECT_FLOAT_EQ(defaultMaterial.specularColor.r, 1.0f);
    EXPECT_FLOAT_EQ(defaultMaterial.specularColor.g, 1.0f);
    EXPECT_FLOAT_EQ(defaultMaterial.specularColor.b, 1.0f);
    EXPECT_FLOAT_EQ(defaultMaterial.specularColor.a, 1.0f);
    
    // Default emissive
    EXPECT_FLOAT_EQ(defaultMaterial.emissiveColor.r, 0.0f);
    EXPECT_FLOAT_EQ(defaultMaterial.emissiveColor.g, 0.0f);
    EXPECT_FLOAT_EQ(defaultMaterial.emissiveColor.b, 0.0f);
    
    // Default shader
    EXPECT_EQ(defaultMaterial.shader, "Phong");
}

TEST_F(Render3DTest, ColorModification) {
    // Test albedo color modification
    material.albedoColor = glm::vec4(0.5f, 0.6f, 0.7f, 0.8f);
    
    EXPECT_FLOAT_EQ(material.albedoColor.r, 0.5f);
    EXPECT_FLOAT_EQ(material.albedoColor.g, 0.6f);
    EXPECT_FLOAT_EQ(material.albedoColor.b, 0.7f);
    EXPECT_FLOAT_EQ(material.albedoColor.a, 0.8f);
    
    // Test emissive color modification
    material.emissiveColor = glm::vec3(0.2f, 0.3f, 0.4f);
    
    EXPECT_FLOAT_EQ(material.emissiveColor.r, 0.2f);
    EXPECT_FLOAT_EQ(material.emissiveColor.g, 0.3f);
    EXPECT_FLOAT_EQ(material.emissiveColor.b, 0.4f);
    
    // Test specular color modification
    material.specularColor = glm::vec4(0.1f, 0.2f, 0.3f, 0.4f);
    EXPECT_FLOAT_EQ(material.specularColor.r, 0.1f);
    EXPECT_FLOAT_EQ(material.specularColor.g, 0.2f);
    EXPECT_FLOAT_EQ(material.specularColor.b, 0.3f);
    EXPECT_FLOAT_EQ(material.specularColor.a, 0.4f);
}

TEST_F(Render3DTest, PBRParameterModification) {
    // Test metallic parameter
    material.metallic = 0.8f;
    EXPECT_FLOAT_EQ(material.metallic, 0.8f);
    
    // Test roughness parameter
    material.roughness = 0.3f;
    EXPECT_FLOAT_EQ(material.roughness, 0.3f);
    
    // Test opacity parameter
    material.opacity = 0.9f;
    EXPECT_FLOAT_EQ(material.opacity, 0.9f);
    
    // Test opaque flag
    material.isOpaque = false;
    EXPECT_FALSE(material.isOpaque);
}

TEST_F(Render3DTest, TextureReferencesDefaultState) {
    // Verify default state of texture references
    EXPECT_FALSE(material.albedoTexture.isValid());
    EXPECT_FALSE(material.normalMap.isValid());
    EXPECT_FALSE(material.metallicMap.isValid());
    EXPECT_FALSE(material.roughnessMap.isValid());
    EXPECT_FALSE(material.emissiveMap.isValid());
    
    // Verify null texture references
    EXPECT_EQ(material.albedoTexture.lock(), nullptr);
    EXPECT_EQ(material.normalMap.lock(), nullptr);
    EXPECT_EQ(material.metallicMap.lock(), nullptr);
    EXPECT_EQ(material.roughnessMap.lock(), nullptr);
    EXPECT_EQ(material.emissiveMap.lock(), nullptr);
}

TEST_F(Render3DTest, CopyConstruction) {
    // Modify original material
    material.albedoColor = glm::vec4(0.1f, 0.2f, 0.3f, 0.4f);
    material.metallic = 0.7f;
    material.roughness = 0.2f;
    
    // Copy construct
    Material copy(material);
    
    // Verify copy has same values
    EXPECT_TRUE(glm::all(glm::epsilonEqual(copy.albedoColor, material.albedoColor, 0.001f)));
    EXPECT_FLOAT_EQ(copy.metallic, material.metallic);
    EXPECT_FLOAT_EQ(copy.roughness, material.roughness);
}

TEST_F(Render3DTest, MoveConstruction) {
    // Modify original material
    material.albedoColor = glm::vec4(0.1f, 0.2f, 0.3f, 0.4f);
    material.metallic = 0.7f;
    material.roughness = 0.2f;
    
    glm::vec4 originalColor = material.albedoColor;
    float originalMetallic = material.metallic;
    float originalRoughness = material.roughness;
    
    // Move construct
    Material moved(std::move(material));
    
    // Verify moved has the values
    EXPECT_TRUE(glm::all(glm::epsilonEqual(moved.albedoColor, originalColor, 0.001f)));
    EXPECT_FLOAT_EQ(moved.metallic, originalMetallic);
    EXPECT_FLOAT_EQ(moved.roughness, originalRoughness);
}

TEST_F(Render3DTest, CopyAssignment) {
    Material source;
    source.albedoColor = glm::vec4(0.5f, 0.6f, 0.7f, 0.8f);
    source.metallic = 0.9f;
    source.roughness = 0.1f;
    
    material = source;
    
    EXPECT_TRUE(glm::all(glm::epsilonEqual(material.albedoColor, source.albedoColor, 0.001f)));
    EXPECT_FLOAT_EQ(material.metallic, source.metallic);
    EXPECT_FLOAT_EQ(material.roughness, source.roughness);
}

TEST_F(Render3DTest, MoveAssignment) {
    Material source;
    source.albedoColor = glm::vec4(0.5f, 0.6f, 0.7f, 0.8f);
    source.metallic = 0.9f;
    source.roughness = 0.1f;
    
    glm::vec4 originalColor = source.albedoColor;
    float originalMetallic = source.metallic;
    float originalRoughness = source.roughness;
    
    material = std::move(source);
    
    EXPECT_TRUE(glm::all(glm::epsilonEqual(material.albedoColor, originalColor, 0.001f)));
    EXPECT_FLOAT_EQ(material.metallic, originalMetallic);
    EXPECT_FLOAT_EQ(material.roughness, originalRoughness);
}

TEST_F(Render3DTest, PBRValueBoundaries) {
    // Test metallic boundaries (0.0 to 1.0)
    material.metallic = 0.0f;
    EXPECT_FLOAT_EQ(material.metallic, 0.0f);
    
    material.metallic = 1.0f;
    EXPECT_FLOAT_EQ(material.metallic, 1.0f);
    
    // Test roughness boundaries (0.0 to 1.0)
    material.roughness = 0.0f;
    EXPECT_FLOAT_EQ(material.roughness, 0.0f);
    
    material.roughness = 1.0f;
    EXPECT_FLOAT_EQ(material.roughness, 1.0f);
    
    // Test opacity boundaries (0.0 to 1.0)
    material.opacity = 0.0f;
    EXPECT_FLOAT_EQ(material.opacity, 0.0f);
    
    material.opacity = 1.0f;
    EXPECT_FLOAT_EQ(material.opacity, 1.0f);
}

TEST_F(Render3DTest, ShaderNameModification) {
    // Test shader name modification  
    EXPECT_EQ(material.shader, "Phong");
    
    material.shader = "PBR";
    EXPECT_EQ(material.shader, "PBR");
    
    material.shader = "Toon";
    EXPECT_EQ(material.shader, "Toon");
    
    material.shader = "";
    EXPECT_EQ(material.shader, "");
}

} // namespace nexo::components