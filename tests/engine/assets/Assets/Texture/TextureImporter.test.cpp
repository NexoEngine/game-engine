//// TextureImporter.test.cpp /////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Guillaume HEIN
//  Date:        13/12/2025
//  Description: Test file for the TextureImporter class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "assets/Assets/Texture/TextureImporter.hpp"
#include "assets/AssetImporterContext.hpp"
#include "assets/AssetImporterInput.hpp"

#include <filesystem>
#include <string>
#include <vector>

using namespace testing;
using namespace nexo::assets;

// Test class that provides access to protected members
class TestTextureImporter : public TextureImporter {
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_ValidExtension_PNG);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_ValidExtension_JPG);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_ValidExtension_JPEG);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_ValidExtension_HDR);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_ValidExtension_EXR);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_ValidExtension_TGA);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_ValidExtension_BMP);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_InvalidExtension_TXT);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_InvalidExtension_OBJ);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_InvalidExtension_FBX);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_NoExtension);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_EmptyPath);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_CaseSensitivity_UppercasePNG);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_CaseSensitivity_MixedCaseJPG);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_PathWithSpaces);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_PathWithSpecialCharacters);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_LongPath);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_RelativePath);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_AbsolutePath);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_DotInFilename);
    FRIEND_TEST(TextureImporterTestFixture, CanReadFile_MultipleExtensions);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_ARGB8888_FormatHint);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_ARGB8888_CaseSensitive);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_EmptyData_ARGB8888);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_InvalidFormatHint);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_EmptyFormatHint);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_EmptyData_EmptyHint);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_LargeBuffer_ARGB8888);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_SmallBuffer_InvalidFormat);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_PNG_Header);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_JPG_Header);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_GarbageData);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_AllZeros);
    FRIEND_TEST(TextureImporterTestFixture, CanReadMemory_AllOnes);
    FRIEND_TEST(TextureImporterTestFixture, EdgeCase_NullByte_InPath);
    FRIEND_TEST(TextureImporterTestFixture, EdgeCase_DotFile);
    FRIEND_TEST(TextureImporterTestFixture, EdgeCase_HiddenFile);
    FRIEND_TEST(TextureImporterTestFixture, EdgeCase_PathTraversal);
    FRIEND_TEST(TextureImporterTestFixture, EdgeCase_WindowsPath);
    FRIEND_TEST(TextureImporterTestFixture, EdgeCase_UNCPath);
    FRIEND_TEST(TextureImporterTestFixture, EdgeCase_DoubleExtension);
    FRIEND_TEST(TextureImporterTestFixture, EdgeCase_TrailingDot);
    FRIEND_TEST(TextureImporterTestFixture, EdgeCase_OnlyDots);
    FRIEND_TEST(TextureImporterTestFixture, EdgeCase_CurrentDirectory);
    FRIEND_TEST(TextureImporterTestFixture, EdgeCase_ParentDirectory);
    FRIEND_TEST(TextureImporterTestFixture, FormatHint_ARGB8888_Exact);
    FRIEND_TEST(TextureImporterTestFixture, FormatHint_ARGB8888_WithWhitespace);
    FRIEND_TEST(TextureImporterTestFixture, FormatHint_ARGB8888_Prefix);
    FRIEND_TEST(TextureImporterTestFixture, FormatHint_ARGB8888_Suffix);
    FRIEND_TEST(TextureImporterTestFixture, FormatHint_RGBA8888);
    FRIEND_TEST(TextureImporterTestFixture, FormatHint_RGB888);
    FRIEND_TEST(TextureImporterTestFixture, FormatHint_SpecialCharacters);
    FRIEND_TEST(TextureImporterTestFixture, FormatHint_VeryLongString);
};

// Test fixture for TextureImporter tests
class TextureImporterTestFixture : public Test {
protected:
    void SetUp() override {
        // Initialize importer before each test
    }

    void TearDown() override {
        // Cleanup after each test
    }

    TestTextureImporter importer;
};

// ============================================================================
// canRead() Tests - Testing the variant dispatch logic
// ============================================================================

TEST_F(TextureImporterTestFixture, CanRead_FileInput_DispatchesToCanReadFile) {
    // Create a file input with a valid path (doesn't need to exist for this test)
    ImporterFileInput fileInput{std::filesystem::temp_directory_path() / "test.png"};
    ImporterInputVariant inputVariant = fileInput;

    // The result depends on whether stbi_info can read the file
    // This test just verifies the dispatch works without crashing
    bool result = importer.canRead(inputVariant);
    EXPECT_FALSE(result);  // File doesn't exist, so should return false
}

TEST_F(TextureImporterTestFixture, CanRead_MemoryInput_DispatchesToCanReadMemory) {
    // Create a memory input with empty data
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = "ARGB8888";
    ImporterInputVariant inputVariant = memoryInput;

    // Should return true for ARGB8888 format hint
    bool result = importer.canRead(inputVariant);
    EXPECT_TRUE(result);
}

TEST_F(TextureImporterTestFixture, CanRead_InvalidVariant_ReturnsFalse) {
    // Create an empty variant (monostate)
    ImporterInputVariant inputVariant;

    // Should return false for uninitialized variant
    bool result = importer.canRead(inputVariant);
    EXPECT_FALSE(result);
}

// ============================================================================
// canReadFile() Tests - Testing file path validation
// ============================================================================

TEST_F(TextureImporterTestFixture, CanReadFile_ValidExtension_PNG) {
    ImporterFileInput fileInput{std::filesystem::path("test.png")};

    // stbi_info will return 0 because file doesn't exist
    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_ValidExtension_JPG) {
    ImporterFileInput fileInput{std::filesystem::path("test.jpg")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_ValidExtension_JPEG) {
    ImporterFileInput fileInput{std::filesystem::path("test.jpeg")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_ValidExtension_HDR) {
    ImporterFileInput fileInput{std::filesystem::path("test.hdr")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_ValidExtension_EXR) {
    ImporterFileInput fileInput{std::filesystem::path("test.exr")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_ValidExtension_TGA) {
    ImporterFileInput fileInput{std::filesystem::path("test.tga")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_ValidExtension_BMP) {
    ImporterFileInput fileInput{std::filesystem::path("test.bmp")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_InvalidExtension_TXT) {
    ImporterFileInput fileInput{std::filesystem::path("test.txt")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, CanReadFile_InvalidExtension_OBJ) {
    ImporterFileInput fileInput{std::filesystem::path("model.obj")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, CanReadFile_InvalidExtension_FBX) {
    ImporterFileInput fileInput{std::filesystem::path("model.fbx")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, CanReadFile_NoExtension) {
    ImporterFileInput fileInput{std::filesystem::path("filenoextension")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, CanReadFile_EmptyPath) {
    ImporterFileInput fileInput{std::filesystem::path("")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, CanReadFile_CaseSensitivity_UppercasePNG) {
    ImporterFileInput fileInput{std::filesystem::path("test.PNG")};

    // The implementation uses stbi_info which checks file existence
    // Case sensitivity depends on filesystem
    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_CaseSensitivity_MixedCaseJPG) {
    ImporterFileInput fileInput{std::filesystem::path("test.JpG")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_PathWithSpaces) {
    ImporterFileInput fileInput{std::filesystem::path("test file.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_PathWithSpecialCharacters) {
    ImporterFileInput fileInput{std::filesystem::path("test@#$.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_LongPath) {
    std::string longPath = "/very/long/path/to/some/deeply/nested/directory/structure/that/goes/on/for/a/while/test.png";
    ImporterFileInput fileInput{std::filesystem::path(longPath)};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_RelativePath) {
    ImporterFileInput fileInput{std::filesystem::path("../relative/path/test.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_AbsolutePath) {
    ImporterFileInput fileInput{std::filesystem::path("/absolute/path/test.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_DotInFilename) {
    ImporterFileInput fileInput{std::filesystem::path("test.file.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, CanReadFile_MultipleExtensions) {
    ImporterFileInput fileInput{std::filesystem::path("test.tar.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

// ============================================================================
// canReadMemory() Tests - Testing memory buffer validation
// ============================================================================

TEST_F(TextureImporterTestFixture, CanReadMemory_ARGB8888_FormatHint) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3, 4, 5, 6, 7};
    memoryInput.formatHint = "ARGB8888";

    // Should return true for ARGB8888 format hint (special case)
    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_TRUE(result);
}

TEST_F(TextureImporterTestFixture, CanReadMemory_ARGB8888_CaseSensitive) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = "argb8888";

    // Case sensitive check - lowercase should not match
    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);  // Invalid data format for stbi
}

TEST_F(TextureImporterTestFixture, CanReadMemory_EmptyData_ARGB8888) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{};
    memoryInput.formatHint = "ARGB8888";

    // Should still return true for ARGB8888 format hint
    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_TRUE(result);
}

TEST_F(TextureImporterTestFixture, CanReadMemory_InvalidFormatHint) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = "INVALID";

    // Should use stbi_info_from_memory which will fail on invalid data
    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, CanReadMemory_EmptyFormatHint) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = "";

    // Should use stbi_info_from_memory which will fail on invalid data
    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, CanReadMemory_EmptyData_EmptyHint) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{};
    memoryInput.formatHint = "";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, CanReadMemory_LargeBuffer_ARGB8888) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>(1024 * 1024, 0xFF);  // 1MB buffer
    memoryInput.formatHint = "ARGB8888";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_TRUE(result);
}

TEST_F(TextureImporterTestFixture, CanReadMemory_SmallBuffer_InvalidFormat) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0};
    memoryInput.formatHint = "PNG";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, CanReadMemory_PNG_Header) {
    ImporterMemoryInput memoryInput;
    // PNG magic number: 89 50 4E 47 0D 0A 1A 0A
    memoryInput.memoryData = std::vector<uint8_t>{0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    memoryInput.formatHint = "PNG";

    // stbi_info_from_memory should recognize this as a PNG header
    // But incomplete, so it might fail
    bool result = TestTextureImporter::canReadMemory(memoryInput);
    // Result depends on stbi implementation
    EXPECT_FALSE(result);  // Incomplete PNG
}

TEST_F(TextureImporterTestFixture, CanReadMemory_JPG_Header) {
    ImporterMemoryInput memoryInput;
    // JPG magic number: FF D8 FF
    memoryInput.memoryData = std::vector<uint8_t>{0xFF, 0xD8, 0xFF};
    memoryInput.formatHint = "JPG";

    // stbi_info_from_memory should recognize this as a JPEG header
    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);  // Incomplete JPEG
}

TEST_F(TextureImporterTestFixture, CanReadMemory_GarbageData) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0xDE, 0xAD, 0xBE, 0xEF};
    memoryInput.formatHint = "";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, CanReadMemory_AllZeros) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>(100, 0x00);
    memoryInput.formatHint = "";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, CanReadMemory_AllOnes) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>(100, 0xFF);
    memoryInput.formatHint = "";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);
}

// ============================================================================
// Format Hint Tests - Testing special format indicators
// ============================================================================

TEST_F(TextureImporterTestFixture, FormatHint_ARGB8888_Exact) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = "ARGB8888";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_TRUE(result);
}

TEST_F(TextureImporterTestFixture, FormatHint_ARGB8888_WithWhitespace) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = " ARGB8888 ";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);  // Whitespace should make it not match
}

TEST_F(TextureImporterTestFixture, FormatHint_ARGB8888_Prefix) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = "PREFIX_ARGB8888";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);  // Should not match with prefix
}

TEST_F(TextureImporterTestFixture, FormatHint_ARGB8888_Suffix) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = "ARGB8888_SUFFIX";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);  // Should not match with suffix
}

TEST_F(TextureImporterTestFixture, FormatHint_RGBA8888) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = "RGBA8888";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);  // Different format, should not match special case
}

TEST_F(TextureImporterTestFixture, FormatHint_RGB888) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = "RGB888";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, FormatHint_SpecialCharacters) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = "@#$%";

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, FormatHint_VeryLongString) {
    ImporterMemoryInput memoryInput;
    memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
    memoryInput.formatHint = std::string(1000, 'A');

    bool result = TestTextureImporter::canReadMemory(memoryInput);
    EXPECT_FALSE(result);
}

// ============================================================================
// Edge Case Tests
// ============================================================================

TEST_F(TextureImporterTestFixture, EdgeCase_NullByte_InPath) {
    // Test with embedded null byte in path
    std::string pathWithNull = "test";
    pathWithNull += '\0';
    pathWithNull += ".png";
    ImporterFileInput fileInput{std::filesystem::path(pathWithNull)};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, EdgeCase_DotFile) {
    ImporterFileInput fileInput{std::filesystem::path(".png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, EdgeCase_HiddenFile) {
    ImporterFileInput fileInput{std::filesystem::path(".hidden.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, EdgeCase_PathTraversal) {
    ImporterFileInput fileInput{std::filesystem::path("../../etc/passwd.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist or not a valid texture
}

TEST_F(TextureImporterTestFixture, EdgeCase_WindowsPath) {
    ImporterFileInput fileInput{std::filesystem::path("C:\\Users\\test\\image.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, EdgeCase_UNCPath) {
    ImporterFileInput fileInput{std::filesystem::path("\\\\server\\share\\image.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, EdgeCase_DoubleExtension) {
    ImporterFileInput fileInput{std::filesystem::path("test.png.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, EdgeCase_TrailingDot) {
    ImporterFileInput fileInput{std::filesystem::path("test.png.")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, EdgeCase_OnlyDots) {
    ImporterFileInput fileInput{std::filesystem::path("...")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);
}

TEST_F(TextureImporterTestFixture, EdgeCase_CurrentDirectory) {
    ImporterFileInput fileInput{std::filesystem::path("./test.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

TEST_F(TextureImporterTestFixture, EdgeCase_ParentDirectory) {
    ImporterFileInput fileInput{std::filesystem::path("../test.png")};

    bool result = TestTextureImporter::canReadFile(fileInput);
    EXPECT_FALSE(result);  // File doesn't exist
}

// ============================================================================
// Combined Variant Tests
// ============================================================================

TEST_F(TextureImporterTestFixture, Combined_FileInput_AllSupportedExtensions) {
    std::vector<std::string> supportedExtensions = {
        "png", "jpg", "jpeg", "hdr", "exr", "tga", "bmp"
    };

    for (const auto& ext : supportedExtensions) {
        std::string filename = "test." + ext;
        ImporterFileInput fileInput{std::filesystem::path(filename)};
        ImporterInputVariant inputVariant = fileInput;

        // File doesn't exist, so should return false
        bool result = importer.canRead(inputVariant);
        EXPECT_FALSE(result) << "Extension: " << ext;
    }
}

TEST_F(TextureImporterTestFixture, Combined_FileInput_UnsupportedExtensions) {
    std::vector<std::string> unsupportedExtensions = {
        "txt", "obj", "fbx", "gltf", "blend", "max", "ma", "mb"
    };

    for (const auto& ext : unsupportedExtensions) {
        std::string filename = "test." + ext;
        ImporterFileInput fileInput{std::filesystem::path(filename)};
        ImporterInputVariant inputVariant = fileInput;

        bool result = importer.canRead(inputVariant);
        EXPECT_FALSE(result) << "Extension: " << ext;
    }
}

TEST_F(TextureImporterTestFixture, Combined_MemoryInput_MultipleFormatHints) {
    std::vector<std::string> formatHints = {
        "ARGB8888", "PNG", "JPG", "JPEG", "", "INVALID"
    };

    for (const auto& hint : formatHints) {
        ImporterMemoryInput memoryInput;
        memoryInput.memoryData = std::vector<uint8_t>{0, 1, 2, 3};
        memoryInput.formatHint = hint;
        ImporterInputVariant inputVariant = memoryInput;

        bool result = importer.canRead(inputVariant);
        if (hint == "ARGB8888") {
            EXPECT_TRUE(result) << "Format hint: " << hint;
        } else {
            EXPECT_FALSE(result) << "Format hint: " << hint;
        }
    }
}
