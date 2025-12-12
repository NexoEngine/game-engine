//// UniformCache.test.cpp //////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for UniformCache class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/UniformCache.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace nexo::renderer {

// =============================================================================
// Float Operations Tests
// =============================================================================

class UniformCacheFloatTest : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheFloatTest, SetFloatMarksDirty) {
    cache.setFloat("myFloat", 3.14f);
    EXPECT_TRUE(cache.isDirty("myFloat"));
}

TEST_F(UniformCacheFloatTest, SetFloatStoresValue) {
    cache.setFloat("myFloat", 3.14f);
    EXPECT_TRUE(cache.hasValue("myFloat"));
}

TEST_F(UniformCacheFloatTest, GetFloatReturnsCorrectValue) {
    cache.setFloat("myFloat", 3.14f);
    auto value = cache.getValue("myFloat");
    ASSERT_TRUE(value.has_value());
    ASSERT_TRUE(std::holds_alternative<float>(*value));
    EXPECT_FLOAT_EQ(std::get<float>(*value), 3.14f);
}

TEST_F(UniformCacheFloatTest, OverwriteFloatWithDifferentValue) {
    cache.setFloat("myFloat", 1.0f);
    cache.clearDirtyFlag("myFloat");
    cache.setFloat("myFloat", 2.0f);

    EXPECT_TRUE(cache.isDirty("myFloat"));
    auto value = cache.getValue("myFloat");
    ASSERT_TRUE(value.has_value());
    EXPECT_FLOAT_EQ(std::get<float>(*value), 2.0f);
}

TEST_F(UniformCacheFloatTest, SetSameFloatDoesNotMarkDirty) {
    cache.setFloat("myFloat", 1.5f);
    cache.clearDirtyFlag("myFloat");
    cache.setFloat("myFloat", 1.5f);

    EXPECT_FALSE(cache.isDirty("myFloat"));
}

// =============================================================================
// Float2 (vec2) Operations Tests
// =============================================================================

class UniformCacheFloat2Test : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheFloat2Test, SetFloat2MarksDirty) {
    cache.setFloat2("myVec2", glm::vec2(1.0f, 2.0f));
    EXPECT_TRUE(cache.isDirty("myVec2"));
}

TEST_F(UniformCacheFloat2Test, SetFloat2StoresValue) {
    cache.setFloat2("myVec2", glm::vec2(1.0f, 2.0f));
    EXPECT_TRUE(cache.hasValue("myVec2"));
}

TEST_F(UniformCacheFloat2Test, GetFloat2ReturnsCorrectValue) {
    glm::vec2 vec(1.5f, 2.5f);
    cache.setFloat2("myVec2", vec);
    auto value = cache.getValue("myVec2");
    ASSERT_TRUE(value.has_value());
    ASSERT_TRUE(std::holds_alternative<glm::vec2>(*value));

    glm::vec2 result = std::get<glm::vec2>(*value);
    EXPECT_FLOAT_EQ(result.x, 1.5f);
    EXPECT_FLOAT_EQ(result.y, 2.5f);
}

TEST_F(UniformCacheFloat2Test, OverwriteFloat2WithDifferentValue) {
    cache.setFloat2("myVec2", glm::vec2(1.0f, 1.0f));
    cache.clearDirtyFlag("myVec2");
    cache.setFloat2("myVec2", glm::vec2(2.0f, 2.0f));

    EXPECT_TRUE(cache.isDirty("myVec2"));
    auto value = cache.getValue("myVec2");
    ASSERT_TRUE(value.has_value());
    glm::vec2 result = std::get<glm::vec2>(*value);
    EXPECT_FLOAT_EQ(result.x, 2.0f);
    EXPECT_FLOAT_EQ(result.y, 2.0f);
}

TEST_F(UniformCacheFloat2Test, SetSameFloat2DoesNotMarkDirty) {
    glm::vec2 vec(1.5f, 2.5f);
    cache.setFloat2("myVec2", vec);
    cache.clearDirtyFlag("myVec2");
    cache.setFloat2("myVec2", vec);

    EXPECT_FALSE(cache.isDirty("myVec2"));
}

// =============================================================================
// Float3 (vec3) Operations Tests
// =============================================================================

class UniformCacheFloat3Test : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheFloat3Test, SetFloat3MarksDirty) {
    cache.setFloat3("myVec3", glm::vec3(1.0f, 2.0f, 3.0f));
    EXPECT_TRUE(cache.isDirty("myVec3"));
}

TEST_F(UniformCacheFloat3Test, SetFloat3StoresValue) {
    cache.setFloat3("myVec3", glm::vec3(1.0f, 2.0f, 3.0f));
    EXPECT_TRUE(cache.hasValue("myVec3"));
}

TEST_F(UniformCacheFloat3Test, GetFloat3ReturnsCorrectValue) {
    glm::vec3 vec(1.5f, 2.5f, 3.5f);
    cache.setFloat3("myVec3", vec);
    auto value = cache.getValue("myVec3");
    ASSERT_TRUE(value.has_value());
    ASSERT_TRUE(std::holds_alternative<glm::vec3>(*value));

    glm::vec3 result = std::get<glm::vec3>(*value);
    EXPECT_FLOAT_EQ(result.x, 1.5f);
    EXPECT_FLOAT_EQ(result.y, 2.5f);
    EXPECT_FLOAT_EQ(result.z, 3.5f);
}

TEST_F(UniformCacheFloat3Test, OverwriteFloat3WithDifferentValue) {
    cache.setFloat3("myVec3", glm::vec3(1.0f, 1.0f, 1.0f));
    cache.clearDirtyFlag("myVec3");
    cache.setFloat3("myVec3", glm::vec3(2.0f, 2.0f, 2.0f));

    EXPECT_TRUE(cache.isDirty("myVec3"));
    auto value = cache.getValue("myVec3");
    ASSERT_TRUE(value.has_value());
    glm::vec3 result = std::get<glm::vec3>(*value);
    EXPECT_FLOAT_EQ(result.x, 2.0f);
    EXPECT_FLOAT_EQ(result.y, 2.0f);
    EXPECT_FLOAT_EQ(result.z, 2.0f);
}

TEST_F(UniformCacheFloat3Test, SetSameFloat3DoesNotMarkDirty) {
    glm::vec3 vec(1.5f, 2.5f, 3.5f);
    cache.setFloat3("myVec3", vec);
    cache.clearDirtyFlag("myVec3");
    cache.setFloat3("myVec3", vec);

    EXPECT_FALSE(cache.isDirty("myVec3"));
}

// =============================================================================
// Float4 (vec4) Operations Tests
// =============================================================================

class UniformCacheFloat4Test : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheFloat4Test, SetFloat4MarksDirty) {
    cache.setFloat4("myVec4", glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
    EXPECT_TRUE(cache.isDirty("myVec4"));
}

TEST_F(UniformCacheFloat4Test, SetFloat4StoresValue) {
    cache.setFloat4("myVec4", glm::vec4(1.0f, 2.0f, 3.0f, 4.0f));
    EXPECT_TRUE(cache.hasValue("myVec4"));
}

TEST_F(UniformCacheFloat4Test, GetFloat4ReturnsCorrectValue) {
    glm::vec4 vec(1.5f, 2.5f, 3.5f, 4.5f);
    cache.setFloat4("myVec4", vec);
    auto value = cache.getValue("myVec4");
    ASSERT_TRUE(value.has_value());
    ASSERT_TRUE(std::holds_alternative<glm::vec4>(*value));

    glm::vec4 result = std::get<glm::vec4>(*value);
    EXPECT_FLOAT_EQ(result.x, 1.5f);
    EXPECT_FLOAT_EQ(result.y, 2.5f);
    EXPECT_FLOAT_EQ(result.z, 3.5f);
    EXPECT_FLOAT_EQ(result.w, 4.5f);
}

TEST_F(UniformCacheFloat4Test, OverwriteFloat4WithDifferentValue) {
    cache.setFloat4("myVec4", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    cache.clearDirtyFlag("myVec4");
    cache.setFloat4("myVec4", glm::vec4(2.0f, 2.0f, 2.0f, 2.0f));

    EXPECT_TRUE(cache.isDirty("myVec4"));
    auto value = cache.getValue("myVec4");
    ASSERT_TRUE(value.has_value());
    glm::vec4 result = std::get<glm::vec4>(*value);
    EXPECT_FLOAT_EQ(result.x, 2.0f);
    EXPECT_FLOAT_EQ(result.y, 2.0f);
    EXPECT_FLOAT_EQ(result.z, 2.0f);
    EXPECT_FLOAT_EQ(result.w, 2.0f);
}

TEST_F(UniformCacheFloat4Test, SetSameFloat4DoesNotMarkDirty) {
    glm::vec4 vec(1.5f, 2.5f, 3.5f, 4.5f);
    cache.setFloat4("myVec4", vec);
    cache.clearDirtyFlag("myVec4");
    cache.setFloat4("myVec4", vec);

    EXPECT_FALSE(cache.isDirty("myVec4"));
}

// =============================================================================
// Int Operations Tests
// =============================================================================

class UniformCacheIntTest : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheIntTest, SetIntMarksDirty) {
    cache.setInt("myInt", 42);
    EXPECT_TRUE(cache.isDirty("myInt"));
}

TEST_F(UniformCacheIntTest, SetIntStoresValue) {
    cache.setInt("myInt", 42);
    EXPECT_TRUE(cache.hasValue("myInt"));
}

TEST_F(UniformCacheIntTest, GetIntReturnsCorrectValue) {
    cache.setInt("myInt", 42);
    auto value = cache.getValue("myInt");
    ASSERT_TRUE(value.has_value());
    ASSERT_TRUE(std::holds_alternative<int>(*value));
    EXPECT_EQ(std::get<int>(*value), 42);
}

TEST_F(UniformCacheIntTest, OverwriteIntWithDifferentValue) {
    cache.setInt("myInt", 10);
    cache.clearDirtyFlag("myInt");
    cache.setInt("myInt", 20);

    EXPECT_TRUE(cache.isDirty("myInt"));
    auto value = cache.getValue("myInt");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(std::get<int>(*value), 20);
}

TEST_F(UniformCacheIntTest, SetSameIntDoesNotMarkDirty) {
    cache.setInt("myInt", 42);
    cache.clearDirtyFlag("myInt");
    cache.setInt("myInt", 42);

    EXPECT_FALSE(cache.isDirty("myInt"));
}

TEST_F(UniformCacheIntTest, SetNegativeInt) {
    cache.setInt("myInt", -100);
    auto value = cache.getValue("myInt");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(std::get<int>(*value), -100);
}

TEST_F(UniformCacheIntTest, SetZeroInt) {
    cache.setInt("myInt", 0);
    auto value = cache.getValue("myInt");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(std::get<int>(*value), 0);
}

// =============================================================================
// Bool Operations Tests
// =============================================================================

class UniformCacheBoolTest : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheBoolTest, SetBoolTrueMarksDirty) {
    cache.setBool("myBool", true);
    EXPECT_TRUE(cache.isDirty("myBool"));
}

TEST_F(UniformCacheBoolTest, SetBoolFalseMarksDirty) {
    cache.setBool("myBool", false);
    EXPECT_TRUE(cache.isDirty("myBool"));
}

TEST_F(UniformCacheBoolTest, SetBoolStoresValue) {
    cache.setBool("myBool", true);
    EXPECT_TRUE(cache.hasValue("myBool"));
}

TEST_F(UniformCacheBoolTest, GetBoolReturnsCorrectValue) {
    cache.setBool("myBool", true);
    auto value = cache.getValue("myBool");
    ASSERT_TRUE(value.has_value());
    ASSERT_TRUE(std::holds_alternative<bool>(*value));
    EXPECT_TRUE(std::get<bool>(*value));
}

TEST_F(UniformCacheBoolTest, OverwriteBoolWithDifferentValue) {
    cache.setBool("myBool", true);
    cache.clearDirtyFlag("myBool");
    cache.setBool("myBool", false);

    EXPECT_TRUE(cache.isDirty("myBool"));
    auto value = cache.getValue("myBool");
    ASSERT_TRUE(value.has_value());
    EXPECT_FALSE(std::get<bool>(*value));
}

TEST_F(UniformCacheBoolTest, SetSameBoolDoesNotMarkDirty) {
    cache.setBool("myBool", true);
    cache.clearDirtyFlag("myBool");
    cache.setBool("myBool", true);

    EXPECT_FALSE(cache.isDirty("myBool"));
}

// =============================================================================
// Matrix (mat4) Operations Tests
// =============================================================================

class UniformCacheMatrixTest : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheMatrixTest, SetMatrixMarksDirty) {
    glm::mat4 identity = glm::mat4(1.0f);
    cache.setMatrix("myMatrix", identity);
    EXPECT_TRUE(cache.isDirty("myMatrix"));
}

TEST_F(UniformCacheMatrixTest, SetMatrixStoresValue) {
    glm::mat4 identity = glm::mat4(1.0f);
    cache.setMatrix("myMatrix", identity);
    EXPECT_TRUE(cache.hasValue("myMatrix"));
}

TEST_F(UniformCacheMatrixTest, GetMatrixReturnsCorrectValue) {
    glm::mat4 identity = glm::mat4(1.0f);
    cache.setMatrix("myMatrix", identity);
    auto value = cache.getValue("myMatrix");
    ASSERT_TRUE(value.has_value());
    ASSERT_TRUE(std::holds_alternative<glm::mat4>(*value));

    glm::mat4 result = std::get<glm::mat4>(*value);
    EXPECT_EQ(result, identity);
}

TEST_F(UniformCacheMatrixTest, OverwriteMatrixWithDifferentValue) {
    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));

    cache.setMatrix("myMatrix", identity);
    cache.clearDirtyFlag("myMatrix");
    cache.setMatrix("myMatrix", translation);

    EXPECT_TRUE(cache.isDirty("myMatrix"));
    auto value = cache.getValue("myMatrix");
    ASSERT_TRUE(value.has_value());
    glm::mat4 result = std::get<glm::mat4>(*value);
    EXPECT_EQ(result, translation);
}

TEST_F(UniformCacheMatrixTest, SetSameMatrixDoesNotMarkDirty) {
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
    cache.setMatrix("myMatrix", matrix);
    cache.clearDirtyFlag("myMatrix");
    cache.setMatrix("myMatrix", matrix);

    EXPECT_FALSE(cache.isDirty("myMatrix"));
}

TEST_F(UniformCacheMatrixTest, SetTransformationMatrix) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::scale(transform, glm::vec3(2.0f, 2.0f, 2.0f));

    cache.setMatrix("myMatrix", transform);
    auto value = cache.getValue("myMatrix");
    ASSERT_TRUE(value.has_value());
    glm::mat4 result = std::get<glm::mat4>(*value);
    EXPECT_EQ(result, transform);
}

// =============================================================================
// Dirty Flag Management Tests
// =============================================================================

class UniformCacheDirtyFlagTest : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheDirtyFlagTest, ClearDirtyFlagClearsDirty) {
    cache.setFloat("myFloat", 3.14f);
    cache.clearDirtyFlag("myFloat");
    EXPECT_FALSE(cache.isDirty("myFloat"));
}

TEST_F(UniformCacheDirtyFlagTest, ClearDirtyFlagKeepsValue) {
    cache.setFloat("myFloat", 3.14f);
    cache.clearDirtyFlag("myFloat");

    EXPECT_TRUE(cache.hasValue("myFloat"));
    auto value = cache.getValue("myFloat");
    ASSERT_TRUE(value.has_value());
    EXPECT_FLOAT_EQ(std::get<float>(*value), 3.14f);
}

TEST_F(UniformCacheDirtyFlagTest, ClearAllDirtyFlagsClearsAllFlags) {
    cache.setFloat("float1", 1.0f);
    cache.setFloat("float2", 2.0f);
    cache.setInt("int1", 42);

    cache.clearAllDirtyFlags();

    EXPECT_FALSE(cache.isDirty("float1"));
    EXPECT_FALSE(cache.isDirty("float2"));
    EXPECT_FALSE(cache.isDirty("int1"));
}

TEST_F(UniformCacheDirtyFlagTest, ClearAllDirtyFlagsKeepsAllValues) {
    cache.setFloat("float1", 1.0f);
    cache.setInt("int1", 42);
    cache.setBool("bool1", true);

    cache.clearAllDirtyFlags();

    EXPECT_TRUE(cache.hasValue("float1"));
    EXPECT_TRUE(cache.hasValue("int1"));
    EXPECT_TRUE(cache.hasValue("bool1"));
}

TEST_F(UniformCacheDirtyFlagTest, IsDirtyReturnsFalseForNonExistent) {
    EXPECT_FALSE(cache.isDirty("nonExistent"));
}

TEST_F(UniformCacheDirtyFlagTest, ClearDirtyFlagOnNonExistentDoesNotCrash) {
    EXPECT_NO_THROW(cache.clearDirtyFlag("nonExistent"));
}

// =============================================================================
// Query Methods Tests
// =============================================================================

class UniformCacheQueryTest : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheQueryTest, HasValueReturnsFalseForEmpty) {
    EXPECT_FALSE(cache.hasValue("nonExistent"));
}

TEST_F(UniformCacheQueryTest, HasValueReturnsTrueAfterSet) {
    cache.setFloat("myFloat", 1.0f);
    EXPECT_TRUE(cache.hasValue("myFloat"));
}

TEST_F(UniformCacheQueryTest, GetValueReturnsNulloptForNonExistent) {
    auto value = cache.getValue("nonExistent");
    EXPECT_FALSE(value.has_value());
}

TEST_F(UniformCacheQueryTest, GetValueReturnsValidOptional) {
    cache.setFloat("myFloat", 1.0f);
    auto value = cache.getValue("myFloat");
    EXPECT_TRUE(value.has_value());
}

// =============================================================================
// Type Overwriting Tests
// =============================================================================

class UniformCacheTypeOverwriteTest : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheTypeOverwriteTest, OverwriteFloatWithInt) {
    cache.setFloat("uniform", 3.14f);
    cache.clearDirtyFlag("uniform");
    cache.setInt("uniform", 42);

    EXPECT_TRUE(cache.isDirty("uniform"));
    auto value = cache.getValue("uniform");
    ASSERT_TRUE(value.has_value());
    ASSERT_TRUE(std::holds_alternative<int>(*value));
    EXPECT_EQ(std::get<int>(*value), 42);
}

TEST_F(UniformCacheTypeOverwriteTest, OverwriteIntWithBool) {
    cache.setInt("uniform", 42);
    cache.clearDirtyFlag("uniform");
    cache.setBool("uniform", true);

    EXPECT_TRUE(cache.isDirty("uniform"));
    auto value = cache.getValue("uniform");
    ASSERT_TRUE(value.has_value());
    ASSERT_TRUE(std::holds_alternative<bool>(*value));
    EXPECT_TRUE(std::get<bool>(*value));
}

TEST_F(UniformCacheTypeOverwriteTest, OverwriteVec2WithVec3) {
    cache.setFloat2("uniform", glm::vec2(1.0f, 2.0f));
    cache.clearDirtyFlag("uniform");
    cache.setFloat3("uniform", glm::vec3(1.0f, 2.0f, 3.0f));

    EXPECT_TRUE(cache.isDirty("uniform"));
    auto value = cache.getValue("uniform");
    ASSERT_TRUE(value.has_value());
    ASSERT_TRUE(std::holds_alternative<glm::vec3>(*value));
}

TEST_F(UniformCacheTypeOverwriteTest, OverwriteMatrixWithFloat) {
    cache.setMatrix("uniform", glm::mat4(1.0f));
    cache.clearDirtyFlag("uniform");
    cache.setFloat("uniform", 5.0f);

    EXPECT_TRUE(cache.isDirty("uniform"));
    auto value = cache.getValue("uniform");
    ASSERT_TRUE(value.has_value());
    ASSERT_TRUE(std::holds_alternative<float>(*value));
    EXPECT_FLOAT_EQ(std::get<float>(*value), 5.0f);
}

// =============================================================================
// Multiple Uniforms Tests
// =============================================================================

class UniformCacheMultipleUniformsTest : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheMultipleUniformsTest, MultipleUniformsIndependent) {
    cache.setFloat("float1", 1.0f);
    cache.setInt("int1", 42);
    cache.setBool("bool1", true);

    EXPECT_TRUE(cache.hasValue("float1"));
    EXPECT_TRUE(cache.hasValue("int1"));
    EXPECT_TRUE(cache.hasValue("bool1"));

    auto floatVal = cache.getValue("float1");
    auto intVal = cache.getValue("int1");
    auto boolVal = cache.getValue("bool1");

    ASSERT_TRUE(floatVal.has_value());
    ASSERT_TRUE(intVal.has_value());
    ASSERT_TRUE(boolVal.has_value());

    EXPECT_FLOAT_EQ(std::get<float>(*floatVal), 1.0f);
    EXPECT_EQ(std::get<int>(*intVal), 42);
    EXPECT_TRUE(std::get<bool>(*boolVal));
}

TEST_F(UniformCacheMultipleUniformsTest, DirtyFlagsIndependent) {
    cache.setFloat("float1", 1.0f);
    cache.setFloat("float2", 2.0f);

    cache.clearDirtyFlag("float1");

    EXPECT_FALSE(cache.isDirty("float1"));
    EXPECT_TRUE(cache.isDirty("float2"));
}

TEST_F(UniformCacheMultipleUniformsTest, MixedTypes) {
    cache.setFloat("uTime", 1.5f);
    cache.setFloat2("uResolution", glm::vec2(1920.0f, 1080.0f));
    cache.setFloat3("uColor", glm::vec3(1.0f, 0.0f, 0.0f));
    cache.setFloat4("uColorWithAlpha", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    cache.setInt("uSampler", 0);
    cache.setBool("uEnabled", true);
    cache.setMatrix("uModelMatrix", glm::mat4(1.0f));

    EXPECT_TRUE(cache.hasValue("uTime"));
    EXPECT_TRUE(cache.hasValue("uResolution"));
    EXPECT_TRUE(cache.hasValue("uColor"));
    EXPECT_TRUE(cache.hasValue("uColorWithAlpha"));
    EXPECT_TRUE(cache.hasValue("uSampler"));
    EXPECT_TRUE(cache.hasValue("uEnabled"));
    EXPECT_TRUE(cache.hasValue("uModelMatrix"));

    EXPECT_TRUE(cache.isDirty("uTime"));
    EXPECT_TRUE(cache.isDirty("uResolution"));
    EXPECT_TRUE(cache.isDirty("uColor"));
    EXPECT_TRUE(cache.isDirty("uColorWithAlpha"));
    EXPECT_TRUE(cache.isDirty("uSampler"));
    EXPECT_TRUE(cache.isDirty("uEnabled"));
    EXPECT_TRUE(cache.isDirty("uModelMatrix"));
}

TEST_F(UniformCacheMultipleUniformsTest, PartialClear) {
    cache.setFloat("float1", 1.0f);
    cache.setFloat("float2", 2.0f);
    cache.setFloat("float3", 3.0f);

    cache.clearDirtyFlag("float1");
    cache.clearDirtyFlag("float3");

    EXPECT_FALSE(cache.isDirty("float1"));
    EXPECT_TRUE(cache.isDirty("float2"));
    EXPECT_FALSE(cache.isDirty("float3"));
}

// =============================================================================
// Empty Cache Tests
// =============================================================================

class UniformCacheEmptyTest : public ::testing::Test {
protected:
    UniformCache cache;
};

TEST_F(UniformCacheEmptyTest, EmptyCacheHasNoValues) {
    EXPECT_FALSE(cache.hasValue("anything"));
}

TEST_F(UniformCacheEmptyTest, EmptyCacheReturnsNullopt) {
    auto value = cache.getValue("anything");
    EXPECT_FALSE(value.has_value());
}

TEST_F(UniformCacheEmptyTest, EmptyCacheNotDirty) {
    EXPECT_FALSE(cache.isDirty("anything"));
}

TEST_F(UniformCacheEmptyTest, ClearAllDirtyFlagsOnEmptyCacheDoesNotCrash) {
    EXPECT_NO_THROW(cache.clearAllDirtyFlags());
}

}  // namespace nexo::renderer
