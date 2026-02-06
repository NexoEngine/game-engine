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

// =============================================================================
// ModelImportParameters Advanced Tests
// =============================================================================

class ModelImportParametersAdvancedTest : public ::testing::Test {};

TEST_F(ModelImportParametersAdvancedTest, MultipleTextureParametersWithDifferentSettings) {
    ModelImportParameters params;

    TextureImportParameters tex1;
    tex1.generateMipmaps = true;
    tex1.convertToSRGB = false;
    tex1.flipVertically = true;
    tex1.format = TextureImportParameters::Format::RGB;
    tex1.maxSize = 2048;
    tex1.compressionQuality = 0.8f;

    TextureImportParameters tex2;
    tex2.generateMipmaps = false;
    tex2.convertToSRGB = true;
    tex2.flipVertically = false;
    tex2.format = TextureImportParameters::Format::BC7;
    tex2.maxSize = 1024;
    tex2.compressionQuality = 0.5f;

    TextureImportParameters tex3;
    tex3.generateMipmaps = true;
    tex3.convertToSRGB = true;
    tex3.flipVertically = true;
    tex3.format = TextureImportParameters::Format::Preserve;
    tex3.maxSize = 4096;
    tex3.compressionQuality = 0.95f;

    params.textureParameters.push_back(tex1);
    params.textureParameters.push_back(tex2);
    params.textureParameters.push_back(tex3);

    nlohmann::json j = params;
    ModelImportParameters restored = j.get<ModelImportParameters>();

    ASSERT_EQ(restored.textureParameters.size(), 3);

    // Verify tex1
    EXPECT_TRUE(restored.textureParameters[0].generateMipmaps);
    EXPECT_FALSE(restored.textureParameters[0].convertToSRGB);
    EXPECT_TRUE(restored.textureParameters[0].flipVertically);
    EXPECT_EQ(restored.textureParameters[0].format, TextureImportParameters::Format::RGB);
    EXPECT_EQ(restored.textureParameters[0].maxSize, 2048);
    EXPECT_FLOAT_EQ(restored.textureParameters[0].compressionQuality, 0.8f);

    // Verify tex2
    EXPECT_FALSE(restored.textureParameters[1].generateMipmaps);
    EXPECT_TRUE(restored.textureParameters[1].convertToSRGB);
    EXPECT_FALSE(restored.textureParameters[1].flipVertically);
    EXPECT_EQ(restored.textureParameters[1].format, TextureImportParameters::Format::BC7);
    EXPECT_EQ(restored.textureParameters[1].maxSize, 1024);
    EXPECT_FLOAT_EQ(restored.textureParameters[1].compressionQuality, 0.5f);

    // Verify tex3
    EXPECT_TRUE(restored.textureParameters[2].generateMipmaps);
    EXPECT_TRUE(restored.textureParameters[2].convertToSRGB);
    EXPECT_TRUE(restored.textureParameters[2].flipVertically);
    EXPECT_EQ(restored.textureParameters[2].format, TextureImportParameters::Format::Preserve);
    EXPECT_EQ(restored.textureParameters[2].maxSize, 4096);
    EXPECT_FLOAT_EQ(restored.textureParameters[2].compressionQuality, 0.95f);
}

TEST_F(ModelImportParametersAdvancedTest, FromJsonEmptyArray) {
    nlohmann::json j = {{"textureParameters", nlohmann::json::array()}};
    ModelImportParameters params = j.get<ModelImportParameters>();
    EXPECT_TRUE(params.textureParameters.empty());
}

TEST_F(ModelImportParametersAdvancedTest, LargeTextureParametersArray) {
    ModelImportParameters params;

    // Add many texture parameters
    for (int i = 0; i < 100; ++i) {
        TextureImportParameters tex;
        tex.maxSize = 512 * (i + 1);
        tex.generateMipmaps = (i % 2 == 0);
        params.textureParameters.push_back(tex);
    }

    nlohmann::json j = params;
    ModelImportParameters restored = j.get<ModelImportParameters>();

    ASSERT_EQ(restored.textureParameters.size(), 100);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(restored.textureParameters[i].maxSize, 512 * (i + 1));
        EXPECT_EQ(restored.textureParameters[i].generateMipmaps, (i % 2 == 0));
    }
}

TEST_F(ModelImportParametersAdvancedTest, NestedStructureRoundTrip) {
    ModelImportParameters params;

    // Create texture with all possible format values
    for (auto format : {TextureImportParameters::Format::Preserve,
                        TextureImportParameters::Format::RGB,
                        TextureImportParameters::Format::RGBA,
                        TextureImportParameters::Format::BC1,
                        TextureImportParameters::Format::BC3,
                        TextureImportParameters::Format::BC7}) {
        TextureImportParameters tex;
        tex.format = format;
        params.textureParameters.push_back(tex);
    }

    nlohmann::json j = params;
    ModelImportParameters restored = j.get<ModelImportParameters>();

    ASSERT_EQ(restored.textureParameters.size(), 6);
    EXPECT_EQ(restored.textureParameters[0].format, TextureImportParameters::Format::Preserve);
    EXPECT_EQ(restored.textureParameters[1].format, TextureImportParameters::Format::RGB);
    EXPECT_EQ(restored.textureParameters[2].format, TextureImportParameters::Format::RGBA);
    EXPECT_EQ(restored.textureParameters[3].format, TextureImportParameters::Format::BC1);
    EXPECT_EQ(restored.textureParameters[4].format, TextureImportParameters::Format::BC3);
    EXPECT_EQ(restored.textureParameters[5].format, TextureImportParameters::Format::BC7);
}

// =============================================================================
// Boolean Flag Combinations Tests
// =============================================================================

class BooleanFlagCombinationsTest : public ::testing::Test {};

TEST_F(BooleanFlagCombinationsTest, AllMeshProcessingFlagsTrue) {
    ModelImportPostProcessParameters params;
    params.calculateTangentSpace = true;
    params.joinIdenticalVertices = true;
    params.generateSmoothNormals = true;
    params.optimizeMeshes = true;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_TRUE(restored.calculateTangentSpace);
    EXPECT_TRUE(restored.joinIdenticalVertices);
    EXPECT_TRUE(restored.generateSmoothNormals);
    EXPECT_TRUE(restored.optimizeMeshes);
}

TEST_F(BooleanFlagCombinationsTest, AllMeshProcessingFlagsFalse) {
    ModelImportPostProcessParameters params;
    params.calculateTangentSpace = false;
    params.joinIdenticalVertices = false;
    params.generateSmoothNormals = false;
    params.optimizeMeshes = false;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_FALSE(restored.calculateTangentSpace);
    EXPECT_FALSE(restored.joinIdenticalVertices);
    EXPECT_FALSE(restored.generateSmoothNormals);
    EXPECT_FALSE(restored.optimizeMeshes);
}

TEST_F(BooleanFlagCombinationsTest, AllSceneOptionsFlagsTrue) {
    ModelImportPostProcessParameters params;
    params.importAnimations = true;
    params.importMaterials = true;
    params.importTextures = true;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_TRUE(restored.importAnimations);
    EXPECT_TRUE(restored.importMaterials);
    EXPECT_TRUE(restored.importTextures);
}

TEST_F(BooleanFlagCombinationsTest, AllSceneOptionsFlagsFalse) {
    ModelImportPostProcessParameters params;
    params.importAnimations = false;
    params.importMaterials = false;
    params.importTextures = false;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_FALSE(restored.importAnimations);
    EXPECT_FALSE(restored.importMaterials);
    EXPECT_FALSE(restored.importTextures);
}

TEST_F(BooleanFlagCombinationsTest, AllTextureOptionsFlagsTrue) {
    ModelImportPostProcessParameters params;
    params.convertToUncompressed = true;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_TRUE(restored.convertToUncompressed);
}

TEST_F(BooleanFlagCombinationsTest, MixedBooleanFlags) {
    ModelImportPostProcessParameters params;
    params.calculateTangentSpace = true;
    params.joinIdenticalVertices = false;
    params.generateSmoothNormals = true;
    params.optimizeMeshes = false;
    params.importAnimations = false;
    params.importMaterials = true;
    params.importTextures = false;
    params.convertToUncompressed = true;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_TRUE(restored.calculateTangentSpace);
    EXPECT_FALSE(restored.joinIdenticalVertices);
    EXPECT_TRUE(restored.generateSmoothNormals);
    EXPECT_FALSE(restored.optimizeMeshes);
    EXPECT_FALSE(restored.importAnimations);
    EXPECT_TRUE(restored.importMaterials);
    EXPECT_FALSE(restored.importTextures);
    EXPECT_TRUE(restored.convertToUncompressed);
}

TEST_F(BooleanFlagCombinationsTest, AllBooleanFlagsTrue) {
    ModelImportPostProcessParameters params;
    params.calculateTangentSpace = true;
    params.joinIdenticalVertices = true;
    params.generateSmoothNormals = true;
    params.optimizeMeshes = true;
    params.importAnimations = true;
    params.importMaterials = true;
    params.importTextures = true;
    params.convertToUncompressed = true;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_TRUE(restored.calculateTangentSpace);
    EXPECT_TRUE(restored.joinIdenticalVertices);
    EXPECT_TRUE(restored.generateSmoothNormals);
    EXPECT_TRUE(restored.optimizeMeshes);
    EXPECT_TRUE(restored.importAnimations);
    EXPECT_TRUE(restored.importMaterials);
    EXPECT_TRUE(restored.importTextures);
    EXPECT_TRUE(restored.convertToUncompressed);
}

TEST_F(BooleanFlagCombinationsTest, AllBooleanFlagsFalse) {
    ModelImportPostProcessParameters params;
    params.calculateTangentSpace = false;
    params.joinIdenticalVertices = false;
    params.generateSmoothNormals = false;
    params.optimizeMeshes = false;
    params.importAnimations = false;
    params.importMaterials = false;
    params.importTextures = false;
    params.convertToUncompressed = false;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_FALSE(restored.calculateTangentSpace);
    EXPECT_FALSE(restored.joinIdenticalVertices);
    EXPECT_FALSE(restored.generateSmoothNormals);
    EXPECT_FALSE(restored.optimizeMeshes);
    EXPECT_FALSE(restored.importAnimations);
    EXPECT_FALSE(restored.importMaterials);
    EXPECT_FALSE(restored.importTextures);
    EXPECT_FALSE(restored.convertToUncompressed);
}

// =============================================================================
// Numeric Value Boundary Tests
// =============================================================================

class NumericBoundaryTest : public ::testing::Test {};

TEST_F(NumericBoundaryTest, MaxBonesZero) {
    ModelImportPostProcessParameters params;
    params.maxBones = 0;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_EQ(restored.maxBones, 0);
}

TEST_F(NumericBoundaryTest, MaxBonesNegative) {
    ModelImportPostProcessParameters params;
    params.maxBones = -1;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_EQ(restored.maxBones, -1);
}

TEST_F(NumericBoundaryTest, MaxBonesVeryLarge) {
    ModelImportPostProcessParameters params;
    params.maxBones = 1000000;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_EQ(restored.maxBones, 1000000);
}

TEST_F(NumericBoundaryTest, GlobalScaleZero) {
    ModelImportPostProcessParameters params;
    params.globalScale = 0.0f;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_FLOAT_EQ(restored.globalScale, 0.0f);
}

TEST_F(NumericBoundaryTest, GlobalScaleNegative) {
    ModelImportPostProcessParameters params;
    params.globalScale = -10.0f;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_FLOAT_EQ(restored.globalScale, -10.0f);
}

TEST_F(NumericBoundaryTest, GlobalScaleVerySmall) {
    ModelImportPostProcessParameters params;
    params.globalScale = 0.0000001f;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_NEAR(restored.globalScale, 0.0000001f, 1e-10);
}

TEST_F(NumericBoundaryTest, GlobalScaleVeryLarge) {
    ModelImportPostProcessParameters params;
    params.globalScale = 999999.9f;

    nlohmann::json j = params;
    auto restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_FLOAT_EQ(restored.globalScale, 999999.9f);
}

// =============================================================================
// JSON Edge Cases Tests
// =============================================================================

class JsonEdgeCasesTest : public ::testing::Test {};

TEST_F(JsonEdgeCasesTest, MissingFieldsThrowsException) {
    // JSON with missing required fields should throw
    nlohmann::json j = {
        {"calculateTangentSpace", true},
        {"maxBones", 90}
        // All other fields missing
    };

    // The implementation requires all fields to be present
    EXPECT_THROW(j.get<ModelImportPostProcessParameters>(), nlohmann::json::out_of_range);
}

TEST_F(JsonEdgeCasesTest, ExtraFieldsIgnored) {
    nlohmann::json j = {
        {"calculateTangentSpace", true},
        {"joinIdenticalVertices", false},
        {"generateSmoothNormals", true},
        {"optimizeMeshes", false},
        {"maxBones", 45},
        {"importAnimations", true},
        {"importMaterials", true},
        {"importTextures", true},
        {"globalScale", 2.0f},
        {"textureQuality", "HIGH"},
        {"convertToUncompressed", true},
        {"unknownField1", "should be ignored"},
        {"unknownField2", 12345},
        {"unknownField3", true}
    };

    // Should not throw and should parse correctly
    ModelImportPostProcessParameters params = j.get<ModelImportPostProcessParameters>();

    EXPECT_TRUE(params.calculateTangentSpace);
    EXPECT_FALSE(params.joinIdenticalVertices);
    EXPECT_TRUE(params.generateSmoothNormals);
    EXPECT_FALSE(params.optimizeMeshes);
    EXPECT_EQ(params.maxBones, 45);
    EXPECT_TRUE(params.importAnimations);
    EXPECT_TRUE(params.importMaterials);
    EXPECT_TRUE(params.importTextures);
    EXPECT_FLOAT_EQ(params.globalScale, 2.0f);
    EXPECT_EQ(params.textureQuality, ModelImportPostProcessParameters::TextureQuality::HIGH);
    EXPECT_TRUE(params.convertToUncompressed);
}

TEST_F(JsonEdgeCasesTest, EmptyJsonForModelImportParametersThrows) {
    nlohmann::json j = nlohmann::json::object();

    // The implementation requires textureParameters field to be present
    EXPECT_THROW(j.get<ModelImportParameters>(), nlohmann::json::out_of_range);
}

TEST_F(JsonEdgeCasesTest, MinimalValidJson) {
    // Minimal JSON with only required structure
    nlohmann::json j = {
        {"textureParameters", nlohmann::json::array()}
    };

    ModelImportParameters params = j.get<ModelImportParameters>();
    EXPECT_TRUE(params.textureParameters.empty());
}

TEST_F(JsonEdgeCasesTest, AllFieldsExplicitlySetToDefaults) {
    nlohmann::json j = {
        {"calculateTangentSpace", false},
        {"joinIdenticalVertices", true},
        {"generateSmoothNormals", false},
        {"optimizeMeshes", true},
        {"maxBones", 60},
        {"importAnimations", true},
        {"importMaterials", true},
        {"importTextures", true},
        {"globalScale", 1.0f},
        {"textureQuality", "MEDIUM"},
        {"convertToUncompressed", false}
    };

    ModelImportPostProcessParameters params = j.get<ModelImportPostProcessParameters>();

    EXPECT_FALSE(params.calculateTangentSpace);
    EXPECT_TRUE(params.joinIdenticalVertices);
    EXPECT_FALSE(params.generateSmoothNormals);
    EXPECT_TRUE(params.optimizeMeshes);
    EXPECT_EQ(params.maxBones, 60);
    EXPECT_TRUE(params.importAnimations);
    EXPECT_TRUE(params.importMaterials);
    EXPECT_TRUE(params.importTextures);
    EXPECT_FLOAT_EQ(params.globalScale, 1.0f);
    EXPECT_EQ(params.textureQuality, ModelImportPostProcessParameters::TextureQuality::MEDIUM);
    EXPECT_FALSE(params.convertToUncompressed);
}

// =============================================================================
// Complete Round-Trip Tests
// =============================================================================

class CompleteRoundTripTest : public ::testing::Test {};

TEST_F(CompleteRoundTripTest, PostProcessParamsAllFieldsModified) {
    ModelImportPostProcessParameters original;
    original.calculateTangentSpace = true;
    original.joinIdenticalVertices = false;
    original.generateSmoothNormals = true;
    original.optimizeMeshes = false;
    original.maxBones = 123;
    original.importAnimations = false;
    original.importMaterials = false;
    original.importTextures = false;
    original.globalScale = 42.5f;
    original.textureQuality = ModelImportPostProcessParameters::TextureQuality::LOW;
    original.convertToUncompressed = true;

    nlohmann::json j = original;
    ModelImportPostProcessParameters restored = j.get<ModelImportPostProcessParameters>();

    EXPECT_EQ(original.calculateTangentSpace, restored.calculateTangentSpace);
    EXPECT_EQ(original.joinIdenticalVertices, restored.joinIdenticalVertices);
    EXPECT_EQ(original.generateSmoothNormals, restored.generateSmoothNormals);
    EXPECT_EQ(original.optimizeMeshes, restored.optimizeMeshes);
    EXPECT_EQ(original.maxBones, restored.maxBones);
    EXPECT_EQ(original.importAnimations, restored.importAnimations);
    EXPECT_EQ(original.importMaterials, restored.importMaterials);
    EXPECT_EQ(original.importTextures, restored.importTextures);
    EXPECT_FLOAT_EQ(original.globalScale, restored.globalScale);
    EXPECT_EQ(original.textureQuality, restored.textureQuality);
    EXPECT_EQ(original.convertToUncompressed, restored.convertToUncompressed);
}

TEST_F(CompleteRoundTripTest, ModelParamsWithComplexTextureSettings) {
    ModelImportParameters original;

    // Add multiple textures with different settings
    TextureImportParameters tex1;
    tex1.generateMipmaps = false;
    tex1.convertToSRGB = false;
    tex1.flipVertically = false;
    tex1.format = TextureImportParameters::Format::BC1;
    tex1.maxSize = 128;
    tex1.compressionQuality = 0.1f;

    TextureImportParameters tex2;
    tex2.generateMipmaps = true;
    tex2.convertToSRGB = true;
    tex2.flipVertically = true;
    tex2.format = TextureImportParameters::Format::RGBA;
    tex2.maxSize = 8192;
    tex2.compressionQuality = 1.0f;

    original.textureParameters.push_back(tex1);
    original.textureParameters.push_back(tex2);

    nlohmann::json j = original;
    ModelImportParameters restored = j.get<ModelImportParameters>();

    ASSERT_EQ(restored.textureParameters.size(), 2);

    EXPECT_EQ(original.textureParameters[0].generateMipmaps, restored.textureParameters[0].generateMipmaps);
    EXPECT_EQ(original.textureParameters[0].convertToSRGB, restored.textureParameters[0].convertToSRGB);
    EXPECT_EQ(original.textureParameters[0].flipVertically, restored.textureParameters[0].flipVertically);
    EXPECT_EQ(original.textureParameters[0].format, restored.textureParameters[0].format);
    EXPECT_EQ(original.textureParameters[0].maxSize, restored.textureParameters[0].maxSize);
    EXPECT_FLOAT_EQ(original.textureParameters[0].compressionQuality, restored.textureParameters[0].compressionQuality);

    EXPECT_EQ(original.textureParameters[1].generateMipmaps, restored.textureParameters[1].generateMipmaps);
    EXPECT_EQ(original.textureParameters[1].convertToSRGB, restored.textureParameters[1].convertToSRGB);
    EXPECT_EQ(original.textureParameters[1].flipVertically, restored.textureParameters[1].flipVertically);
    EXPECT_EQ(original.textureParameters[1].format, restored.textureParameters[1].format);
    EXPECT_EQ(original.textureParameters[1].maxSize, restored.textureParameters[1].maxSize);
    EXPECT_FLOAT_EQ(original.textureParameters[1].compressionQuality, restored.textureParameters[1].compressionQuality);
}

TEST_F(CompleteRoundTripTest, MultipleRoundTrips) {
    ModelImportPostProcessParameters original;
    original.calculateTangentSpace = true;
    original.maxBones = 75;
    original.globalScale = 3.14f;
    original.textureQuality = ModelImportPostProcessParameters::TextureQuality::HIGH;

    // First round trip
    nlohmann::json j1 = original;
    ModelImportPostProcessParameters restored1 = j1.get<ModelImportPostProcessParameters>();

    // Second round trip
    nlohmann::json j2 = restored1;
    ModelImportPostProcessParameters restored2 = j2.get<ModelImportPostProcessParameters>();

    // Third round trip
    nlohmann::json j3 = restored2;
    ModelImportPostProcessParameters restored3 = j3.get<ModelImportPostProcessParameters>();

    // All should be identical
    EXPECT_EQ(original.calculateTangentSpace, restored3.calculateTangentSpace);
    EXPECT_EQ(original.maxBones, restored3.maxBones);
    EXPECT_FLOAT_EQ(original.globalScale, restored3.globalScale);
    EXPECT_EQ(original.textureQuality, restored3.textureQuality);
}

// =============================================================================
// TextureQuality Enum Comprehensive Tests
// =============================================================================

class TextureQualityComprehensiveTest : public ::testing::Test {};

TEST_F(TextureQualityComprehensiveTest, AllEnumValuesToJson) {
    // Test LOW
    {
        ModelImportPostProcessParameters params;
        params.textureQuality = ModelImportPostProcessParameters::TextureQuality::LOW;
        nlohmann::json j = params;
        EXPECT_EQ(j["textureQuality"].get<std::string>(), "LOW");
    }

    // Test MEDIUM
    {
        ModelImportPostProcessParameters params;
        params.textureQuality = ModelImportPostProcessParameters::TextureQuality::MEDIUM;
        nlohmann::json j = params;
        EXPECT_EQ(j["textureQuality"].get<std::string>(), "MEDIUM");
    }

    // Test HIGH
    {
        ModelImportPostProcessParameters params;
        params.textureQuality = ModelImportPostProcessParameters::TextureQuality::HIGH;
        nlohmann::json j = params;
        EXPECT_EQ(j["textureQuality"].get<std::string>(), "HIGH");
    }
}

TEST_F(TextureQualityComprehensiveTest, AllEnumValuesFromJson) {
    // Base JSON with all required fields
    auto makeFullJson = [](const std::string& quality) {
        return nlohmann::json{
            {"calculateTangentSpace", false},
            {"joinIdenticalVertices", true},
            {"generateSmoothNormals", false},
            {"optimizeMeshes", true},
            {"maxBones", 60},
            {"importAnimations", true},
            {"importMaterials", true},
            {"importTextures", true},
            {"globalScale", 1.0f},
            {"textureQuality", quality},
            {"convertToUncompressed", false}
        };
    };

    // Test LOW
    {
        nlohmann::json j = makeFullJson("LOW");
        auto params = j.get<ModelImportPostProcessParameters>();
        EXPECT_EQ(params.textureQuality, ModelImportPostProcessParameters::TextureQuality::LOW);
    }

    // Test MEDIUM
    {
        nlohmann::json j = makeFullJson("MEDIUM");
        auto params = j.get<ModelImportPostProcessParameters>();
        EXPECT_EQ(params.textureQuality, ModelImportPostProcessParameters::TextureQuality::MEDIUM);
    }

    // Test HIGH
    {
        nlohmann::json j = makeFullJson("HIGH");
        auto params = j.get<ModelImportPostProcessParameters>();
        EXPECT_EQ(params.textureQuality, ModelImportPostProcessParameters::TextureQuality::HIGH);
    }
}

TEST_F(TextureQualityComprehensiveTest, EnumValueComparisons) {
    EXPECT_LT(static_cast<int>(ModelImportPostProcessParameters::TextureQuality::LOW),
              static_cast<int>(ModelImportPostProcessParameters::TextureQuality::MEDIUM));
    EXPECT_LT(static_cast<int>(ModelImportPostProcessParameters::TextureQuality::MEDIUM),
              static_cast<int>(ModelImportPostProcessParameters::TextureQuality::HIGH));
}

// =============================================================================
// Default Value Verification Tests
// =============================================================================

class DefaultValueVerificationTest : public ::testing::Test {};

TEST_F(DefaultValueVerificationTest, PostProcessAllDefaults) {
    ModelImportPostProcessParameters params;

    // Mesh processing
    EXPECT_FALSE(params.calculateTangentSpace);
    EXPECT_TRUE(params.joinIdenticalVertices);
    EXPECT_FALSE(params.generateSmoothNormals);
    EXPECT_TRUE(params.optimizeMeshes);
    EXPECT_EQ(params.maxBones, 60);

    // Scene options
    EXPECT_TRUE(params.importAnimations);
    EXPECT_TRUE(params.importMaterials);
    EXPECT_TRUE(params.importTextures);
    EXPECT_FLOAT_EQ(params.globalScale, 1.0f);

    // Texture options
    EXPECT_EQ(params.textureQuality, ModelImportPostProcessParameters::TextureQuality::MEDIUM);
    EXPECT_FALSE(params.convertToUncompressed);
}

TEST_F(DefaultValueVerificationTest, DefaultsMatchJsonRoundTrip) {
    // Create default params, serialize to JSON, then deserialize back
    ModelImportPostProcessParameters cppDefaults;

    nlohmann::json fullJson = cppDefaults;  // to_json
    ModelImportPostProcessParameters jsonDefaults = fullJson.get<ModelImportPostProcessParameters>();

    // Round-trip should preserve all default values
    EXPECT_EQ(cppDefaults.calculateTangentSpace, jsonDefaults.calculateTangentSpace);
    EXPECT_EQ(cppDefaults.joinIdenticalVertices, jsonDefaults.joinIdenticalVertices);
    EXPECT_EQ(cppDefaults.generateSmoothNormals, jsonDefaults.generateSmoothNormals);
    EXPECT_EQ(cppDefaults.optimizeMeshes, jsonDefaults.optimizeMeshes);
    EXPECT_EQ(cppDefaults.maxBones, jsonDefaults.maxBones);
    EXPECT_EQ(cppDefaults.importAnimations, jsonDefaults.importAnimations);
    EXPECT_EQ(cppDefaults.importMaterials, jsonDefaults.importMaterials);
    EXPECT_EQ(cppDefaults.importTextures, jsonDefaults.importTextures);
    EXPECT_FLOAT_EQ(cppDefaults.globalScale, jsonDefaults.globalScale);
    EXPECT_EQ(cppDefaults.textureQuality, jsonDefaults.textureQuality);
    EXPECT_EQ(cppDefaults.convertToUncompressed, jsonDefaults.convertToUncompressed);
}

TEST_F(DefaultValueVerificationTest, EmptyJsonThrows) {
    // The implementation requires all fields - empty JSON should throw
    nlohmann::json emptyJson = nlohmann::json::object();
    EXPECT_THROW(emptyJson.get<ModelImportPostProcessParameters>(), nlohmann::json::out_of_range);
}

}  // namespace nexo::assets
