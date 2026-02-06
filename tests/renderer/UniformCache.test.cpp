//// UniformCache.test.cpp /////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for UniformCache (value caching with dirty flags)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/UniformCache.hpp"

namespace nexo::renderer {

class UniformCacheTest : public ::testing::Test {
protected:
    UniformCache cache;
};

// =============================================================================
// Float Tests
// =============================================================================

TEST_F(UniformCacheTest, SetFloatStoresValue) {
    cache.setFloat("testFloat", 1.5f);
    EXPECT_TRUE(cache.hasValue("testFloat"));
    auto value = cache.getValue("testFloat");
    ASSERT_TRUE(value.has_value());
    EXPECT_FLOAT_EQ(std::get<float>(*value), 1.5f);
}

TEST_F(UniformCacheTest, SetFloatMarksDirty) {
    cache.setFloat("testFloat", 1.5f);
    EXPECT_TRUE(cache.isDirty("testFloat"));
}

TEST_F(UniformCacheTest, SetFloatSameValueNotDirty) {
    cache.setFloat("testFloat", 1.5f);
    cache.clearDirtyFlag("testFloat");
    cache.setFloat("testFloat", 1.5f);  // Same value
    EXPECT_FALSE(cache.isDirty("testFloat"));
}

TEST_F(UniformCacheTest, SetFloatDifferentValueMarksDirty) {
    cache.setFloat("testFloat", 1.5f);
    cache.clearDirtyFlag("testFloat");
    cache.setFloat("testFloat", 2.0f);  // Different value
    EXPECT_TRUE(cache.isDirty("testFloat"));
}

// =============================================================================
// Float2 (vec2) Tests
// =============================================================================

TEST_F(UniformCacheTest, SetFloat2StoresValue) {
    cache.setFloat2("testVec2", glm::vec2(1.0f, 2.0f));
    auto value = cache.getValue("testVec2");
    ASSERT_TRUE(value.has_value());
    glm::vec2 v = std::get<glm::vec2>(*value);
    EXPECT_FLOAT_EQ(v.x, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 2.0f);
}

TEST_F(UniformCacheTest, SetFloat2MarksDirty) {
    cache.setFloat2("testVec2", glm::vec2(1.0f, 2.0f));
    EXPECT_TRUE(cache.isDirty("testVec2"));
}

TEST_F(UniformCacheTest, SetFloat2SameValueNotDirty) {
    cache.setFloat2("testVec2", glm::vec2(1.0f, 2.0f));
    cache.clearDirtyFlag("testVec2");
    cache.setFloat2("testVec2", glm::vec2(1.0f, 2.0f));
    EXPECT_FALSE(cache.isDirty("testVec2"));
}

// =============================================================================
// Float3 (vec3) Tests
// =============================================================================

TEST_F(UniformCacheTest, SetFloat3StoresValue) {
    cache.setFloat3("testVec3", glm::vec3(1.0f, 2.0f, 3.0f));
    auto value = cache.getValue("testVec3");
    ASSERT_TRUE(value.has_value());
    glm::vec3 v = std::get<glm::vec3>(*value);
    EXPECT_FLOAT_EQ(v.x, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 2.0f);
    EXPECT_FLOAT_EQ(v.z, 3.0f);
}

TEST_F(UniformCacheTest, SetFloat3MarksDirty) {
    cache.setFloat3("testVec3", glm::vec3(1.0f, 2.0f, 3.0f));
    EXPECT_TRUE(cache.isDirty("testVec3"));
}

TEST_F(UniformCacheTest, SetFloat3SameValueNotDirty) {
    cache.setFloat3("testVec3", glm::vec3(1.0f, 2.0f, 3.0f));
    cache.clearDirtyFlag("testVec3");
    cache.setFloat3("testVec3", glm::vec3(1.0f, 2.0f, 3.0f));
    EXPECT_FALSE(cache.isDirty("testVec3"));
}

// =============================================================================
// Float4 (vec4) Tests
// =============================================================================

TEST_F(UniformCacheTest, SetFloat4StoresValue) {
    cache.setFloat4("testVec4", glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
    auto value = cache.getValue("testVec4");
    ASSERT_TRUE(value.has_value());
    glm::vec4 v = std::get<glm::vec4>(*value);
    EXPECT_FLOAT_EQ(v.x, 1.0f);
    EXPECT_FLOAT_EQ(v.y, 2.0f);
    EXPECT_FLOAT_EQ(v.z, 3.0f);
    EXPECT_FLOAT_EQ(v.w, 4.0f);
}

TEST_F(UniformCacheTest, SetFloat4MarksDirty) {
    cache.setFloat4("testVec4", glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
    EXPECT_TRUE(cache.isDirty("testVec4"));
}

TEST_F(UniformCacheTest, SetFloat4SameValueNotDirty) {
    cache.setFloat4("testVec4", glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
    cache.clearDirtyFlag("testVec4");
    cache.setFloat4("testVec4", glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
    EXPECT_FALSE(cache.isDirty("testVec4"));
}

// =============================================================================
// Int Tests
// =============================================================================

TEST_F(UniformCacheTest, SetIntStoresValue) {
    cache.setInt("testInt", 42);
    auto value = cache.getValue("testInt");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(std::get<int>(*value), 42);
}

TEST_F(UniformCacheTest, SetIntMarksDirty) {
    cache.setInt("testInt", 42);
    EXPECT_TRUE(cache.isDirty("testInt"));
}

TEST_F(UniformCacheTest, SetIntSameValueNotDirty) {
    cache.setInt("testInt", 42);
    cache.clearDirtyFlag("testInt");
    cache.setInt("testInt", 42);
    EXPECT_FALSE(cache.isDirty("testInt"));
}

TEST_F(UniformCacheTest, SetIntDifferentValueMarksDirty) {
    cache.setInt("testInt", 42);
    cache.clearDirtyFlag("testInt");
    cache.setInt("testInt", 100);
    EXPECT_TRUE(cache.isDirty("testInt"));
}

TEST_F(UniformCacheTest, SetIntNegativeValue) {
    cache.setInt("testInt", -42);
    auto value = cache.getValue("testInt");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(std::get<int>(*value), -42);
}

// =============================================================================
// Bool Tests
// =============================================================================

TEST_F(UniformCacheTest, SetBoolTrueStoresValue) {
    cache.setBool("testBool", true);
    auto value = cache.getValue("testBool");
    ASSERT_TRUE(value.has_value());
    EXPECT_TRUE(std::get<bool>(*value));
}

TEST_F(UniformCacheTest, SetBoolFalseStoresValue) {
    cache.setBool("testBool", false);
    auto value = cache.getValue("testBool");
    ASSERT_TRUE(value.has_value());
    EXPECT_FALSE(std::get<bool>(*value));
}

TEST_F(UniformCacheTest, SetBoolMarksDirty) {
    cache.setBool("testBool", true);
    EXPECT_TRUE(cache.isDirty("testBool"));
}

TEST_F(UniformCacheTest, SetBoolSameValueNotDirty) {
    cache.setBool("testBool", true);
    cache.clearDirtyFlag("testBool");
    cache.setBool("testBool", true);
    EXPECT_FALSE(cache.isDirty("testBool"));
}

TEST_F(UniformCacheTest, SetBoolDifferentValueMarksDirty) {
    cache.setBool("testBool", true);
    cache.clearDirtyFlag("testBool");
    cache.setBool("testBool", false);
    EXPECT_TRUE(cache.isDirty("testBool"));
}

// =============================================================================
// Matrix Tests
// =============================================================================

TEST_F(UniformCacheTest, SetMatrixStoresValue) {
    glm::mat4 matrix(1.0f);  // Identity matrix
    cache.setMatrix("testMatrix", matrix);
    auto value = cache.getValue("testMatrix");
    ASSERT_TRUE(value.has_value());
    glm::mat4 m = std::get<glm::mat4>(*value);
    EXPECT_EQ(m, matrix);
}

TEST_F(UniformCacheTest, SetMatrixMarksDirty) {
    cache.setMatrix("testMatrix", glm::mat4(1.0f));
    EXPECT_TRUE(cache.isDirty("testMatrix"));
}

TEST_F(UniformCacheTest, SetMatrixSameValueNotDirty) {
    glm::mat4 matrix(1.0f);
    cache.setMatrix("testMatrix", matrix);
    cache.clearDirtyFlag("testMatrix");
    cache.setMatrix("testMatrix", matrix);
    EXPECT_FALSE(cache.isDirty("testMatrix"));
}

TEST_F(UniformCacheTest, SetMatrixDifferentValueMarksDirty) {
    cache.setMatrix("testMatrix", glm::mat4(1.0f));
    cache.clearDirtyFlag("testMatrix");
    cache.setMatrix("testMatrix", glm::mat4(2.0f));
    EXPECT_TRUE(cache.isDirty("testMatrix"));
}

// =============================================================================
// hasValue Tests
// =============================================================================

TEST_F(UniformCacheTest, HasValueReturnsFalseForNonExistent) {
    EXPECT_FALSE(cache.hasValue("nonExistent"));
}

TEST_F(UniformCacheTest, HasValueReturnsTrueAfterSet) {
    cache.setFloat("testFloat", 1.0f);
    EXPECT_TRUE(cache.hasValue("testFloat"));
}

// =============================================================================
// getValue Tests
// =============================================================================

TEST_F(UniformCacheTest, GetValueReturnsNulloptForNonExistent) {
    auto value = cache.getValue("nonExistent");
    EXPECT_FALSE(value.has_value());
}

// =============================================================================
// isDirty Tests
// =============================================================================

TEST_F(UniformCacheTest, IsDirtyReturnsFalseForNonExistent) {
    EXPECT_FALSE(cache.isDirty("nonExistent"));
}

TEST_F(UniformCacheTest, IsDirtyReturnsTrueAfterSet) {
    cache.setFloat("testFloat", 1.0f);
    EXPECT_TRUE(cache.isDirty("testFloat"));
}

TEST_F(UniformCacheTest, IsDirtyReturnsFalseAfterClear) {
    cache.setFloat("testFloat", 1.0f);
    cache.clearDirtyFlag("testFloat");
    EXPECT_FALSE(cache.isDirty("testFloat"));
}

// =============================================================================
// clearDirtyFlag Tests
// =============================================================================

TEST_F(UniformCacheTest, ClearDirtyFlagClearsFlag) {
    cache.setFloat("testFloat", 1.0f);
    EXPECT_TRUE(cache.isDirty("testFloat"));
    cache.clearDirtyFlag("testFloat");
    EXPECT_FALSE(cache.isDirty("testFloat"));
}

TEST_F(UniformCacheTest, ClearDirtyFlagDoesNotAffectOthers) {
    cache.setFloat("float1", 1.0f);
    cache.setFloat("float2", 2.0f);
    cache.clearDirtyFlag("float1");
    EXPECT_FALSE(cache.isDirty("float1"));
    EXPECT_TRUE(cache.isDirty("float2"));
}

// =============================================================================
// clearAllDirtyFlags Tests
// =============================================================================

TEST_F(UniformCacheTest, ClearAllDirtyFlagsClearsAllFlags) {
    cache.setFloat("float1", 1.0f);
    cache.setFloat("float2", 2.0f);
    cache.setInt("int1", 42);
    cache.setBool("bool1", true);

    cache.clearAllDirtyFlags();

    EXPECT_FALSE(cache.isDirty("float1"));
    EXPECT_FALSE(cache.isDirty("float2"));
    EXPECT_FALSE(cache.isDirty("int1"));
    EXPECT_FALSE(cache.isDirty("bool1"));
}

TEST_F(UniformCacheTest, ClearAllDirtyFlagsPreservesValues) {
    cache.setFloat("testFloat", 1.5f);
    cache.clearAllDirtyFlags();

    auto value = cache.getValue("testFloat");
    ASSERT_TRUE(value.has_value());
    EXPECT_FLOAT_EQ(std::get<float>(*value), 1.5f);
}

// =============================================================================
// Type Change Tests
// =============================================================================

TEST_F(UniformCacheTest, TypeChangeMarksDirty) {
    cache.setFloat("test", 1.0f);
    cache.clearDirtyFlag("test");
    cache.setInt("test", 1);  // Different type
    EXPECT_TRUE(cache.isDirty("test"));
}

TEST_F(UniformCacheTest, TypeChangeUpdatesValue) {
    cache.setFloat("test", 1.0f);
    cache.setInt("test", 42);
    auto value = cache.getValue("test");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(std::get<int>(*value), 42);
}

// =============================================================================
// Multiple Uniforms Tests
// =============================================================================

TEST_F(UniformCacheTest, MultipleUniformsIndependent) {
    cache.setFloat("uniform1", 1.0f);
    cache.setFloat("uniform2", 2.0f);
    cache.setFloat("uniform3", 3.0f);

    EXPECT_TRUE(cache.hasValue("uniform1"));
    EXPECT_TRUE(cache.hasValue("uniform2"));
    EXPECT_TRUE(cache.hasValue("uniform3"));

    auto v1 = cache.getValue("uniform1");
    auto v2 = cache.getValue("uniform2");
    auto v3 = cache.getValue("uniform3");

    EXPECT_FLOAT_EQ(std::get<float>(*v1), 1.0f);
    EXPECT_FLOAT_EQ(std::get<float>(*v2), 2.0f);
    EXPECT_FLOAT_EQ(std::get<float>(*v3), 3.0f);
}

TEST_F(UniformCacheTest, DifferentTypesStoredTogether) {
    cache.setFloat("myFloat", 1.5f);
    cache.setInt("myInt", 42);
    cache.setBool("myBool", true);
    cache.setFloat3("myVec3", glm::vec3(1.0f, 2.0f, 3.0f));
    cache.setMatrix("myMatrix", glm::mat4(1.0f));

    EXPECT_TRUE(cache.hasValue("myFloat"));
    EXPECT_TRUE(cache.hasValue("myInt"));
    EXPECT_TRUE(cache.hasValue("myBool"));
    EXPECT_TRUE(cache.hasValue("myVec3"));
    EXPECT_TRUE(cache.hasValue("myMatrix"));

    EXPECT_FLOAT_EQ(std::get<float>(*cache.getValue("myFloat")), 1.5f);
    EXPECT_EQ(std::get<int>(*cache.getValue("myInt")), 42);
    EXPECT_TRUE(std::get<bool>(*cache.getValue("myBool")));
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST_F(UniformCacheTest, EmptyStringAsName) {
    cache.setFloat("", 1.0f);
    EXPECT_TRUE(cache.hasValue(""));
    auto value = cache.getValue("");
    ASSERT_TRUE(value.has_value());
    EXPECT_FLOAT_EQ(std::get<float>(*value), 1.0f);
}

TEST_F(UniformCacheTest, LongNameAsKey) {
    std::string longName = "this_is_a_very_long_uniform_name_that_might_be_used_in_some_shaders";
    cache.setFloat(longName, 1.0f);
    EXPECT_TRUE(cache.hasValue(longName));
}

TEST_F(UniformCacheTest, ZeroFloat) {
    cache.setFloat("zero", 0.0f);
    auto value = cache.getValue("zero");
    ASSERT_TRUE(value.has_value());
    EXPECT_FLOAT_EQ(std::get<float>(*value), 0.0f);
}

TEST_F(UniformCacheTest, NegativeFloat) {
    cache.setFloat("negative", -1.5f);
    auto value = cache.getValue("negative");
    ASSERT_TRUE(value.has_value());
    EXPECT_FLOAT_EQ(std::get<float>(*value), -1.5f);
}

}  // namespace nexo::renderer
