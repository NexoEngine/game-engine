//// AssetImporterInput.test.cpp ///////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for AssetImporterInput types
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/AssetImporterInput.hpp"
#include <type_traits>
#include <filesystem>

namespace nexo::assets {

// =============================================================================
// ImporterFileInput Tests
// =============================================================================

class ImporterFileInputTest : public ::testing::Test {};

TEST_F(ImporterFileInputTest, IsAggregate) {
    static_assert(std::is_aggregate_v<ImporterFileInput>);
    SUCCEED();
}

TEST_F(ImporterFileInputTest, CanBeDefaultConstructed) {
    ImporterFileInput input{};
    EXPECT_TRUE(input.filePath.empty());
}

TEST_F(ImporterFileInputTest, CanBeInitializedWithPath) {
    ImporterFileInput input{"/path/to/file.obj"};
    EXPECT_EQ(input.filePath.string(), "/path/to/file.obj");
}

TEST_F(ImporterFileInputTest, CanBeInitializedWithFilesystemPath) {
    std::filesystem::path path = "/path/to/texture.png";
    ImporterFileInput input{path};
    EXPECT_EQ(input.filePath, path);
}

TEST_F(ImporterFileInputTest, CanBeCopied) {
    ImporterFileInput original{"/original/path.txt"};
    ImporterFileInput copy = original;
    EXPECT_EQ(copy.filePath, original.filePath);
}

TEST_F(ImporterFileInputTest, CanBeMoved) {
    ImporterFileInput original{"/original/path.txt"};
    ImporterFileInput moved = std::move(original);
    EXPECT_EQ(moved.filePath.string(), "/original/path.txt");
}

TEST_F(ImporterFileInputTest, PathCanBeModified) {
    ImporterFileInput input{"/initial.txt"};
    input.filePath = "/modified.txt";
    EXPECT_EQ(input.filePath.string(), "/modified.txt");
}

TEST_F(ImporterFileInputTest, PathSupportsExtension) {
    ImporterFileInput input{"/path/to/model.fbx"};
    EXPECT_EQ(input.filePath.extension(), ".fbx");
}

TEST_F(ImporterFileInputTest, PathSupportsFilename) {
    ImporterFileInput input{"/path/to/model.obj"};
    EXPECT_EQ(input.filePath.filename(), "model.obj");
}

TEST_F(ImporterFileInputTest, PathSupportsParentPath) {
    ImporterFileInput input{"/path/to/model.obj"};
    EXPECT_EQ(input.filePath.parent_path(), "/path/to");
}

// =============================================================================
// ImporterMemoryInput Tests
// =============================================================================

class ImporterMemoryInputTest : public ::testing::Test {};

TEST_F(ImporterMemoryInputTest, IsAggregate) {
    static_assert(std::is_aggregate_v<ImporterMemoryInput>);
    SUCCEED();
}

TEST_F(ImporterMemoryInputTest, CanBeDefaultConstructed) {
    ImporterMemoryInput input{};
    EXPECT_TRUE(input.memoryData.empty());
    EXPECT_TRUE(input.formatHint.empty());
}

TEST_F(ImporterMemoryInputTest, CanBeInitializedWithData) {
    std::vector<uint8_t> data = {0x00, 0x01, 0x02, 0x03};
    ImporterMemoryInput input{data, ".png"};

    EXPECT_EQ(input.memoryData.size(), 4u);
    EXPECT_EQ(input.formatHint, ".png");
}

TEST_F(ImporterMemoryInputTest, CanBeCopied) {
    ImporterMemoryInput original{{0xAA, 0xBB}, "ARGB8888"};
    ImporterMemoryInput copy = original;

    EXPECT_EQ(copy.memoryData, original.memoryData);
    EXPECT_EQ(copy.formatHint, original.formatHint);
}

TEST_F(ImporterMemoryInputTest, CanBeMoved) {
    std::vector<uint8_t> data = {0x00, 0x01, 0x02};
    ImporterMemoryInput original{std::move(data), ".obj"};
    ImporterMemoryInput moved = std::move(original);

    EXPECT_EQ(moved.memoryData.size(), 3u);
    EXPECT_EQ(moved.formatHint, ".obj");
}

TEST_F(ImporterMemoryInputTest, DataCanBeModified) {
    ImporterMemoryInput input{};
    input.memoryData = {0xFF, 0xFE, 0xFD};
    input.formatHint = ".fbx";

    EXPECT_EQ(input.memoryData.size(), 3u);
    EXPECT_EQ(input.formatHint, ".fbx");
}

TEST_F(ImporterMemoryInputTest, CanStoreLargeData) {
    std::vector<uint8_t> largeData(1024 * 1024);  // 1MB
    std::fill(largeData.begin(), largeData.end(), 0xAB);

    ImporterMemoryInput input{std::move(largeData), "raw"};

    EXPECT_EQ(input.memoryData.size(), 1024u * 1024u);
    EXPECT_EQ(input.memoryData[0], 0xAB);
    EXPECT_EQ(input.memoryData[1024 * 1024 - 1], 0xAB);
}

TEST_F(ImporterMemoryInputTest, FormatHintCanBeEmpty) {
    ImporterMemoryInput input{{0x01, 0x02}, ""};
    EXPECT_TRUE(input.formatHint.empty());
    EXPECT_FALSE(input.memoryData.empty());
}

// =============================================================================
// ImporterInputVariant Tests
// =============================================================================

class ImporterInputVariantTest : public ::testing::Test {};

TEST_F(ImporterInputVariantTest, CanHoldFileInput) {
    ImporterInputVariant variant = ImporterFileInput{"/path/to/file.obj"};
    EXPECT_TRUE(std::holds_alternative<ImporterFileInput>(variant));
    EXPECT_FALSE(std::holds_alternative<ImporterMemoryInput>(variant));
}

TEST_F(ImporterInputVariantTest, CanHoldMemoryInput) {
    ImporterInputVariant variant = ImporterMemoryInput{{0x01, 0x02}, ".png"};
    EXPECT_FALSE(std::holds_alternative<ImporterFileInput>(variant));
    EXPECT_TRUE(std::holds_alternative<ImporterMemoryInput>(variant));
}

TEST_F(ImporterInputVariantTest, CanGetFileInput) {
    ImporterInputVariant variant = ImporterFileInput{"/test/path.txt"};

    auto& fileInput = std::get<ImporterFileInput>(variant);
    EXPECT_EQ(fileInput.filePath.string(), "/test/path.txt");
}

TEST_F(ImporterInputVariantTest, CanGetMemoryInput) {
    ImporterInputVariant variant = ImporterMemoryInput{{0xAA, 0xBB}, "format"};

    auto& memInput = std::get<ImporterMemoryInput>(variant);
    EXPECT_EQ(memInput.memoryData.size(), 2u);
    EXPECT_EQ(memInput.formatHint, "format");
}

TEST_F(ImporterInputVariantTest, GetIfReturnsNullForWrongType) {
    ImporterInputVariant variant = ImporterFileInput{"/path.obj"};

    auto* filePtr = std::get_if<ImporterFileInput>(&variant);
    auto* memPtr = std::get_if<ImporterMemoryInput>(&variant);

    EXPECT_NE(filePtr, nullptr);
    EXPECT_EQ(memPtr, nullptr);
}

TEST_F(ImporterInputVariantTest, CanSwitchBetweenTypes) {
    ImporterInputVariant variant = ImporterFileInput{"/initial.obj"};
    EXPECT_TRUE(std::holds_alternative<ImporterFileInput>(variant));

    variant = ImporterMemoryInput{{0x01}, ".png"};
    EXPECT_TRUE(std::holds_alternative<ImporterMemoryInput>(variant));
}

TEST_F(ImporterInputVariantTest, CanBeUsedWithVisit) {
    ImporterInputVariant fileVariant = ImporterFileInput{"/file.obj"};
    ImporterInputVariant memVariant = ImporterMemoryInput{{0x01}, ".png"};

    auto visitor = [](auto&& input) -> std::string {
        using T = std::decay_t<decltype(input)>;
        if constexpr (std::is_same_v<T, ImporterFileInput>) {
            return "file";
        } else if constexpr (std::is_same_v<T, ImporterMemoryInput>) {
            return "memory";
        }
    };

    EXPECT_EQ(std::visit(visitor, fileVariant), "file");
    EXPECT_EQ(std::visit(visitor, memVariant), "memory");
}

TEST_F(ImporterInputVariantTest, CanBeCopied) {
    ImporterInputVariant original = ImporterFileInput{"/test.obj"};
    ImporterInputVariant copy = original;

    EXPECT_TRUE(std::holds_alternative<ImporterFileInput>(copy));
    EXPECT_EQ(std::get<ImporterFileInput>(copy).filePath.string(), "/test.obj");
}

TEST_F(ImporterInputVariantTest, CanBeMoved) {
    ImporterInputVariant original = ImporterMemoryInput{{0x01, 0x02, 0x03}, ".fbx"};
    ImporterInputVariant moved = std::move(original);

    EXPECT_TRUE(std::holds_alternative<ImporterMemoryInput>(moved));
    EXPECT_EQ(std::get<ImporterMemoryInput>(moved).memoryData.size(), 3u);
}

// =============================================================================
// Integration Tests
// =============================================================================

class ImporterInputIntegrationTest : public ::testing::Test {};

TEST_F(ImporterInputIntegrationTest, FileInputPathOperations) {
    ImporterFileInput input{"/assets/models/character.fbx"};

    EXPECT_EQ(input.filePath.extension(), ".fbx");
    EXPECT_EQ(input.filePath.stem(), "character");
    EXPECT_EQ(input.filePath.filename(), "character.fbx");
}

TEST_F(ImporterInputIntegrationTest, MemoryInputWithImageData) {
    // Simulate PNG header
    std::vector<uint8_t> pngHeader = {
        0x89, 0x50, 0x4E, 0x47,  // PNG signature
        0x0D, 0x0A, 0x1A, 0x0A   // Rest of signature
    };

    ImporterMemoryInput input{pngHeader, ".png"};

    EXPECT_EQ(input.memoryData.size(), 8u);
    EXPECT_EQ(input.memoryData[0], 0x89);
    EXPECT_EQ(input.memoryData[1], 0x50);  // 'P'
}

TEST_F(ImporterInputIntegrationTest, VariantCanStoreEitherType) {
    std::vector<ImporterInputVariant> inputs;

    inputs.push_back(ImporterFileInput{"/model1.obj"});
    inputs.push_back(ImporterMemoryInput{{0x01}, ".png"});
    inputs.push_back(ImporterFileInput{"/model2.fbx"});

    EXPECT_EQ(inputs.size(), 3u);
    EXPECT_TRUE(std::holds_alternative<ImporterFileInput>(inputs[0]));
    EXPECT_TRUE(std::holds_alternative<ImporterMemoryInput>(inputs[1]));
    EXPECT_TRUE(std::holds_alternative<ImporterFileInput>(inputs[2]));
}

}  // namespace nexo::assets
