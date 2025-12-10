//// ModelParameters.test.cpp /////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for ModelImportParameters and ModelImportPostProcessParameters
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/Assets/Model/ModelParameters.hpp"

namespace nexo::assets {

// =============================================================================
// ModelImportParameters Tests
// =============================================================================

class ModelImportParametersTest : public ::testing::Test {};

TEST_F(ModelImportParametersTest, DefaultTextureParametersEmpty) {
    ModelImportParameters params;
    EXPECT_TRUE(params.textureParameters.empty());
}

TEST_F(ModelImportParametersTest, ToJsonEmptyTextureParams) {
    ModelImportParameters params;
    nlohmann::json j = params;

    EXPECT_TRUE(j.contains("textureParameters"));
    EXPECT_TRUE(j["textureParameters"].is_array());
    EXPECT_TRUE(j["textureParameters"].empty());
}

TEST_F(ModelImportParametersTest, ToJsonWithTextureParams) {
    ModelImportParameters params;

    TextureImportParameters tex1;
    tex1.generateMipmaps = false;
    tex1.maxSize = 1024;

    TextureImportParameters tex2;
    tex2.flipVertically = false;
    tex2.compressionQuality = 0.5f;

    params.textureParameters.push_back(tex1);
    params.textureParameters.push_back(tex2);

    nlohmann::json j = params;

    EXPECT_EQ(j["textureParameters"].size(), 2);
    EXPECT_FALSE(j["textureParameters"][0]["generateMipmaps"].get<bool>());
    EXPECT_EQ(j["textureParameters"][0]["maxSize"].get<int>(), 1024);
    EXPECT_FALSE(j["textureParameters"][1]["flipVertically"].get<bool>());
}

TEST_F(ModelImportParametersTest, RoundTripWithTextureParams) {
    ModelImportParameters original;

    TextureImportParameters tex;
    tex.generateMipmaps = false;
    tex.convertToSRGB = false;
    tex.format = TextureImportParameters::Format::BC7;
    original.textureParameters.push_back(tex);

    nlohmann::json j = original;
    ModelImportParameters restored = j.get<ModelImportParameters>();

    ASSERT_EQ(restored.textureParameters.size(), 1);
    EXPECT_FALSE(restored.textureParameters[0].generateMipmaps);
    EXPECT_FALSE(restored.textureParameters[0].convertToSRGB);
    EXPECT_EQ(restored.textureParameters[0].format, TextureImportParameters::Format::BC7);
}

// =============================================================================
// ModelImportPostProcessParameters Default Values Tests
// =============================================================================

class ModelPostProcessDefaultsTest : public ::testing::Test {};

TEST_F(ModelPostProcessDefaultsTest, CalculateTangentSpaceDefaultFalse) {
    ModelImportPostProcessParameters params;
    EXPECT_FALSE(params.calculateTangentSpace);
}

TEST_F(ModelPostProcessDefaultsTest, JoinIdenticalVerticesDefaultTrue) {
    ModelImportPostProcessParameters params;
    EXPECT_TRUE(params.joinIdenticalVertices);
}

TEST_F(ModelPostProcessDefaultsTest, GenerateSmoothNormalsDefaultFalse) {
    ModelImportPostProcessParameters params;
    EXPECT_FALSE(params.generateSmoothNormals);
}

TEST_F(ModelPostProcessDefaultsTest, OptimizeMeshesDefaultTrue) {
    ModelImportPostProcessParameters params;
    EXPECT_TRUE(params.optimizeMeshes);
}

TEST_F(ModelPostProcessDefaultsTest, MaxBonesDefault60) {
    ModelImportPostProcessParameters params;
    EXPECT_EQ(params.maxBones, 60);
}

TEST_F(ModelPostProcessDefaultsTest, ImportAnimationsDefaultTrue) {
    ModelImportPostProcessParameters params;
    EXPECT_TRUE(params.importAnimations);
}

TEST_F(ModelPostProcessDefaultsTest, ImportMaterialsDefaultTrue) {
    ModelImportPostProcessParameters params;
    EXPECT_TRUE(params.importMaterials);
}

TEST_F(ModelPostProcessDefaultsTest, ImportTexturesDefaultTrue) {
    ModelImportPostProcessParameters params;
    EXPECT_TRUE(params.importTextures);
}

TEST_F(ModelPostProcessDefaultsTest, GlobalScaleDefault1) {
    ModelImportPostProcessParameters params;
    EXPECT_FLOAT_EQ(params.globalScale, 1.0f);
}

TEST_F(ModelPostProcessDefaultsTest, TextureQualityDefaultMedium) {
    ModelImportPostProcessParameters params;
    EXPECT_EQ(params.textureQuality, ModelImportPostProcessParameters::TextureQuality::MEDIUM);
}

TEST_F(ModelPostProcessDefaultsTest, ConvertToUncompressedDefaultFalse) {
    ModelImportPostProcessParameters params;
    EXPECT_FALSE(params.convertToUncompressed);
}

// =============================================================================
// TextureQuality Enum Tests
// =============================================================================

class TextureQualityEnumTest : public ::testing::Test {};

TEST_F(TextureQualityEnumTest, EnumValuesExist) {
    EXPECT_EQ(static_cast<int>(ModelImportPostProcessParameters::TextureQuality::LOW), 0);
    EXPECT_EQ(static_cast<int>(ModelImportPostProcessParameters::TextureQuality::MEDIUM), 1);
    EXPECT_EQ(static_cast<int>(ModelImportPostProcessParameters::TextureQuality::HIGH), 2);
}

// =============================================================================
// ModelImportPostProcessParameters JSON Serialization Tests
// =============================================================================

class ModelPostProcessJsonTest : public ::testing::Test {};

TEST_F(ModelPostProcessJsonTest, DefaultValuesToJson) {
    ModelImportPostProcessParameters params;
    nlohmann::json j = params;

    EXPECT_FALSE(j["calculateTangentSpace"].get<bool>());
    EXPECT_TRUE(j["joinIdenticalVertices"].get<bool>());
    EXPECT_FALSE(j["generateSmoothNormals"].get<bool>());
    EXPECT_TRUE(j["optimizeMeshes"].get<bool>());
    EXPECT_EQ(j["maxBones"].get<int>(), 60);
    EXPECT_TRUE(j["importAnimations"].get<bool>());
    EXPECT_TRUE(j["importMaterials"].get<bool>());
    EXPECT_TRUE(j["importTextures"].get<bool>());
    EXPECT_FLOAT_EQ(j["globalScale"].get<float>(), 1.0f);
    EXPECT_EQ(j["textureQuality"].get<std::string>(), "MEDIUM");
    EXPECT_FALSE(j["convertToUncompressed"].get<bool>());
}

TEST_F(ModelPostProcessJsonTest, CustomValuesToJson) {
    ModelImportPostProcessParameters params;
    params.calculateTangentSpace = true;
    params.joinIdenticalVertices = false;
    params.generateSmoothNormals = true;
    params.optimizeMeshes = false;
    params.maxBones = 120;
    params.importAnimations = false;
    params.importMaterials = false;
    params.importTextures = false;
    params.globalScale = 0.01f;
    params.textureQuality = ModelImportPostProcessParameters::TextureQuality::HIGH;
    params.convertToUncompressed = true;

    nlohmann::json j = params;

    EXPECT_TRUE(j["calculateTangentSpace"].get<bool>());
    EXPECT_FALSE(j["joinIdenticalVertices"].get<bool>());
    EXPECT_TRUE(j["generateSmoothNormals"].get<bool>());
    EXPECT_FALSE(j["optimizeMeshes"].get<bool>());
    EXPECT_EQ(j["maxBones"].get<int>(), 120);
    EXPECT_FALSE(j["importAnimations"].get<bool>());
    EXPECT_FALSE(j["importMaterials"].get<bool>());
    EXPECT_FALSE(j["importTextures"].get<bool>());
    EXPECT_FLOAT_EQ(j["globalScale"].get<float>(), 0.01f);
    EXPECT_EQ(j["textureQuality"].get<std::string>(), "HIGH");
    EXPECT_TRUE(j["convertToUncompressed"].get<bool>());
}

TEST_F(ModelPostProcessJsonTest, FromJsonRestoresValues) {
    nlohmann::json j = {
        {"calculateTangentSpace", true},
        {"joinIdenticalVertices", false},
        {"generateSmoothNormals", true},
        {"optimizeMeshes", false},
        {"maxBones", 30},
        {"importAnimations", false},
        {"importMaterials", false},
        {"importTextures", false},
        {"globalScale", 100.0f},
        {"textureQuality", "LOW"},
        {"convertToUncompressed", true}
    };

    ModelImportPostProcessParameters params = j.get<ModelImportPostProcessParameters>();

    EXPECT_TRUE(params.calculateTangentSpace);
    EXPECT_FALSE(params.joinIdenticalVertices);
    EXPECT_TRUE(params.generateSmoothNormals);
    EXPECT_FALSE(params.optimizeMeshes);
    EXPECT_EQ(params.maxBones, 30);
    EXPECT_FALSE(params.importAnimations);
    EXPECT_FALSE(params.importMaterials);
    EXPECT_FALSE(params.importTextures);
    EXPECT_FLOAT_EQ(params.globalScale, 100.0f);
    EXPECT_EQ(params.textureQuality, ModelImportPostProcessParameters::TextureQuality::LOW);
    EXPECT_TRUE(params.convertToUncompressed);
}

TEST_F(ModelPostProcessJsonTest, RoundTripSerialization) {
    ModelImportPostProcessParameters original;
    original.calculateTangentSpace = true;
    original.maxBones = 45;
    original.globalScale = 0.1f;
    original.textureQuality = ModelImportPostProcessParameters::TextureQuality::HIGH;

    nlohmann::json j = original;
    ModelImportPostProcessParameters restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_EQ(original.calculateTangentSpace, restored.calculateTangentSpace);
    EXPECT_EQ(original.maxBones, restored.maxBones);
    EXPECT_FLOAT_EQ(original.globalScale, restored.globalScale);
    EXPECT_EQ(original.textureQuality, restored.textureQuality);
}

TEST_F(ModelPostProcessJsonTest, TextureQualityEnumSerialization) {
    ModelImportPostProcessParameters params;

    // Test LOW
    params.textureQuality = ModelImportPostProcessParameters::TextureQuality::LOW;
    nlohmann::json j1 = params;
    EXPECT_EQ(j1["textureQuality"].get<std::string>(), "LOW");
    auto restored1 = j1.get<ModelImportPostProcessParameters>();
    EXPECT_EQ(restored1.textureQuality, ModelImportPostProcessParameters::TextureQuality::LOW);

    // Test MEDIUM
    params.textureQuality = ModelImportPostProcessParameters::TextureQuality::MEDIUM;
    nlohmann::json j2 = params;
    EXPECT_EQ(j2["textureQuality"].get<std::string>(), "MEDIUM");
    auto restored2 = j2.get<ModelImportPostProcessParameters>();
    EXPECT_EQ(restored2.textureQuality, ModelImportPostProcessParameters::TextureQuality::MEDIUM);

    // Test HIGH
    params.textureQuality = ModelImportPostProcessParameters::TextureQuality::HIGH;
    nlohmann::json j3 = params;
    EXPECT_EQ(j3["textureQuality"].get<std::string>(), "HIGH");
    auto restored3 = j3.get<ModelImportPostProcessParameters>();
    EXPECT_EQ(restored3.textureQuality, ModelImportPostProcessParameters::TextureQuality::HIGH);
}

TEST_F(ModelPostProcessJsonTest, GlobalScaleBoundaries) {
    ModelImportPostProcessParameters params;

    // Very small scale
    params.globalScale = 0.001f;
    nlohmann::json j1 = params;
    auto restored1 = j1.get<ModelImportPostProcessParameters>();
    EXPECT_FLOAT_EQ(restored1.globalScale, 0.001f);

    // Very large scale
    params.globalScale = 1000.0f;
    nlohmann::json j2 = params;
    auto restored2 = j2.get<ModelImportPostProcessParameters>();
    EXPECT_FLOAT_EQ(restored2.globalScale, 1000.0f);
}

}  // namespace nexo::assets
