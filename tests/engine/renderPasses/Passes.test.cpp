//// Passes.test.cpp ////////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for render pass type enum
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderPasses/Passes.hpp"
#include <type_traits>
#include <set>

namespace nexo::renderer {

// =============================================================================
// Passes Enum Type Tests
// =============================================================================

class PassesEnumTypeTest : public ::testing::Test {};

TEST_F(PassesEnumTypeTest, IsEnum) {
    static_assert(std::is_enum_v<Passes>);
    SUCCEED();
}

TEST_F(PassesEnumTypeTest, UnderlyingTypeIsPassId) {
    static_assert(std::is_same_v<std::underlying_type_t<Passes>, PassId>);
    SUCCEED();
}

TEST_F(PassesEnumTypeTest, PassIdIsUint32) {
    static_assert(std::is_same_v<PassId, uint32_t>);
    SUCCEED();
}

// =============================================================================
// Passes Enum Values Tests
// =============================================================================

class PassesEnumValuesTest : public ::testing::Test {};

TEST_F(PassesEnumValuesTest, ForwardIsValue0) {
    EXPECT_EQ(static_cast<PassId>(Passes::FORWARD), 0u);
}

TEST_F(PassesEnumValuesTest, GridIsValue1) {
    EXPECT_EQ(static_cast<PassId>(Passes::GRID), 1u);
}

TEST_F(PassesEnumValuesTest, MaskIsValue2) {
    EXPECT_EQ(static_cast<PassId>(Passes::MASK), 2u);
}

TEST_F(PassesEnumValuesTest, OutlineIsValue3) {
    EXPECT_EQ(static_cast<PassId>(Passes::OUTLINE), 3u);
}

TEST_F(PassesEnumValuesTest, NbPassesIsValue4) {
    EXPECT_EQ(static_cast<PassId>(Passes::NB_PASSES), 4u);
}

// =============================================================================
// Passes Count Tests
// =============================================================================

class PassesCountTest : public ::testing::Test {};

TEST_F(PassesCountTest, NbPassesCountsAllPasses) {
    // NB_PASSES should equal the number of actual passes
    EXPECT_EQ(Passes::NB_PASSES, 4u);
}

TEST_F(PassesCountTest, NbPassesGreaterThanZero) {
    EXPECT_GT(Passes::NB_PASSES, 0u);
}

TEST_F(PassesCountTest, CanUseNbPassesForArraySize) {
    // NB_PASSES should be usable as array size
    int passData[Passes::NB_PASSES] = {};
    EXPECT_EQ(sizeof(passData) / sizeof(int), static_cast<size_t>(Passes::NB_PASSES));
}

// =============================================================================
// Passes Uniqueness Tests
// =============================================================================

class PassesUniquenessTest : public ::testing::Test {};

TEST_F(PassesUniquenessTest, AllPassesAreDistinct) {
    std::set<PassId> values;
    values.insert(Passes::FORWARD);
    values.insert(Passes::GRID);
    values.insert(Passes::MASK);
    values.insert(Passes::OUTLINE);

    // All 4 passes should be distinct
    EXPECT_EQ(values.size(), 4u);
}

TEST_F(PassesUniquenessTest, PassesAreSequential) {
    EXPECT_EQ(Passes::GRID, Passes::FORWARD + 1);
    EXPECT_EQ(Passes::MASK, Passes::GRID + 1);
    EXPECT_EQ(Passes::OUTLINE, Passes::MASK + 1);
    EXPECT_EQ(Passes::NB_PASSES, Passes::OUTLINE + 1);
}

// =============================================================================
// Passes Conversion Tests
// =============================================================================

class PassesConversionTest : public ::testing::Test {};

TEST_F(PassesConversionTest, CanConvertToPassId) {
    PassId id = static_cast<PassId>(Passes::FORWARD);
    EXPECT_EQ(id, 0u);
}

TEST_F(PassesConversionTest, CanConvertFromPassId) {
    PassId id = 2;
    Passes pass = static_cast<Passes>(id);
    EXPECT_EQ(pass, Passes::MASK);
}

TEST_F(PassesConversionTest, RoundTripConversion) {
    for (PassId i = 0; i < Passes::NB_PASSES; ++i) {
        Passes pass = static_cast<Passes>(i);
        PassId converted = static_cast<PassId>(pass);
        EXPECT_EQ(converted, i);
    }
}

// =============================================================================
// Passes Iteration Tests
// =============================================================================

class PassesIterationTest : public ::testing::Test {};

TEST_F(PassesIterationTest, CanIterateOverAllPasses) {
    int count = 0;
    for (PassId i = 0; i < Passes::NB_PASSES; ++i) {
        count++;
    }
    EXPECT_EQ(count, 4);
}

TEST_F(PassesIterationTest, CanUseInSwitchStatement) {
    auto getPassName = [](Passes pass) -> std::string {
        switch (pass) {
            case Passes::FORWARD: return "FORWARD";
            case Passes::GRID: return "GRID";
            case Passes::MASK: return "MASK";
            case Passes::OUTLINE: return "OUTLINE";
            case Passes::NB_PASSES: return "NB_PASSES";
            default: return "UNKNOWN";
        }
    };

    EXPECT_EQ(getPassName(Passes::FORWARD), "FORWARD");
    EXPECT_EQ(getPassName(Passes::GRID), "GRID");
    EXPECT_EQ(getPassName(Passes::MASK), "MASK");
    EXPECT_EQ(getPassName(Passes::OUTLINE), "OUTLINE");
}

// =============================================================================
// Passes Comparison Tests
// =============================================================================

class PassesComparisonTest : public ::testing::Test {};

TEST_F(PassesComparisonTest, CanCompareEquality) {
    EXPECT_EQ(Passes::FORWARD, Passes::FORWARD);
    EXPECT_NE(Passes::FORWARD, Passes::GRID);
}

TEST_F(PassesComparisonTest, CanCompareOrdering) {
    EXPECT_LT(Passes::FORWARD, Passes::GRID);
    EXPECT_LT(Passes::GRID, Passes::MASK);
    EXPECT_LT(Passes::MASK, Passes::OUTLINE);
    EXPECT_LT(Passes::OUTLINE, Passes::NB_PASSES);
}

TEST_F(PassesComparisonTest, ForwardIsSmallest) {
    EXPECT_LE(Passes::FORWARD, Passes::GRID);
    EXPECT_LE(Passes::FORWARD, Passes::MASK);
    EXPECT_LE(Passes::FORWARD, Passes::OUTLINE);
}

// =============================================================================
// Passes Usage Pattern Tests
// =============================================================================

class PassesUsagePatternTest : public ::testing::Test {};

TEST_F(PassesUsagePatternTest, CanUseAsArrayIndex) {
    int passData[Passes::NB_PASSES] = {10, 20, 30, 40};

    EXPECT_EQ(passData[Passes::FORWARD], 10);
    EXPECT_EQ(passData[Passes::GRID], 20);
    EXPECT_EQ(passData[Passes::MASK], 30);
    EXPECT_EQ(passData[Passes::OUTLINE], 40);
}

TEST_F(PassesUsagePatternTest, CanValidatePassId) {
    auto isValidPass = [](PassId id) {
        return id < Passes::NB_PASSES;
    };

    EXPECT_TRUE(isValidPass(Passes::FORWARD));
    EXPECT_TRUE(isValidPass(Passes::GRID));
    EXPECT_TRUE(isValidPass(Passes::MASK));
    EXPECT_TRUE(isValidPass(Passes::OUTLINE));
    EXPECT_FALSE(isValidPass(Passes::NB_PASSES));
    EXPECT_FALSE(isValidPass(100));
}

TEST_F(PassesUsagePatternTest, CanIterateAndProcess) {
    std::vector<std::string> passNames;
    const char* names[] = {"FORWARD", "GRID", "MASK", "OUTLINE"};

    for (PassId i = 0; i < Passes::NB_PASSES; ++i) {
        passNames.push_back(names[i]);
    }

    EXPECT_EQ(passNames.size(), 4u);
    EXPECT_EQ(passNames[0], "FORWARD");
    EXPECT_EQ(passNames[3], "OUTLINE");
}

// =============================================================================
// Constexpr Tests
// =============================================================================

class PassesConstexprTest : public ::testing::Test {};

TEST_F(PassesConstexprTest, ValuesAreConstexpr) {
    constexpr PassId forward = Passes::FORWARD;
    constexpr PassId grid = Passes::GRID;
    constexpr PassId mask = Passes::MASK;
    constexpr PassId outline = Passes::OUTLINE;
    constexpr PassId nbPasses = Passes::NB_PASSES;

    static_assert(forward == 0);
    static_assert(grid == 1);
    static_assert(mask == 2);
    static_assert(outline == 3);
    static_assert(nbPasses == 4);

    SUCCEED();
}

TEST_F(PassesConstexprTest, CanUseInStaticAssert) {
    static_assert(Passes::NB_PASSES > 0);
    static_assert(Passes::FORWARD < Passes::NB_PASSES);
    static_assert(Passes::OUTLINE < Passes::NB_PASSES);

    SUCCEED();
}

}  // namespace nexo::renderer
