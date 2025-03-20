//// AssetCatalog.test.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        20/03/2025
//  Description: Unit tests for the AssetCatalog class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/AssetCatalog.hpp"
#include "assets/Asset.hpp"
#include "assets/Assets/Texture/Texture.hpp"
#include "assets/Assets/Model/Model.hpp"

namespace nexo::assets {

    class MockAssetCatalog : public AssetCatalog {
        public:
            MockAssetCatalog() = default;
            ~MockAssetCatalog() = default;

            // Mock methods if needed
    };

    class AssetCatalogTest : public ::testing::Test {
        protected:
            AssetCatalogTest() : assetCatalog() {
            }

            ~AssetCatalogTest() override {
            }

            void SetUp() override {
            }

            void TearDown() override {
            }

            MockAssetCatalog assetCatalog;
    };

    TEST_F(AssetCatalogTest, RegisterAndRetrieveAssetById) {
        // Register an asset
        const AssetLocation location("text@test/texture");
        const auto textureAsset = new Texture();
        const auto ref = assetCatalog.registerAsset(location, textureAsset);
        ASSERT_TRUE(ref.isValid());

        const auto id = ref.lock()->getID();

        // Retrieve by ID
        const auto retrievedRef = assetCatalog.getAsset(id);

        EXPECT_TRUE(retrievedRef.isValid());
        EXPECT_EQ(retrievedRef.lock()->getID(), id);
    }

    TEST_F(AssetCatalogTest, RegisterAndRetrieveAssetByLocation) {
        // Register an asset
        const AssetLocation location("text@test/texture");
        const auto textureAsset = new Texture();
        const auto ref = assetCatalog.registerAsset(location, textureAsset);
        ASSERT_TRUE(ref.isValid());
        ASSERT_TRUE(ref);

        // Retrieve by location
        const auto retrievedRef = assetCatalog.getAsset(location);

        EXPECT_TRUE(retrievedRef.isValid());
        EXPECT_TRUE(retrievedRef);
        EXPECT_EQ(retrievedRef.lock()->getID(), ref.lock()->getID());
    }

    TEST_F(AssetCatalogTest, DeleteAssetById) {
        AssetLocation location("text@test/texture");
        const auto textureAsset = new Texture();
        const auto ref = assetCatalog.registerAsset(location, textureAsset);
        const auto id = ref.lock()->getID();

        // Delete by ID
        assetCatalog.deleteAsset(id);

        // Asset should no longer be retrievable
        const auto retrievedRef = assetCatalog.getAsset(id);
        EXPECT_FALSE(retrievedRef.isValid());
        EXPECT_FALSE(retrievedRef);
        EXPECT_FALSE(retrievedRef.lock());

        EXPECT_FALSE(ref.isValid());
        EXPECT_FALSE(ref);
        EXPECT_FALSE(ref.lock());
    }

    TEST_F(AssetCatalogTest, DeleteAssetByReference) {
        AssetLocation location("text@test/texture");
        const auto textureAsset = new Texture();
        const auto ref = assetCatalog.registerAsset(location, textureAsset);

        // Delete by reference
        assetCatalog.deleteAsset(ref);

        // Asset should no longer be retrievable
        const auto retrievedRef = assetCatalog.getAsset(location);
        EXPECT_FALSE(retrievedRef.isValid());
        EXPECT_FALSE(retrievedRef);
        EXPECT_FALSE(retrievedRef.lock());

        EXPECT_FALSE(ref.isValid());
        EXPECT_FALSE(ref);
        EXPECT_FALSE(ref.lock());
    }

    TEST_F(AssetCatalogTest, GetAssetsReturnsAllAssets) {
        const auto textureAsset = new Texture();
        const auto modelAsset = new Model();

        // Register multiple assets
        assetCatalog.registerAsset(AssetLocation("text@test/texture"), textureAsset);
        assetCatalog.registerAsset(AssetLocation("model@test/model"), modelAsset);

        // Get all assets
        auto assets = assetCatalog.getAssets();

        // Should have 2 assets
        EXPECT_EQ(assets.size(), 2);
        for (const auto& assetRef : assets) {
            EXPECT_TRUE(assetRef);
            EXPECT_TRUE(assetRef.isValid());
            EXPECT_TRUE(assetRef.lock());
        }
    }

    TEST_F(AssetCatalogTest, GetAssetsReturnsAllAssetsViews) {
        const auto textureAsset = new Texture();
        const auto modelAsset = new Model();

        // Register multiple assets
        const auto textRef = assetCatalog.registerAsset(AssetLocation("text@test/texture"), textureAsset);
        const auto modelRef = assetCatalog.registerAsset(AssetLocation("model@test/model"), modelAsset);

        // Get all assets as a view
        auto assetsView = assetCatalog.getAssetsView();

        // Should have 2 assets
        EXPECT_EQ(assetsView.size(), 2);
        for (const GenericAssetRef& assetRef : assetsView) {
            EXPECT_TRUE(assetRef);
            EXPECT_TRUE(assetRef.isValid());
            EXPECT_TRUE(assetRef.lock());
        }

        // Delete all assets
        assetCatalog.deleteAsset(textRef);
        assetCatalog.deleteAsset(modelRef);

        assetsView = assetCatalog.getAssetsView();
        // Should have 0 assets
        EXPECT_EQ(assetsView.size(), 0);
    }

    TEST_F(AssetCatalogTest, MultipleAssetsDeleteOne) {
        const auto textureAsset = new Texture();
        const auto modelAsset = new Model();

        // Register multiple assets
        const auto textRef = assetCatalog.registerAsset(AssetLocation("text@test/texture"), textureAsset);
        const auto modelRef = assetCatalog.registerAsset(AssetLocation("model@test/model"), modelAsset);
        const auto modelId = modelRef.lock()->getID();

        // Get all assets
        auto assets = assetCatalog.getAssets();

        // Should have 2 assets
        EXPECT_EQ(assets.size(), 2);
        for (const auto& assetRef : assets) {
            EXPECT_TRUE(assetRef);
            EXPECT_TRUE(assetRef.isValid());
            EXPECT_TRUE(assetRef.lock());
        }

        // Delete model
        assetCatalog.deleteAsset(modelRef);
        EXPECT_FALSE(modelRef.isValid());
        EXPECT_FALSE(modelRef);
        EXPECT_FALSE(modelRef.lock());

        assets = assetCatalog.getAssets();
        // Should have 1 asset
        EXPECT_EQ(assets.size(), 1);
        EXPECT_EQ(assets[0].lock()->getID(), textRef.lock()->getID());

        // Check that the model asset is no longer retrievable
        auto retrievedRef = assetCatalog.getAsset(modelId);
        EXPECT_FALSE(retrievedRef.isValid());
        EXPECT_FALSE(retrievedRef);
        EXPECT_FALSE(retrievedRef.lock());

        // Check that the texture asset is still retrievable
        retrievedRef = assetCatalog.getAsset(textRef.lock()->getID());
        EXPECT_TRUE(retrievedRef.isValid());
        EXPECT_TRUE(retrievedRef);
        EXPECT_TRUE(retrievedRef.lock());
    }

    TEST_F(AssetCatalogTest, GetNonExistentAssetReturnsInvalidRef) {
        // Try to get asset with non-existent ID
        constexpr AssetID nonExistentId;  // Default-constructed UUID should be nil
        auto ref = assetCatalog.getAsset(nonExistentId);

        EXPECT_FALSE(ref.isValid());
        EXPECT_FALSE(ref);
        EXPECT_FALSE(ref.lock());

        // Try to get asset with non-existent location
        AssetLocation nonExistentLocation("test@does/not/exist");
        ref = assetCatalog.getAsset(nonExistentLocation);

        EXPECT_FALSE(ref.isValid());
        EXPECT_FALSE(ref);
        EXPECT_FALSE(ref.lock());
    }

    TEST_F(AssetCatalogTest, GetNoAssets)
    {
        const AssetLocation nonExistentLocation("test@does/not/exist");
        assetCatalog.registerAsset(nonExistentLocation, nullptr);

        assetCatalog.deleteAsset(AssetID{});
        const auto assets = assetCatalog.getAssets();
        EXPECT_EQ(assets.size(), 0);

        auto assetsView = assetCatalog.getAssetsView();
        EXPECT_EQ(assetsView.size(), 0);
    }

    class AssetCatalogSingletonTest : public ::testing::Test {
        protected:
        AssetCatalogSingletonTest() {
        }

        ~AssetCatalogSingletonTest() override {
        }

        // Do not call AssetCatalog::getInstance() before tests
        // to avoid creating the singleton instance before the test SingletonCreationMultithreaded

        void TearDown() override {
            // Clean up the singleton instance
            const auto assets = AssetCatalog::getInstance().getAssets();
            for (auto& asset : assets) {
                AssetCatalog::getInstance().deleteAsset(asset);
            }
        }
    };

    TEST_F(AssetCatalogSingletonTest, SingletonCreationMultithreaded)
    {
        // Get instance at the same time on multiple threads, verify that they are the same
        std::vector<std::thread> threads;

        std::vector<AssetCatalog *> instances;
        std::mutex instancesMutex;

        constexpr int numThreads = 5;

        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([&instances, &instancesMutex]() {
                auto& instance = AssetCatalog::getInstance();
                EXPECT_TRUE(instance.getAssetsView().empty());

                // Store the instance in a thread-safe manner
                instancesMutex.lock();
                instances.push_back(&instance);
                instancesMutex.unlock();
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }
        // Check that all instances are the same
        for (const auto& instance : instances) {
            EXPECT_EQ(instance, &AssetCatalog::getInstance());
        }
    }

    TEST_F(AssetCatalogSingletonTest, SingletonInstance)
    {
        auto& instance1 = AssetCatalog::getInstance();
        auto& instance2 = AssetCatalog::getInstance();

        EXPECT_EQ(&instance1, &instance2);
        EXPECT_TRUE(AssetCatalog::getInstance().getAssetsView().empty());
    }

    TEST_F(AssetCatalogSingletonTest, SingletonRegisterAndRetrieve)
    {
        auto& instance = AssetCatalog::getInstance();

        const AssetLocation location("text@test/texture");
        const auto textureAsset = new Texture();
        const auto ref = instance.registerAsset(location, textureAsset);
        ASSERT_TRUE(ref.isValid());

        const auto id = ref.lock()->getID();

        // Retrieve by ID
        const auto retrievedRef = instance.getAsset(id);

        EXPECT_TRUE(retrievedRef.isValid());
        EXPECT_EQ(retrievedRef.lock()->getID(), id);
    }

    TEST_F(AssetCatalogSingletonTest, SingletonShouldBeEmpty)
    {
        EXPECT_TRUE(AssetCatalog::getInstance().getAssetsView().empty());
    }

    TEST_F(AssetCatalogSingletonTest, SingletonDeleteAsset)
    {
        auto& instance = AssetCatalog::getInstance();

        const AssetLocation location("text@test/texture");
        const auto textureAsset = new Texture();
        const auto ref = instance.registerAsset(location, textureAsset);
        ASSERT_TRUE(ref.isValid());

        const auto id = ref.lock()->getID();

        // Delete by ID
        instance.deleteAsset(id);

        // Asset should no longer be retrievable
        const auto retrievedRef = instance.getAsset(id);
        EXPECT_FALSE(retrievedRef.isValid());
        EXPECT_FALSE(retrievedRef);
        EXPECT_FALSE(retrievedRef.lock());
        EXPECT_FALSE(ref.isValid());
        EXPECT_FALSE(ref);
    }


    // TODO: Tests for getAssetsOfType and getAssetsOfTypeView would need to be added once the static_assert in these methods is resolved
} // namespace nexo::assets
