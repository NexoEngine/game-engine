//// AssetImporterContext.test.cpp ////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        04/04/2025
//  Description: Unit tests for the AssetImporterContext class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/AssetImporterContext.hpp"
#include "assets/Asset.hpp"
#include "assets/AssetCatalog.hpp"
#include "assets/Assets/Texture/Texture.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "json.hpp"

namespace nexo::assets {

    struct TestParams {
        int intValue{};
        float floatValue{};
        std::string stringValue;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(TestParams,
            intValue,
            floatValue,
            stringValue
        )
    };

    class AssetImporterContextTest : public ::testing::Test {
    protected:
        void SetUp() override
        {
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
        }

        AssetImporterContext context;
    };

    TEST_F(AssetImporterContextTest, GetMainAssetEmptyOnCreation)
    {
        EXPECT_EQ(context.getMainAsset(), nullptr);
    }

    TEST_F(AssetImporterContextTest, SetAndGetMainAsset)
    {
        auto* asset = new Texture();
        context.setMainAsset(asset);
        EXPECT_EQ(context.getMainAsset(), asset);
    }

    TEST_F(AssetImporterContextTest, GetDependenciesEmptyOnCreation)
    {
        EXPECT_TRUE(context.getDependencies().empty());
    }

    TEST_F(AssetImporterContextTest, AddAndGetDependency)
    {
        // Register an asset first to get a valid reference
        auto& catalog = AssetCatalog::getInstance();
        auto* asset = new Texture();
        auto ref = catalog.registerAsset(AssetLocation("test@texture/dependency"), asset);
        EXPECT_TRUE(ref);

        // Add as dependency
        context.addDependency(ref);

        // Check dependency was added
        auto dependencies = context.getDependencies();
        ASSERT_EQ(dependencies.size(), 1);
        EXPECT_EQ(dependencies[0].lock()->getID(), ref.lock()->getID());
    }

    TEST_F(AssetImporterContextTest, AddMultipleDependencies)
    {
        auto& catalog = AssetCatalog::getInstance();

        // Create and register multiple assets
        auto* texture = new Texture();
        auto* model = new Model();
        auto textureRef = catalog.registerAsset(AssetLocation("text@path"), texture);
        auto modelRef = catalog.registerAsset(AssetLocation("model@path"), model);
        EXPECT_TRUE(textureRef);
        EXPECT_TRUE(modelRef);

        // Add dependencies
        context.addDependency(textureRef);
        context.addDependency(modelRef);

        // Check dependencies were added
        auto dependencies = context.getDependencies();
        ASSERT_EQ(dependencies.size(), 2);
        EXPECT_EQ(dependencies[0].lock()->getID(), textureRef.lock()->getID());
        EXPECT_EQ(dependencies[1].lock()->getID(), modelRef.lock()->getID());
    }

    TEST_F(AssetImporterContextTest, SetAndGetJsonParameters)
    {
        json params = {
            {"name", "test"},
            {"value", 42},
            {"enabled", true}
        };

        context.setParameters(params);
        auto retrievedParams = context.getParameters();

        EXPECT_EQ(retrievedParams["name"], "test");
        EXPECT_EQ(retrievedParams["value"], 42);
        EXPECT_EQ(retrievedParams["enabled"], true);
    }

    TEST_F(AssetImporterContextTest, SetAndGetTypedParameters)
    {
        const TestParams params {
            .intValue = 123,
            .floatValue = 3.14f,
            .stringValue = "test"
        };

        context.setParameters(params);
        auto [intValue, floatValue, stringValue] = context.getParameters<TestParams>();

        EXPECT_EQ(intValue, 123);
        EXPECT_FLOAT_EQ(floatValue, 3.14f);
        EXPECT_EQ(stringValue, "test");
    }

    TEST_F(AssetImporterContextTest, GenUniqueDependencyLocation)
    {
        context.location = AssetLocation("test@folder/main");

        // Generate a unique name for a texture dependency
        const auto depName1 = context.genUniqueDependencyLocation<Texture>();
        EXPECT_EQ(depName1.getFullLocation(), "test_TEXTURE1@folder/main");

        // Generate another unique name
        const auto depName2 = context.genUniqueDependencyLocation<Texture>();
        EXPECT_EQ(depName2.getFullLocation(), "test_TEXTURE2@folder/main");

        // Names should be different
        EXPECT_NE(depName1.getFullLocation(), depName2.getFullLocation());
    }

    TEST_F(AssetImporterContextTest, GenUniqueDependencyLocationWithExistingAsset)
    {
        context.location = AssetLocation("test@folder/main");

        // Generate a name
        auto depName1 = context.genUniqueDependencyLocation<Texture>();
        EXPECT_EQ(depName1.getFullLocation(), "test_TEXTURE1@folder/main");

        // Register an asset with that name
        auto& catalog = AssetCatalog::getInstance();
        auto* asset = new Texture();
        EXPECT_TRUE(catalog.registerAsset(depName1, asset));

        // Generate another name - should be different
        const auto depName2 = context.genUniqueDependencyLocation<Texture>();
        EXPECT_NE(depName1.getFullLocation(), depName2.getFullLocation());
        EXPECT_EQ(depName2.getFullLocation(), "test_TEXTURE2@folder/main");

        // The new name should not exist in catalog
        EXPECT_FALSE(catalog.getAsset(depName2).isValid());
    }

    TEST_F(AssetImporterContextTest, GenUniqueDependencyLocationWithCollidingName)
    {
        context.location = AssetLocation("test@folder/main");

        // Generate a name
        auto depName1 = context.genUniqueDependencyLocation<Model>();

        // Register an asset with that name
        auto& catalog = AssetCatalog::getInstance();
        auto* asset = new Model();
        EXPECT_TRUE(catalog.registerAsset(depName1, asset));
        EXPECT_EQ(depName1.getFullLocation(), "test_MODEL1@folder/main");

        // Let's register an asset with the same name as a future dependency
        auto* asset2 = new Model();
        EXPECT_TRUE(catalog.registerAsset(AssetLocation("test_MODEL2@folder/main"), asset2));

        // Generate another dep name: should prevent collision
        const auto depName2 = context.genUniqueDependencyLocation<Model>();
        EXPECT_EQ(depName2.getFullLocation(), "test_MODEL3@folder/main");
    }

    TEST_F(AssetImporterContextTest, DefaultContextValues)
    {
        EXPECT_EQ(context.location.getFullLocation(), "default");
        EXPECT_EQ(context.getMainAsset(), nullptr);
        EXPECT_TRUE(context.getDependencies().empty());
        EXPECT_TRUE(context.getParameters().is_null());
    }

    TEST_F(AssetImporterContextTest, InputPropertyExists)
    {
        // Just test that the input property exists and can be assigned
        const ImporterInputVariant input = ImporterFileInput{.filePath = std::filesystem::path("test.png")};
        context.input = input;

        // Check if we can access the input
        ASSERT_TRUE(std::holds_alternative<ImporterFileInput>(context.input));
        EXPECT_EQ(std::get<ImporterFileInput>(context.input).filePath, "test.png");
    }

}
