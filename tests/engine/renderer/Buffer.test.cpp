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

TEST_F(BufferLayoutTest, LayoutWithMatrices) {
    NxBufferLayout layout({
        {NxShaderDataType::MAT4, "aTransform"},
        {NxShaderDataType::MAT3, "aNormalMatrix"}
    });

    auto elements = layout.getElements();
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(elements[0].size, 64u);  // MAT4 = 4x4 floats = 64 bytes
    EXPECT_EQ(elements[1].offset, 64u);
    EXPECT_EQ(elements[1].size, 36u);  // MAT3 = 3x3 floats = 36 bytes
    EXPECT_EQ(layout.getStride(), 100u);  // 64 + 36
}

TEST_F(BufferLayoutTest, LayoutWithBool) {
    NxBufferLayout layout({
        {NxShaderDataType::BOOL, "aIsVisible"},
        {NxShaderDataType::FLOAT3, "aPosition"}
    });

    auto elements = layout.getElements();
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(elements[0].size, 1u);  // BOOL = 1 byte
    EXPECT_EQ(elements[1].offset, 1u);
    EXPECT_EQ(elements[1].size, 12u);
    EXPECT_EQ(layout.getStride(), 13u);  // 1 + 12
}

TEST_F(BufferLayoutTest, LayoutWithMixedTypes) {
    // Test a realistic vertex format with mixed types
    NxBufferLayout layout({
        {NxShaderDataType::FLOAT3, "aPosition"},    // 12 bytes, offset 0
        {NxShaderDataType::INT, "aEntityId"},       // 4 bytes, offset 12
        {NxShaderDataType::FLOAT2, "aTexCoord"},    // 8 bytes, offset 16
        {NxShaderDataType::BOOL, "aSelected"}       // 1 byte, offset 24
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 4u);
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(elements[1].offset, 12u);
    EXPECT_EQ(elements[2].offset, 16u);
    EXPECT_EQ(elements[3].offset, 24u);
    EXPECT_EQ(layout.getStride(), 25u);  // 12 + 4 + 8 + 1
}

TEST_F(BufferLayoutTest, LayoutWithAllFloatVariants) {
    NxBufferLayout layout({
        {NxShaderDataType::FLOAT, "aValue1"},
        {NxShaderDataType::FLOAT2, "aValue2"},
        {NxShaderDataType::FLOAT3, "aValue3"},
        {NxShaderDataType::FLOAT4, "aValue4"}
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 4u);
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(elements[0].size, 4u);
    EXPECT_EQ(elements[1].offset, 4u);
    EXPECT_EQ(elements[1].size, 8u);
    EXPECT_EQ(elements[2].offset, 12u);
    EXPECT_EQ(elements[2].size, 12u);
    EXPECT_EQ(elements[3].offset, 24u);
    EXPECT_EQ(elements[3].size, 16u);
    EXPECT_EQ(layout.getStride(), 40u);  // 4 + 8 + 12 + 16
}

TEST_F(BufferLayoutTest, LayoutWithAllIntVariants) {
    NxBufferLayout layout({
        {NxShaderDataType::INT, "aValue1"},
        {NxShaderDataType::INT2, "aValue2"},
        {NxShaderDataType::INT3, "aValue3"},
        {NxShaderDataType::INT4, "aValue4"}
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 4u);
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(elements[0].size, 4u);
    EXPECT_EQ(elements[1].offset, 4u);
    EXPECT_EQ(elements[1].size, 8u);
    EXPECT_EQ(elements[2].offset, 12u);
    EXPECT_EQ(elements[2].size, 12u);
    EXPECT_EQ(elements[3].offset, 24u);
    EXPECT_EQ(elements[3].size, 16u);
    EXPECT_EQ(layout.getStride(), 40u);  // 4 + 8 + 12 + 16
}

TEST_F(BufferLayoutTest, SingleLargeElement) {
    NxBufferLayout layout({
        {NxShaderDataType::MAT4, "aTransform"}
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 1u);
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(elements[0].size, 64u);
    EXPECT_EQ(layout.getStride(), 64u);
}

TEST_F(BufferLayoutTest, SingleSmallElement) {
    NxBufferLayout layout({
        {NxShaderDataType::BOOL, "aFlag"}
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 1u);
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(elements[0].size, 1u);
    EXPECT_EQ(layout.getStride(), 1u);
}

TEST_F(BufferLayoutTest, VerifyNormalizedFlag) {
    NxBufferLayout layout({
        {NxShaderDataType::FLOAT4, "aColor", true},
        {NxShaderDataType::FLOAT3, "aPosition", false}
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 2u);
    EXPECT_TRUE(elements[0].normalized);
    EXPECT_FALSE(elements[1].normalized);
}

// =============================================================================
// Edge Cases and Boundary Conditions
// =============================================================================

class BufferLayoutEdgeCasesTest : public ::testing::Test {};

TEST_F(BufferLayoutEdgeCasesTest, LargeNumberOfElements) {
    // Test with many elements to verify offset calculation handles larger numbers
    NxBufferLayout layout({
        {NxShaderDataType::FLOAT4, "elem0"},
        {NxShaderDataType::FLOAT4, "elem1"},
        {NxShaderDataType::FLOAT4, "elem2"},
        {NxShaderDataType::FLOAT4, "elem3"},
        {NxShaderDataType::FLOAT4, "elem4"},
        {NxShaderDataType::FLOAT4, "elem5"},
        {NxShaderDataType::FLOAT4, "elem6"},
        {NxShaderDataType::FLOAT4, "elem7"},
        {NxShaderDataType::FLOAT4, "elem8"},
        {NxShaderDataType::FLOAT4, "elem9"}
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 10u);

    // Check each element has correct offset (multiples of 16)
    for (size_t i = 0; i < 10; ++i) {
        EXPECT_EQ(elements[i].offset, i * 16u);
        EXPECT_EQ(elements[i].size, 16u);
    }

    EXPECT_EQ(layout.getStride(), 160u);  // 10 * 16
}

TEST_F(BufferLayoutEdgeCasesTest, AlternatingSmallAndLargeElements) {
    NxBufferLayout layout({
        {NxShaderDataType::BOOL, "small1"},      // 1 byte, offset 0
        {NxShaderDataType::MAT4, "large1"},      // 64 bytes, offset 1
        {NxShaderDataType::BOOL, "small2"},      // 1 byte, offset 65
        {NxShaderDataType::MAT3, "large2"}       // 36 bytes, offset 66
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 4u);
    EXPECT_EQ(elements[0].offset, 0u);
    EXPECT_EQ(elements[1].offset, 1u);
    EXPECT_EQ(elements[2].offset, 65u);
    EXPECT_EQ(elements[3].offset, 66u);
    EXPECT_EQ(layout.getStride(), 102u);  // 1 + 64 + 1 + 36
}

TEST_F(BufferLayoutEdgeCasesTest, SkinnedMeshVertexLayout) {
    // Realistic skinned mesh vertex layout
    NxBufferLayout layout({
        {NxShaderDataType::FLOAT3, "aPosition"},     // 12 bytes
        {NxShaderDataType::FLOAT3, "aNormal"},       // 12 bytes
        {NxShaderDataType::FLOAT3, "aTangent"},      // 12 bytes
        {NxShaderDataType::FLOAT2, "aTexCoord"},     // 8 bytes
        {NxShaderDataType::INT4, "aBoneIDs"},        // 16 bytes
        {NxShaderDataType::FLOAT4, "aBoneWeights"}   // 16 bytes
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 6u);

    unsigned int expected_offsets[] = {0, 12, 24, 36, 44, 60};
    unsigned int expected_sizes[] = {12, 12, 12, 8, 16, 16};

    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(elements[i].offset, expected_offsets[i]);
        EXPECT_EQ(elements[i].size, expected_sizes[i]);
    }

    EXPECT_EQ(layout.getStride(), 76u);  // Sum of all sizes
}

TEST_F(BufferLayoutEdgeCasesTest, Instance2DLayout) {
    // 2D sprite instancing layout
    NxBufferLayout layout({
        {NxShaderDataType::FLOAT2, "aPosition"},     // 8 bytes
        {NxShaderDataType::FLOAT2, "aSize"},         // 8 bytes
        {NxShaderDataType::FLOAT, "aRotation"},      // 4 bytes
        {NxShaderDataType::FLOAT4, "aColor"},        // 16 bytes
        {NxShaderDataType::INT, "aTextureIndex"}     // 4 bytes
    });

    auto elements = layout.getElements();
    ASSERT_EQ(elements.size(), 5u);
    EXPECT_EQ(layout.getStride(), 40u);  // 8 + 8 + 4 + 16 + 4
}

// =============================================================================
// Component Count Edge Cases
// =============================================================================

class ComponentCountEdgeCasesTest : public ::testing::Test {};

TEST_F(ComponentCountEdgeCasesTest, DefaultConstructedElementReturnsZero) {
    NxBufferElements elem;  // Default constructor sets type to NONE
    EXPECT_EQ(elem.getComponentCount(), 0u);
}

TEST_F(ComponentCountEdgeCasesTest, NoneTypeReturnsZero) {
    NxBufferElements elem(NxShaderDataType::NONE, "test");
    EXPECT_EQ(elem.getComponentCount(), 0u);
}

// =============================================================================
// Size Calculation Edge Cases
// =============================================================================

class ShaderDataTypeSizeEdgeCasesTest : public ::testing::Test {};

TEST_F(ShaderDataTypeSizeEdgeCasesTest, VerifyAllTypesNonZeroExceptNone) {
    // Verify all types have size > 0 except NONE
    EXPECT_GT(shaderDataTypeSize(NxShaderDataType::FLOAT), 0u);
    EXPECT_GT(shaderDataTypeSize(NxShaderDataType::FLOAT2), 0u);
    EXPECT_GT(shaderDataTypeSize(NxShaderDataType::FLOAT3), 0u);
    EXPECT_GT(shaderDataTypeSize(NxShaderDataType::FLOAT4), 0u);
    EXPECT_GT(shaderDataTypeSize(NxShaderDataType::MAT3), 0u);
    EXPECT_GT(shaderDataTypeSize(NxShaderDataType::MAT4), 0u);
    EXPECT_GT(shaderDataTypeSize(NxShaderDataType::INT), 0u);
    EXPECT_GT(shaderDataTypeSize(NxShaderDataType::INT2), 0u);
    EXPECT_GT(shaderDataTypeSize(NxShaderDataType::INT3), 0u);
    EXPECT_GT(shaderDataTypeSize(NxShaderDataType::INT4), 0u);
    EXPECT_GT(shaderDataTypeSize(NxShaderDataType::BOOL), 0u);
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::NONE), 0u);
}

TEST_F(ShaderDataTypeSizeEdgeCasesTest, VerifyIntAndFloatSameSizes) {
    // Verify corresponding int and float types have same sizes
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT),
              shaderDataTypeSize(NxShaderDataType::FLOAT));
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT2),
              shaderDataTypeSize(NxShaderDataType::FLOAT2));
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT3),
              shaderDataTypeSize(NxShaderDataType::FLOAT3));
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT4),
              shaderDataTypeSize(NxShaderDataType::FLOAT4));
}

TEST_F(ShaderDataTypeSizeEdgeCasesTest, VerifyMatrixSizesCorrect) {
    // MAT3 should be 3x3 floats = 9 floats = 36 bytes
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::MAT3), 36u);
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::MAT3),
              9 * shaderDataTypeSize(NxShaderDataType::FLOAT));

    // MAT4 should be 4x4 floats = 16 floats = 64 bytes
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::MAT4), 64u);
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::MAT4),
              16 * shaderDataTypeSize(NxShaderDataType::FLOAT));
}

TEST_F(ShaderDataTypeSizeEdgeCasesTest, VerifyVectorSizeProgression) {
    // Verify that FLOAT2 is 2x FLOAT, FLOAT3 is 3x FLOAT, etc.
    unsigned int float_size = shaderDataTypeSize(NxShaderDataType::FLOAT);
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::FLOAT2), float_size * 2);
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::FLOAT3), float_size * 3);
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::FLOAT4), float_size * 4);

    unsigned int int_size = shaderDataTypeSize(NxShaderDataType::INT);
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT2), int_size * 2);
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT3), int_size * 3);
    EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT4), int_size * 4);
}

}  // namespace nexo::renderer
