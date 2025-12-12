//// ManagedTypedef.test.cpp ///////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for scripting ManagedTypedef type aliases
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "scripting/native/ManagedTypedef.hpp"
#include <type_traits>
#include <limits>

namespace nexo::scripting {

class ManagedTypedefTest : public ::testing::Test {};

// =============================================================================
// Size Tests - Verify C# compatible type sizes
// =============================================================================

TEST_F(ManagedTypedefTest, ByteSize) {
    EXPECT_EQ(sizeof(Byte), 1u);
}

TEST_F(ManagedTypedefTest, SByteSize) {
    EXPECT_EQ(sizeof(SByte), 1u);
}

TEST_F(ManagedTypedefTest, Int16Size) {
    EXPECT_EQ(sizeof(Int16), 2u);
}

TEST_F(ManagedTypedefTest, Int32Size) {
    EXPECT_EQ(sizeof(Int32), 4u);
}

TEST_F(ManagedTypedefTest, Int64Size) {
    EXPECT_EQ(sizeof(Int64), 8u);
}

TEST_F(ManagedTypedefTest, UInt16Size) {
    EXPECT_EQ(sizeof(UInt16), 2u);
}

TEST_F(ManagedTypedefTest, UInt32Size) {
    EXPECT_EQ(sizeof(UInt32), 4u);
}

TEST_F(ManagedTypedefTest, UInt64Size) {
    EXPECT_EQ(sizeof(UInt64), 8u);
}

TEST_F(ManagedTypedefTest, SingleSize) {
    EXPECT_EQ(sizeof(Single), 4u);
}

TEST_F(ManagedTypedefTest, DoubleSize) {
    EXPECT_EQ(sizeof(Double), 8u);
}

TEST_F(ManagedTypedefTest, BooleanSize) {
    EXPECT_EQ(sizeof(Boolean), 1u);
}

TEST_F(ManagedTypedefTest, CharSize) {
    EXPECT_EQ(sizeof(Char), 2u);  // Unicode 16-bit character
}

// =============================================================================
// Signedness Tests
// =============================================================================

TEST_F(ManagedTypedefTest, ByteIsUnsigned) {
    EXPECT_TRUE(std::is_unsigned_v<Byte>);
}

TEST_F(ManagedTypedefTest, SByteIsSigned) {
    EXPECT_TRUE(std::is_signed_v<SByte>);
}

TEST_F(ManagedTypedefTest, Int16IsSigned) {
    EXPECT_TRUE(std::is_signed_v<Int16>);
}

TEST_F(ManagedTypedefTest, Int32IsSigned) {
    EXPECT_TRUE(std::is_signed_v<Int32>);
}

TEST_F(ManagedTypedefTest, Int64IsSigned) {
    EXPECT_TRUE(std::is_signed_v<Int64>);
}

TEST_F(ManagedTypedefTest, UInt16IsUnsigned) {
    EXPECT_TRUE(std::is_unsigned_v<UInt16>);
}

TEST_F(ManagedTypedefTest, UInt32IsUnsigned) {
    EXPECT_TRUE(std::is_unsigned_v<UInt32>);
}

TEST_F(ManagedTypedefTest, UInt64IsUnsigned) {
    EXPECT_TRUE(std::is_unsigned_v<UInt64>);
}

// =============================================================================
// Type Category Tests
// =============================================================================

TEST_F(ManagedTypedefTest, IntegerTypesAreIntegral) {
    EXPECT_TRUE(std::is_integral_v<Byte>);
    EXPECT_TRUE(std::is_integral_v<SByte>);
    EXPECT_TRUE(std::is_integral_v<Int16>);
    EXPECT_TRUE(std::is_integral_v<Int32>);
    EXPECT_TRUE(std::is_integral_v<Int64>);
    EXPECT_TRUE(std::is_integral_v<UInt16>);
    EXPECT_TRUE(std::is_integral_v<UInt32>);
    EXPECT_TRUE(std::is_integral_v<UInt64>);
}

TEST_F(ManagedTypedefTest, FloatingPointTypesAreFloatingPoint) {
    EXPECT_TRUE(std::is_floating_point_v<Single>);
    EXPECT_TRUE(std::is_floating_point_v<Double>);
}

TEST_F(ManagedTypedefTest, BooleanIsBool) {
    EXPECT_TRUE((std::is_same_v<Boolean, bool>));
}

TEST_F(ManagedTypedefTest, CharIsIntegral) {
    EXPECT_TRUE(std::is_integral_v<Char>);
    EXPECT_TRUE(std::is_unsigned_v<Char>);
}

TEST_F(ManagedTypedefTest, IntPtrIsPointer) {
    EXPECT_TRUE(std::is_pointer_v<IntPtr>);
}

// =============================================================================
// Range Tests - Verify numeric limits match C# types
// =============================================================================

TEST_F(ManagedTypedefTest, ByteRange) {
    EXPECT_EQ(std::numeric_limits<Byte>::min(), 0);
    EXPECT_EQ(std::numeric_limits<Byte>::max(), 255);
}

TEST_F(ManagedTypedefTest, SByteRange) {
    EXPECT_EQ(std::numeric_limits<SByte>::min(), -128);
    EXPECT_EQ(std::numeric_limits<SByte>::max(), 127);
}

TEST_F(ManagedTypedefTest, Int16Range) {
    EXPECT_EQ(std::numeric_limits<Int16>::min(), -32768);
    EXPECT_EQ(std::numeric_limits<Int16>::max(), 32767);
}

TEST_F(ManagedTypedefTest, UInt16Range) {
    EXPECT_EQ(std::numeric_limits<UInt16>::min(), 0);
    EXPECT_EQ(std::numeric_limits<UInt16>::max(), 65535);
}

TEST_F(ManagedTypedefTest, Int32Range) {
    EXPECT_EQ(std::numeric_limits<Int32>::min(), -2147483648);
    EXPECT_EQ(std::numeric_limits<Int32>::max(), 2147483647);
}

TEST_F(ManagedTypedefTest, UInt32Range) {
    EXPECT_EQ(std::numeric_limits<UInt32>::min(), 0u);
    EXPECT_EQ(std::numeric_limits<UInt32>::max(), 4294967295u);
}

// =============================================================================
// Vector Type Tests
// =============================================================================

TEST_F(ManagedTypedefTest, Vector3Size) {
    EXPECT_EQ(sizeof(Vector3), 12u);  // 3 floats
}

TEST_F(ManagedTypedefTest, Vector4Size) {
    EXPECT_EQ(sizeof(Vector4), 16u);  // 4 floats
}

TEST_F(ManagedTypedefTest, Vector3IsGlmVec3) {
    EXPECT_TRUE((std::is_same_v<Vector3, glm::vec3>));
}

TEST_F(ManagedTypedefTest, Vector4IsGlmVec4) {
    EXPECT_TRUE((std::is_same_v<Vector4, glm::vec4>));
}

TEST_F(ManagedTypedefTest, Vector3Components) {
    Vector3 v{1.0f, 2.0f, 3.0f};
    EXPECT_FLOAT_EQ(v.x, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 2.0f);
    EXPECT_FLOAT_EQ(v.z, 3.0f);
}

TEST_F(ManagedTypedefTest, Vector4Components) {
    Vector4 v{1.0f, 2.0f, 3.0f, 4.0f};
    EXPECT_FLOAT_EQ(v.x, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 2.0f);
    EXPECT_FLOAT_EQ(v.z, 3.0f);
    EXPECT_FLOAT_EQ(v.w, 4.0f);
}

// =============================================================================
// Type Traits Tests
// =============================================================================

TEST_F(ManagedTypedefTest, AllNumericTypesAreArithmetic) {
    EXPECT_TRUE(std::is_arithmetic_v<Byte>);
    EXPECT_TRUE(std::is_arithmetic_v<SByte>);
    EXPECT_TRUE(std::is_arithmetic_v<Int16>);
    EXPECT_TRUE(std::is_arithmetic_v<Int32>);
    EXPECT_TRUE(std::is_arithmetic_v<Int64>);
    EXPECT_TRUE(std::is_arithmetic_v<UInt16>);
    EXPECT_TRUE(std::is_arithmetic_v<UInt32>);
    EXPECT_TRUE(std::is_arithmetic_v<UInt64>);
    EXPECT_TRUE(std::is_arithmetic_v<Single>);
    EXPECT_TRUE(std::is_arithmetic_v<Double>);
}

TEST_F(ManagedTypedefTest, AllTypesAreTriviallyCopyable) {
    EXPECT_TRUE(std::is_trivially_copyable_v<Byte>);
    EXPECT_TRUE(std::is_trivially_copyable_v<SByte>);
    EXPECT_TRUE(std::is_trivially_copyable_v<Int16>);
    EXPECT_TRUE(std::is_trivially_copyable_v<Int32>);
    EXPECT_TRUE(std::is_trivially_copyable_v<Int64>);
    EXPECT_TRUE(std::is_trivially_copyable_v<UInt16>);
    EXPECT_TRUE(std::is_trivially_copyable_v<UInt32>);
    EXPECT_TRUE(std::is_trivially_copyable_v<UInt64>);
    EXPECT_TRUE(std::is_trivially_copyable_v<Single>);
    EXPECT_TRUE(std::is_trivially_copyable_v<Double>);
    EXPECT_TRUE(std::is_trivially_copyable_v<Boolean>);
    EXPECT_TRUE(std::is_trivially_copyable_v<Char>);
}

}  // namespace nexo::scripting
