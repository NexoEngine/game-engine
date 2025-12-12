//// PixelConversion.test.cpp //////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for pixel format conversion functions
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include "renderer/Texture.hpp"

namespace nexo::renderer {

class PixelConversionTest : public ::testing::Test {
protected:
    // Helper function to create a pixel array from ARGB values
    std::vector<uint8_t> createPixelBuffer(const std::vector<uint32_t>& argb_pixels) {
        std::vector<uint8_t> buffer(argb_pixels.size() * 4);
        auto* pixels = reinterpret_cast<uint32_t*>(buffer.data());
        for (size_t i = 0; i < argb_pixels.size(); ++i) {
            pixels[i] = argb_pixels[i];
        }
        return buffer;
    }

    // Helper function to extract pixels as uint32_t values
    std::vector<uint32_t> extractPixels(const std::vector<uint8_t>& buffer) {
        std::vector<uint32_t> pixels(buffer.size() / 4);
        const auto* pixel_data = reinterpret_cast<const uint32_t*>(buffer.data());
        for (size_t i = 0; i < pixels.size(); ++i) {
            pixels[i] = pixel_data[i];
        }
        return pixels;
    }
};

// Single Pixel Conversion Tests

TEST_F(PixelConversionTest, ConvertPureRed) {
    // ARGB: 0xFFFF0000 (opaque red) -> RGBA: 0xFF0000FF
    auto buffer = createPixelBuffer({0xFFFF0000});
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    EXPECT_EQ(pixels[0], 0xFF0000FF);
}

TEST_F(PixelConversionTest, ConvertPureGreen) {
    // ARGB: 0xFF00FF00 (opaque green) -> RGBA: 0x00FF00FF
    auto buffer = createPixelBuffer({0xFF00FF00});
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    EXPECT_EQ(pixels[0], 0x00FF00FF);
}

TEST_F(PixelConversionTest, ConvertPureBlue) {
    // ARGB: 0xFF0000FF (opaque blue) -> RGBA: 0x0000FFFF
    auto buffer = createPixelBuffer({0xFF0000FF});
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    EXPECT_EQ(pixels[0], 0x0000FFFF);
}

TEST_F(PixelConversionTest, ConvertPureWhite) {
    // ARGB: 0xFFFFFFFF (opaque white) -> RGBA: 0xFFFFFFFF
    auto buffer = createPixelBuffer({0xFFFFFFFF});
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    EXPECT_EQ(pixels[0], 0xFFFFFFFF);
}

TEST_F(PixelConversionTest, ConvertPureBlack) {
    // ARGB: 0xFF000000 (opaque black) -> RGBA: 0x000000FF
    auto buffer = createPixelBuffer({0xFF000000});
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    EXPECT_EQ(pixels[0], 0x000000FF);
}

TEST_F(PixelConversionTest, ConvertTransparent) {
    // ARGB: 0x00000000 (fully transparent) -> RGBA: 0x00000000
    auto buffer = createPixelBuffer({0x00000000});
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    EXPECT_EQ(pixels[0], 0x00000000);
}

// Alpha Channel Tests

TEST_F(PixelConversionTest, ConvertSemiTransparentRed) {
    // ARGB: 0x80FF0000 (semi-transparent red) -> RGBA: 0xFF000080
    auto buffer = createPixelBuffer({0x80FF0000});
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    EXPECT_EQ(pixels[0], 0xFF000080);
}

TEST_F(PixelConversionTest, ConvertVariousAlphaValues) {
    // Test different alpha values with the same color
    std::vector<uint32_t> argb_values = {
        0x00AABBCC, // Alpha = 0x00
        0x40AABBCC, // Alpha = 0x40
        0x80AABBCC, // Alpha = 0x80
        0xC0AABBCC, // Alpha = 0xC0
        0xFFAABBCC  // Alpha = 0xFF
    };
    std::vector<uint32_t> expected_rgba = {
        0xAABBCC00,
        0xAABBCC40,
        0xAABBCC80,
        0xAABBCCC0,
        0xAABBCCFF
    };

    auto buffer = createPixelBuffer(argb_values);
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);

    for (size_t i = 0; i < pixels.size(); ++i) {
        EXPECT_EQ(pixels[i], expected_rgba[i]) << "Failed at index " << i;
    }
}

TEST_F(PixelConversionTest, ConvertQuarterAlpha) {
    // ARGB: 0x3F00FF00 (quarter alpha green) -> RGBA: 0x00FF003F
    auto buffer = createPixelBuffer({0x3F00FF00});
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    EXPECT_EQ(pixels[0], 0x00FF003F);
}

// Multiple Pixels Tests

TEST_F(PixelConversionTest, ConvertFourPixels) {
    std::vector<uint32_t> argb_pixels = {
        0xFFFF0000, // Red
        0xFF00FF00, // Green
        0xFF0000FF, // Blue
        0xFFFFFFFF  // White
    };
    std::vector<uint32_t> expected_rgba = {
        0xFF0000FF,
        0x00FF00FF,
        0x0000FFFF,
        0xFFFFFFFF
    };

    auto buffer = createPixelBuffer(argb_pixels);
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);

    ASSERT_EQ(pixels.size(), 4);
    for (size_t i = 0; i < pixels.size(); ++i) {
        EXPECT_EQ(pixels[i], expected_rgba[i]) << "Failed at pixel " << i;
    }
}

TEST_F(PixelConversionTest, ConvertSixteenPixels) {
    std::vector<uint32_t> argb_pixels = {
        0xFFFF0000, 0xFF00FF00, 0xFF0000FF, 0xFFFFFFFF,
        0xFF000000, 0x00000000, 0x80FF0000, 0x80FFFFFF,
        0x11223344, 0x55667788, 0x99AABBCC, 0xDDEEFF00,
        0xAABBCCDD, 0x12345678, 0xFEDCBA98, 0xC0FFEE00
    };
    std::vector<uint32_t> expected_rgba = {
        0xFF0000FF, 0x00FF00FF, 0x0000FFFF, 0xFFFFFFFF,
        0x000000FF, 0x00000000, 0xFF000080, 0xFFFFFF80,
        0x22334411, 0x66778855, 0xAABBCC99, 0xEEFF00DD,
        0xBBCCDDAA, 0x34567812, 0xDCBA98FE, 0xFFEE00C0
    };

    auto buffer = createPixelBuffer(argb_pixels);
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);

    ASSERT_EQ(pixels.size(), 16);
    for (size_t i = 0; i < pixels.size(); ++i) {
        EXPECT_EQ(pixels[i], expected_rgba[i]) << "Failed at pixel " << i;
    }
}

TEST_F(PixelConversionTest, ConvertMixedColors) {
    std::vector<uint32_t> argb_pixels = {
        0xFF123456, // Mixed color 1
        0x80ABCDEF, // Mixed color 2 with transparency
        0x00FEDCBA, // Mixed color 3 fully transparent
        0xC0246810  // Mixed color 4
    };
    std::vector<uint32_t> expected_rgba = {
        0x123456FF,
        0xABCDEF80,
        0xFEDCBA00,
        0x246810C0
    };

    auto buffer = createPixelBuffer(argb_pixels);
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);

    ASSERT_EQ(pixels.size(), 4);
    for (size_t i = 0; i < pixels.size(); ++i) {
        EXPECT_EQ(pixels[i], expected_rgba[i]) << "Failed at pixel " << i;
    }
}

// Edge Cases

TEST_F(PixelConversionTest, ConvertEmptyBuffer) {
    std::vector<uint8_t> buffer;
    // Should not crash on empty buffer
    EXPECT_NO_THROW(NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size()));
}

TEST_F(PixelConversionTest, ConvertSinglePixel) {
    // ARGB: 0x12345678 -> RGBA: 0x34567812
    auto buffer = createPixelBuffer({0x12345678});
    ASSERT_EQ(buffer.size(), 4);
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    ASSERT_EQ(pixels.size(), 1);
    EXPECT_EQ(pixels[0], 0x34567812);
}

TEST_F(PixelConversionTest, ConvertZeroSize) {
    std::vector<uint8_t> buffer(4, 0xFF);
    // Passing size 0 should not modify the buffer
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), 0);
    EXPECT_EQ(buffer[0], 0xFF);
    EXPECT_EQ(buffer[1], 0xFF);
    EXPECT_EQ(buffer[2], 0xFF);
    EXPECT_EQ(buffer[3], 0xFF);
}

// Bit Manipulation Verification Tests

TEST_F(PixelConversionTest, VerifyBitShiftLeft) {
    // Verify that RGB channels shift left correctly
    // ARGB: 0x00RGBXYZ where XYZ is any alpha
    // RGBA: 0xRGBXYZ00 | 0x000000XYZ = 0xRGBXYZXYZ... but we only take lower bits
    auto buffer = createPixelBuffer({0xAB123456});
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    // ARGB: 0xAB123456 -> RGBA: 0x123456AB
    EXPECT_EQ(pixels[0], 0x123456AB);
}

TEST_F(PixelConversionTest, VerifyAlphaExtraction) {
    // Verify that alpha channel is correctly extracted and placed
    // Test with only alpha set
    auto buffer = createPixelBuffer({0xAB000000});
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    // ARGB: 0xAB000000 -> RGBA: 0x000000AB
    EXPECT_EQ(pixels[0], 0x000000AB);
}

TEST_F(PixelConversionTest, InPlaceModification) {
    // Verify that the conversion is truly in-place
    std::vector<uint8_t> buffer = {0x00, 0x00, 0xFF, 0xFF}; // ARGB: 0xFFFF0000 (little-endian)
    uint8_t* original_ptr = buffer.data();
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    EXPECT_EQ(original_ptr, buffer.data()) << "Buffer pointer should not change";
    // Verify the conversion happened
    auto pixels = extractPixels(buffer);
    EXPECT_EQ(pixels[0], 0xFF0000FF);
}

TEST_F(PixelConversionTest, PreserveAllBits) {
    // Ensure all bits are preserved during conversion
    auto buffer = createPixelBuffer({0xAAAA5555});
    NxTextureFormatConvertArgb8ToRgba8(buffer.data(), buffer.size());
    auto pixels = extractPixels(buffer);
    // ARGB: 0xAAAA5555 -> RGBA: 0xAA5555AA
    EXPECT_EQ(pixels[0], 0xAA5555AA);
}

} // namespace nexo::renderer
