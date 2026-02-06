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

// =============================================================================
// Edge Case Tests - Boundary Values
// =============================================================================

TEST_F(ManagedTypedefTest, Int64Range) {
    EXPECT_EQ(std::numeric_limits<Int64>::min(), -9223372036854775807LL - 1);
    EXPECT_EQ(std::numeric_limits<Int64>::max(), 9223372036854775807LL);
}

TEST_F(ManagedTypedefTest, UInt64Range) {
    EXPECT_EQ(std::numeric_limits<UInt64>::min(), 0ull);
    EXPECT_EQ(std::numeric_limits<UInt64>::max(), 18446744073709551615ull);
}

TEST_F(ManagedTypedefTest, CharRange) {
    EXPECT_EQ(std::numeric_limits<Char>::min(), 0);
    EXPECT_EQ(std::numeric_limits<Char>::max(), 65535);
}

TEST_F(ManagedTypedefTest, SingleRange) {
    EXPECT_TRUE(std::isfinite(std::numeric_limits<Single>::min()));
    EXPECT_TRUE(std::isfinite(std::numeric_limits<Single>::max()));
    EXPECT_GT(std::numeric_limits<Single>::max(), 0.0f);
}

TEST_F(ManagedTypedefTest, DoubleRange) {
    EXPECT_TRUE(std::isfinite(std::numeric_limits<Double>::min()));
    EXPECT_TRUE(std::isfinite(std::numeric_limits<Double>::max()));
    EXPECT_GT(std::numeric_limits<Double>::max(), 0.0);
}

// =============================================================================
// Edge Case Tests - Zero and Special Values
// =============================================================================

TEST_F(ManagedTypedefTest, ZeroValues) {
    Byte b = 0;
    SByte sb = 0;
    Int16 i16 = 0;
    Int32 i32 = 0;
    Int64 i64 = 0;
    UInt16 u16 = 0;
    UInt32 u32 = 0;
    UInt64 u64 = 0;
    Single f = 0.0f;
    Double d = 0.0;
    Boolean bool_val = false;
    Char c = 0;

    EXPECT_EQ(b, 0);
    EXPECT_EQ(sb, 0);
    EXPECT_EQ(i16, 0);
    EXPECT_EQ(i32, 0);
    EXPECT_EQ(i64, 0);
    EXPECT_EQ(u16, 0);
    EXPECT_EQ(u32, 0);
    EXPECT_EQ(u64, 0);
    EXPECT_EQ(f, 0.0f);
    EXPECT_EQ(d, 0.0);
    EXPECT_EQ(bool_val, false);
    EXPECT_EQ(c, 0);
}

TEST_F(ManagedTypedefTest, MaxValues) {
    Byte b = std::numeric_limits<Byte>::max();
    SByte sb = std::numeric_limits<SByte>::max();
    Int16 i16 = std::numeric_limits<Int16>::max();
    Int32 i32 = std::numeric_limits<Int32>::max();
    Int64 i64 = std::numeric_limits<Int64>::max();

    EXPECT_EQ(b, 255);
    EXPECT_EQ(sb, 127);
    EXPECT_EQ(i16, 32767);
    EXPECT_EQ(i32, 2147483647);
    EXPECT_EQ(i64, 9223372036854775807LL);
}

TEST_F(ManagedTypedefTest, MinValues) {
    SByte sb = std::numeric_limits<SByte>::min();
    Int16 i16 = std::numeric_limits<Int16>::min();
    Int32 i32 = std::numeric_limits<Int32>::min();
    Int64 i64 = std::numeric_limits<Int64>::min();

    EXPECT_EQ(sb, -128);
    EXPECT_EQ(i16, -32768);
    EXPECT_EQ(i32, -2147483648);
    EXPECT_EQ(i64, -9223372036854775807LL - 1);
}

TEST_F(ManagedTypedefTest, FloatingPointSpecialValues) {
    // Test NaN
    Single nanF = std::numeric_limits<Single>::quiet_NaN();
    Double nanD = std::numeric_limits<Double>::quiet_NaN();
    EXPECT_TRUE(std::isnan(nanF));
    EXPECT_TRUE(std::isnan(nanD));

    // Test Infinity
    Single infF = std::numeric_limits<Single>::infinity();
    Double infD = std::numeric_limits<Double>::infinity();
    EXPECT_TRUE(std::isinf(infF));
    EXPECT_TRUE(std::isinf(infD));

    // Test negative infinity
    Single negInfF = -std::numeric_limits<Single>::infinity();
    Double negInfD = -std::numeric_limits<Double>::infinity();
    EXPECT_TRUE(std::isinf(negInfF));
    EXPECT_TRUE(std::isinf(negInfD));
    EXPECT_LT(negInfF, 0.0f);
    EXPECT_LT(negInfD, 0.0);
}

// =============================================================================
// Edge Case Tests - Type Conversions
// =============================================================================

TEST_F(ManagedTypedefTest, SignedToUnsignedConversion) {
    Int32 signed_val = -1;
    UInt32 unsigned_val = static_cast<UInt32>(signed_val);

    // -1 as signed becomes max unsigned value
    EXPECT_EQ(unsigned_val, 4294967295u);
}

TEST_F(ManagedTypedefTest, OverflowBehavior) {
    Byte b = 255;
    b = static_cast<Byte>(b + 1);  // Overflow
    EXPECT_EQ(b, 0);

    SByte sb = 127;
    sb = static_cast<SByte>(sb + 1);  // Overflow
    EXPECT_EQ(sb, -128);
}

TEST_F(ManagedTypedefTest, NarrowingConversions) {
    Int64 large = 0x123456789ABCDEFLL;
    Int32 narrow = static_cast<Int32>(large);

    // Lower 32 bits should be preserved
    EXPECT_EQ(narrow, static_cast<Int32>(large & 0xFFFFFFFF));
}

TEST_F(ManagedTypedefTest, WideningConversions) {
    SByte small = -1;
    Int64 wide = small;

    // Sign extension should occur
    EXPECT_EQ(wide, -1LL);
}

// =============================================================================
// Edge Case Tests - Vector Operations
// =============================================================================

TEST_F(ManagedTypedefTest, Vector3DefaultConstruction) {
    Vector3 v;
    // Default values are uninitialized, but we can construct it
    EXPECT_NO_THROW({ Vector3 test; });
}

TEST_F(ManagedTypedefTest, Vector3ValueConstruction) {
    Vector3 v{1.0f, 2.0f, 3.0f};
    EXPECT_FLOAT_EQ(v.x, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 2.0f);
    EXPECT_FLOAT_EQ(v.z, 3.0f);
}

TEST_F(ManagedTypedefTest, Vector3ZeroVector) {
    Vector3 v{0.0f, 0.0f, 0.0f};
    EXPECT_FLOAT_EQ(v.x, 0.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
    EXPECT_FLOAT_EQ(v.z, 0.0f);
}

TEST_F(ManagedTypedefTest, Vector3NegativeValues) {
    Vector3 v{-1.0f, -2.0f, -3.0f};
    EXPECT_FLOAT_EQ(v.x, -1.0f);
    EXPECT_FLOAT_EQ(v.y, -2.0f);
    EXPECT_FLOAT_EQ(v.z, -3.0f);
}

TEST_F(ManagedTypedefTest, Vector3LargeValues) {
    Single large = 1e20f;
    Vector3 v{large, large, large};
    EXPECT_FLOAT_EQ(v.x, large);
    EXPECT_FLOAT_EQ(v.y, large);
    EXPECT_FLOAT_EQ(v.z, large);
}

TEST_F(ManagedTypedefTest, Vector3SmallValues) {
    Single small = 1e-20f;
    Vector3 v{small, small, small};
    EXPECT_FLOAT_EQ(v.x, small);
    EXPECT_FLOAT_EQ(v.y, small);
    EXPECT_FLOAT_EQ(v.z, small);
}

TEST_F(ManagedTypedefTest, Vector4DefaultConstruction) {
    Vector4 v;
    EXPECT_NO_THROW({ Vector4 test; });
}

TEST_F(ManagedTypedefTest, Vector4ValueConstruction) {
    Vector4 v{1.0f, 2.0f, 3.0f, 4.0f};
    EXPECT_FLOAT_EQ(v.x, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 2.0f);
    EXPECT_FLOAT_EQ(v.z, 3.0f);
    EXPECT_FLOAT_EQ(v.w, 4.0f);
}

TEST_F(ManagedTypedefTest, Vector4ZeroVector) {
    Vector4 v{0.0f, 0.0f, 0.0f, 0.0f};
    EXPECT_FLOAT_EQ(v.x, 0.0f);
    EXPECT_FLOAT_EQ(v.y, 0.0f);
    EXPECT_FLOAT_EQ(v.z, 0.0f);
    EXPECT_FLOAT_EQ(v.w, 0.0f);
}

TEST_F(ManagedTypedefTest, Vector4NegativeValues) {
    Vector4 v{-1.0f, -2.0f, -3.0f, -4.0f};
    EXPECT_FLOAT_EQ(v.x, -1.0f);
    EXPECT_FLOAT_EQ(v.y, -2.0f);
    EXPECT_FLOAT_EQ(v.z, -3.0f);
    EXPECT_FLOAT_EQ(v.w, -4.0f);
}

TEST_F(ManagedTypedefTest, VectorArithmeticOperations) {
    Vector3 a{1.0f, 2.0f, 3.0f};
    Vector3 b{4.0f, 5.0f, 6.0f};

    Vector3 sum = a + b;
    EXPECT_FLOAT_EQ(sum.x, 5.0f);
    EXPECT_FLOAT_EQ(sum.y, 7.0f);
    EXPECT_FLOAT_EQ(sum.z, 9.0f);

    Vector3 diff = b - a;
    EXPECT_FLOAT_EQ(diff.x, 3.0f);
    EXPECT_FLOAT_EQ(diff.y, 3.0f);
    EXPECT_FLOAT_EQ(diff.z, 3.0f);
}

TEST_F(ManagedTypedefTest, VectorScalarMultiplication) {
    Vector3 v{1.0f, 2.0f, 3.0f};
    Vector3 scaled = v * 2.0f;

    EXPECT_FLOAT_EQ(scaled.x, 2.0f);
    EXPECT_FLOAT_EQ(scaled.y, 4.0f);
    EXPECT_FLOAT_EQ(scaled.z, 6.0f);
}

// =============================================================================
// Edge Case Tests - Pointer Type
// =============================================================================

TEST_F(ManagedTypedefTest, IntPtrNullptr) {
    IntPtr ptr = nullptr;
    EXPECT_EQ(ptr, nullptr);
}

TEST_F(ManagedTypedefTest, IntPtrValidPointer) {
    int value = 42;
    IntPtr ptr = &value;
    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(*static_cast<int*>(ptr), 42);
}

TEST_F(ManagedTypedefTest, IntPtrCasting) {
    int value = 100;
    IntPtr ptr = static_cast<IntPtr>(&value);
    int* typedPtr = static_cast<int*>(ptr);

    EXPECT_EQ(*typedPtr, 100);
}

// =============================================================================
// Edge Case Tests - Boolean Values
// =============================================================================

TEST_F(ManagedTypedefTest, BooleanTrueFalse) {
    Boolean t = true;
    Boolean f = false;

    EXPECT_TRUE(t);
    EXPECT_FALSE(f);
}

TEST_F(ManagedTypedefTest, BooleanConversion) {
    Boolean b1 = static_cast<Boolean>(1);
    Boolean b2 = static_cast<Boolean>(0);
    Boolean b3 = static_cast<Boolean>(42);  // Non-zero

    EXPECT_TRUE(b1);
    EXPECT_FALSE(b2);
    EXPECT_TRUE(b3);  // Any non-zero is true
}

// =============================================================================
// Edge Case Tests - Array Usage
// =============================================================================

TEST_F(ManagedTypedefTest, ArrayOfIntegers) {
    Int32 arr[5] = {1, 2, 3, 4, 5};

    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[4], 5);
}

TEST_F(ManagedTypedefTest, ArrayOfFloats) {
    Single arr[3] = {1.5f, 2.5f, 3.5f};

    EXPECT_FLOAT_EQ(arr[0], 1.5f);
    EXPECT_FLOAT_EQ(arr[2], 3.5f);
}

TEST_F(ManagedTypedefTest, ArrayOfVectors) {
    Vector3 positions[3] = {
        {0.0f, 0.0f, 0.0f},
        {1.0f, 1.0f, 1.0f},
        {2.0f, 2.0f, 2.0f}
    };

    EXPECT_FLOAT_EQ(positions[1].x, 1.0f);
    EXPECT_FLOAT_EQ(positions[2].z, 2.0f);
}

// =============================================================================
// Edge Case Tests - STL Container Compatibility
// =============================================================================

TEST_F(ManagedTypedefTest, VectorContainer) {
    std::vector<Int32> vec = {1, 2, 3, 4, 5};

    EXPECT_EQ(vec.size(), 5u);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[4], 5);
}

TEST_F(ManagedTypedefTest, VectorOfVectors) {
    std::vector<Vector3> positions;
    positions.push_back({1.0f, 2.0f, 3.0f});
    positions.push_back({4.0f, 5.0f, 6.0f});

    EXPECT_EQ(positions.size(), 2u);
    EXPECT_FLOAT_EQ(positions[0].x, 1.0f);
    EXPECT_FLOAT_EQ(positions[1].z, 6.0f);
}

TEST_F(ManagedTypedefTest, UnorderedMapWithManagedTypes) {
    std::unordered_map<Int32, Single> lookup;
    lookup[1] = 1.5f;
    lookup[2] = 2.5f;

    EXPECT_FLOAT_EQ(lookup[1], 1.5f);
    EXPECT_FLOAT_EQ(lookup[2], 2.5f);
}

// =============================================================================
// Edge Case Tests - Precision and Accuracy
// =============================================================================

TEST_F(ManagedTypedefTest, SinglePrecisionLimits) {
    Single smallest = std::numeric_limits<Single>::min();
    Single largest = std::numeric_limits<Single>::max();
    Single epsilon = std::numeric_limits<Single>::epsilon();

    EXPECT_GT(smallest, 0.0f);
    EXPECT_GT(largest, 1.0f);
    EXPECT_GT(epsilon, 0.0f);
    EXPECT_LT(epsilon, 1.0f);
}

TEST_F(ManagedTypedefTest, DoublePrecisionLimits) {
    Double smallest = std::numeric_limits<Double>::min();
    Double largest = std::numeric_limits<Double>::max();
    Double epsilon = std::numeric_limits<Double>::epsilon();

    EXPECT_GT(smallest, 0.0);
    EXPECT_GT(largest, 1.0);
    EXPECT_GT(epsilon, 0.0);
    EXPECT_LT(epsilon, 1.0);
}

TEST_F(ManagedTypedefTest, FloatingPointComparison) {
    Single a = 0.1f + 0.2f;
    Single b = 0.3f;

    // Direct comparison might fail due to precision
    Single epsilon = std::numeric_limits<Single>::epsilon() * 10;
    EXPECT_NEAR(a, b, epsilon);
}

// =============================================================================
// Edge Case Tests - Char Type (Unicode)
// =============================================================================

TEST_F(ManagedTypedefTest, CharBasicASCII) {
    Char c = 'A';
    EXPECT_EQ(c, 65);
}

TEST_F(ManagedTypedefTest, CharExtendedRange) {
    Char c = 256;  // Beyond ASCII
    EXPECT_EQ(c, 256);
}

TEST_F(ManagedTypedefTest, CharMaxValue) {
    Char c = std::numeric_limits<Char>::max();
    EXPECT_EQ(c, 65535);
}

// =============================================================================
// Edge Case Tests - Const and Volatile Qualifiers
// =============================================================================

TEST_F(ManagedTypedefTest, ConstValues) {
    const Int32 constInt = 42;
    const Single constFloat = 3.14f;
    const Vector3 constVec{1.0f, 2.0f, 3.0f};

    EXPECT_EQ(constInt, 42);
    EXPECT_FLOAT_EQ(constFloat, 3.14f);
    EXPECT_FLOAT_EQ(constVec.x, 1.0f);
}

TEST_F(ManagedTypedefTest, VolatileQualifier) {
    volatile Int32 vol = 100;
    EXPECT_EQ(vol, 100);

    vol = 200;
    EXPECT_EQ(vol, 200);
}

}  // namespace nexo::scripting
