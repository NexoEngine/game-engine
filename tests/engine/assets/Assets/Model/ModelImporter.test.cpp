//// ModelImporter.test.cpp /////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/04/2025
//  Description: Test file for the ModelImporter class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "assets/Assets/Model/ModelImporter.hpp"
#include "assets/AssetImporterContext.hpp"
#include "assets/AssetRef.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "assets/Assets/Model/ModelParameters.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <filesystem>
#include <string>
#include <vector>

#include "Path.hpp"
#include "../tests/renderer/contexts/opengl.hpp"

using namespace testing;
using namespace nexo::assets;

class TestModelImporter : public ModelImporter {
    FRIEND_TEST(ModelImporterTestFixture, CanReadSupportsValidExtensions);
    FRIEND_TEST(ModelImporterTestFixture, ConvertAssimpMatrixToGLM);
    FRIEND_TEST(ModelImporterTestFixture, ConvertAssimpHintToNxTextureFormat);
    FRIEND_TEST(ModelImporterTestFixture, ImportImplSetsMainAsset);
    FRIEND_TEST(ModelImporterTestFixture, ProcessMeshHandlesEmptyMesh);

};

// Test fixture for ModelImporter tests
class ModelImporterTestFixture : public nexo::renderer::OpenGLTest {
protected:
    void SetUp() override {
        OpenGLTest::SetUp();
        // Clean up the catalog before each test
        auto& catalog = AssetCatalog::getInstance();
        for (auto& asset : catalog.getAssets()) {
            catalog.deleteAsset(asset);
        }
    }

    void TearDown() override
    {
        // Clean up the catalog after each test
        auto& catalog = AssetCatalog::getInstance();
        for (auto& asset : catalog.getAssets()) {
            catalog.deleteAsset(asset);
        }
        OpenGLTest::TearDown();
    }

    TestModelImporter importer;
};

// Test canRead method
TEST_F(ModelImporterTestFixture, CanReadSupportsValidExtensions) {
    // Test with valid extension
    ImporterFileInput fileInput{std::filesystem::path("model.fbx")};
    ImporterInputVariant inputVariant = fileInput;
    EXPECT_TRUE(importer.canRead(inputVariant));

    // Test with another valid extension
    fileInput = ImporterFileInput{std::filesystem::path("model.obj")};
    inputVariant = fileInput;
    EXPECT_TRUE(importer.canRead(inputVariant));

    // Test with invalid extension
    fileInput = ImporterFileInput{std::filesystem::path("model.invalid")};
    inputVariant = fileInput;
    EXPECT_FALSE(importer.canRead(inputVariant));
}

// Test convertAssimpMatrixToGLM method
TEST_F(ModelImporterTestFixture, ConvertAssimpMatrixToGLM) {
    // Create a test aiMatrix4x4
    aiMatrix4x4 aiMat;
    aiMat.a1 = 1.0f; aiMat.a2 = 2.0f; aiMat.a3 = 3.0f; aiMat.a4 = 4.0f;
    aiMat.b1 = 5.0f; aiMat.b2 = 6.0f; aiMat.b3 = 7.0f; aiMat.b4 = 8.0f;
    aiMat.c1 = 9.0f; aiMat.c2 = 10.0f; aiMat.c3 = 11.0f; aiMat.c4 = 12.0f;
    aiMat.d1 = 13.0f; aiMat.d2 = 14.0f; aiMat.d3 = 15.0f; aiMat.d4 = 16.0f;

    // Use the static method to convert the matrix
    glm::mat4 glmMat = TestModelImporter::convertAssimpMatrixToGLM(aiMat);

    // Check if the conversion is correct
    EXPECT_FLOAT_EQ(glmMat[0][0], 1.0f);
    EXPECT_FLOAT_EQ(glmMat[0][1], 5.0f);
    EXPECT_FLOAT_EQ(glmMat[0][2], 9.0f);
    EXPECT_FLOAT_EQ(glmMat[0][3], 13.0f);

    EXPECT_FLOAT_EQ(glmMat[1][0], 2.0f);
    EXPECT_FLOAT_EQ(glmMat[1][1], 6.0f);
    EXPECT_FLOAT_EQ(glmMat[1][2], 10.0f);
    EXPECT_FLOAT_EQ(glmMat[1][3], 14.0f);

    EXPECT_FLOAT_EQ(glmMat[2][0], 3.0f);
    EXPECT_FLOAT_EQ(glmMat[2][1], 7.0f);
    EXPECT_FLOAT_EQ(glmMat[2][2], 11.0f);
    EXPECT_FLOAT_EQ(glmMat[2][3], 15.0f);

    EXPECT_FLOAT_EQ(glmMat[3][0], 4.0f);
    EXPECT_FLOAT_EQ(glmMat[3][1], 8.0f);
    EXPECT_FLOAT_EQ(glmMat[3][2], 12.0f);
    EXPECT_FLOAT_EQ(glmMat[3][3], 16.0f);
}

// Test convertAssimpHintToNxTextureFormat method
TEST_F(ModelImporterTestFixture, ConvertAssimpHintToNxTextureFormat) {
    // Test various format hints
    EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat("rgba8888"), nexo::renderer::NxTextureFormat::RGBA8);
    EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat("rgba8880"), nexo::renderer::NxTextureFormat::RGB8);
    EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat("rgba8800"), nexo::renderer::NxTextureFormat::RG8);
    EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat("rgba8000"), nexo::renderer::NxTextureFormat::R8);

    // Tests invalid format because of length
    EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat("rgba88888"), nexo::renderer::NxTextureFormat::INVALID);
    EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat("rgba888"), nexo::renderer::NxTextureFormat::INVALID);
    EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat("rgba88"), nexo::renderer::NxTextureFormat::INVALID);
    EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat(""), nexo::renderer::NxTextureFormat::INVALID);

    // Test invalid format hints
    EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat("invalid0"), nexo::renderer::NxTextureFormat::INVALID);
    EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat("rgba7777"), nexo::renderer::NxTextureFormat::INVALID);

    // Test all permutations of rgba
    std::string hint = "abgr0000";
    do {
        hint.replace(hint.begin() + 4, hint.begin() + 8, "0000");

        const auto rPos = std::ranges::find(hint, 'r');
        const auto gPos = std::ranges::find(hint, 'g');
        const auto bPos = std::ranges::find(hint, 'b');
        const auto aPos = std::ranges::find(hint, 'a');

        hint[rPos - hint.begin() + 4] = '8';
        EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat(hint.c_str()), nexo::renderer::NxTextureFormat::R8);

        if (rPos >= gPos)
            continue;
        hint[gPos - hint.begin() + 4] = '8';
        EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat(hint.c_str()), nexo::renderer::NxTextureFormat::RG8);

        if (gPos >= bPos)
            continue;
        hint[bPos - hint.begin() + 4] = '8';
        EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat(hint.c_str()), nexo::renderer::NxTextureFormat::RGB8);

        if (bPos >= aPos)
            continue;
        hint[aPos - hint.begin() + 4] = '8';
        EXPECT_EQ(importer.convertAssimpHintToNxTextureFormat(hint.c_str()), nexo::renderer::NxTextureFormat::RGBA8);
        GTEST_LOG_(INFO) << "HINT: " << hint;
    } while (std::next_permutation(hint.begin(), hint.begin() + 4));
}

// Additional test for processing empty mesh
TEST_F(ModelImporterTestFixture, ImportCubeModel) {
    auto& catalog = AssetCatalog::getInstance();

    AssetImporterContext ctx = {};
    ctx.location = AssetLocation("test::cube_model@test_folder");
    ctx.input = ImporterFileInput{std::filesystem::path(nexo::Path::resolvePathRelativeToExe("../tests/engine/assets/Assets/Model/cube.obj"))};
    importer.import(ctx);

    EXPECT_NE(ctx.getMainAsset(), nullptr);
    EXPECT_EQ(ctx.getMainAsset()->getType(), Model::TYPE);
    EXPECT_EQ(ctx.getMainAsset()->isLoaded(), true);

    const auto allAssets = catalog.getAssets();
    EXPECT_EQ(allAssets.size(), 3); // 2 materials + 1 texture

    auto model = static_cast<Model*>(ctx.getMainAsset().get());
    // Now verify the model data
    const auto& modelData = model->getData();
    ASSERT_NE(modelData, nullptr);

    const auto root = modelData->meshes;

    // Root mesh in this case has zero mesh but 1 child
    ASSERT_EQ(modelData->meshes.size(), 0);
    ASSERT_EQ(modelData->children.size(), 1);
    const auto child = modelData->children[0];
    EXPECT_EQ(child.children.size(), 0);
    EXPECT_EQ(child.meshes.size(), 1);
    const auto childMesh = child.meshes[0];

    // Get the mesh and verify its properties
    EXPECT_EQ(childMesh.name, "Cube");

    // A cube should have 8 vertices and 12 triangles (36 indices)
    //EXPECT_EQ(childMesh.vertices.size(), 24); // 24 because each vertex is duplicated for different face normals/UVs
    EXPECT_EQ(childMesh.vao->getIndexBuffer()->getCount(), 36);  // 6 faces × 2 triangles × 3 vertices

    // Check the Material reference
    const auto material = childMesh.material.lock();
    EXPECT_NE(material, nullptr);

    const auto materialData = material->getData().get();
    EXPECT_NE(materialData, nullptr);

    EXPECT_NE(materialData->albedoTexture, nullptr);
    const auto albedoTextureAsset = materialData->albedoTexture.lock();
    EXPECT_NE(albedoTextureAsset, nullptr);
    EXPECT_EQ(albedoTextureAsset->getType(), Texture::TYPE);

    const auto albedoTexture = albedoTextureAsset->getData().get();
    EXPECT_NE(albedoTexture->texture, nullptr);
    EXPECT_EQ(albedoTexture->texture->getWidth(), 64);
    EXPECT_EQ(albedoTexture->texture->getHeight(), 64);

    // Check material properties from the MTL file
    EXPECT_FLOAT_EQ(materialData->specularColor.r, 0.5f);
    EXPECT_FLOAT_EQ(materialData->specularColor.g, 0.5f);
    EXPECT_FLOAT_EQ(materialData->specularColor.b, 0.5f);

    EXPECT_FLOAT_EQ(materialData->emissiveColor.r, 0.0f);
    EXPECT_FLOAT_EQ(materialData->emissiveColor.g, 0.0f);
    EXPECT_FLOAT_EQ(materialData->emissiveColor.b, 0.0f);

    // Check roughness and metallic properties if supported
    EXPECT_FLOAT_EQ(materialData->roughness, 0.5f); // Pr in MTL
    EXPECT_FLOAT_EQ(materialData->metallic, 0.7f);  // Pm in MTL
}
