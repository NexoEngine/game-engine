//// SubTexture2D.test.cpp ////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for SubTexture2D class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "renderer/SubTexture2D.hpp"

namespace nexo::renderer {

// =============================================================================
// Mock Texture for Testing
// =============================================================================

class MockTexture2D : public NxTexture2D {
public:
    MockTexture2D(unsigned int width, unsigned int height)
        : m_width(width), m_height(height), m_id(++s_nextId) {}

    [[nodiscard]] unsigned int getWidth() const override { return m_width; }
    [[nodiscard]] unsigned int getHeight() const override { return m_height; }
    [[nodiscard]] unsigned int getMaxTextureSize() const override { return 4096; }
    [[nodiscard]] unsigned int getId() const override { return m_id; }

    void bind(unsigned int /*slot*/ = 0) const override {}
    void unbind(unsigned int /*slot*/ = 0) const override {}
    void setData(void* /*data*/, size_t /*size*/) override {}

private:
    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_id;
    static inline unsigned int s_nextId = 0;
};

// =============================================================================
// SubTexture2D Constructor Tests
// =============================================================================

class SubTexture2DConstructorTest : public ::testing::Test {
protected:
    std::shared_ptr<MockTexture2D> texture = std::make_shared<MockTexture2D>(256, 256);
};

TEST_F(SubTexture2DConstructorTest, StoresTextureReference) {
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(1.0f, 1.0f);
    NxSubTexture2D subTexture(texture, min, max);

    EXPECT_EQ(subTexture.getTexture(), texture);
}

TEST_F(SubTexture2DConstructorTest, SetsCorrectTextureCoords_FullTexture) {
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(1.0f, 1.0f);
    NxSubTexture2D subTexture(texture, min, max);

    const glm::vec2* coords = subTexture.getTextureCoords();

    // Bottom-left
    EXPECT_FLOAT_EQ(coords[0].x, 0.0f);
    EXPECT_FLOAT_EQ(coords[0].y, 0.0f);
    // Bottom-right
    EXPECT_FLOAT_EQ(coords[1].x, 1.0f);
    EXPECT_FLOAT_EQ(coords[1].y, 0.0f);
    // Top-right
    EXPECT_FLOAT_EQ(coords[2].x, 1.0f);
    EXPECT_FLOAT_EQ(coords[2].y, 1.0f);
    // Top-left
    EXPECT_FLOAT_EQ(coords[3].x, 0.0f);
    EXPECT_FLOAT_EQ(coords[3].y, 1.0f);
}

TEST_F(SubTexture2DConstructorTest, SetsCorrectTextureCoords_PartialTexture) {
    glm::vec2 min(0.25f, 0.25f);
    glm::vec2 max(0.75f, 0.75f);
    NxSubTexture2D subTexture(texture, min, max);

    const glm::vec2* coords = subTexture.getTextureCoords();

    // Bottom-left
    EXPECT_FLOAT_EQ(coords[0].x, 0.25f);
    EXPECT_FLOAT_EQ(coords[0].y, 0.25f);
    // Bottom-right
    EXPECT_FLOAT_EQ(coords[1].x, 0.75f);
    EXPECT_FLOAT_EQ(coords[1].y, 0.25f);
    // Top-right
    EXPECT_FLOAT_EQ(coords[2].x, 0.75f);
    EXPECT_FLOAT_EQ(coords[2].y, 0.75f);
    // Top-left
    EXPECT_FLOAT_EQ(coords[3].x, 0.25f);
    EXPECT_FLOAT_EQ(coords[3].y, 0.75f);
}

// =============================================================================
// SubTexture2D CreateFromCoords Tests
// =============================================================================

class SubTexture2DCreateFromCoordsTest : public ::testing::Test {};

TEST_F(SubTexture2DCreateFromCoordsTest, CreatesSubTextureAtOrigin) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);
    glm::vec2 coords(0, 0);      // Grid position (0, 0)
    glm::vec2 cellSize(64, 64);  // 64x64 pixel cells

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(0,0), max=(64/256, 64/256) = (0.25, 0.25)
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.0f);    // min.x
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.0f);    // min.y
    EXPECT_FLOAT_EQ(texCoords[2].x, 0.25f);   // max.x
    EXPECT_FLOAT_EQ(texCoords[2].y, 0.25f);   // max.y
}

TEST_F(SubTexture2DCreateFromCoordsTest, CreatesSubTextureAtOffset) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);
    glm::vec2 coords(1, 1);      // Grid position (1, 1)
    glm::vec2 cellSize(64, 64);  // 64x64 pixel cells

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(64/256, 64/256) = (0.25, 0.25), max=(128/256, 128/256) = (0.5, 0.5)
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.25f);   // min.x
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.25f);   // min.y
    EXPECT_FLOAT_EQ(texCoords[2].x, 0.5f);    // max.x
    EXPECT_FLOAT_EQ(texCoords[2].y, 0.5f);    // max.y
}

TEST_F(SubTexture2DCreateFromCoordsTest, CreatesSubTextureWithCustomSpriteSize) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);
    glm::vec2 coords(0, 0);        // Grid position (0, 0)
    glm::vec2 cellSize(32, 32);    // 32x32 pixel cells
    glm::vec2 spriteSize(2, 2);    // 2x2 grid cells (64x64 pixels)

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize, spriteSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(0,0), max=(64/256, 64/256) = (0.25, 0.25)
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.0f);    // min.x
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.0f);    // min.y
    EXPECT_FLOAT_EQ(texCoords[2].x, 0.25f);   // max.x
    EXPECT_FLOAT_EQ(texCoords[2].y, 0.25f);   // max.y
}

TEST_F(SubTexture2DCreateFromCoordsTest, CreatesSubTextureWithNonSquareTexture) {
    auto texture = std::make_shared<MockTexture2D>(512, 256);  // Non-square texture
    glm::vec2 coords(0, 0);
    glm::vec2 cellSize(64, 64);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(0,0), max=(64/512, 64/256) = (0.125, 0.25)
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.0f);    // min.x
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.0f);    // min.y
    EXPECT_FLOAT_EQ(texCoords[2].x, 0.125f);  // max.x (64/512)
    EXPECT_FLOAT_EQ(texCoords[2].y, 0.25f);   // max.y (64/256)
}

TEST_F(SubTexture2DCreateFromCoordsTest, CreatesSubTextureAtLastCell) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);
    glm::vec2 coords(3, 3);      // Last cell in 4x4 grid
    glm::vec2 cellSize(64, 64);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(192/256, 192/256) = (0.75, 0.75), max=(1.0, 1.0)
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.75f);   // min.x
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.75f);   // min.y
    EXPECT_FLOAT_EQ(texCoords[2].x, 1.0f);    // max.x
    EXPECT_FLOAT_EQ(texCoords[2].y, 1.0f);    // max.y
}

// =============================================================================
// SubTexture2D Texture Reference Tests
// =============================================================================

class SubTexture2DTextureRefTest : public ::testing::Test {};

TEST_F(SubTexture2DTextureRefTest, GetTextureReturnsSharedPtr) {
    auto texture = std::make_shared<MockTexture2D>(128, 128);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(1.0f, 1.0f);
    NxSubTexture2D subTexture(texture, min, max);

    EXPECT_EQ(subTexture.getTexture().use_count(), 2);  // texture + subTexture
}

TEST_F(SubTexture2DTextureRefTest, MultipleSubTexturesShareTexture) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);

    auto sub1 = NxSubTexture2D::createFromCoords(texture, {0, 0}, {64, 64});
    auto sub2 = NxSubTexture2D::createFromCoords(texture, {1, 0}, {64, 64});
    auto sub3 = NxSubTexture2D::createFromCoords(texture, {2, 0}, {64, 64});

    EXPECT_EQ(sub1->getTexture(), sub2->getTexture());
    EXPECT_EQ(sub2->getTexture(), sub3->getTexture());
    EXPECT_EQ(texture.use_count(), 4);  // Original + 3 subtextures
}

// =============================================================================
// SubTexture2D Coordinate Calculation Tests
// =============================================================================

class SubTexture2DCoordCalculationTest : public ::testing::Test {};

TEST_F(SubTexture2DCoordCalculationTest, CoordsAreNormalized) {
    auto texture = std::make_shared<MockTexture2D>(1024, 1024);
    glm::vec2 coords(5, 10);
    glm::vec2 cellSize(32, 32);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // All coordinates should be in [0, 1] range
    for (int i = 0; i < 4; ++i) {
        EXPECT_GE(texCoords[i].x, 0.0f);
        EXPECT_LE(texCoords[i].x, 1.0f);
        EXPECT_GE(texCoords[i].y, 0.0f);
        EXPECT_LE(texCoords[i].y, 1.0f);
    }
}

TEST_F(SubTexture2DCoordCalculationTest, CoordsOrderIsCorrect) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);
    glm::vec2 coords(1, 1);
    glm::vec2 cellSize(64, 64);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Verify winding order: bottom-left, bottom-right, top-right, top-left
    // Bottom-left should have smallest x and y
    EXPECT_LT(texCoords[0].x, texCoords[1].x);  // bottom-left.x < bottom-right.x
    EXPECT_EQ(texCoords[0].y, texCoords[1].y);  // same y for bottom edge
    EXPECT_EQ(texCoords[1].x, texCoords[2].x);  // same x for right edge
    EXPECT_LT(texCoords[1].y, texCoords[2].y);  // bottom-right.y < top-right.y
    EXPECT_GT(texCoords[2].x, texCoords[3].x);  // top-right.x > top-left.x
    EXPECT_EQ(texCoords[2].y, texCoords[3].y);  // same y for top edge
}

// =============================================================================
// SubTexture2D Edge Coordinate Tests
// =============================================================================

class SubTexture2DEdgeCoordinateTest : public ::testing::Test {};

TEST_F(SubTexture2DEdgeCoordinateTest, CoordsAtTextureBoundary) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);
    glm::vec2 coords(3, 3);      // Last cell in 4x4 grid
    glm::vec2 cellSize(64, 64);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Should reach exactly to texture boundary (1.0)
    EXPECT_FLOAT_EQ(texCoords[2].x, 1.0f);  // max.x should be exactly 1.0
    EXPECT_FLOAT_EQ(texCoords[2].y, 1.0f);  // max.y should be exactly 1.0
}

TEST_F(SubTexture2DEdgeCoordinateTest, VerySmallSpriteSize_1x1Pixel) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);
    glm::vec2 coords(0, 0);
    glm::vec2 cellSize(1, 1);  // 1x1 pixel cell

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(0,0), max=(1/256, 1/256)
    float expected = 1.0f / 256.0f;
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[2].x, expected);
    EXPECT_FLOAT_EQ(texCoords[2].y, expected);
}

TEST_F(SubTexture2DEdgeCoordinateTest, VerySmallSpriteSize_2x2Pixel) {
    auto texture = std::make_shared<MockTexture2D>(1024, 1024);
    glm::vec2 coords(10, 10);
    glm::vec2 cellSize(2, 2);  // 2x2 pixel cell

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(20/1024, 20/1024), max=(22/1024, 22/1024)
    float minCoord = 20.0f / 1024.0f;
    float maxCoord = 22.0f / 1024.0f;
    EXPECT_FLOAT_EQ(texCoords[0].x, minCoord);
    EXPECT_FLOAT_EQ(texCoords[0].y, minCoord);
    EXPECT_FLOAT_EQ(texCoords[2].x, maxCoord);
    EXPECT_FLOAT_EQ(texCoords[2].y, maxCoord);
}

TEST_F(SubTexture2DEdgeCoordinateTest, SpriteSizeLargerThanCellSize) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);
    glm::vec2 coords(0, 0);
    glm::vec2 cellSize(32, 32);    // 32x32 pixel cells
    glm::vec2 spriteSize(4, 4);    // 4x4 grid cells = 128x128 pixels

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize, spriteSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(0,0), max=(128/256, 128/256) = (0.5, 0.5)
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[2].x, 0.5f);
    EXPECT_FLOAT_EQ(texCoords[2].y, 0.5f);
}

TEST_F(SubTexture2DEdgeCoordinateTest, SpriteSizeCoversEntireTexture) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);
    glm::vec2 coords(0, 0);
    glm::vec2 cellSize(64, 64);    // 64x64 pixel cells
    glm::vec2 spriteSize(4, 4);    // 4x4 grid cells = entire 256x256 texture

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize, spriteSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Should cover entire texture
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[2].x, 1.0f);
    EXPECT_FLOAT_EQ(texCoords[2].y, 1.0f);
}

TEST_F(SubTexture2DEdgeCoordinateTest, SinglePixelAtTextureCorner) {
    auto texture = std::make_shared<MockTexture2D>(128, 128);
    glm::vec2 coords(127, 127);    // Last pixel
    glm::vec2 cellSize(1, 1);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(127/128, 127/128), max=(1.0, 1.0)
    float minCoord = 127.0f / 128.0f;
    EXPECT_FLOAT_EQ(texCoords[0].x, minCoord);
    EXPECT_FLOAT_EQ(texCoords[0].y, minCoord);
    EXPECT_FLOAT_EQ(texCoords[2].x, 1.0f);
    EXPECT_FLOAT_EQ(texCoords[2].y, 1.0f);
}

// =============================================================================
// SubTexture2D Precision Tests
// =============================================================================

class SubTexture2DPrecisionTest : public ::testing::Test {};

TEST_F(SubTexture2DPrecisionTest, NonPowerOfTwoTextureDimensions_Width) {
    auto texture = std::make_shared<MockTexture2D>(300, 256);  // 300 is not power of 2
    glm::vec2 coords(1, 1);
    glm::vec2 cellSize(50, 64);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(50/300, 64/256), max=(100/300, 128/256)
    float minX = 50.0f / 300.0f;
    float maxX = 100.0f / 300.0f;
    float minY = 64.0f / 256.0f;
    float maxY = 128.0f / 256.0f;

    EXPECT_FLOAT_EQ(texCoords[0].x, minX);
    EXPECT_FLOAT_EQ(texCoords[0].y, minY);
    EXPECT_FLOAT_EQ(texCoords[2].x, maxX);
    EXPECT_FLOAT_EQ(texCoords[2].y, maxY);
}

TEST_F(SubTexture2DPrecisionTest, NonPowerOfTwoTextureDimensions_Height) {
    auto texture = std::make_shared<MockTexture2D>(512, 500);  // 500 is not power of 2
    glm::vec2 coords(2, 3);
    glm::vec2 cellSize(64, 100);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(128/512, 300/500), max=(192/512, 400/500)
    float minX = 128.0f / 512.0f;
    float maxX = 192.0f / 512.0f;
    float minY = 300.0f / 500.0f;
    float maxY = 400.0f / 500.0f;

    EXPECT_FLOAT_EQ(texCoords[0].x, minX);
    EXPECT_FLOAT_EQ(texCoords[0].y, minY);
    EXPECT_FLOAT_EQ(texCoords[2].x, maxX);
    EXPECT_FLOAT_EQ(texCoords[2].y, maxY);
}

TEST_F(SubTexture2DPrecisionTest, NonPowerOfTwoTextureDimensions_BothDimensions) {
    auto texture = std::make_shared<MockTexture2D>(300, 200);  // Neither power of 2
    glm::vec2 coords(0, 0);
    glm::vec2 cellSize(30, 25);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(0,0), max=(30/300, 25/200) = (0.1, 0.125)
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[2].x, 0.1f);
    EXPECT_FLOAT_EQ(texCoords[2].y, 0.125f);
}

TEST_F(SubTexture2DPrecisionTest, OddTextureDimensions) {
    auto texture = std::make_shared<MockTexture2D>(333, 333);  // Odd dimensions
    glm::vec2 coords(1, 1);
    glm::vec2 cellSize(33, 33);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(33/333, 33/333), max=(66/333, 66/333)
    float minCoord = 33.0f / 333.0f;
    float maxCoord = 66.0f / 333.0f;

    EXPECT_FLOAT_EQ(texCoords[0].x, minCoord);
    EXPECT_FLOAT_EQ(texCoords[0].y, minCoord);
    EXPECT_FLOAT_EQ(texCoords[2].x, maxCoord);
    EXPECT_FLOAT_EQ(texCoords[2].y, maxCoord);
}

TEST_F(SubTexture2DPrecisionTest, PrimeNumberDimensions) {
    auto texture = std::make_shared<MockTexture2D>(251, 257);  // Prime numbers
    glm::vec2 coords(2, 3);
    glm::vec2 cellSize(50, 51);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(100/251, 153/257), max=(150/251, 204/257)
    float minX = 100.0f / 251.0f;
    float maxX = 150.0f / 251.0f;
    float minY = 153.0f / 257.0f;
    float maxY = 204.0f / 257.0f;

    EXPECT_FLOAT_EQ(texCoords[0].x, minX);
    EXPECT_FLOAT_EQ(texCoords[0].y, minY);
    EXPECT_FLOAT_EQ(texCoords[2].x, maxX);
    EXPECT_FLOAT_EQ(texCoords[2].y, maxY);
}

TEST_F(SubTexture2DPrecisionTest, VeryLargeTextureDimensions) {
    auto texture = std::make_shared<MockTexture2D>(4096, 4096);  // Large texture
    glm::vec2 coords(50, 50);
    glm::vec2 cellSize(64, 64);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(3200/4096, 3200/4096), max=(3264/4096, 3264/4096)
    float minCoord = 3200.0f / 4096.0f;
    float maxCoord = 3264.0f / 4096.0f;

    EXPECT_FLOAT_EQ(texCoords[0].x, minCoord);
    EXPECT_FLOAT_EQ(texCoords[0].y, minCoord);
    EXPECT_FLOAT_EQ(texCoords[2].x, maxCoord);
    EXPECT_FLOAT_EQ(texCoords[2].y, maxCoord);

    // Verify coordinates are still normalized
    EXPECT_GE(texCoords[2].x, 0.0f);
    EXPECT_LE(texCoords[2].x, 1.0f);
    EXPECT_GE(texCoords[2].y, 0.0f);
    EXPECT_LE(texCoords[2].y, 1.0f);
}

// =============================================================================
// SubTexture2D Corner Cases
// =============================================================================

class SubTexture2DCornerCasesTest : public ::testing::Test {};

TEST_F(SubTexture2DCornerCasesTest, DefaultSpriteSize_1x1) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);
    glm::vec2 coords(2, 2);
    glm::vec2 cellSize(64, 64);
    glm::vec2 spriteSize(1, 1);  // Explicit default

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize, spriteSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Should be same as not providing spriteSize parameter
    // Expected: min=(128/256, 128/256) = (0.5, 0.5), max=(192/256, 192/256) = (0.75, 0.75)
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.5f);
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.5f);
    EXPECT_FLOAT_EQ(texCoords[2].x, 0.75f);
    EXPECT_FLOAT_EQ(texCoords[2].y, 0.75f);
}

TEST_F(SubTexture2DCornerCasesTest, DefaultSpriteSize_ImplicitVsExplicit) {
    auto texture = std::make_shared<MockTexture2D>(256, 256);
    glm::vec2 coords(1, 1);
    glm::vec2 cellSize(32, 32);

    auto subTexture1 = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    auto subTexture2 = NxSubTexture2D::createFromCoords(texture, coords, cellSize, {1, 1});

    const glm::vec2* texCoords1 = subTexture1->getTextureCoords();
    const glm::vec2* texCoords2 = subTexture2->getTextureCoords();

    // Both should produce identical results
    for (int i = 0; i < 4; ++i) {
        EXPECT_FLOAT_EQ(texCoords1[i].x, texCoords2[i].x);
        EXPECT_FLOAT_EQ(texCoords1[i].y, texCoords2[i].y);
    }
}

TEST_F(SubTexture2DCornerCasesTest, LargeGridPosition_X) {
    auto texture = std::make_shared<MockTexture2D>(4096, 256);
    glm::vec2 coords(63, 0);     // Large X coordinate (last in 64 cell row)
    glm::vec2 cellSize(64, 64);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(4032/4096, 0), max=(1.0, 64/256)
    float minX = 4032.0f / 4096.0f;
    EXPECT_FLOAT_EQ(texCoords[0].x, minX);
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[2].x, 1.0f);
    EXPECT_FLOAT_EQ(texCoords[2].y, 0.25f);
}

TEST_F(SubTexture2DCornerCasesTest, LargeGridPosition_Y) {
    auto texture = std::make_shared<MockTexture2D>(256, 4096);
    glm::vec2 coords(0, 63);     // Large Y coordinate
    glm::vec2 cellSize(64, 64);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(0, 4032/4096), max=(64/256, 1.0)
    float minY = 4032.0f / 4096.0f;
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[0].y, minY);
    EXPECT_FLOAT_EQ(texCoords[2].x, 0.25f);
    EXPECT_FLOAT_EQ(texCoords[2].y, 1.0f);
}

TEST_F(SubTexture2DCornerCasesTest, LargeGridPosition_BothAxes) {
    auto texture = std::make_shared<MockTexture2D>(2048, 2048);
    glm::vec2 coords(31, 31);    // Large coordinates in both axes
    glm::vec2 cellSize(64, 64);

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(1984/2048, 1984/2048), max=(2048/2048, 2048/2048) = (1.0, 1.0)
    float minCoord = 1984.0f / 2048.0f;
    EXPECT_FLOAT_EQ(texCoords[0].x, minCoord);
    EXPECT_FLOAT_EQ(texCoords[0].y, minCoord);
    EXPECT_FLOAT_EQ(texCoords[2].x, 1.0f);
    EXPECT_FLOAT_EQ(texCoords[2].y, 1.0f);
}

TEST_F(SubTexture2DCornerCasesTest, AsymmetricSpriteSize) {
    auto texture = std::make_shared<MockTexture2D>(512, 512);
    glm::vec2 coords(0, 0);
    glm::vec2 cellSize(32, 32);
    glm::vec2 spriteSize(4, 2);  // 4 cells wide, 2 cells tall

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize, spriteSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(0,0), max=(128/512, 64/512) = (0.25, 0.125)
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.0f);
    EXPECT_FLOAT_EQ(texCoords[2].x, 0.25f);
    EXPECT_FLOAT_EQ(texCoords[2].y, 0.125f);
}

TEST_F(SubTexture2DCornerCasesTest, AsymmetricCellSize) {
    auto texture = std::make_shared<MockTexture2D>(320, 240);
    glm::vec2 coords(2, 1);
    glm::vec2 cellSize(80, 60);  // 4:3 aspect ratio cells

    auto subTexture = NxSubTexture2D::createFromCoords(texture, coords, cellSize);
    const glm::vec2* texCoords = subTexture->getTextureCoords();

    // Expected: min=(160/320, 60/240) = (0.5, 0.25), max=(240/320, 120/240) = (0.75, 0.5)
    EXPECT_FLOAT_EQ(texCoords[0].x, 0.5f);
    EXPECT_FLOAT_EQ(texCoords[0].y, 0.25f);
    EXPECT_FLOAT_EQ(texCoords[2].x, 0.75f);
    EXPECT_FLOAT_EQ(texCoords[2].y, 0.5f);
}

TEST_F(SubTexture2DCornerCasesTest, ZeroCoordinates_MultipleSpriteSizes) {
    auto texture = std::make_shared<MockTexture2D>(512, 512);
    glm::vec2 coords(0, 0);  // Always at origin
    glm::vec2 cellSize(64, 64);

    // Test different sprite sizes all starting at (0,0)
    auto sub1x1 = NxSubTexture2D::createFromCoords(texture, coords, cellSize, {1, 1});
    auto sub2x2 = NxSubTexture2D::createFromCoords(texture, coords, cellSize, {2, 2});
    auto sub3x3 = NxSubTexture2D::createFromCoords(texture, coords, cellSize, {3, 3});

    const glm::vec2* tc1 = sub1x1->getTextureCoords();
    const glm::vec2* tc2 = sub2x2->getTextureCoords();
    const glm::vec2* tc3 = sub3x3->getTextureCoords();

    // All should start at (0,0)
    EXPECT_FLOAT_EQ(tc1[0].x, 0.0f);
    EXPECT_FLOAT_EQ(tc2[0].x, 0.0f);
    EXPECT_FLOAT_EQ(tc3[0].x, 0.0f);

    // But end at different positions
    EXPECT_FLOAT_EQ(tc1[2].x, 0.125f);  // 64/512
    EXPECT_FLOAT_EQ(tc2[2].x, 0.25f);   // 128/512
    EXPECT_FLOAT_EQ(tc3[2].x, 0.375f);  // 192/512
}

}  // namespace nexo::renderer
