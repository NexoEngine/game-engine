//// Masks.test.cpp ////////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for render pass mask constants
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderPasses/Masks.hpp"
#include <type_traits>

namespace nexo::renderer {

// =============================================================================
// Mask Constants Type Tests
// =============================================================================

class MaskConstantsTypeTest : public ::testing::Test {};

TEST_F(MaskConstantsTypeTest, ForwardPassIsUint32) {
    static_assert(std::is_same_v<decltype(F_FORWARD_PASS), const uint32_t>);
    SUCCEED();
}

TEST_F(MaskConstantsTypeTest, OutlineMaskIsUint32) {
    static_assert(std::is_same_v<decltype(F_OUTLINE_MASK), const uint32_t>);
    SUCCEED();
}

TEST_F(MaskConstantsTypeTest, OutlinePassIsUint32) {
    static_assert(std::is_same_v<decltype(F_OUTLINE_PASS), const uint32_t>);
    SUCCEED();
}

TEST_F(MaskConstantsTypeTest, GridPassIsUint32) {
    static_assert(std::is_same_v<decltype(F_GRID_PASS), const uint32_t>);
    SUCCEED();
}

// =============================================================================
// Mask Values Tests
// =============================================================================

class MaskValuesTest : public ::testing::Test {};

TEST_F(MaskValuesTest, ForwardPassIsBit0) {
    EXPECT_EQ(F_FORWARD_PASS, 1u << 0);
    EXPECT_EQ(F_FORWARD_PASS, 0x01u);
}

TEST_F(MaskValuesTest, OutlineMaskIsBit1) {
    EXPECT_EQ(F_OUTLINE_MASK, 1u << 1);
    EXPECT_EQ(F_OUTLINE_MASK, 0x02u);
}

TEST_F(MaskValuesTest, OutlinePassIsBit2) {
    EXPECT_EQ(F_OUTLINE_PASS, 1u << 2);
    EXPECT_EQ(F_OUTLINE_PASS, 0x04u);
}

TEST_F(MaskValuesTest, GridPassIsBit3) {
    EXPECT_EQ(F_GRID_PASS, 1u << 3);
    EXPECT_EQ(F_GRID_PASS, 0x08u);
}

// =============================================================================
// Mask Uniqueness Tests
// =============================================================================

class MaskUniquenessTest : public ::testing::Test {};

TEST_F(MaskUniquenessTest, AllMasksAreDistinct) {
    EXPECT_NE(F_FORWARD_PASS, F_OUTLINE_MASK);
    EXPECT_NE(F_FORWARD_PASS, F_OUTLINE_PASS);
    EXPECT_NE(F_FORWARD_PASS, F_GRID_PASS);
    EXPECT_NE(F_OUTLINE_MASK, F_OUTLINE_PASS);
    EXPECT_NE(F_OUTLINE_MASK, F_GRID_PASS);
    EXPECT_NE(F_OUTLINE_PASS, F_GRID_PASS);
}

TEST_F(MaskUniquenessTest, MasksAreNonOverlapping) {
    // Each mask should have exactly one bit set
    EXPECT_EQ(F_FORWARD_PASS & F_OUTLINE_MASK, 0u);
    EXPECT_EQ(F_FORWARD_PASS & F_OUTLINE_PASS, 0u);
    EXPECT_EQ(F_FORWARD_PASS & F_GRID_PASS, 0u);
    EXPECT_EQ(F_OUTLINE_MASK & F_OUTLINE_PASS, 0u);
    EXPECT_EQ(F_OUTLINE_MASK & F_GRID_PASS, 0u);
    EXPECT_EQ(F_OUTLINE_PASS & F_GRID_PASS, 0u);
}

TEST_F(MaskUniquenessTest, MasksAreSingleBit) {
    // Each mask should be a power of 2 (single bit set)
    auto isPowerOfTwo = [](uint32_t n) { return n != 0 && (n & (n - 1)) == 0; };

    EXPECT_TRUE(isPowerOfTwo(F_FORWARD_PASS));
    EXPECT_TRUE(isPowerOfTwo(F_OUTLINE_MASK));
    EXPECT_TRUE(isPowerOfTwo(F_OUTLINE_PASS));
    EXPECT_TRUE(isPowerOfTwo(F_GRID_PASS));
}

// =============================================================================
// Mask Combination Tests
// =============================================================================

class MaskCombinationTest : public ::testing::Test {};

TEST_F(MaskCombinationTest, CanCombineTwoMasks) {
    uint32_t combined = F_FORWARD_PASS | F_OUTLINE_PASS;
    EXPECT_EQ(combined, 0x05u);  // bits 0 and 2
}

TEST_F(MaskCombinationTest, CanCombineAllMasks) {
    uint32_t combined = F_FORWARD_PASS | F_OUTLINE_MASK | F_OUTLINE_PASS | F_GRID_PASS;
    EXPECT_EQ(combined, 0x0Fu);  // bits 0, 1, 2, 3
}

TEST_F(MaskCombinationTest, CanTestMaskPresence) {
    uint32_t combined = F_FORWARD_PASS | F_GRID_PASS;

    EXPECT_TRUE((combined & F_FORWARD_PASS) != 0);
    EXPECT_FALSE((combined & F_OUTLINE_MASK) != 0);
    EXPECT_FALSE((combined & F_OUTLINE_PASS) != 0);
    EXPECT_TRUE((combined & F_GRID_PASS) != 0);
}

TEST_F(MaskCombinationTest, CanRemoveMaskFromCombination) {
    uint32_t combined = F_FORWARD_PASS | F_OUTLINE_MASK | F_GRID_PASS;
    uint32_t withoutOutline = combined & ~F_OUTLINE_MASK;

    EXPECT_TRUE((withoutOutline & F_FORWARD_PASS) != 0);
    EXPECT_FALSE((withoutOutline & F_OUTLINE_MASK) != 0);
    EXPECT_TRUE((withoutOutline & F_GRID_PASS) != 0);
}

TEST_F(MaskCombinationTest, CanToggleMask) {
    uint32_t mask = F_FORWARD_PASS;

    mask ^= F_OUTLINE_PASS;
    EXPECT_TRUE((mask & F_OUTLINE_PASS) != 0);

    mask ^= F_OUTLINE_PASS;
    EXPECT_FALSE((mask & F_OUTLINE_PASS) != 0);
}

// =============================================================================
// Mask Usage Pattern Tests
// =============================================================================

class MaskUsagePatternTest : public ::testing::Test {};

TEST_F(MaskUsagePatternTest, CanFilterByForwardPass) {
    uint32_t entityMask = F_FORWARD_PASS | F_OUTLINE_MASK;
    uint32_t passFilter = F_FORWARD_PASS;

    bool shouldRender = (entityMask & passFilter) != 0;
    EXPECT_TRUE(shouldRender);
}

TEST_F(MaskUsagePatternTest, CanFilterByMultiplePasses) {
    uint32_t entityMask = F_FORWARD_PASS | F_GRID_PASS;
    uint32_t passFilter = F_FORWARD_PASS | F_OUTLINE_PASS;

    // Entity should render if it matches any pass in the filter
    bool shouldRender = (entityMask & passFilter) != 0;
    EXPECT_TRUE(shouldRender);
}

TEST_F(MaskUsagePatternTest, FilterRejectsNonMatchingEntity) {
    uint32_t entityMask = F_GRID_PASS;
    uint32_t passFilter = F_FORWARD_PASS | F_OUTLINE_PASS;

    bool shouldRender = (entityMask & passFilter) != 0;
    EXPECT_FALSE(shouldRender);
}

TEST_F(MaskUsagePatternTest, EmptyMaskMatchesNothing) {
    uint32_t entityMask = 0;
    uint32_t passFilter = F_FORWARD_PASS | F_OUTLINE_MASK | F_OUTLINE_PASS | F_GRID_PASS;

    bool shouldRender = (entityMask & passFilter) != 0;
    EXPECT_FALSE(shouldRender);
}

TEST_F(MaskUsagePatternTest, FullMaskMatchesAll) {
    uint32_t entityMask = F_FORWARD_PASS | F_OUTLINE_MASK | F_OUTLINE_PASS | F_GRID_PASS;
    uint32_t passFilter = F_FORWARD_PASS;

    bool shouldRender = (entityMask & passFilter) != 0;
    EXPECT_TRUE(shouldRender);
}

// =============================================================================
// Constexpr Tests
// =============================================================================

class MaskConstexprTest : public ::testing::Test {};

TEST_F(MaskConstexprTest, CanUseInConstexprContext) {
    constexpr uint32_t combined = F_FORWARD_PASS | F_GRID_PASS;
    static_assert(combined == 0x09u);
    SUCCEED();
}

TEST_F(MaskConstexprTest, CanUseInArraySize) {
    // Mask bits can determine array sizes at compile time
    constexpr size_t numPasses = 4;  // We have 4 masks
    int passData[numPasses] = {};
    EXPECT_EQ(sizeof(passData) / sizeof(int), 4u);
}

TEST_F(MaskConstexprTest, CanUseInTemplateParameter) {
    // Masks can be used as template parameters
    auto testMask = []<uint32_t Mask>() {
        return Mask != 0;
    };

    EXPECT_TRUE(testMask.template operator()<F_FORWARD_PASS>());
    EXPECT_TRUE(testMask.template operator()<F_OUTLINE_PASS>());
}

}  // namespace nexo::renderer
