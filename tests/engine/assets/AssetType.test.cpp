//// AssetType.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for AssetType enum and JSON serialization
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/Asset.hpp"

namespace nexo::assets {

// =============================================================================
// AssetType Enum Tests
// =============================================================================

class AssetTypeTest : public ::testing::Test {};

TEST_F(AssetTypeTest, EnumValuesExist) {
    EXPECT_EQ(static_cast<int>(AssetType::UNKNOWN), 0);
    EXPECT_EQ(static_cast<int>(AssetType::TEXTURE), 1);
    EXPECT_EQ(static_cast<int>(AssetType::MATERIAL), 2);
    EXPECT_EQ(static_cast<int>(AssetType::MODEL), 3);
    EXPECT_EQ(static_cast<int>(AssetType::SOUND), 4);
    EXPECT_EQ(static_cast<int>(AssetType::MUSIC), 5);
    EXPECT_EQ(static_cast<int>(AssetType::FONT), 6);
    EXPECT_EQ(static_cast<int>(AssetType::SHADER), 7);
    EXPECT_EQ(static_cast<int>(AssetType::SCRIPT), 8);
}

TEST_F(AssetTypeTest, CountMatchesEnumValues) {
    EXPECT_EQ(static_cast<int>(AssetType::_COUNT), 9);
}

// =============================================================================
// getAssetTypeName Tests
// =============================================================================

class GetAssetTypeNameTest : public ::testing::Test {};

TEST_F(GetAssetTypeNameTest, UnknownType) {
    EXPECT_STREQ(getAssetTypeName(AssetType::UNKNOWN), "UNKNOWN");
}

TEST_F(GetAssetTypeNameTest, TextureType) {
    EXPECT_STREQ(getAssetTypeName(AssetType::TEXTURE), "TEXTURE");
}

TEST_F(GetAssetTypeNameTest, MaterialType) {
    EXPECT_STREQ(getAssetTypeName(AssetType::MATERIAL), "MATERIAL");
}

TEST_F(GetAssetTypeNameTest, ModelType) {
    EXPECT_STREQ(getAssetTypeName(AssetType::MODEL), "MODEL");
}

TEST_F(GetAssetTypeNameTest, SoundType) {
    EXPECT_STREQ(getAssetTypeName(AssetType::SOUND), "SOUND");
}

TEST_F(GetAssetTypeNameTest, MusicType) {
    EXPECT_STREQ(getAssetTypeName(AssetType::MUSIC), "MUSIC");
}

TEST_F(GetAssetTypeNameTest, FontType) {
    EXPECT_STREQ(getAssetTypeName(AssetType::FONT), "FONT");
}

TEST_F(GetAssetTypeNameTest, ShaderType) {
    EXPECT_STREQ(getAssetTypeName(AssetType::SHADER), "SHADER");
}

TEST_F(GetAssetTypeNameTest, ScriptType) {
    EXPECT_STREQ(getAssetTypeName(AssetType::SCRIPT), "SCRIPT");
}

// =============================================================================
// JSON Serialization Tests (to_json)
// =============================================================================

class AssetTypeToJsonTest : public ::testing::Test {};

TEST_F(AssetTypeToJsonTest, UnknownToJson) {
    nlohmann::json j;
    to_json(j, AssetType::UNKNOWN);
    EXPECT_EQ(j, "UNKNOWN");
}

TEST_F(AssetTypeToJsonTest, TextureToJson) {
    nlohmann::json j;
    to_json(j, AssetType::TEXTURE);
    EXPECT_EQ(j, "TEXTURE");
}

TEST_F(AssetTypeToJsonTest, MaterialToJson) {
    nlohmann::json j;
    to_json(j, AssetType::MATERIAL);
    EXPECT_EQ(j, "MATERIAL");
}

TEST_F(AssetTypeToJsonTest, ModelToJson) {
    nlohmann::json j;
    to_json(j, AssetType::MODEL);
    EXPECT_EQ(j, "MODEL");
}

TEST_F(AssetTypeToJsonTest, SoundToJson) {
    nlohmann::json j;
    to_json(j, AssetType::SOUND);
    EXPECT_EQ(j, "SOUND");
}

TEST_F(AssetTypeToJsonTest, MusicToJson) {
    nlohmann::json j;
    to_json(j, AssetType::MUSIC);
    EXPECT_EQ(j, "MUSIC");
}

TEST_F(AssetTypeToJsonTest, FontToJson) {
    nlohmann::json j;
    to_json(j, AssetType::FONT);
    EXPECT_EQ(j, "FONT");
}

TEST_F(AssetTypeToJsonTest, ShaderToJson) {
    nlohmann::json j;
    to_json(j, AssetType::SHADER);
    EXPECT_EQ(j, "SHADER");
}

TEST_F(AssetTypeToJsonTest, ScriptToJson) {
    nlohmann::json j;
    to_json(j, AssetType::SCRIPT);
    EXPECT_EQ(j, "SCRIPT");
}

// =============================================================================
// JSON Deserialization Tests (from_json)
// =============================================================================

class AssetTypeFromJsonTest : public ::testing::Test {};

TEST_F(AssetTypeFromJsonTest, UnknownFromJson) {
    nlohmann::json j = "UNKNOWN";
    AssetType type;
    from_json(j, type);
    EXPECT_EQ(type, AssetType::UNKNOWN);
}

TEST_F(AssetTypeFromJsonTest, TextureFromJson) {
    nlohmann::json j = "TEXTURE";
    AssetType type;
    from_json(j, type);
    EXPECT_EQ(type, AssetType::TEXTURE);
}

TEST_F(AssetTypeFromJsonTest, MaterialFromJson) {
    nlohmann::json j = "MATERIAL";
    AssetType type;
    from_json(j, type);
    EXPECT_EQ(type, AssetType::MATERIAL);
}

TEST_F(AssetTypeFromJsonTest, ModelFromJson) {
    nlohmann::json j = "MODEL";
    AssetType type;
    from_json(j, type);
    EXPECT_EQ(type, AssetType::MODEL);
}

TEST_F(AssetTypeFromJsonTest, SoundFromJson) {
    nlohmann::json j = "SOUND";
    AssetType type;
    from_json(j, type);
    EXPECT_EQ(type, AssetType::SOUND);
}

TEST_F(AssetTypeFromJsonTest, MusicFromJson) {
    nlohmann::json j = "MUSIC";
    AssetType type;
    from_json(j, type);
    EXPECT_EQ(type, AssetType::MUSIC);
}

TEST_F(AssetTypeFromJsonTest, FontFromJson) {
    nlohmann::json j = "FONT";
    AssetType type;
    from_json(j, type);
    EXPECT_EQ(type, AssetType::FONT);
}

TEST_F(AssetTypeFromJsonTest, ShaderFromJson) {
    nlohmann::json j = "SHADER";
    AssetType type;
    from_json(j, type);
    EXPECT_EQ(type, AssetType::SHADER);
}

TEST_F(AssetTypeFromJsonTest, ScriptFromJson) {
    nlohmann::json j = "SCRIPT";
    AssetType type;
    from_json(j, type);
    EXPECT_EQ(type, AssetType::SCRIPT);
}

TEST_F(AssetTypeFromJsonTest, InvalidStringReturnsUnknown) {
    nlohmann::json j = "INVALID_TYPE";
    AssetType type = AssetType::TEXTURE;  // Set to non-unknown to verify change
    from_json(j, type);
    EXPECT_EQ(type, AssetType::UNKNOWN);
}

TEST_F(AssetTypeFromJsonTest, EmptyStringReturnsUnknown) {
    nlohmann::json j = "";
    AssetType type = AssetType::MODEL;
    from_json(j, type);
    EXPECT_EQ(type, AssetType::UNKNOWN);
}

TEST_F(AssetTypeFromJsonTest, LowercaseReturnsUnknown) {
    // from_json is case-sensitive
    nlohmann::json j = "texture";
    AssetType type = AssetType::MODEL;
    from_json(j, type);
    EXPECT_EQ(type, AssetType::UNKNOWN);
}

// =============================================================================
// Round-trip Serialization Tests
// =============================================================================

class AssetTypeRoundTripTest : public ::testing::Test {};

TEST_F(AssetTypeRoundTripTest, AllTypesRoundTrip) {
    AssetType types[] = {
        AssetType::UNKNOWN,
        AssetType::TEXTURE,
        AssetType::MATERIAL,
        AssetType::MODEL,
        AssetType::SOUND,
        AssetType::MUSIC,
        AssetType::FONT,
        AssetType::SHADER,
        AssetType::SCRIPT
    };

    for (AssetType original : types) {
        nlohmann::json j;
        to_json(j, original);

        AssetType restored;
        from_json(j, restored);

        EXPECT_EQ(original, restored) << "Round-trip failed for type: " << getAssetTypeName(original);
    }
}

// =============================================================================
// AssetStatus Enum Tests
// =============================================================================

class AssetStatusTest : public ::testing::Test {};

TEST_F(AssetStatusTest, EnumValuesExist) {
    EXPECT_EQ(static_cast<int>(AssetStatus::UNLOADED), 0);
    EXPECT_EQ(static_cast<int>(AssetStatus::LOADED), 1);
    EXPECT_EQ(static_cast<int>(AssetStatus::ERROR), 2);
}

}  // namespace nexo::assets
