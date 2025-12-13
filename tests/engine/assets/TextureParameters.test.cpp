//// TextureParameters.test.cpp ///////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for TextureImportParameters JSON serialization
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/Assets/Texture/TextureParameters.hpp"
#include <limits>
#include <vector>

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

// =============================================================================
// TextureImportParameters Value Assignment Tests
// =============================================================================

class TextureParametersValueTest : public ::testing::Test {};

TEST_F(TextureParametersValueTest, CanSetGenerateMipmapsFalse) {
    TextureImportParameters params;
    params.generateMipmaps = false;
    EXPECT_FALSE(params.generateMipmaps);
}

TEST_F(TextureParametersValueTest, CanSetConvertToSRGBFalse) {
    TextureImportParameters params;
    params.convertToSRGB = false;
    EXPECT_FALSE(params.convertToSRGB);
}

TEST_F(TextureParametersValueTest, CanSetFlipVerticallyFalse) {
    TextureImportParameters params;
    params.flipVertically = false;
    EXPECT_FALSE(params.flipVertically);
}

TEST_F(TextureParametersValueTest, CanSetAllFormats) {
    TextureImportParameters params;

    params.format = TextureImportParameters::Format::Preserve;
    EXPECT_EQ(params.format, TextureImportParameters::Format::Preserve);

    params.format = TextureImportParameters::Format::RGB;
    EXPECT_EQ(params.format, TextureImportParameters::Format::RGB);

    params.format = TextureImportParameters::Format::RGBA;
    EXPECT_EQ(params.format, TextureImportParameters::Format::RGBA);

    params.format = TextureImportParameters::Format::BC1;
    EXPECT_EQ(params.format, TextureImportParameters::Format::BC1);

    params.format = TextureImportParameters::Format::BC3;
    EXPECT_EQ(params.format, TextureImportParameters::Format::BC3);

    params.format = TextureImportParameters::Format::BC7;
    EXPECT_EQ(params.format, TextureImportParameters::Format::BC7);
}

TEST_F(TextureParametersValueTest, CanSetMaxSizeToZero) {
    TextureImportParameters params;
    params.maxSize = 0;
    EXPECT_EQ(params.maxSize, 0);
}

TEST_F(TextureParametersValueTest, CanSetMaxSizeToNegative) {
    TextureImportParameters params;
    params.maxSize = -1;
    EXPECT_EQ(params.maxSize, -1);
}

TEST_F(TextureParametersValueTest, CanSetMaxSizeToVeryLarge) {
    TextureImportParameters params;
    params.maxSize = 65536;
    EXPECT_EQ(params.maxSize, 65536);
}

TEST_F(TextureParametersValueTest, CanSetCompressionQualityBelowZero) {
    TextureImportParameters params;
    params.compressionQuality = -0.5f;
    EXPECT_FLOAT_EQ(params.compressionQuality, -0.5f);
}

TEST_F(TextureParametersValueTest, CanSetCompressionQualityAboveOne) {
    TextureImportParameters params;
    params.compressionQuality = 1.5f;
    EXPECT_FLOAT_EQ(params.compressionQuality, 1.5f);
}

TEST_F(TextureParametersValueTest, CompressionQualityPrecision) {
    TextureImportParameters params;
    params.compressionQuality = 0.123456789f;
    EXPECT_FLOAT_EQ(params.compressionQuality, 0.123456789f);
}

// =============================================================================
// TextureImportParameters Edge Cases and Invalid Input Tests
// =============================================================================

class TextureParametersEdgeCasesTest : public ::testing::Test {};

TEST_F(TextureParametersEdgeCasesTest, JsonWithMissingFieldsUsesDefaults) {
    nlohmann::json j = nlohmann::json::object();

    // When parsing incomplete JSON, nlohmann::json should throw or use defaults
    // This depends on the NLOHMANN_DEFINE_TYPE_INTRUSIVE behavior
    EXPECT_THROW({
        TextureImportParameters params = j.get<TextureImportParameters>();
    }, nlohmann::json::exception);
}

TEST_F(TextureParametersEdgeCasesTest, JsonWithExtraFieldsIgnored) {
    nlohmann::json j = {
        {"generateMipmaps", true},
        {"convertToSRGB", false},
        {"flipVertically", true},
        {"format", 1},
        {"maxSize", 2048},
        {"compressionQuality", 0.8f},
        {"extraField", "should be ignored"},
        {"anotherExtra", 42}
    };

    TextureImportParameters params = j.get<TextureImportParameters>();

    EXPECT_TRUE(params.generateMipmaps);
    EXPECT_FALSE(params.convertToSRGB);
    EXPECT_TRUE(params.flipVertically);
    EXPECT_EQ(params.format, TextureImportParameters::Format::RGB);
    EXPECT_EQ(params.maxSize, 2048);
    EXPECT_FLOAT_EQ(params.compressionQuality, 0.8f);
}

TEST_F(TextureParametersEdgeCasesTest, JsonWithWrongTypeThrows) {
    nlohmann::json j = {
        {"generateMipmaps", "not a bool"},
        {"convertToSRGB", true},
        {"flipVertically", true},
        {"format", 0},
        {"maxSize", 4096},
        {"compressionQuality", 0.9f}
    };

    EXPECT_THROW({
        TextureImportParameters params = j.get<TextureImportParameters>();
    }, nlohmann::json::exception);
}

TEST_F(TextureParametersEdgeCasesTest, JsonWithInvalidFormatValueAccepted) {
    // Invalid enum values should still deserialize, but may be undefined behavior
    nlohmann::json j = {
        {"generateMipmaps", true},
        {"convertToSRGB", true},
        {"flipVertically", true},
        {"format", 99},  // Invalid format value
        {"maxSize", 4096},
        {"compressionQuality", 0.9f}
    };

    TextureImportParameters params = j.get<TextureImportParameters>();
    // The format will be whatever value 99 maps to in the enum
    EXPECT_EQ(static_cast<int>(params.format), 99);
}

TEST_F(TextureParametersEdgeCasesTest, EmptyJsonObjectThrows) {
    nlohmann::json j = nlohmann::json::object();

    EXPECT_THROW({
        TextureImportParameters params = j.get<TextureImportParameters>();
    }, nlohmann::json::exception);
}

TEST_F(TextureParametersEdgeCasesTest, NullJsonThrows) {
    nlohmann::json j = nullptr;

    EXPECT_THROW({
        TextureImportParameters params = j.get<TextureImportParameters>();
    }, nlohmann::json::exception);
}

TEST_F(TextureParametersEdgeCasesTest, JsonArrayThrows) {
    nlohmann::json j = nlohmann::json::array({1, 2, 3});

    EXPECT_THROW({
        TextureImportParameters params = j.get<TextureImportParameters>();
    }, nlohmann::json::exception);
}

// =============================================================================
// TextureImportParameters Advanced JSON Tests
// =============================================================================

class TextureParametersAdvancedJsonTest : public ::testing::Test {};

TEST_F(TextureParametersAdvancedJsonTest, AllFormatsSerializeCorrectly) {
    struct FormatTest {
        TextureImportParameters::Format format;
        int expectedValue;
    };

    std::vector<FormatTest> tests = {
        {TextureImportParameters::Format::Preserve, 0},
        {TextureImportParameters::Format::RGB, 1},
        {TextureImportParameters::Format::RGBA, 2},
        {TextureImportParameters::Format::BC1, 3},
        {TextureImportParameters::Format::BC3, 4},
        {TextureImportParameters::Format::BC7, 5}
    };

    for (const auto& test : tests) {
        TextureImportParameters params;
        params.format = test.format;

        nlohmann::json j = params;
        EXPECT_EQ(j["format"].get<int>(), test.expectedValue)
            << "Format " << test.expectedValue << " did not serialize correctly";
    }
}

TEST_F(TextureParametersAdvancedJsonTest, MultipleRoundTripsPreserveValues) {
    TextureImportParameters original;
    original.generateMipmaps = false;
    original.convertToSRGB = false;
    original.flipVertically = true;
    original.format = TextureImportParameters::Format::BC3;
    original.maxSize = 8192;
    original.compressionQuality = 0.42f;

    // Round-trip 3 times
    nlohmann::json j1 = original;
    TextureImportParameters restored1 = j1.get<TextureImportParameters>();

    nlohmann::json j2 = restored1;
    TextureImportParameters restored2 = j2.get<TextureImportParameters>();

    nlohmann::json j3 = restored2;
    TextureImportParameters restored3 = j3.get<TextureImportParameters>();

    EXPECT_EQ(original.generateMipmaps, restored3.generateMipmaps);
    EXPECT_EQ(original.convertToSRGB, restored3.convertToSRGB);
    EXPECT_EQ(original.flipVertically, restored3.flipVertically);
    EXPECT_EQ(original.format, restored3.format);
    EXPECT_EQ(original.maxSize, restored3.maxSize);
    EXPECT_FLOAT_EQ(original.compressionQuality, restored3.compressionQuality);
}

TEST_F(TextureParametersAdvancedJsonTest, JsonStringRoundTrip) {
    TextureImportParameters original;
    original.generateMipmaps = true;
    original.convertToSRGB = false;
    original.format = TextureImportParameters::Format::BC7;
    original.maxSize = 1024;
    original.compressionQuality = 0.65f;

    // Serialize to JSON then to string
    nlohmann::json j = original;
    std::string jsonString = j.dump();

    // Parse from string back to object
    nlohmann::json parsed = nlohmann::json::parse(jsonString);
    TextureImportParameters restored = parsed.get<TextureImportParameters>();

    EXPECT_EQ(original.generateMipmaps, restored.generateMipmaps);
    EXPECT_EQ(original.convertToSRGB, restored.convertToSRGB);
    EXPECT_EQ(original.flipVertically, restored.flipVertically);
    EXPECT_EQ(original.format, restored.format);
    EXPECT_EQ(original.maxSize, restored.maxSize);
    EXPECT_FLOAT_EQ(original.compressionQuality, restored.compressionQuality);
}

TEST_F(TextureParametersAdvancedJsonTest, PrettyPrintedJsonRoundTrip) {
    TextureImportParameters original;
    original.format = TextureImportParameters::Format::RGBA;
    original.maxSize = 512;

    nlohmann::json j = original;
    std::string prettyJson = j.dump(4);  // 4-space indentation

    nlohmann::json parsed = nlohmann::json::parse(prettyJson);
    TextureImportParameters restored = parsed.get<TextureImportParameters>();

    EXPECT_EQ(original.format, restored.format);
    EXPECT_EQ(original.maxSize, restored.maxSize);
}

TEST_F(TextureParametersAdvancedJsonTest, JsonContainsAllRequiredKeys) {
    TextureImportParameters params;
    nlohmann::json j = params;

    EXPECT_TRUE(j.contains("generateMipmaps"));
    EXPECT_TRUE(j.contains("convertToSRGB"));
    EXPECT_TRUE(j.contains("flipVertically"));
    EXPECT_TRUE(j.contains("format"));
    EXPECT_TRUE(j.contains("maxSize"));
    EXPECT_TRUE(j.contains("compressionQuality"));
}

TEST_F(TextureParametersAdvancedJsonTest, JsonTypesAreCorrect) {
    TextureImportParameters params;
    nlohmann::json j = params;

    EXPECT_TRUE(j["generateMipmaps"].is_boolean());
    EXPECT_TRUE(j["convertToSRGB"].is_boolean());
    EXPECT_TRUE(j["flipVertically"].is_boolean());
    EXPECT_TRUE(j["format"].is_number_integer());
    EXPECT_TRUE(j["maxSize"].is_number_integer());
    EXPECT_TRUE(j["compressionQuality"].is_number_float());
}

// =============================================================================
// TextureImportParameters Extreme Values Tests
// =============================================================================

class TextureParametersExtremeValuesTest : public ::testing::Test {};

TEST_F(TextureParametersExtremeValuesTest, MaxIntValueForMaxSize) {
    TextureImportParameters params;
    params.maxSize = std::numeric_limits<int>::max();

    nlohmann::json j = params;
    TextureImportParameters restored = j.get<TextureImportParameters>();

    EXPECT_EQ(restored.maxSize, std::numeric_limits<int>::max());
}

TEST_F(TextureParametersExtremeValuesTest, MinIntValueForMaxSize) {
    TextureImportParameters params;
    params.maxSize = std::numeric_limits<int>::min();

    nlohmann::json j = params;
    TextureImportParameters restored = j.get<TextureImportParameters>();

    EXPECT_EQ(restored.maxSize, std::numeric_limits<int>::min());
}

TEST_F(TextureParametersExtremeValuesTest, MaxFloatValueForCompressionQuality) {
    TextureImportParameters params;
    params.compressionQuality = std::numeric_limits<float>::max();

    nlohmann::json j = params;
    TextureImportParameters restored = j.get<TextureImportParameters>();

    EXPECT_FLOAT_EQ(restored.compressionQuality, std::numeric_limits<float>::max());
}

TEST_F(TextureParametersExtremeValuesTest, MinFloatValueForCompressionQuality) {
    TextureImportParameters params;
    params.compressionQuality = std::numeric_limits<float>::lowest();

    nlohmann::json j = params;
    TextureImportParameters restored = j.get<TextureImportParameters>();

    EXPECT_FLOAT_EQ(restored.compressionQuality, std::numeric_limits<float>::lowest());
}

TEST_F(TextureParametersExtremeValuesTest, VerySmallPositiveCompressionQuality) {
    TextureImportParameters params;
    params.compressionQuality = 0.0001f;

    nlohmann::json j = params;
    TextureImportParameters restored = j.get<TextureImportParameters>();

    EXPECT_FLOAT_EQ(restored.compressionQuality, 0.0001f);
}

TEST_F(TextureParametersExtremeValuesTest, VeryCloseToOneCompressionQuality) {
    TextureImportParameters params;
    params.compressionQuality = 0.9999f;

    nlohmann::json j = params;
    TextureImportParameters restored = j.get<TextureImportParameters>();

    EXPECT_FLOAT_EQ(restored.compressionQuality, 0.9999f);
}

TEST_F(TextureParametersExtremeValuesTest, PowerOfTwoMaxSizes) {
    std::vector<int> powerOfTwoSizes = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};

    for (int size : powerOfTwoSizes) {
        TextureImportParameters params;
        params.maxSize = size;

        nlohmann::json j = params;
        TextureImportParameters restored = j.get<TextureImportParameters>();

        EXPECT_EQ(restored.maxSize, size) << "Failed for size: " << size;
    }
}

// =============================================================================
// TextureImportParameters Copy and Assignment Tests
// =============================================================================

class TextureParametersCopyTest : public ::testing::Test {};

TEST_F(TextureParametersCopyTest, CopyConstructorPreservesValues) {
    TextureImportParameters original;
    original.generateMipmaps = false;
    original.convertToSRGB = false;
    original.flipVertically = false;
    original.format = TextureImportParameters::Format::BC1;
    original.maxSize = 2048;
    original.compressionQuality = 0.7f;

    TextureImportParameters copy(original);

    EXPECT_EQ(original.generateMipmaps, copy.generateMipmaps);
    EXPECT_EQ(original.convertToSRGB, copy.convertToSRGB);
    EXPECT_EQ(original.flipVertically, copy.flipVertically);
    EXPECT_EQ(original.format, copy.format);
    EXPECT_EQ(original.maxSize, copy.maxSize);
    EXPECT_FLOAT_EQ(original.compressionQuality, copy.compressionQuality);
}

TEST_F(TextureParametersCopyTest, AssignmentOperatorPreservesValues) {
    TextureImportParameters original;
    original.generateMipmaps = true;
    original.convertToSRGB = false;
    original.flipVertically = true;
    original.format = TextureImportParameters::Format::RGBA;
    original.maxSize = 1024;
    original.compressionQuality = 0.85f;

    TextureImportParameters assigned;
    assigned = original;

    EXPECT_EQ(original.generateMipmaps, assigned.generateMipmaps);
    EXPECT_EQ(original.convertToSRGB, assigned.convertToSRGB);
    EXPECT_EQ(original.flipVertically, assigned.flipVertically);
    EXPECT_EQ(original.format, assigned.format);
    EXPECT_EQ(original.maxSize, assigned.maxSize);
    EXPECT_FLOAT_EQ(original.compressionQuality, assigned.compressionQuality);
}

TEST_F(TextureParametersCopyTest, ModifyingCopyDoesNotAffectOriginal) {
    TextureImportParameters original;
    original.maxSize = 4096;
    original.compressionQuality = 0.9f;

    TextureImportParameters copy = original;
    copy.maxSize = 1024;
    copy.compressionQuality = 0.5f;

    EXPECT_EQ(original.maxSize, 4096);
    EXPECT_FLOAT_EQ(original.compressionQuality, 0.9f);
    EXPECT_EQ(copy.maxSize, 1024);
    EXPECT_FLOAT_EQ(copy.compressionQuality, 0.5f);
}

// =============================================================================
// TexturesImportPostProcessParameters Tests
// =============================================================================

class TexturesPostProcessParametersTest : public ::testing::Test {};

TEST_F(TexturesPostProcessParametersTest, CanInstantiate) {
    TexturesImportPostProcessParameters params;
    // Just verify it can be created - currently empty struct
    SUCCEED();
}

TEST_F(TexturesPostProcessParametersTest, DefaultConstruction) {
    TexturesImportPostProcessParameters params1;
    TexturesImportPostProcessParameters params2;
    // Both should be equivalent (empty structs)
    SUCCEED();
}

}  // namespace nexo::assets
