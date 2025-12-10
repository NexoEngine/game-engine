//// Buffer.test.cpp //////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for NxShaderDataType, NxBufferElements, and NxBufferLayout
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/Buffer.hpp"

namespace nexo::renderer {

// =============================================================================
// NxShaderDataType Enum Tests
// =============================================================================

class ShaderDataTypeEnumTest : public ::testing::Test {};

TEST_F(ShaderDataTypeEnumTest, NoneValueIsZero) {
    EXPECT_EQ(static_cast<int>(NxShaderDataType::NONE), 0);
}

TEST_F(ShaderDataTypeEnumTest, FloatTypesExist) {
    EXPECT_EQ(static_cast<int>(NxShaderDataType::FLOAT), 1);
    EXPECT_EQ(static_cast<int>(NxShaderDataType::FLOAT2), 2);
    EXPECT_EQ(static_cast<int>(NxShaderDataType::FLOAT3), 3);
    EXPECT_EQ(static_cast<int>(NxShaderDataType::FLOAT4), 4);
}

TEST_F(ShaderDataTypeEnumTest, MatrixTypesExist) {
    EXPECT_EQ(static_cast<int>(NxShaderDataType::MAT3), 5);
    EXPECT_EQ(static_cast<int>(NxShaderDataType::MAT4), 6);
}

TEST_F(ShaderDataTypeEnumTest, IntTypesExist) {
    EXPECT_EQ(static_cast<int>(NxShaderDataType::INT), 7);
    EXPECT_EQ(static_cast<int>(NxShaderDataType::INT2), 8);
    EXPECT_EQ(static_cast<int>(NxShaderDataType::INT3), 9);
    EXPECT_EQ(static_cast<int>(NxShaderDataType::INT4), 10);
}

TEST_F(ShaderDataTypeEnumTest, BoolTypeExists) {
    EXPECT_EQ(static_cast<int>(NxShaderDataType::BOOL), 11);
}

// =============================================================================
// shaderDataTypeSize Function Tests
// =============================================================================

class ShaderDataTypeSizeTest : public ::testing::Test {};

TEST_F(ShaderDataTypeSizeTest, NoneReturnsZero) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::NONE), 0u);
}

TEST_F(ShaderDataTypeSizeTest, FloatReturns4) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::FLOAT), 4u);
}

TEST_F(ShaderDataTypeSizeTest, Float2Returns8) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::FLOAT2), 8u);
}

TEST_F(ShaderDataTypeSizeTest, Float3Returns12) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::FLOAT3), 12u);
}

TEST_F(ShaderDataTypeSizeTest, Float4Returns16) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::FLOAT4), 16u);
}

TEST_F(ShaderDataTypeSizeTest, Mat3Returns36) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::MAT3), 36u);
}

TEST_F(ShaderDataTypeSizeTest, Mat4Returns64) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::MAT4), 64u);
}

TEST_F(ShaderDataTypeSizeTest, IntReturns4) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT), 4u);
}

TEST_F(ShaderDataTypeSizeTest, Int2Returns8) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT2), 8u);
}

TEST_F(ShaderDataTypeSizeTest, Int3Returns12) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT3), 12u);
}

TEST_F(ShaderDataTypeSizeTest, Int4Returns16) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT4), 16u);
}

TEST_F(ShaderDataTypeSizeTest, BoolReturns1) {
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::BOOL), 1u);
}

// =============================================================================
// NxBufferElements Tests
// =============================================================================

class BufferElementsTest : public ::testing::Test {};

TEST_F(BufferElementsTest, DefaultConstructor) {
    NxBufferElements elem;
    EXPECT_TRUE(elem.name.empty());
    EXPECT_EQ(elem.type, NxShaderDataType::NONE);
    EXPECT_EQ(elem.size, 0u);
    EXPECT_EQ(elem.offset, 0u);
    EXPECT_FALSE(elem.normalized);
}

TEST_F(BufferElementsTest, ConstructorWithFloat3) {
    NxBufferElements elem(NxShaderDataType::FLOAT3, "aPosition");
    EXPECT_EQ(elem.name, "aPosition");
    EXPECT_EQ(elem.type, NxShaderDataType::FLOAT3);
    EXPECT_EQ(elem.size, 12u);
    EXPECT_EQ(elem.offset, 0u);
    EXPECT_FALSE(elem.normalized);
}

TEST_F(BufferElementsTest, ConstructorWithNormalized) {
    NxBufferElements elem(NxShaderDataType::FLOAT4, "aColor", true);
    EXPECT_EQ(elem.name, "aColor");
    EXPECT_EQ(elem.type, NxShaderDataType::FLOAT4);
    EXPECT_EQ(elem.size, 16u);
    EXPECT_TRUE(elem.normalized);
}

TEST_F(BufferElementsTest, GetComponentCountFloat) {
    NxBufferElements elem(NxShaderDataType::FLOAT, "test");
    EXPECT_EQ(elem.getComponentCount(), 1u);
}

TEST_F(BufferElementsTest, GetComponentCountFloat2) {
    NxBufferElements elem(NxShaderDataType::FLOAT2, "test");
    EXPECT_EQ(elem.getComponentCount(), 2u);
}

TEST_F(BufferElementsTest, GetComponentCountFloat3) {
    NxBufferElements elem(NxShaderDataType::FLOAT3, "test");
    EXPECT_EQ(elem.getComponentCount(), 3u);
}

TEST_F(BufferElementsTest, GetComponentCountFloat4) {
    NxBufferElements elem(NxShaderDataType::FLOAT4, "test");
    EXPECT_EQ(elem.getComponentCount(), 4u);
}

TEST_F(BufferElementsTest, GetComponentCountInt) {
    NxBufferElements elem(NxShaderDataType::INT, "test");
    EXPECT_EQ(elem.getComponentCount(), 1u);
}

TEST_F(BufferElementsTest, GetComponentCountInt2) {
    NxBufferElements elem(NxShaderDataType::INT2, "test");
    EXPECT_EQ(elem.getComponentCount(), 2u);
}

TEST_F(BufferElementsTest, GetComponentCountInt3) {
    NxBufferElements elem(NxShaderDataType::INT3, "test");
    EXPECT_EQ(elem.getComponentCount(), 3u);
}

TEST_F(BufferElementsTest, GetComponentCountInt4) {
    NxBufferElements elem(NxShaderDataType::INT4, "test");
    EXPECT_EQ(elem.getComponentCount(), 4u);
}

TEST_F(BufferElementsTest, GetComponentCountMat3) {
    NxBufferElements elem(NxShaderDataType::MAT3, "test");
    EXPECT_EQ(elem.getComponentCount(), 9u);
}

TEST_F(BufferElementsTest, GetComponentCountMat4) {
    NxBufferElements elem(NxShaderDataType::MAT4, "test");
    EXPECT_EQ(elem.getComponentCount(), 16u);
}

TEST_F(BufferElementsTest, GetComponentCountBool) {
    NxBufferElements elem(NxShaderDataType::BOOL, "test");
    EXPECT_EQ(elem.getComponentCount(), 1u);
}

// =============================================================================
// NxBufferLayout Tests
// =============================================================================

class BufferLayoutTest : public ::testing::Test {};

TEST_F(BufferLayoutTest, DefaultConstructor) {
    NxBufferLayout layout;
    EXPECT_TRUE(layout.getElements().empty());
    EXPECT_EQ(layout.getStride(), 0u);
}

TEST_F(BufferLayoutTest, SingleElementLayout) {
    NxBufferLayout layout({
        {NxShaderDataType::FLOAT3, "aPosition"}
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 1u);
    EXPECT_EQ(elements[0].name, "aPosition");
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(layout.getStride(), 12u);
}

TEST_F(BufferLayoutTest, MultipleElementsLayout) {
    NxBufferLayout layout({
        {NxShaderDataType::FLOAT3, "aPosition"},
        {NxShaderDataType::FLOAT3, "aNormal"},
        {NxShaderDataType::FLOAT2, "aTexCoord"}
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 3u);

    EXPECT_EQ(elements[0].name, "aPosition");
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(elements[0].size, 12u);

    EXPECT_EQ(elements[1].name, "aNormal");
    EXPECT_EQ(elements[1].offset, 12u);
    EXPECT_EQ(elements[1].size, 12u);

    EXPECT_EQ(elements[2].name, "aTexCoord");
    EXPECT_EQ(elements[2].offset, 24u);
    EXPECT_EQ(elements[2].size, 8u);

    EXPECT_EQ(layout.getStride(), 32u);
}

TEST_F(BufferLayoutTest, StrideCalculation) {
    NxBufferLayout layout({
        {NxShaderDataType::FLOAT4, "aPosition"},  // 16
        {NxShaderDataType::FLOAT4, "aColor"},     // 16
        {NxShaderDataType::FLOAT2, "aTexCoord"}   // 8
    });
    EXPECT_EQ(layout.getStride(), 40u);
}

TEST_F(BufferLayoutTest, IteratorBeginEnd) {
    NxBufferLayout layout({
        {NxShaderDataType::FLOAT3, "aPosition"},
        {NxShaderDataType::FLOAT2, "aTexCoord"}
    });

    int count = 0;
    for ([[maybe_unused]] auto& elem : layout) {
        count++;
    }
    EXPECT_EQ(count, 2);
}

TEST_F(BufferLayoutTest, ConstIterator) {
    const NxBufferLayout layout({
        {NxShaderDataType::FLOAT3, "aPosition"}
    });

    int count = 0;
    for ([[maybe_unused]] const auto& elem : layout) {
        count++;
    }
    EXPECT_EQ(count, 1);
}

TEST_F(BufferLayoutTest, ComplexVertexFormat) {
    // Common vertex format: position + normal + tangent + texcoord
    NxBufferLayout layout({
        {NxShaderDataType::FLOAT3, "aPosition"},   // 12 bytes, offset 0
        {NxShaderDataType::FLOAT3, "aNormal"},     // 12 bytes, offset 12
        {NxShaderDataType::FLOAT3, "aTangent"},    // 12 bytes, offset 24
        {NxShaderDataType::FLOAT2, "aTexCoord"}    // 8 bytes, offset 36
    });

    auto elements = layout.getElements();
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(elements[1].offset, 12u);
    EXPECT_EQ(elements[2].offset, 24u);
    EXPECT_EQ(elements[3].offset, 36u);
    EXPECT_EQ(layout.getStride(), 44u);
}

TEST_F(BufferLayoutTest, LayoutWithIntegers) {
    NxBufferLayout layout({
        {NxShaderDataType::INT4, "aBoneIDs"},
        {NxShaderDataType::FLOAT4, "aWeights"}
    });

    auto elements = layout.getElements();
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(elements[0].size, 16u);
    EXPECT_EQ(elements[1].offset, 16u);
    EXPECT_EQ(elements[1].size, 16u);
    EXPECT_EQ(layout.getStride(), 32u);
}

}  // namespace nexo::renderer
