//// DrawCommand.test.cpp //////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for CommandType enum and DrawCommand structure
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/DrawCommand.hpp"
#include <limits>

namespace nexo::renderer {

// =============================================================================
// CommandType Enum Tests
// =============================================================================

class CommandTypeEnumTest : public ::testing::Test {};

TEST_F(CommandTypeEnumTest, MeshValueIs0) {
    EXPECT_EQ(static_cast<int>(CommandType::MESH), 0);
}

TEST_F(CommandTypeEnumTest, FullScreenValueIs1) {
    EXPECT_EQ(static_cast<int>(CommandType::FULL_SCREEN), 1);
}

TEST_F(CommandTypeEnumTest, ValuesAreDistinct) {
    EXPECT_NE(CommandType::MESH, CommandType::FULL_SCREEN);
}

// =============================================================================
// DrawCommand Default Values Tests
// =============================================================================

class DrawCommandDefaultsTest : public ::testing::Test {};

TEST_F(DrawCommandDefaultsTest, DefaultTypeIsMesh) {
    DrawCommand cmd;
    EXPECT_EQ(cmd.type, CommandType::MESH);
}

TEST_F(DrawCommandDefaultsTest, DefaultVaoIsNull) {
    DrawCommand cmd;
    EXPECT_EQ(cmd.vao, nullptr);
}

TEST_F(DrawCommandDefaultsTest, DefaultShaderIsNull) {
    DrawCommand cmd;
    EXPECT_EQ(cmd.shader, nullptr);
}

TEST_F(DrawCommandDefaultsTest, DefaultUniformsEmpty) {
    DrawCommand cmd;
    EXPECT_TRUE(cmd.uniforms.empty());
}

TEST_F(DrawCommandDefaultsTest, DefaultFilterMaskAllOnes) {
    DrawCommand cmd;
    EXPECT_EQ(cmd.filterMask, 0xFFFFFFFF);
}

TEST_F(DrawCommandDefaultsTest, DefaultIsOpaqueTrue) {
    DrawCommand cmd;
    EXPECT_TRUE(cmd.isOpaque);
}

// =============================================================================
// DrawCommand Field Modification Tests
// =============================================================================

class DrawCommandFieldsTest : public ::testing::Test {};

TEST_F(DrawCommandFieldsTest, SetTypeToFullScreen) {
    DrawCommand cmd;
    cmd.type = CommandType::FULL_SCREEN;
    EXPECT_EQ(cmd.type, CommandType::FULL_SCREEN);
}

TEST_F(DrawCommandFieldsTest, SetFilterMask) {
    DrawCommand cmd;
    cmd.filterMask = 0x0000000F;
    EXPECT_EQ(cmd.filterMask, 0x0000000Fu);
}

TEST_F(DrawCommandFieldsTest, SetFilterMaskToZero) {
    DrawCommand cmd;
    cmd.filterMask = 0;
    EXPECT_EQ(cmd.filterMask, 0u);
}

TEST_F(DrawCommandFieldsTest, SetIsOpaqueFalse) {
    DrawCommand cmd;
    cmd.isOpaque = false;
    EXPECT_FALSE(cmd.isOpaque);
}

TEST_F(DrawCommandFieldsTest, AddUniform) {
    DrawCommand cmd;
    cmd.uniforms["uTime"] = 1.5f;
    EXPECT_EQ(cmd.uniforms.size(), 1u);
    EXPECT_FLOAT_EQ(std::get<float>(cmd.uniforms["uTime"]), 1.5f);
}

TEST_F(DrawCommandFieldsTest, AddMultipleUniforms) {
    DrawCommand cmd;
    cmd.uniforms["uTime"] = 1.0f;
    cmd.uniforms["uSampler"] = 0;
    cmd.uniforms["uEnabled"] = true;
    EXPECT_EQ(cmd.uniforms.size(), 3u);
}

// =============================================================================
// DrawCommand Filter Mask Operations Tests
// =============================================================================

class DrawCommandFilterMaskTest : public ::testing::Test {};

TEST_F(DrawCommandFilterMaskTest, BitwiseAndOperation) {
    DrawCommand cmd;
    cmd.filterMask = 0xFF00FF00;
    uint32_t layer = 0x00FF0000;
    EXPECT_EQ(cmd.filterMask & layer, 0x00000000u);
}

TEST_F(DrawCommandFilterMaskTest, BitwiseAndMatch) {
    DrawCommand cmd;
    cmd.filterMask = 0xFF00FF00;
    uint32_t layer = 0xFF000000;
    EXPECT_EQ(cmd.filterMask & layer, 0xFF000000u);
}

TEST_F(DrawCommandFilterMaskTest, SingleBitMask) {
    DrawCommand cmd;
    cmd.filterMask = 0x00000001;  // Only layer 0
    EXPECT_TRUE((cmd.filterMask & 0x00000001) != 0);
    EXPECT_FALSE((cmd.filterMask & 0x00000002) != 0);
}

TEST_F(DrawCommandFilterMaskTest, MultipleBitMask) {
    DrawCommand cmd;
    cmd.filterMask = 0x0000000F;  // Layers 0-3
    EXPECT_TRUE((cmd.filterMask & 0x00000001) != 0);
    EXPECT_TRUE((cmd.filterMask & 0x00000002) != 0);
    EXPECT_TRUE((cmd.filterMask & 0x00000004) != 0);
    EXPECT_TRUE((cmd.filterMask & 0x00000008) != 0);
    EXPECT_FALSE((cmd.filterMask & 0x00000010) != 0);
}

// =============================================================================
// DrawCommand Copy Tests
// =============================================================================

class DrawCommandCopyTest : public ::testing::Test {};

TEST_F(DrawCommandCopyTest, CopyConstructor) {
    DrawCommand original;
    original.type = CommandType::FULL_SCREEN;
    original.filterMask = 0x12345678;
    original.isOpaque = false;
    original.uniforms["uTest"] = 42;

    DrawCommand copy = original;

    EXPECT_EQ(copy.type, CommandType::FULL_SCREEN);
    EXPECT_EQ(copy.filterMask, 0x12345678u);
    EXPECT_FALSE(copy.isOpaque);
    EXPECT_EQ(copy.uniforms.size(), 1u);
}

TEST_F(DrawCommandCopyTest, AssignmentOperator) {
    DrawCommand original;
    original.type = CommandType::FULL_SCREEN;
    original.filterMask = 0xABCDEF00;

    DrawCommand assigned;
    assigned = original;

    EXPECT_EQ(assigned.type, CommandType::FULL_SCREEN);
    EXPECT_EQ(assigned.filterMask, 0xABCDEF00u);
}

// =============================================================================
// DrawCommand Construction Tests
// =============================================================================

class DrawCommandConstructionTest : public ::testing::Test {};

TEST_F(DrawCommandConstructionTest, ConstructWithMeshType) {
    DrawCommand cmd{};
    cmd.type = CommandType::MESH;
    EXPECT_EQ(cmd.type, CommandType::MESH);
}

TEST_F(DrawCommandConstructionTest, ConstructWithFullScreenType) {
    DrawCommand cmd{};
    cmd.type = CommandType::FULL_SCREEN;
    EXPECT_EQ(cmd.type, CommandType::FULL_SCREEN);
}

TEST_F(DrawCommandConstructionTest, ConstructWithCustomFilterMask) {
    DrawCommand cmd{};
    cmd.filterMask = 0x00FF00FF;
    EXPECT_EQ(cmd.filterMask, 0x00FF00FFu);
}

TEST_F(DrawCommandConstructionTest, ConstructAsTransparent) {
    DrawCommand cmd{};
    cmd.isOpaque = false;
    EXPECT_FALSE(cmd.isOpaque);
}

TEST_F(DrawCommandConstructionTest, ConstructWithAllFieldsSet) {
    DrawCommand cmd{};
    cmd.type = CommandType::FULL_SCREEN;
    cmd.filterMask = 0x12345678;
    cmd.isOpaque = false;
    cmd.uniforms["uTest"] = 1.5f;

    EXPECT_EQ(cmd.type, CommandType::FULL_SCREEN);
    EXPECT_EQ(cmd.filterMask, 0x12345678u);
    EXPECT_FALSE(cmd.isOpaque);
    EXPECT_EQ(cmd.uniforms.size(), 1u);
}

// =============================================================================
// Uniform Variant Handling Tests
// =============================================================================

class UniformVariantTest : public ::testing::Test {};

TEST_F(UniformVariantTest, StoreFloatUniform) {
    DrawCommand cmd;
    cmd.uniforms["uTime"] = 3.14159f;

    ASSERT_TRUE(std::holds_alternative<float>(cmd.uniforms["uTime"]));
    EXPECT_FLOAT_EQ(std::get<float>(cmd.uniforms["uTime"]), 3.14159f);
}

TEST_F(UniformVariantTest, StoreVec2Uniform) {
    DrawCommand cmd;
    glm::vec2 vec(1.0f, 2.0f);
    cmd.uniforms["uPosition"] = vec;

    ASSERT_TRUE(std::holds_alternative<glm::vec2>(cmd.uniforms["uPosition"]));
    auto stored = std::get<glm::vec2>(cmd.uniforms["uPosition"]);
    EXPECT_FLOAT_EQ(stored.x, 1.0f);
    EXPECT_FLOAT_EQ(stored.y, 2.0f);
}

TEST_F(UniformVariantTest, StoreVec3Uniform) {
    DrawCommand cmd;
    glm::vec3 vec(1.0f, 2.0f, 3.0f);
    cmd.uniforms["uColor"] = vec;

    ASSERT_TRUE(std::holds_alternative<glm::vec3>(cmd.uniforms["uColor"]));
    auto stored = std::get<glm::vec3>(cmd.uniforms["uColor"]);
    EXPECT_FLOAT_EQ(stored.x, 1.0f);
    EXPECT_FLOAT_EQ(stored.y, 2.0f);
    EXPECT_FLOAT_EQ(stored.z, 3.0f);
}

TEST_F(UniformVariantTest, StoreVec4Uniform) {
    DrawCommand cmd;
    glm::vec4 vec(1.0f, 2.0f, 3.0f, 4.0f);
    cmd.uniforms["uColorAlpha"] = vec;

    ASSERT_TRUE(std::holds_alternative<glm::vec4>(cmd.uniforms["uColorAlpha"]));
    auto stored = std::get<glm::vec4>(cmd.uniforms["uColorAlpha"]);
    EXPECT_FLOAT_EQ(stored.x, 1.0f);
    EXPECT_FLOAT_EQ(stored.y, 2.0f);
    EXPECT_FLOAT_EQ(stored.z, 3.0f);
    EXPECT_FLOAT_EQ(stored.w, 4.0f);
}

TEST_F(UniformVariantTest, StoreIntUniform) {
    DrawCommand cmd;
    cmd.uniforms["uSampler"] = 5;

    ASSERT_TRUE(std::holds_alternative<int>(cmd.uniforms["uSampler"]));
    EXPECT_EQ(std::get<int>(cmd.uniforms["uSampler"]), 5);
}

TEST_F(UniformVariantTest, StoreBoolUniform) {
    DrawCommand cmd;
    cmd.uniforms["uEnabled"] = true;

    ASSERT_TRUE(std::holds_alternative<bool>(cmd.uniforms["uEnabled"]));
    EXPECT_TRUE(std::get<bool>(cmd.uniforms["uEnabled"]));
}

TEST_F(UniformVariantTest, StoreBoolUniformFalse) {
    DrawCommand cmd;
    cmd.uniforms["uDisabled"] = false;

    ASSERT_TRUE(std::holds_alternative<bool>(cmd.uniforms["uDisabled"]));
    EXPECT_FALSE(std::get<bool>(cmd.uniforms["uDisabled"]));
}

TEST_F(UniformVariantTest, StoreMat4Uniform) {
    DrawCommand cmd;
    glm::mat4 matrix = glm::mat4(1.0f);
    cmd.uniforms["uProjection"] = matrix;

    ASSERT_TRUE(std::holds_alternative<glm::mat4>(cmd.uniforms["uProjection"]));
    auto stored = std::get<glm::mat4>(cmd.uniforms["uProjection"]);
    EXPECT_FLOAT_EQ(stored[0][0], 1.0f);
    EXPECT_FLOAT_EQ(stored[1][1], 1.0f);
    EXPECT_FLOAT_EQ(stored[2][2], 1.0f);
    EXPECT_FLOAT_EQ(stored[3][3], 1.0f);
}

TEST_F(UniformVariantTest, StoreMultipleDifferentTypes) {
    DrawCommand cmd;
    cmd.uniforms["uTime"] = 1.5f;
    cmd.uniforms["uPosition"] = glm::vec2(1.0f, 2.0f);
    cmd.uniforms["uColor"] = glm::vec3(1.0f, 0.5f, 0.0f);
    cmd.uniforms["uTransform"] = glm::mat4(1.0f);
    cmd.uniforms["uSampler"] = 0;
    cmd.uniforms["uEnabled"] = true;

    EXPECT_EQ(cmd.uniforms.size(), 6u);
    ASSERT_TRUE(std::holds_alternative<float>(cmd.uniforms["uTime"]));
    ASSERT_TRUE(std::holds_alternative<glm::vec2>(cmd.uniforms["uPosition"]));
    ASSERT_TRUE(std::holds_alternative<glm::vec3>(cmd.uniforms["uColor"]));
    ASSERT_TRUE(std::holds_alternative<glm::mat4>(cmd.uniforms["uTransform"]));
    ASSERT_TRUE(std::holds_alternative<int>(cmd.uniforms["uSampler"]));
    ASSERT_TRUE(std::holds_alternative<bool>(cmd.uniforms["uEnabled"]));
}

TEST_F(UniformVariantTest, OverwriteUniformWithDifferentType) {
    DrawCommand cmd;
    cmd.uniforms["uValue"] = 5;
    ASSERT_TRUE(std::holds_alternative<int>(cmd.uniforms["uValue"]));

    cmd.uniforms["uValue"] = 3.14f;
    ASSERT_TRUE(std::holds_alternative<float>(cmd.uniforms["uValue"]));
    EXPECT_FLOAT_EQ(std::get<float>(cmd.uniforms["uValue"]), 3.14f);
}

TEST_F(UniformVariantTest, RemoveUniform) {
    DrawCommand cmd;
    cmd.uniforms["uTemp"] = 1.0f;
    EXPECT_EQ(cmd.uniforms.size(), 1u);

    cmd.uniforms.erase("uTemp");
    EXPECT_EQ(cmd.uniforms.size(), 0u);
}

TEST_F(UniformVariantTest, CheckUniformExists) {
    DrawCommand cmd;
    cmd.uniforms["uTest"] = 1.0f;

    EXPECT_TRUE(cmd.uniforms.contains("uTest"));
    EXPECT_FALSE(cmd.uniforms.contains("uNonExistent"));
}

TEST_F(UniformVariantTest, ZeroValueUniforms) {
    DrawCommand cmd;
    cmd.uniforms["uZeroFloat"] = 0.0f;
    cmd.uniforms["uZeroInt"] = 0;
    cmd.uniforms["uZeroVec"] = glm::vec3(0.0f);

    EXPECT_FLOAT_EQ(std::get<float>(cmd.uniforms["uZeroFloat"]), 0.0f);
    EXPECT_EQ(std::get<int>(cmd.uniforms["uZeroInt"]), 0);
    auto vec = std::get<glm::vec3>(cmd.uniforms["uZeroVec"]);
    EXPECT_FLOAT_EQ(vec.x, 0.0f);
    EXPECT_FLOAT_EQ(vec.y, 0.0f);
    EXPECT_FLOAT_EQ(vec.z, 0.0f);
}

TEST_F(UniformVariantTest, NegativeValueUniforms) {
    DrawCommand cmd;
    cmd.uniforms["uNegFloat"] = -5.5f;
    cmd.uniforms["uNegInt"] = -42;
    cmd.uniforms["uNegVec"] = glm::vec3(-1.0f, -2.0f, -3.0f);

    EXPECT_FLOAT_EQ(std::get<float>(cmd.uniforms["uNegFloat"]), -5.5f);
    EXPECT_EQ(std::get<int>(cmd.uniforms["uNegInt"]), -42);
    auto vec = std::get<glm::vec3>(cmd.uniforms["uNegVec"]);
    EXPECT_FLOAT_EQ(vec.x, -1.0f);
    EXPECT_FLOAT_EQ(vec.y, -2.0f);
    EXPECT_FLOAT_EQ(vec.z, -3.0f);
}

// =============================================================================
// Command Comparison Tests
// =============================================================================

class DrawCommandComparisonTest : public ::testing::Test {};

TEST_F(DrawCommandComparisonTest, TwoDefaultCommandsHaveSameValues) {
    DrawCommand cmd1;
    DrawCommand cmd2;

    EXPECT_EQ(cmd1.type, cmd2.type);
    EXPECT_EQ(cmd1.filterMask, cmd2.filterMask);
    EXPECT_EQ(cmd1.isOpaque, cmd2.isOpaque);
    EXPECT_EQ(cmd1.uniforms.size(), cmd2.uniforms.size());
}

TEST_F(DrawCommandComparisonTest, CommandsWithDifferentTypes) {
    DrawCommand cmd1;
    cmd1.type = CommandType::MESH;

    DrawCommand cmd2;
    cmd2.type = CommandType::FULL_SCREEN;

    EXPECT_NE(cmd1.type, cmd2.type);
}

TEST_F(DrawCommandComparisonTest, CommandsWithDifferentFilterMasks) {
    DrawCommand cmd1;
    cmd1.filterMask = 0x11111111;

    DrawCommand cmd2;
    cmd2.filterMask = 0x22222222;

    EXPECT_NE(cmd1.filterMask, cmd2.filterMask);
}

TEST_F(DrawCommandComparisonTest, CommandsWithDifferentOpacity) {
    DrawCommand cmd1;
    cmd1.isOpaque = true;

    DrawCommand cmd2;
    cmd2.isOpaque = false;

    EXPECT_NE(cmd1.isOpaque, cmd2.isOpaque);
}

TEST_F(DrawCommandComparisonTest, CommandsWithDifferentUniformCount) {
    DrawCommand cmd1;
    cmd1.uniforms["u1"] = 1.0f;

    DrawCommand cmd2;
    cmd2.uniforms["u1"] = 1.0f;
    cmd2.uniforms["u2"] = 2.0f;

    EXPECT_NE(cmd1.uniforms.size(), cmd2.uniforms.size());
}

// =============================================================================
// State Tracking Tests
// =============================================================================

class DrawCommandStateTest : public ::testing::Test {};

TEST_F(DrawCommandStateTest, DefaultStateIsValid) {
    DrawCommand cmd;

    EXPECT_EQ(cmd.type, CommandType::MESH);
    EXPECT_EQ(cmd.vao, nullptr);
    EXPECT_EQ(cmd.shader, nullptr);
    EXPECT_TRUE(cmd.uniforms.empty());
    EXPECT_EQ(cmd.filterMask, 0xFFFFFFFF);
    EXPECT_TRUE(cmd.isOpaque);
}

TEST_F(DrawCommandStateTest, MeshCommandRequiresVAO) {
    DrawCommand cmd;
    cmd.type = CommandType::MESH;

    // Mesh commands should have a VAO set in practice
    // (we can't create one here without OpenGL context)
    EXPECT_EQ(cmd.vao, nullptr);
}

TEST_F(DrawCommandStateTest, FullScreenCommandDoesNotRequireVAO) {
    DrawCommand cmd;
    cmd.type = CommandType::FULL_SCREEN;

    // Full screen commands use the built-in quad
    EXPECT_EQ(cmd.vao, nullptr);
}

TEST_F(DrawCommandStateTest, TransparentCommandIsOpaqueFalse) {
    DrawCommand cmd;
    cmd.isOpaque = false;

    EXPECT_FALSE(cmd.isOpaque);
}

TEST_F(DrawCommandStateTest, OpaqueCommandIsOpaqueTrue) {
    DrawCommand cmd;
    cmd.isOpaque = true;

    EXPECT_TRUE(cmd.isOpaque);
}

TEST_F(DrawCommandStateTest, FilterMaskAllBitsSet) {
    DrawCommand cmd;
    cmd.filterMask = 0xFFFFFFFF;

    // Should match all layers
    for (int i = 0; i < 32; i++) {
        uint32_t layer = 1u << i;
        EXPECT_TRUE((cmd.filterMask & layer) != 0);
    }
}

TEST_F(DrawCommandStateTest, FilterMaskNoBitsSet) {
    DrawCommand cmd;
    cmd.filterMask = 0x00000000;

    // Should match no layers
    for (int i = 0; i < 32; i++) {
        uint32_t layer = 1u << i;
        EXPECT_FALSE((cmd.filterMask & layer) != 0);
    }
}

TEST_F(DrawCommandStateTest, FilterMaskSpecificLayers) {
    DrawCommand cmd;
    cmd.filterMask = 0x00000005;  // Binary: 0101 (layers 0 and 2)

    EXPECT_TRUE((cmd.filterMask & 0x00000001) != 0);   // Layer 0
    EXPECT_FALSE((cmd.filterMask & 0x00000002) != 0);  // Layer 1
    EXPECT_TRUE((cmd.filterMask & 0x00000004) != 0);   // Layer 2
    EXPECT_FALSE((cmd.filterMask & 0x00000008) != 0);  // Layer 3
}

// =============================================================================
// Full Screen vs Mesh Command Tests
// =============================================================================

class CommandTypeDifferenceTest : public ::testing::Test {};

TEST_F(CommandTypeDifferenceTest, MeshCommandType) {
    DrawCommand meshCmd;
    meshCmd.type = CommandType::MESH;

    EXPECT_EQ(meshCmd.type, CommandType::MESH);
    EXPECT_NE(meshCmd.type, CommandType::FULL_SCREEN);
}

TEST_F(CommandTypeDifferenceTest, FullScreenCommandType) {
    DrawCommand fsCmd;
    fsCmd.type = CommandType::FULL_SCREEN;

    EXPECT_EQ(fsCmd.type, CommandType::FULL_SCREEN);
    EXPECT_NE(fsCmd.type, CommandType::MESH);
}

TEST_F(CommandTypeDifferenceTest, MeshCommandWithNullVAO) {
    DrawCommand cmd;
    cmd.type = CommandType::MESH;
    cmd.vao = nullptr;

    EXPECT_EQ(cmd.type, CommandType::MESH);
    EXPECT_EQ(cmd.vao, nullptr);
}

TEST_F(CommandTypeDifferenceTest, FullScreenCommandWithNullVAO) {
    DrawCommand cmd;
    cmd.type = CommandType::FULL_SCREEN;
    cmd.vao = nullptr;  // Full screen uses getFullscreenQuad()

    EXPECT_EQ(cmd.type, CommandType::FULL_SCREEN);
    EXPECT_EQ(cmd.vao, nullptr);
}

TEST_F(CommandTypeDifferenceTest, BothTypesCanHaveUniforms) {
    DrawCommand meshCmd;
    meshCmd.type = CommandType::MESH;
    meshCmd.uniforms["uTest"] = 1.0f;

    DrawCommand fsCmd;
    fsCmd.type = CommandType::FULL_SCREEN;
    fsCmd.uniforms["uTest"] = 1.0f;

    EXPECT_EQ(meshCmd.uniforms.size(), 1u);
    EXPECT_EQ(fsCmd.uniforms.size(), 1u);
}

TEST_F(CommandTypeDifferenceTest, BothTypesCanHaveFilterMask) {
    DrawCommand meshCmd;
    meshCmd.type = CommandType::MESH;
    meshCmd.filterMask = 0x12345678;

    DrawCommand fsCmd;
    fsCmd.type = CommandType::FULL_SCREEN;
    fsCmd.filterMask = 0x12345678;

    EXPECT_EQ(meshCmd.filterMask, 0x12345678u);
    EXPECT_EQ(fsCmd.filterMask, 0x12345678u);
}

TEST_F(CommandTypeDifferenceTest, BothTypesCanBeOpaqueOrTransparent) {
    DrawCommand meshOpaque;
    meshOpaque.type = CommandType::MESH;
    meshOpaque.isOpaque = true;

    DrawCommand meshTransparent;
    meshTransparent.type = CommandType::MESH;
    meshTransparent.isOpaque = false;

    DrawCommand fsOpaque;
    fsOpaque.type = CommandType::FULL_SCREEN;
    fsOpaque.isOpaque = true;

    DrawCommand fsTransparent;
    fsTransparent.type = CommandType::FULL_SCREEN;
    fsTransparent.isOpaque = false;

    EXPECT_TRUE(meshOpaque.isOpaque);
    EXPECT_FALSE(meshTransparent.isOpaque);
    EXPECT_TRUE(fsOpaque.isOpaque);
    EXPECT_FALSE(fsTransparent.isOpaque);
}

// =============================================================================
// Edge Cases Tests
// =============================================================================

class DrawCommandEdgeCasesTest : public ::testing::Test {};

TEST_F(DrawCommandEdgeCasesTest, EmptyUniforms) {
    DrawCommand cmd;
    EXPECT_TRUE(cmd.uniforms.empty());
    EXPECT_EQ(cmd.uniforms.size(), 0u);
}

TEST_F(DrawCommandEdgeCasesTest, NullShader) {
    DrawCommand cmd;
    cmd.shader = nullptr;
    EXPECT_EQ(cmd.shader, nullptr);
}

TEST_F(DrawCommandEdgeCasesTest, NullVAO) {
    DrawCommand cmd;
    cmd.vao = nullptr;
    EXPECT_EQ(cmd.vao, nullptr);
}

TEST_F(DrawCommandEdgeCasesTest, MaxFilterMask) {
    DrawCommand cmd;
    cmd.filterMask = 0xFFFFFFFF;
    EXPECT_EQ(cmd.filterMask, 0xFFFFFFFFu);
}

TEST_F(DrawCommandEdgeCasesTest, MinFilterMask) {
    DrawCommand cmd;
    cmd.filterMask = 0x00000000;
    EXPECT_EQ(cmd.filterMask, 0x00000000u);
}

TEST_F(DrawCommandEdgeCasesTest, VeryLargeUniformCount) {
    DrawCommand cmd;
    for (int i = 0; i < 100; i++) {
        cmd.uniforms["u" + std::to_string(i)] = static_cast<float>(i);
    }
    EXPECT_EQ(cmd.uniforms.size(), 100u);
}

TEST_F(DrawCommandEdgeCasesTest, UniformWithEmptyName) {
    DrawCommand cmd;
    cmd.uniforms[""] = 1.0f;
    EXPECT_EQ(cmd.uniforms.size(), 1u);
    EXPECT_TRUE(cmd.uniforms.contains(""));
}

TEST_F(DrawCommandEdgeCasesTest, UniformWithLongName) {
    DrawCommand cmd;
    std::string longName(1000, 'a');
    cmd.uniforms[longName] = 1.0f;
    EXPECT_EQ(cmd.uniforms.size(), 1u);
    EXPECT_TRUE(cmd.uniforms.contains(longName));
}

TEST_F(DrawCommandEdgeCasesTest, UniformWithSpecialCharacters) {
    DrawCommand cmd;
    cmd.uniforms["u_Test123!@#"] = 1.0f;
    EXPECT_EQ(cmd.uniforms.size(), 1u);
    EXPECT_TRUE(cmd.uniforms.contains("u_Test123!@#"));
}

TEST_F(DrawCommandEdgeCasesTest, FloatExtremeValues) {
    DrawCommand cmd;
    cmd.uniforms["uMax"] = std::numeric_limits<float>::max();
    cmd.uniforms["uMin"] = std::numeric_limits<float>::lowest();
    cmd.uniforms["uInf"] = std::numeric_limits<float>::infinity();

    EXPECT_FLOAT_EQ(std::get<float>(cmd.uniforms["uMax"]), std::numeric_limits<float>::max());
    EXPECT_FLOAT_EQ(std::get<float>(cmd.uniforms["uMin"]), std::numeric_limits<float>::lowest());
    EXPECT_FLOAT_EQ(std::get<float>(cmd.uniforms["uInf"]), std::numeric_limits<float>::infinity());
}

TEST_F(DrawCommandEdgeCasesTest, IntExtremeValues) {
    DrawCommand cmd;
    cmd.uniforms["uMaxInt"] = std::numeric_limits<int>::max();
    cmd.uniforms["uMinInt"] = std::numeric_limits<int>::min();

    EXPECT_EQ(std::get<int>(cmd.uniforms["uMaxInt"]), std::numeric_limits<int>::max());
    EXPECT_EQ(std::get<int>(cmd.uniforms["uMinInt"]), std::numeric_limits<int>::min());
}

TEST_F(DrawCommandEdgeCasesTest, MultipleCommandsIndependent) {
    DrawCommand cmd1;
    cmd1.uniforms["uTest"] = 1.0f;

    DrawCommand cmd2;
    cmd2.uniforms["uTest"] = 2.0f;

    EXPECT_FLOAT_EQ(std::get<float>(cmd1.uniforms["uTest"]), 1.0f);
    EXPECT_FLOAT_EQ(std::get<float>(cmd2.uniforms["uTest"]), 2.0f);
}

TEST_F(DrawCommandEdgeCasesTest, ClearAllUniforms) {
    DrawCommand cmd;
    cmd.uniforms["u1"] = 1.0f;
    cmd.uniforms["u2"] = 2.0f;
    cmd.uniforms["u3"] = 3.0f;
    EXPECT_EQ(cmd.uniforms.size(), 3u);

    cmd.uniforms.clear();
    EXPECT_EQ(cmd.uniforms.size(), 0u);
    EXPECT_TRUE(cmd.uniforms.empty());
}

TEST_F(DrawCommandEdgeCasesTest, ReplaceAllFieldsMultipleTimes) {
    DrawCommand cmd;

    for (int i = 0; i < 10; i++) {
        cmd.type = (i % 2 == 0) ? CommandType::MESH : CommandType::FULL_SCREEN;
        cmd.filterMask = i;
        cmd.isOpaque = (i % 2 == 0);
        cmd.uniforms.clear();
        cmd.uniforms["u"] = static_cast<float>(i);
    }

    EXPECT_EQ(cmd.type, CommandType::FULL_SCREEN);
    EXPECT_EQ(cmd.filterMask, 9u);
    EXPECT_FALSE(cmd.isOpaque);
    EXPECT_EQ(cmd.uniforms.size(), 1u);
    EXPECT_FLOAT_EQ(std::get<float>(cmd.uniforms["u"]), 9.0f);
}

TEST_F(DrawCommandEdgeCasesTest, Mat4IdentityMatrix) {
    DrawCommand cmd;
    glm::mat4 identity = glm::mat4(1.0f);
    cmd.uniforms["uIdentity"] = identity;

    auto stored = std::get<glm::mat4>(cmd.uniforms["uIdentity"]);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                EXPECT_FLOAT_EQ(stored[i][j], 1.0f);
            } else {
                EXPECT_FLOAT_EQ(stored[i][j], 0.0f);
            }
        }
    }
}

TEST_F(DrawCommandEdgeCasesTest, Mat4ZeroMatrix) {
    DrawCommand cmd;
    glm::mat4 zero = glm::mat4(0.0f);
    cmd.uniforms["uZero"] = zero;

    auto stored = std::get<glm::mat4>(cmd.uniforms["uZero"]);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_FLOAT_EQ(stored[i][j], 0.0f);
        }
    }
}

}  // namespace nexo::renderer
