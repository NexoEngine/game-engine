//// ShaderMetadata.test.cpp //////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for shader-related metadata structures and enums
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/Shader.hpp"
#include "renderer/Attributes.hpp"
#include "renderer/UniformCache.hpp"

namespace nexo::renderer {

// =============================================================================
// NxShaderUniforms Enum Tests
// =============================================================================

class ShaderUniformsEnumTest : public ::testing::Test {};

TEST_F(ShaderUniformsEnumTest, ViewProjectionExists) {
    NxShaderUniforms uniform = NxShaderUniforms::VIEW_PROJECTION;
    EXPECT_EQ(static_cast<int>(uniform), 0);
}

TEST_F(ShaderUniformsEnumTest, ModelMatrixExists) {
    EXPECT_EQ(static_cast<int>(NxShaderUniforms::MODEL_MATRIX), 1);
}

TEST_F(ShaderUniformsEnumTest, CameraPositionExists) {
    EXPECT_EQ(static_cast<int>(NxShaderUniforms::CAMERA_POSITION), 2);
}

TEST_F(ShaderUniformsEnumTest, TextureSamplerExists) {
    EXPECT_EQ(static_cast<int>(NxShaderUniforms::TEXTURE_SAMPLER), 3);
}

TEST_F(ShaderUniformsEnumTest, DirLightExists) {
    EXPECT_EQ(static_cast<int>(NxShaderUniforms::DIR_LIGHT), 4);
}

TEST_F(ShaderUniformsEnumTest, AmbientLightExists) {
    EXPECT_EQ(static_cast<int>(NxShaderUniforms::AMBIENT_LIGHT), 5);
}

TEST_F(ShaderUniformsEnumTest, PointLightArrayExists) {
    EXPECT_EQ(static_cast<int>(NxShaderUniforms::POINT_LIGHT_ARRAY), 6);
}

TEST_F(ShaderUniformsEnumTest, NbPointLightExists) {
    EXPECT_EQ(static_cast<int>(NxShaderUniforms::NB_POINT_LIGHT), 7);
}

TEST_F(ShaderUniformsEnumTest, SpotLightArrayExists) {
    EXPECT_EQ(static_cast<int>(NxShaderUniforms::SPOT_LIGHT_ARRAY), 8);
}

TEST_F(ShaderUniformsEnumTest, NbSpotLightExists) {
    EXPECT_EQ(static_cast<int>(NxShaderUniforms::NB_SPOT_LIGHT), 9);
}

TEST_F(ShaderUniformsEnumTest, MaterialExists) {
    EXPECT_EQ(static_cast<int>(NxShaderUniforms::MATERIAL), 10);
}

// =============================================================================
// ShaderUniformsName Map Tests
// =============================================================================

class ShaderUniformsNameTest : public ::testing::Test {};

TEST_F(ShaderUniformsNameTest, ViewProjectionName) {
    auto it = ShaderUniformsName.find(NxShaderUniforms::VIEW_PROJECTION);
    ASSERT_NE(it, ShaderUniformsName.end());
    EXPECT_EQ(it->second, "uViewProjection");
}

TEST_F(ShaderUniformsNameTest, ModelMatrixName) {
    auto it = ShaderUniformsName.find(NxShaderUniforms::MODEL_MATRIX);
    ASSERT_NE(it, ShaderUniformsName.end());
    EXPECT_EQ(it->second, "uMatModel");
}

TEST_F(ShaderUniformsNameTest, CameraPositionName) {
    auto it = ShaderUniformsName.find(NxShaderUniforms::CAMERA_POSITION);
    ASSERT_NE(it, ShaderUniformsName.end());
    EXPECT_EQ(it->second, "uCamPos");
}

TEST_F(ShaderUniformsNameTest, TextureSamplerName) {
    auto it = ShaderUniformsName.find(NxShaderUniforms::TEXTURE_SAMPLER);
    ASSERT_NE(it, ShaderUniformsName.end());
    EXPECT_EQ(it->second, "uTexture");
}

TEST_F(ShaderUniformsNameTest, DirLightName) {
    auto it = ShaderUniformsName.find(NxShaderUniforms::DIR_LIGHT);
    ASSERT_NE(it, ShaderUniformsName.end());
    EXPECT_EQ(it->second, "uDirLight");
}

TEST_F(ShaderUniformsNameTest, AmbientLightName) {
    auto it = ShaderUniformsName.find(NxShaderUniforms::AMBIENT_LIGHT);
    ASSERT_NE(it, ShaderUniformsName.end());
    EXPECT_EQ(it->second, "uAmbientLight");
}

TEST_F(ShaderUniformsNameTest, PointLightArrayName) {
    auto it = ShaderUniformsName.find(NxShaderUniforms::POINT_LIGHT_ARRAY);
    ASSERT_NE(it, ShaderUniformsName.end());
    EXPECT_EQ(it->second, "uPointLights");
}

TEST_F(ShaderUniformsNameTest, NbPointLightName) {
    auto it = ShaderUniformsName.find(NxShaderUniforms::NB_POINT_LIGHT);
    ASSERT_NE(it, ShaderUniformsName.end());
    EXPECT_EQ(it->second, "uNbPointLights");
}

TEST_F(ShaderUniformsNameTest, SpotLightArrayName) {
    auto it = ShaderUniformsName.find(NxShaderUniforms::SPOT_LIGHT_ARRAY);
    ASSERT_NE(it, ShaderUniformsName.end());
    EXPECT_EQ(it->second, "uSpotLights");
}

TEST_F(ShaderUniformsNameTest, NbSpotLightName) {
    auto it = ShaderUniformsName.find(NxShaderUniforms::NB_SPOT_LIGHT);
    ASSERT_NE(it, ShaderUniformsName.end());
    EXPECT_EQ(it->second, "uNbSpotLights");
}

TEST_F(ShaderUniformsNameTest, MaterialName) {
    auto it = ShaderUniformsName.find(NxShaderUniforms::MATERIAL);
    ASSERT_NE(it, ShaderUniformsName.end());
    EXPECT_EQ(it->second, "uMaterial");
}

TEST_F(ShaderUniformsNameTest, AllUniformsHaveNames) {
    EXPECT_EQ(ShaderUniformsName.size(), 11);
}

// =============================================================================
// UniformInfo Struct Tests
// =============================================================================

class UniformInfoTest : public ::testing::Test {};

TEST_F(UniformInfoTest, DefaultConstruction) {
    UniformInfo info;
    EXPECT_TRUE(info.name.empty());
}

TEST_F(UniformInfoTest, SetNameField) {
    UniformInfo info;
    info.name = "uTestUniform";
    EXPECT_EQ(info.name, "uTestUniform");
}

TEST_F(UniformInfoTest, SetLocationField) {
    UniformInfo info;
    info.location = 5;
    EXPECT_EQ(info.location, 5);
}

TEST_F(UniformInfoTest, SetTypeField) {
    UniformInfo info;
    info.type = 0x1406;  // GL_FLOAT
    EXPECT_EQ(info.type, 0x1406u);
}

TEST_F(UniformInfoTest, SetSizeField) {
    UniformInfo info;
    info.size = 16;
    EXPECT_EQ(info.size, 16);
}

TEST_F(UniformInfoTest, FullConfiguration) {
    UniformInfo info;
    info.name = "uModelMatrix";
    info.location = 3;
    info.type = 0x8B5C;  // GL_MAT4
    info.size = 1;

    EXPECT_EQ(info.name, "uModelMatrix");
    EXPECT_EQ(info.location, 3);
    EXPECT_EQ(info.type, 0x8B5Cu);
    EXPECT_EQ(info.size, 1);
}

// =============================================================================
// AttributeInfo Struct Tests
// =============================================================================

class AttributeInfoTest : public ::testing::Test {};

TEST_F(AttributeInfoTest, DefaultConstruction) {
    AttributeInfo info;
    EXPECT_TRUE(info.name.empty());
}

TEST_F(AttributeInfoTest, SetNameField) {
    AttributeInfo info;
    info.name = "aPosition";
    EXPECT_EQ(info.name, "aPosition");
}

TEST_F(AttributeInfoTest, SetLocationField) {
    AttributeInfo info;
    info.location = 0;
    EXPECT_EQ(info.location, 0);
}

TEST_F(AttributeInfoTest, SetTypeField) {
    AttributeInfo info;
    info.type = 0x8B52;  // GL_FLOAT_VEC4
    EXPECT_EQ(info.type, 0x8B52u);
}

TEST_F(AttributeInfoTest, SetSizeField) {
    AttributeInfo info;
    info.size = 3;
    EXPECT_EQ(info.size, 3);
}

TEST_F(AttributeInfoTest, FullConfiguration) {
    AttributeInfo info;
    info.name = "aNormal";
    info.location = 1;
    info.type = 0x8B51;  // GL_FLOAT_VEC3
    info.size = 1;

    EXPECT_EQ(info.name, "aNormal");
    EXPECT_EQ(info.location, 1);
    EXPECT_EQ(info.type, 0x8B51u);
    EXPECT_EQ(info.size, 1);
}

// =============================================================================
// RequiredAttributes Tests
// =============================================================================

class RequiredAttributesTest : public ::testing::Test {};

TEST_F(RequiredAttributesTest, DefaultAllFalse) {
    RequiredAttributes attrs;
    EXPECT_EQ(attrs.bitsUnion.bits, 0);
}

TEST_F(RequiredAttributesTest, SetPositionFlag) {
    RequiredAttributes attrs;
    attrs.bitsUnion.flags.position = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.position);
}

TEST_F(RequiredAttributesTest, SetNormalFlag) {
    RequiredAttributes attrs;
    attrs.bitsUnion.flags.normal = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.normal);
}

TEST_F(RequiredAttributesTest, SetTangentFlag) {
    RequiredAttributes attrs;
    attrs.bitsUnion.flags.tangent = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.tangent);
}

TEST_F(RequiredAttributesTest, SetBitangentFlag) {
    RequiredAttributes attrs;
    attrs.bitsUnion.flags.bitangent = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.bitangent);
}

TEST_F(RequiredAttributesTest, SetUv0Flag) {
    RequiredAttributes attrs;
    attrs.bitsUnion.flags.uv0 = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.uv0);
}

TEST_F(RequiredAttributesTest, SetLightmapUVFlag) {
    RequiredAttributes attrs;
    attrs.bitsUnion.flags.lightmapUV = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.lightmapUV);
}

TEST_F(RequiredAttributesTest, MultipleFlags) {
    RequiredAttributes attrs;
    attrs.bitsUnion.flags.position = true;
    attrs.bitsUnion.flags.normal = true;
    attrs.bitsUnion.flags.uv0 = true;

    EXPECT_TRUE(attrs.bitsUnion.flags.position);
    EXPECT_TRUE(attrs.bitsUnion.flags.normal);
    EXPECT_TRUE(attrs.bitsUnion.flags.uv0);
    EXPECT_FALSE(attrs.bitsUnion.flags.tangent);
}

TEST_F(RequiredAttributesTest, EqualityOperatorSame) {
    RequiredAttributes attrs1;
    attrs1.bitsUnion.flags.position = true;
    attrs1.bitsUnion.flags.normal = true;

    RequiredAttributes attrs2;
    attrs2.bitsUnion.flags.position = true;
    attrs2.bitsUnion.flags.normal = true;

    EXPECT_TRUE(attrs1 == attrs2);
}

TEST_F(RequiredAttributesTest, EqualityOperatorDifferent) {
    RequiredAttributes attrs1;
    attrs1.bitsUnion.flags.position = true;

    RequiredAttributes attrs2;
    attrs2.bitsUnion.flags.normal = true;

    EXPECT_FALSE(attrs1 == attrs2);
}

TEST_F(RequiredAttributesTest, CompatibleWithSameAttrs) {
    RequiredAttributes required;
    required.bitsUnion.flags.position = true;
    required.bitsUnion.flags.normal = true;

    RequiredAttributes mesh;
    mesh.bitsUnion.flags.position = true;
    mesh.bitsUnion.flags.normal = true;

    EXPECT_TRUE(required.compatibleWith(mesh));
}

TEST_F(RequiredAttributesTest, CompatibleWithMoreAttrs) {
    RequiredAttributes required;
    required.bitsUnion.flags.position = true;

    RequiredAttributes mesh;
    mesh.bitsUnion.flags.position = true;
    mesh.bitsUnion.flags.normal = true;
    mesh.bitsUnion.flags.uv0 = true;

    EXPECT_TRUE(required.compatibleWith(mesh));
}

TEST_F(RequiredAttributesTest, NotCompatibleWithFewerAttrs) {
    RequiredAttributes required;
    required.bitsUnion.flags.position = true;
    required.bitsUnion.flags.normal = true;
    required.bitsUnion.flags.uv0 = true;

    RequiredAttributes mesh;
    mesh.bitsUnion.flags.position = true;

    EXPECT_FALSE(required.compatibleWith(mesh));
}

// =============================================================================
// UniformCache Tests
// =============================================================================

class UniformCacheTest : public ::testing::Test {
protected:
    UniformCache cache;

    void SetUp() override {
        std::unordered_map<std::string, ShaderUniformInfo> uniforms = {
            {"uTime",        {"uTime",        GL_FLOAT,      1, 0}},
            {"uResolution",  {"uResolution",  GL_FLOAT_VEC2, 1, 1}},
            {"uLightPos",    {"uLightPos",    GL_FLOAT_VEC3, 1, 2}},
            {"uColor",       {"uColor",       GL_FLOAT_VEC4, 1, 3}},
            {"uSampler",     {"uSampler",     GL_INT,        1, 4}},
            {"uEnabled",     {"uEnabled",     GL_BOOL,       1, 5}},
            {"uModelMatrix", {"uModelMatrix", GL_FLOAT_MAT4, 1, 6}},
            {"uValue",       {"uValue",       GL_FLOAT,      1, 7}},
            {"uA",           {"uA",           GL_FLOAT,      1, 8}},
            {"uB",           {"uB",           GL_INT,        1, 9}},
            {"uC",           {"uC",           GL_BOOL,       1, 10}},
        };
        cache.initialize(uniforms);
    }
};

TEST_F(UniformCacheTest, SetFloat) {
    EXPECT_TRUE(cache.setFloat("uTime", 1.5f));
    EXPECT_TRUE(cache.hasValue("uTime"));
    EXPECT_TRUE(cache.isDirty("uTime"));
}

TEST_F(UniformCacheTest, SetFloat2) {
    EXPECT_TRUE(cache.setFloat2("uResolution", glm::vec2(1920.0f, 1080.0f)));
    EXPECT_TRUE(cache.hasValue("uResolution"));
}

TEST_F(UniformCacheTest, SetFloat3) {
    EXPECT_TRUE(cache.setFloat3("uLightPos", glm::vec3(1.0f, 2.0f, 3.0f)));
    EXPECT_TRUE(cache.hasValue("uLightPos"));
}

TEST_F(UniformCacheTest, SetFloat4) {
    EXPECT_TRUE(cache.setFloat4("uColor", glm::vec4(1.0f, 0.5f, 0.25f, 1.0f)));
    EXPECT_TRUE(cache.hasValue("uColor"));
}

TEST_F(UniformCacheTest, SetInt) {
    EXPECT_TRUE(cache.setInt("uSampler", 5));
    EXPECT_TRUE(cache.hasValue("uSampler"));
}

TEST_F(UniformCacheTest, SetBool) {
    EXPECT_TRUE(cache.setBool("uEnabled", true));
    EXPECT_TRUE(cache.hasValue("uEnabled"));
}

TEST_F(UniformCacheTest, SetMatrix) {
    glm::mat4 identity(1.0f);
    EXPECT_TRUE(cache.setMatrix("uModelMatrix", identity));
    EXPECT_TRUE(cache.hasValue("uModelMatrix"));
}

TEST_F(UniformCacheTest, HasValueFalseForMissing) {
    EXPECT_FALSE(cache.hasValue("uNonExistent"));
}

TEST_F(UniformCacheTest, IsDirtyAfterSet) {
    cache.setFloat("uValue", 1.0f);
    EXPECT_TRUE(cache.isDirty("uValue"));
}

TEST_F(UniformCacheTest, ClearDirtyFlag) {
    cache.setFloat("uValue", 1.0f);
    cache.clearDirtyFlag("uValue");
    EXPECT_FALSE(cache.isDirty("uValue"));
}

TEST_F(UniformCacheTest, ClearAllDirtyFlags) {
    cache.setFloat("uA", 1.0f);
    cache.setInt("uB", 2);
    cache.setBool("uC", true);

    cache.clearAllDirtyFlags();

    EXPECT_FALSE(cache.isDirty("uA"));
    EXPECT_FALSE(cache.isDirty("uB"));
    EXPECT_FALSE(cache.isDirty("uC"));
}

TEST_F(UniformCacheTest, UpdateValueMarksDirty) {
    cache.setFloat("uValue", 1.0f);
    cache.clearDirtyFlag("uValue");
    EXPECT_FALSE(cache.isDirty("uValue"));

    cache.setFloat("uValue", 2.0f);
    EXPECT_TRUE(cache.isDirty("uValue"));
}

}  // namespace nexo::renderer
