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

}  // namespace nexo::renderer
