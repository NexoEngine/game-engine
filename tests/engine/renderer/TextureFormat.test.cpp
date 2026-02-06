//// TextureFormat.test.cpp ////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for NxTextureFormat enum and conversion functions
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/Texture.hpp"

namespace nexo::renderer {

// =============================================================================
// NxTextureFormat Enum Tests
// =============================================================================

class NxTextureFormatEnumTest : public ::testing::Test {};

TEST_F(NxTextureFormatEnumTest, InvalidValueIsZero) {
    EXPECT_EQ(static_cast<int>(NxTextureFormat::INVALID), 0);
}

TEST_F(NxTextureFormatEnumTest, R8ValueIs1) {
    EXPECT_EQ(static_cast<int>(NxTextureFormat::R8), 1);
}

TEST_F(NxTextureFormatEnumTest, RG8ValueIs2) {
    EXPECT_EQ(static_cast<int>(NxTextureFormat::RG8), 2);
}

TEST_F(NxTextureFormatEnumTest, RGB8ValueIs3) {
    EXPECT_EQ(static_cast<int>(NxTextureFormat::RGB8), 3);
}

TEST_F(NxTextureFormatEnumTest, RGBA8ValueIs4) {
    EXPECT_EQ(static_cast<int>(NxTextureFormat::RGBA8), 4);
}

TEST_F(NxTextureFormatEnumTest, NbFormatsValueIs5) {
    EXPECT_EQ(static_cast<int>(NxTextureFormat::_NB_FORMATS_), 5);
}

TEST_F(NxTextureFormatEnumTest, AllFormatsDistinct) {
    EXPECT_NE(NxTextureFormat::INVALID, NxTextureFormat::R8);
    EXPECT_NE(NxTextureFormat::R8, NxTextureFormat::RG8);
    EXPECT_NE(NxTextureFormat::RG8, NxTextureFormat::RGB8);
    EXPECT_NE(NxTextureFormat::RGB8, NxTextureFormat::RGBA8);
}

// =============================================================================
// NxTextureFormatToString Tests
// =============================================================================

class TextureFormatToStringTest : public ::testing::Test {};

TEST_F(TextureFormatToStringTest, InvalidReturnsInvalid) {
    EXPECT_EQ(NxTextureFormatToString(NxTextureFormat::INVALID), "INVALID");
}

TEST_F(TextureFormatToStringTest, R8ReturnsR8) {
    EXPECT_EQ(NxTextureFormatToString(NxTextureFormat::R8), "R8");
}

TEST_F(TextureFormatToStringTest, RG8ReturnsRG8) {
    EXPECT_EQ(NxTextureFormatToString(NxTextureFormat::RG8), "RG8");
}

TEST_F(TextureFormatToStringTest, RGB8ReturnsRGB8) {
    EXPECT_EQ(NxTextureFormatToString(NxTextureFormat::RGB8), "RGB8");
}

TEST_F(TextureFormatToStringTest, RGBA8ReturnsRGBA8) {
    EXPECT_EQ(NxTextureFormatToString(NxTextureFormat::RGBA8), "RGBA8");
}

TEST_F(TextureFormatToStringTest, UnknownReturnsInvalid) {
    // Cast an invalid value to test default case
    auto unknownFormat = static_cast<NxTextureFormat>(999);
    EXPECT_EQ(NxTextureFormatToString(unknownFormat), "INVALID");
}

// =============================================================================
// NxTextureFormatFromString Tests
// =============================================================================

class TextureFormatFromStringTest : public ::testing::Test {};

TEST_F(TextureFormatFromStringTest, R8StringReturnsR8) {
    EXPECT_EQ(NxTextureFormatFromString("R8"), NxTextureFormat::R8);
}

TEST_F(TextureFormatFromStringTest, RG8StringReturnsRG8) {
    EXPECT_EQ(NxTextureFormatFromString("RG8"), NxTextureFormat::RG8);
}

TEST_F(TextureFormatFromStringTest, RGB8StringReturnsRGB8) {
    EXPECT_EQ(NxTextureFormatFromString("RGB8"), NxTextureFormat::RGB8);
}

TEST_F(TextureFormatFromStringTest, RGBA8StringReturnsRGBA8) {
    EXPECT_EQ(NxTextureFormatFromString("RGBA8"), NxTextureFormat::RGBA8);
}

TEST_F(TextureFormatFromStringTest, InvalidStringReturnsInvalid) {
    EXPECT_EQ(NxTextureFormatFromString("INVALID"), NxTextureFormat::INVALID);
}

TEST_F(TextureFormatFromStringTest, UnknownStringReturnsInvalid) {
    EXPECT_EQ(NxTextureFormatFromString("UNKNOWN"), NxTextureFormat::INVALID);
}

TEST_F(TextureFormatFromStringTest, EmptyStringReturnsInvalid) {
    EXPECT_EQ(NxTextureFormatFromString(""), NxTextureFormat::INVALID);
}

TEST_F(TextureFormatFromStringTest, LowercaseAccepted) {
    // Implementation is case-insensitive
    EXPECT_EQ(NxTextureFormatFromString("rgba8"), NxTextureFormat::RGBA8);
}

// =============================================================================
// Round-trip Tests
// =============================================================================

class TextureFormatRoundTripTest : public ::testing::Test {};

TEST_F(TextureFormatRoundTripTest, R8RoundTrip) {
    auto str = NxTextureFormatToString(NxTextureFormat::R8);
    EXPECT_EQ(NxTextureFormatFromString(str), NxTextureFormat::R8);
}

TEST_F(TextureFormatRoundTripTest, RG8RoundTrip) {
    auto str = NxTextureFormatToString(NxTextureFormat::RG8);
    EXPECT_EQ(NxTextureFormatFromString(str), NxTextureFormat::RG8);
}

TEST_F(TextureFormatRoundTripTest, RGB8RoundTrip) {
    auto str = NxTextureFormatToString(NxTextureFormat::RGB8);
    EXPECT_EQ(NxTextureFormatFromString(str), NxTextureFormat::RGB8);
}

TEST_F(TextureFormatRoundTripTest, RGBA8RoundTrip) {
    auto str = NxTextureFormatToString(NxTextureFormat::RGBA8);
    EXPECT_EQ(NxTextureFormatFromString(str), NxTextureFormat::RGBA8);
}

}  // namespace nexo::renderer
