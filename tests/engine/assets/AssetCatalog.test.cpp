//// AssetCatalog.test.cpp ////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
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
#include <boost/uuid/uuid_hash.hpp>

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
        auto textureAsset = std::make_unique<Texture>();
        const auto ref = assetCatalog.registerAsset(location, std::move(textureAsset));
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
        auto textureAsset = std::make_unique<Texture>();
        const auto ref = assetCatalog.registerAsset(location, std::move(textureAsset));
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
        auto textureAsset = std::make_unique<Texture>();
        const auto ref          = assetCatalog.registerAsset(location, std::move(textureAsset));
        const auto id           = ref.lock()->getID();

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
        auto textureAsset = std::make_unique<Texture>();
        const auto ref = assetCatalog.registerAsset(location, std::move(textureAsset));

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
        auto textureAsset = std::make_unique<Texture>();
        auto modelAsset = std::make_unique<Model>();

        // Register multiple assets
        assetCatalog.registerAsset(AssetLocation("text@test/texture"), std::move(textureAsset));
        assetCatalog.registerAsset(AssetLocation("model@test/model"), std::move(modelAsset));

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
        auto textureAsset = std::make_unique<Texture>();
        auto modelAsset = std::make_unique<Model>();

        // Register multiple assets
        const auto textRef  = assetCatalog.registerAsset(AssetLocation("text@test/texture"), std::move(textureAsset));
        const auto modelRef = assetCatalog.registerAsset(AssetLocation("model@test/model"), std::move(modelAsset));

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
        auto textureAsset = std::make_unique<Texture>();
        auto modelAsset = std::make_unique<Model>();

        // Register multiple assets
        const auto textRef  = assetCatalog.registerAsset(AssetLocation("text@test/texture"), std::move(textureAsset));
        const auto modelRef = assetCatalog.registerAsset(AssetLocation("model@test/model"), std::move(modelAsset));
        const auto modelId  = modelRef.lock()->getID();

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
        auto textureAsset = std::make_unique<Texture>();
        const auto ref = instance.registerAsset(location, std::move(textureAsset));
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
        auto textureAsset = std::make_unique<Texture>();
        const auto ref = instance.registerAsset(location, std::move(textureAsset));
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

    // ========================================================================
    // Additional Comprehensive Tests for Uncovered Functionality
    // ========================================================================

    // ------------------------------------------------------------------------
    // Asset Registration and Retrieval Tests
    // ------------------------------------------------------------------------

    TEST_F(AssetCatalogTest, RegisterAssetWithNullptrReturnsInvalidRef) {
        const AssetLocation location("null@test/asset");
        const auto ref = assetCatalog.registerAsset(location, nullptr);

        EXPECT_FALSE(ref.isValid());
        EXPECT_FALSE(ref);
        EXPECT_FALSE(ref.lock());
    }

    TEST_F(AssetCatalogTest, RegisterMultipleAssetsWithDifferentLocations) {
        auto texture1 = std::make_unique<Texture>();
        auto texture2 = std::make_unique<Texture>();
        auto texture3 = std::make_unique<Texture>();

        const auto ref1 = assetCatalog.registerAsset(AssetLocation("tex1@path/one"), std::move(texture1));
        const auto ref2 = assetCatalog.registerAsset(AssetLocation("tex2@path/two"), std::move(texture2));
        const auto ref3 = assetCatalog.registerAsset(AssetLocation("tex3@path/three"), std::move(texture3));

        EXPECT_TRUE(ref1.isValid());
        EXPECT_TRUE(ref2.isValid());
        EXPECT_TRUE(ref3.isValid());

        EXPECT_NE(ref1.lock()->getID(), ref2.lock()->getID());
        EXPECT_NE(ref2.lock()->getID(), ref3.lock()->getID());
        EXPECT_NE(ref1.lock()->getID(), ref3.lock()->getID());

        const auto assets = assetCatalog.getAssets();
        EXPECT_EQ(assets.size(), 3);
    }

    TEST_F(AssetCatalogTest, RegisterAssetWithSameLocationMultipleTimes) {
        // This tests the current behavior where duplicate locations are allowed
        // (as per TODO comment in AssetCatalog.cpp line 93)
        const AssetLocation location("duplicate@test/location");

        auto asset1 = std::make_unique<Texture>();
        auto asset2 = std::make_unique<Texture>();

        const auto ref1 = assetCatalog.registerAsset(location, std::move(asset1));
        const auto ref2 = assetCatalog.registerAsset(location, std::move(asset2));

        EXPECT_TRUE(ref1.isValid());
        EXPECT_TRUE(ref2.isValid());

        // Both should be registered with different IDs
        EXPECT_NE(ref1.lock()->getID(), ref2.lock()->getID());

        // getAsset by location should return one of them (O(n) search returns first match)
        const auto retrieved = assetCatalog.getAsset(location);
        EXPECT_TRUE(retrieved.isValid());
    }

    TEST_F(AssetCatalogTest, RetrieveAssetByIdAfterRegistration) {
        const AssetLocation location("retrieve@test/asset");
        auto asset = std::make_unique<Model>();
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));

        ASSERT_TRUE(ref.isValid());
        const auto id = ref.lock()->getID();

        // Retrieve using the ID
        const auto retrievedRef = assetCatalog.getAsset(id);

        EXPECT_TRUE(retrievedRef.isValid());
        EXPECT_EQ(retrievedRef.lock()->getID(), id);
        EXPECT_EQ(retrievedRef.lock().get(), ref.lock().get());
    }

    TEST_F(AssetCatalogTest, AssetRefIsValidCheck) {
        const AssetLocation location("valid@test/ref");
        auto asset = std::make_unique<Texture>();
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));

        EXPECT_TRUE(ref.isValid());
        EXPECT_TRUE(static_cast<bool>(ref));

        assetCatalog.deleteAsset(ref);

        EXPECT_FALSE(ref.isValid());
        EXPECT_FALSE(static_cast<bool>(ref));
    }

    // ------------------------------------------------------------------------
    // Asset Lookup Tests
    // ------------------------------------------------------------------------

    TEST_F(AssetCatalogTest, LookupByInvalidAssetID) {
        AssetID invalidId; // Default-constructed UUID (nil UUID)
        const auto ref = assetCatalog.getAsset(invalidId);

        EXPECT_FALSE(ref.isValid());
        EXPECT_FALSE(ref);
    }

    TEST_F(AssetCatalogTest, LookupByNonExistentLocation) {
        const AssetLocation nonExistent("missing@nowhere/found");
        const auto ref = assetCatalog.getAsset(nonExistent);

        EXPECT_FALSE(ref.isValid());
        EXPECT_FALSE(ref);
    }

    TEST_F(AssetCatalogTest, LookupByLocationWithSpecialCharacters) {
        const AssetLocation location("special-name_123@path/to/asset");
        auto asset = std::make_unique<Texture>();
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));

        ASSERT_TRUE(ref.isValid());

        const auto retrieved = assetCatalog.getAsset(location);
        EXPECT_TRUE(retrieved.isValid());
        EXPECT_EQ(retrieved.lock()->getID(), ref.lock()->getID());
    }

    // ------------------------------------------------------------------------
    // Asset Removal Tests
    // ------------------------------------------------------------------------

    TEST_F(AssetCatalogTest, DeleteAssetByIdReturnsTrueOnSuccess) {
        const AssetLocation location("delete@test/asset");
        auto asset = std::make_unique<Texture>();
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));
        const auto id = ref.lock()->getID();

        const bool result = assetCatalog.deleteAsset(id);

        EXPECT_TRUE(result);
        EXPECT_FALSE(ref.isValid());
    }

    TEST_F(AssetCatalogTest, DeleteAssetByIdReturnsFalseOnNonExistent) {
        AssetID nonExistentId; // Nil UUID
        const bool result = assetCatalog.deleteAsset(nonExistentId);

        EXPECT_FALSE(result);
    }

    TEST_F(AssetCatalogTest, DeleteAssetByReferenceReturnsTrueOnSuccess) {
        const AssetLocation location("delete@test/ref");
        auto asset = std::make_unique<Model>();
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));

        const bool result = assetCatalog.deleteAsset(ref);

        EXPECT_TRUE(result);
        EXPECT_FALSE(ref.isValid());
    }

    TEST_F(AssetCatalogTest, DeleteAssetByInvalidReferenceReturnsFalse) {
        GenericAssetRef invalidRef;
        const bool result = assetCatalog.deleteAsset(invalidRef);

        EXPECT_FALSE(result);
    }

    TEST_F(AssetCatalogTest, DeleteAlreadyDeletedAsset) {
        const AssetLocation location("double-delete@test/asset");
        auto asset = std::make_unique<Texture>();
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));
        const auto id = ref.lock()->getID();

        // First deletion
        const bool firstDelete = assetCatalog.deleteAsset(id);
        EXPECT_TRUE(firstDelete);

        // Second deletion should return false
        const bool secondDelete = assetCatalog.deleteAsset(id);
        EXPECT_FALSE(secondDelete);
    }

    TEST_F(AssetCatalogTest, DeleteOneAssetDoesNotAffectOthers) {
        auto asset1 = std::make_unique<Texture>();
        auto asset2 = std::make_unique<Texture>();
        auto asset3 = std::make_unique<Texture>();

        const auto ref1 = assetCatalog.registerAsset(AssetLocation("asset1@path"), std::move(asset1));
        const auto ref2 = assetCatalog.registerAsset(AssetLocation("asset2@path"), std::move(asset2));
        const auto ref3 = assetCatalog.registerAsset(AssetLocation("asset3@path"), std::move(asset3));

        const auto id2 = ref2.lock()->getID();

        // Delete only asset2
        assetCatalog.deleteAsset(id2);

        EXPECT_TRUE(ref1.isValid());
        EXPECT_FALSE(ref2.isValid());
        EXPECT_TRUE(ref3.isValid());

        const auto assets = assetCatalog.getAssets();
        EXPECT_EQ(assets.size(), 2);
    }

    // ------------------------------------------------------------------------
    // Iterator and Range Operations Tests
    // ------------------------------------------------------------------------

    TEST_F(AssetCatalogTest, GetAssetsReturnsEmptyVectorWhenNoAssets) {
        const auto assets = assetCatalog.getAssets();
        EXPECT_TRUE(assets.empty());
        EXPECT_EQ(assets.size(), 0);
    }

    TEST_F(AssetCatalogTest, GetAssetsViewReturnsEmptyViewWhenNoAssets) {
        const auto assetsView = assetCatalog.getAssetsView();
        EXPECT_EQ(assetsView.size(), 0);

        // Verify iterator behavior
        auto it = assetsView.begin();
        auto end = assetsView.end();
        EXPECT_EQ(it, end);
    }

    TEST_F(AssetCatalogTest, IterateOverAssetsWithRange) {
        auto texture1 = std::make_unique<Texture>();
        auto texture2 = std::make_unique<Texture>();
        auto model1 = std::make_unique<Model>();

        assetCatalog.registerAsset(AssetLocation("tex1@path"), std::move(texture1));
        assetCatalog.registerAsset(AssetLocation("tex2@path"), std::move(texture2));
        assetCatalog.registerAsset(AssetLocation("model@path"), std::move(model1));

        const auto assetsView = assetCatalog.getAssetsView();
        int count = 0;

        for (const GenericAssetRef& assetRef : assetsView) {
            EXPECT_TRUE(assetRef.isValid());
            EXPECT_TRUE(assetRef.lock());
            count++;
        }

        EXPECT_EQ(count, 3);
    }

    TEST_F(AssetCatalogTest, GetAssetsViewConsistentWithGetAssets) {
        auto asset1 = std::make_unique<Texture>();
        auto asset2 = std::make_unique<Model>();

        assetCatalog.registerAsset(AssetLocation("asset1@path"), std::move(asset1));
        assetCatalog.registerAsset(AssetLocation("asset2@path"), std::move(asset2));

        const auto assets = assetCatalog.getAssets();
        const auto assetsView = assetCatalog.getAssetsView();

        EXPECT_EQ(assets.size(), assetsView.size());
        EXPECT_EQ(assets.size(), 2);
    }

    TEST_F(AssetCatalogTest, AssetsViewUpdatesAfterDeletion) {
        auto texture1 = std::make_unique<Texture>();
        auto texture2 = std::make_unique<Texture>();

        const auto ref1 = assetCatalog.registerAsset(AssetLocation("tex1@path"), std::move(texture1));
        const auto ref2 = assetCatalog.registerAsset(AssetLocation("tex2@path"), std::move(texture2));

        auto assetsView = assetCatalog.getAssetsView();
        EXPECT_EQ(assetsView.size(), 2);

        assetCatalog.deleteAsset(ref1);

        assetsView = assetCatalog.getAssetsView();
        EXPECT_EQ(assetsView.size(), 1);
    }

    // ------------------------------------------------------------------------
    // Edge Cases Tests
    // ------------------------------------------------------------------------

    TEST_F(AssetCatalogTest, RegisterAndDeleteManyAssets) {
        constexpr int numAssets = 100;
        std::vector<GenericAssetRef> refs;
        refs.reserve(numAssets);

        // Register many assets
        for (int i = 0; i < numAssets; ++i) {
            auto asset = std::make_unique<Texture>();
            AssetLocation location("asset" + std::to_string(i) + "@path");
            refs.push_back(assetCatalog.registerAsset(location, std::move(asset)));
        }

        const auto assets = assetCatalog.getAssets();
        EXPECT_EQ(assets.size(), numAssets);

        // Delete half of them
        for (int i = 0; i < numAssets / 2; ++i) {
            assetCatalog.deleteAsset(refs[i]);
        }

        const auto remainingAssets = assetCatalog.getAssets();
        EXPECT_EQ(remainingAssets.size(), numAssets - numAssets / 2);
    }

    TEST_F(AssetCatalogTest, AssetIDUniquenessAcrossMultipleRegistrations) {
        constexpr int numAssets = 50;
        std::unordered_set<AssetID> ids;

        for (int i = 0; i < numAssets; ++i) {
            auto asset = std::make_unique<Texture>();
            AssetLocation location("asset" + std::to_string(i) + "@path");
            const auto ref = assetCatalog.registerAsset(location, std::move(asset));

            ASSERT_TRUE(ref.isValid());
            const auto id = ref.lock()->getID();

            // Ensure no duplicate IDs
            EXPECT_TRUE(ids.insert(id).second) << "Duplicate AssetID detected";
        }

        EXPECT_EQ(ids.size(), numAssets);
    }

    TEST_F(AssetCatalogTest, AssetRefComparison) {
        auto asset1 = std::make_unique<Texture>();
        auto asset2 = std::make_unique<Texture>();

        const auto ref1 = assetCatalog.registerAsset(AssetLocation("asset1@path"), std::move(asset1));
        const auto ref2 = assetCatalog.registerAsset(AssetLocation("asset2@path"), std::move(asset2));
        const auto ref1Copy = ref1;

        EXPECT_EQ(ref1, ref1Copy);
        EXPECT_NE(ref1, ref2);
        EXPECT_NE(ref1Copy, ref2);
    }

    TEST_F(AssetCatalogTest, AssetRefNullComparison) {
        GenericAssetRef nullRef;

        EXPECT_EQ(nullRef, nullptr);
        // Two default-constructed nullRefs are equal (both have expired weak_ptr)
        EXPECT_EQ(nullRef, GenericAssetRef());

        auto asset = std::make_unique<Texture>();
        const auto validRef = assetCatalog.registerAsset(AssetLocation("asset@path"), std::move(asset));

        EXPECT_NE(validRef, nullptr);

        assetCatalog.deleteAsset(validRef);
        EXPECT_EQ(validRef, nullptr);
    }

    // ------------------------------------------------------------------------
    // Asset Rename Tests
    // ------------------------------------------------------------------------

    TEST_F(AssetCatalogTest, RenameAssetByIdSuccess) {
        auto asset = std::make_unique<Texture>();
        const AssetLocation location("original@path/to/asset");
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));
        const auto id = ref.lock()->getID();

        const bool result = assetCatalog.renameAsset(id, "newName");

        EXPECT_TRUE(result);
        EXPECT_EQ(ref.lock()->getMetadata().location.getName().data(), "newName");
    }

    TEST_F(AssetCatalogTest, RenameAssetByReferenceSuccess) {
        auto asset = std::make_unique<Model>();
        const AssetLocation location("original@path/to/asset");
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));

        const bool result = assetCatalog.renameAsset(ref, "renamedAsset");

        EXPECT_TRUE(result);
        EXPECT_EQ(ref.lock()->getMetadata().location.getName().data(), "renamedAsset");
    }

    TEST_F(AssetCatalogTest, RenameAssetWithEmptyNameFails) {
        auto asset = std::make_unique<Texture>();
        const AssetLocation location("original@path/to/asset");
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));
        const auto id = ref.lock()->getID();

        const bool result = assetCatalog.renameAsset(id, "");

        EXPECT_FALSE(result);
        EXPECT_EQ(ref.lock()->getMetadata().location.getName().data(), "original");
    }

    TEST_F(AssetCatalogTest, RenameNonExistentAssetFails) {
        AssetID nonExistentId;
        const bool result = assetCatalog.renameAsset(nonExistentId, "newName");

        EXPECT_FALSE(result);
    }

    TEST_F(AssetCatalogTest, RenameInvalidReferenceFails) {
        GenericAssetRef invalidRef;
        const bool result = assetCatalog.renameAsset(invalidRef, "newName");

        EXPECT_FALSE(result);
    }

    // ------------------------------------------------------------------------
    // Asset Move Tests
    // ------------------------------------------------------------------------

    TEST_F(AssetCatalogTest, MoveAssetByReferenceSuccess) {
        auto asset = std::make_unique<Texture>();
        const AssetLocation location("asset@old/path");
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));

        assetCatalog.moveAsset(ref, "new/path/location");

        EXPECT_EQ(ref.lock()->getMetadata().location.getPath(), "new/path/location");
    }

    TEST_F(AssetCatalogTest, MoveAssetByIdSuccess) {
        auto asset = std::make_unique<Model>();
        const AssetLocation location("asset@old/path");
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));
        const auto id = ref.lock()->getID();

        assetCatalog.moveAsset(id, "another/new/path");

        EXPECT_EQ(ref.lock()->getMetadata().location.getPath(), "another/new/path");
    }

    TEST_F(AssetCatalogTest, MoveAssetToEmptyPath) {
        auto asset = std::make_unique<Texture>();
        const AssetLocation location("asset@original/path");
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));

        assetCatalog.moveAsset(ref, "");

        EXPECT_EQ(ref.lock()->getMetadata().location.getPath(), "");
    }

    TEST_F(AssetCatalogTest, MoveNonExistentAssetDoesNothing) {
        AssetID nonExistentId;

        // Should not crash
        assetCatalog.moveAsset(nonExistentId, "some/path");
        SUCCEED();
    }

    TEST_F(AssetCatalogTest, MoveInvalidReferenceDoesNothing) {
        GenericAssetRef invalidRef;

        // Should not crash
        assetCatalog.moveAsset(invalidRef, "some/path");
        SUCCEED();
    }

    // ------------------------------------------------------------------------
    // Asset Metadata Tests
    // ------------------------------------------------------------------------

    TEST_F(AssetCatalogTest, RegisteredAssetHasCorrectMetadata) {
        const AssetLocation location("testAsset@test/path/location");
        auto asset = std::make_unique<Texture>();
        const auto ref = assetCatalog.registerAsset(location, std::move(asset));

        ASSERT_TRUE(ref.isValid());
        const auto metadata = ref.lock()->getMetadata();

        EXPECT_EQ(metadata.location.getName().data(), "testAsset");
        EXPECT_EQ(metadata.location.getPath(), "test/path/location");
        EXPECT_EQ(metadata.type, AssetType::TEXTURE);
        EXPECT_FALSE(metadata.id.is_nil());
    }

    TEST_F(AssetCatalogTest, AssetLocationEquality) {
        const AssetLocation loc1("asset@path/to/file");
        const AssetLocation loc2("asset@path/to/file");
        const AssetLocation loc3("different@path/to/file");

        EXPECT_TRUE(loc1 == loc2);
        EXPECT_FALSE(loc1 == loc3);
    }

    // ------------------------------------------------------------------------
    // Stress and Boundary Tests
    // ------------------------------------------------------------------------

    TEST_F(AssetCatalogTest, RegisterDeleteRegisterSameLocation) {
        const AssetLocation location("reusable@path/to/asset");

        // First registration
        auto asset1 = std::make_unique<Texture>();
        const auto ref1 = assetCatalog.registerAsset(location, std::move(asset1));
        const auto id1 = ref1.lock()->getID();

        // Delete
        assetCatalog.deleteAsset(ref1);
        EXPECT_FALSE(ref1.isValid());

        // Re-register with same location
        auto asset2 = std::make_unique<Texture>();
        const auto ref2 = assetCatalog.registerAsset(location, std::move(asset2));
        const auto id2 = ref2.lock()->getID();

        EXPECT_TRUE(ref2.isValid());
        EXPECT_NE(id1, id2); // Different IDs
    }

    TEST_F(AssetCatalogTest, MultipleReferencesToSameAsset) {
        auto asset = std::make_unique<Texture>();
        const AssetLocation location("shared@path");
        const auto ref1 = assetCatalog.registerAsset(location, std::move(asset));

        // Get another reference to the same asset
        const auto ref2 = assetCatalog.getAsset(ref1.lock()->getID());

        EXPECT_EQ(ref1.lock().get(), ref2.lock().get());
        EXPECT_EQ(ref1.lock()->getID(), ref2.lock()->getID());

        // Delete using one reference
        assetCatalog.deleteAsset(ref1);

        // Both references should be invalid
        EXPECT_FALSE(ref1.isValid());
        EXPECT_FALSE(ref2.isValid());
    }

    TEST_F(AssetCatalogTest, AssetTypeVerification) {
        auto texture = std::make_unique<Texture>();
        auto model = std::make_unique<Model>();

        const auto texRef = assetCatalog.registerAsset(AssetLocation("tex@path"), std::move(texture));
        const auto modelRef = assetCatalog.registerAsset(AssetLocation("model@path"), std::move(model));

        EXPECT_EQ(texRef.lock()->getType(), AssetType::TEXTURE);
        EXPECT_EQ(modelRef.lock()->getType(), AssetType::MODEL);
    }

    // ------------------------------------------------------------------------
    // Singleton Specific Additional Tests
    // ------------------------------------------------------------------------

    TEST_F(AssetCatalogSingletonTest, SingletonPersistsAcrossMultipleAccesses) {
        auto& instance1 = AssetCatalog::getInstance();

        const AssetLocation location("persist@test/asset");
        auto asset = std::make_unique<Texture>();
        const auto ref = instance1.registerAsset(location, std::move(asset));

        auto& instance2 = AssetCatalog::getInstance();
        const auto retrievedRef = instance2.getAsset(location);

        EXPECT_TRUE(retrievedRef.isValid());
        EXPECT_EQ(ref.lock()->getID(), retrievedRef.lock()->getID());
    }

} // namespace nexo::assets
