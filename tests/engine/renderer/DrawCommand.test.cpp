//// DrawCommand.test.cpp //////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for CommandType enum and DrawCommand structure
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/DrawCommand.hpp"

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

}  // namespace nexo::renderer
