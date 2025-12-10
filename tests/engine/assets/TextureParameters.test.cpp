//// TextureParameters.test.cpp ///////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for TextureImportParameters JSON serialization
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/Assets/Texture/TextureParameters.hpp"

namespace nexo::assets {

// =============================================================================
// TextureImportParameters Default Values Tests
// =============================================================================

class TextureImportParametersDefaultsTest : public ::testing::Test {};

TEST_F(TextureImportParametersDefaultsTest, GenerateMipmapsDefaultTrue) {
    TextureImportParameters params;
    EXPECT_TRUE(params.generateMipmaps);
}

TEST_F(TextureImportParametersDefaultsTest, ConvertToSRGBDefaultTrue) {
    TextureImportParameters params;
    EXPECT_TRUE(params.convertToSRGB);
}

TEST_F(TextureImportParametersDefaultsTest, FlipVerticallyDefaultTrue) {
    TextureImportParameters params;
    EXPECT_TRUE(params.flipVertically);
}

TEST_F(TextureImportParametersDefaultsTest, FormatDefaultPreserve) {
    TextureImportParameters params;
    EXPECT_EQ(params.format, TextureImportParameters::Format::Preserve);
}

TEST_F(TextureImportParametersDefaultsTest, MaxSizeDefault4096) {
    TextureImportParameters params;
    EXPECT_EQ(params.maxSize, 4096);
}

TEST_F(TextureImportParametersDefaultsTest, CompressionQualityDefault0_9) {
    TextureImportParameters params;
    EXPECT_FLOAT_EQ(params.compressionQuality, 0.9f);
}

// =============================================================================
// TextureImportParameters Format Enum Tests
// =============================================================================

class TextureFormatEnumTest : public ::testing::Test {};

TEST_F(TextureFormatEnumTest, FormatEnumValuesExist) {
    EXPECT_EQ(static_cast<int>(TextureImportParameters::Format::Preserve), 0);
    EXPECT_EQ(static_cast<int>(TextureImportParameters::Format::RGB), 1);
    EXPECT_EQ(static_cast<int>(TextureImportParameters::Format::RGBA), 2);
    EXPECT_EQ(static_cast<int>(TextureImportParameters::Format::BC1), 3);
    EXPECT_EQ(static_cast<int>(TextureImportParameters::Format::BC3), 4);
    EXPECT_EQ(static_cast<int>(TextureImportParameters::Format::BC7), 5);
}

// =============================================================================
// TextureImportParameters JSON Serialization Tests
// =============================================================================

class TextureParametersJsonTest : public ::testing::Test {};

TEST_F(TextureParametersJsonTest, DefaultValuesToJson) {
    TextureImportParameters params;
    nlohmann::json j = params;

    EXPECT_TRUE(j["generateMipmaps"].get<bool>());
    EXPECT_TRUE(j["convertToSRGB"].get<bool>());
    EXPECT_TRUE(j["flipVertically"].get<bool>());
    EXPECT_EQ(j["maxSize"].get<int>(), 4096);
    EXPECT_FLOAT_EQ(j["compressionQuality"].get<float>(), 0.9f);
}

TEST_F(TextureParametersJsonTest, CustomValuesToJson) {
    TextureImportParameters params;
    params.generateMipmaps = false;
    params.convertToSRGB = false;
    params.flipVertically = false;
    params.format = TextureImportParameters::Format::RGBA;
    params.maxSize = 2048;
    params.compressionQuality = 0.5f;

    nlohmann::json j = params;

    EXPECT_FALSE(j["generateMipmaps"].get<bool>());
    EXPECT_FALSE(j["convertToSRGB"].get<bool>());
    EXPECT_FALSE(j["flipVertically"].get<bool>());
    EXPECT_EQ(j["maxSize"].get<int>(), 2048);
    EXPECT_FLOAT_EQ(j["compressionQuality"].get<float>(), 0.5f);
}

TEST_F(TextureParametersJsonTest, FromJsonRestoresValues) {
    nlohmann::json j = {
        {"generateMipmaps", false},
        {"convertToSRGB", false},
        {"flipVertically", false},
        {"format", 2},  // RGBA
        {"maxSize", 1024},
        {"compressionQuality", 0.75f}
    };

    TextureImportParameters params = j.get<TextureImportParameters>();

    EXPECT_FALSE(params.generateMipmaps);
    EXPECT_FALSE(params.convertToSRGB);
    EXPECT_FALSE(params.flipVertically);
    EXPECT_EQ(params.format, TextureImportParameters::Format::RGBA);
    EXPECT_EQ(params.maxSize, 1024);
    EXPECT_FLOAT_EQ(params.compressionQuality, 0.75f);
}

TEST_F(TextureParametersJsonTest, RoundTripSerialization) {
    TextureImportParameters original;
    original.generateMipmaps = false;
    original.convertToSRGB = true;
    original.flipVertically = false;
    original.format = TextureImportParameters::Format::BC7;
    original.maxSize = 512;
    original.compressionQuality = 0.95f;

    nlohmann::json j = original;
    TextureImportParameters restored = j.get<TextureImportParameters>();

    EXPECT_EQ(original.generateMipmaps, restored.generateMipmaps);
    EXPECT_EQ(original.convertToSRGB, restored.convertToSRGB);
    EXPECT_EQ(original.flipVertically, restored.flipVertically);
    EXPECT_EQ(original.format, restored.format);
    EXPECT_EQ(original.maxSize, restored.maxSize);
    EXPECT_FLOAT_EQ(original.compressionQuality, restored.compressionQuality);
}

TEST_F(TextureParametersJsonTest, AllFormatsRoundTrip) {
    TextureImportParameters::Format formats[] = {
        TextureImportParameters::Format::Preserve,
        TextureImportParameters::Format::RGB,
        TextureImportParameters::Format::RGBA,
        TextureImportParameters::Format::BC1,
        TextureImportParameters::Format::BC3,
        TextureImportParameters::Format::BC7
    };

    for (auto format : formats) {
        TextureImportParameters original;
        original.format = format;

        nlohmann::json j = original;
        TextureImportParameters restored = j.get<TextureImportParameters>();

        EXPECT_EQ(original.format, restored.format)
            << "Round-trip failed for format: " << static_cast<int>(format);
    }
}

TEST_F(TextureParametersJsonTest, MaxSizeBoundaries) {
    TextureImportParameters params;

    // Test small size
    params.maxSize = 64;
    nlohmann::json j1 = params;
    auto restored1 = j1.get<TextureImportParameters>();
    EXPECT_EQ(restored1.maxSize, 64);

    // Test large size
    params.maxSize = 16384;
    nlohmann::json j2 = params;
    auto restored2 = j2.get<TextureImportParameters>();
    EXPECT_EQ(restored2.maxSize, 16384);
}

TEST_F(TextureParametersJsonTest, CompressionQualityBoundaries) {
    TextureImportParameters params;

    // Test minimum
    params.compressionQuality = 0.0f;
    nlohmann::json j1 = params;
    auto restored1 = j1.get<TextureImportParameters>();
    EXPECT_FLOAT_EQ(restored1.compressionQuality, 0.0f);

    // Test maximum
    params.compressionQuality = 1.0f;
    nlohmann::json j2 = params;
    auto restored2 = j2.get<TextureImportParameters>();
    EXPECT_FLOAT_EQ(restored2.compressionQuality, 1.0f);
}

}  // namespace nexo::assets
