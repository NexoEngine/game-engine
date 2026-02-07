//// UniformCache.test.cpp /////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for UniformCache (value caching with dirty flags)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <glad/glad.h>
#include "renderer/UniformCache.hpp"

namespace nexo::renderer {

class UniformCacheTest : public ::testing::Test {
protected:
    UniformCache cache;

    void SetUp() override {
        std::unordered_map<std::string, ShaderUniformInfo> uniforms = {
            {"testFloat",   {"testFloat",   GL_FLOAT,      1, 0}},
            {"testVec2",    {"testVec2",    GL_FLOAT_VEC2, 1, 1}},
            {"testVec3",    {"testVec3",    GL_FLOAT_VEC3, 1, 2}},
            {"testVec4",    {"testVec4",    GL_FLOAT_VEC4, 1, 3}},
            {"testInt",     {"testInt",     GL_INT,        1, 4}},
            {"testBool",    {"testBool",    GL_BOOL,       1, 5}},
            {"testMatrix",  {"testMatrix",  GL_FLOAT_MAT4, 1, 6}},
            {"float1",      {"float1",      GL_FLOAT,      1, 7}},
            {"float2",      {"float2",      GL_FLOAT,      1, 8}},
            {"uniform1",    {"uniform1",    GL_FLOAT,      1, 9}},
            {"uniform2",    {"uniform2",    GL_FLOAT,      1, 10}},
            {"uniform3",    {"uniform3",    GL_FLOAT,      1, 11}},
            {"myFloat",     {"myFloat",     GL_FLOAT,      1, 12}},
            {"myInt",       {"myInt",       GL_INT,        1, 13}},
            {"myBool",      {"myBool",      GL_BOOL,       1, 14}},
            {"myVec3",      {"myVec3",      GL_FLOAT_VEC3, 1, 15}},
            {"myMatrix",    {"myMatrix",    GL_FLOAT_MAT4, 1, 16}},
            {"",            {"",            GL_FLOAT,      1, 17}},
            {"this_is_a_very_long_uniform_name_that_might_be_used_in_some_shaders",
             {"this_is_a_very_long_uniform_name_that_might_be_used_in_some_shaders", GL_FLOAT, 1, 18}},
            {"zero",        {"zero",        GL_FLOAT,      1, 19}},
            {"negative",    {"negative",    GL_FLOAT,      1, 20}},
            {"test",        {"test",        GL_FLOAT,      1, 21}},
            {"int1",        {"int1",        GL_INT,        1, 22}},
            {"bool1",       {"bool1",       GL_BOOL,       1, 23}},
        };
        cache.initialize(uniforms);
    }
};

// =============================================================================
// Float Tests
// =============================================================================

TEST_F(UniformCacheTest, SetFloatStoresValue) {
    EXPECT_TRUE(cache.setFloat("testFloat", 1.5f));
    EXPECT_TRUE(cache.hasValue("testFloat"));
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
    EXPECT_TRUE(cache.setFloat2("testVec2", glm::vec2(1.0f, 2.0f)));
    EXPECT_TRUE(cache.hasValue("testVec2"));
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
    EXPECT_TRUE(cache.setFloat3("testVec3", glm::vec3(1.0f, 2.0f, 3.0f)));
    EXPECT_TRUE(cache.hasValue("testVec3"));
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
    EXPECT_TRUE(cache.setFloat4("testVec4", glm::vec4(1.0f, 2.0f, 3.0f, 4.0f)));
    EXPECT_TRUE(cache.hasValue("testVec4"));
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
    EXPECT_TRUE(cache.setInt("testInt", 42));
    EXPECT_TRUE(cache.hasValue("testInt"));
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
    EXPECT_TRUE(cache.setInt("testInt", -42));
    EXPECT_TRUE(cache.hasValue("testInt"));
}

// =============================================================================
// Bool Tests
// =============================================================================

TEST_F(UniformCacheTest, SetBoolTrueStoresValue) {
    EXPECT_TRUE(cache.setBool("testBool", true));
    EXPECT_TRUE(cache.hasValue("testBool"));
}

TEST_F(UniformCacheTest, SetBoolFalseStoresValue) {
    // Buffer is zero-initialized, so setting false (0) first would be a no-op.
    // Set true first to ensure a non-zero baseline, then verify false is stored.
    cache.setBool("testBool", true);
    cache.clearDirtyFlag("testBool");
    EXPECT_TRUE(cache.setBool("testBool", false));
    EXPECT_TRUE(cache.hasValue("testBool"));
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
    glm::mat4 matrix(1.0f);
    EXPECT_TRUE(cache.setMatrix("testMatrix", matrix));
    EXPECT_TRUE(cache.hasValue("testMatrix"));
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

    // Value should still exist after clearing dirty flags
    EXPECT_TRUE(cache.hasValue("testFloat"));
    // Setting the same value should not mark dirty (proves value was preserved)
    cache.setFloat("testFloat", 1.5f);
    EXPECT_FALSE(cache.isDirty("testFloat"));
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
    EXPECT_TRUE(cache.hasValue("test"));
    EXPECT_TRUE(cache.isDirty("test"));
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

    // Each uniform should be independently dirty
    EXPECT_TRUE(cache.isDirty("uniform1"));
    EXPECT_TRUE(cache.isDirty("uniform2"));
    EXPECT_TRUE(cache.isDirty("uniform3"));
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
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST_F(UniformCacheTest, EmptyStringAsName) {
    cache.setFloat("", 1.0f);
    EXPECT_TRUE(cache.hasValue(""));
}

TEST_F(UniformCacheTest, LongNameAsKey) {
    std::string longName = "this_is_a_very_long_uniform_name_that_might_be_used_in_some_shaders";
    cache.setFloat(longName, 1.0f);
    EXPECT_TRUE(cache.hasValue(longName));
}

TEST_F(UniformCacheTest, ZeroFloat) {
    cache.setFloat("zero", 0.0f);
    EXPECT_TRUE(cache.hasValue("zero"));
}

TEST_F(UniformCacheTest, NegativeFloat) {
    cache.setFloat("negative", -1.5f);
    EXPECT_TRUE(cache.hasValue("negative"));
}

}  // namespace nexo::renderer
