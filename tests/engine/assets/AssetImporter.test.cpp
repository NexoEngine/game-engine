//// AssetImporter.test.cpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        21/03/2025
//  Description: Unit tests for the AssetImporter class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "assets/AssetImporter.hpp"
#include "assets/AssetImporterBase.hpp"
#include "assets/AssetImporterContext.hpp"
#include "assets/Asset.hpp"
#include "assets/AssetCatalog.hpp"
#include "assets/Assets/Texture/Texture.hpp"
#include "assets/Assets/Model/Model.hpp"

namespace nexo::assets {

    // Mock AssetImporterBase class
    class MockImporter final : public AssetImporterBase {
    public:
        MOCK_METHOD(bool, canRead, (const ImporterInputVariant& inputVariant), (override));
        MOCK_METHOD(void, importImpl, (AssetImporterContext& ctx), (override));
    };



    class AssetImporterTest : public ::testing::Test {
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
    };

    class MockAssetImporter : public AssetImporter {
        FRIEND_TEST(AssetImporterTest, ImporterAssetUsingImporterSucess);
        FRIEND_TEST(AssetImporterTest, ImporterAssetAutoSucess);
        FRIEND_TEST(AssetImporterTest, ImporterPriorityOrder);
        public:
        MockAssetImporter() : AssetImporter(nullptr)
        {
        }

        ~MockAssetImporter() = default;
    };


    /**
     * @brief Test the import of an asset using a specific importer
     *
     */
    TEST_F(AssetImporterTest, ImporterAssetUsingImporterSucess)
    {
        MockAssetImporter importer;
        const auto mockImporter = new MockImporter();
        const AssetLocation location("test::myAsset@path");
        ImporterFileInput input;
        const auto expectedAsset = new Texture();

        // Simulate successful import
        EXPECT_CALL(*mockImporter, canRead(testing::_)).Times(0); // Never called
        EXPECT_CALL(*mockImporter, importImpl(testing::_))
            .WillOnce(testing::Invoke([&](AssetImporterContext& ctx) {
                ctx.setMainAsset(expectedAsset);
            }));

        importer.registerImporter<Texture>(mockImporter, 100);

        const auto assetRef = importer.importAssetUsingImporter(location, input, mockImporter);

        ASSERT_TRUE(assetRef.isValid());
        ASSERT_EQ(assetRef.lock().get(), expectedAsset);
        ASSERT_EQ(assetRef.lock()->getMetadata().location.getFullLocation(), location.getFullLocation());
    }

    TEST_F(AssetImporterTest, ImporterAssetAutoSucess)
    {
        MockAssetImporter importer;
        const auto mockImporter = new MockImporter();
        const AssetLocation location("test::myAsset@path");
        ImporterFileInput input;
        const auto expectedAsset = new Texture();

        // Setup expectations
        EXPECT_CALL(*mockImporter, canRead(testing::_)).WillOnce(testing::Return(true));
        EXPECT_CALL(*mockImporter, importImpl(testing::_))
            .WillOnce(testing::Invoke([&](AssetImporterContext& ctx) {
                ctx.setMainAsset(expectedAsset);
            }));

        // Register the mock importer
        importer.registerImporter<Texture>(mockImporter, 100);

        // Call the method
        const auto assetRef = importer.importAssetAuto(location, input);

        // Assertions
        ASSERT_TRUE(assetRef.isValid());
        ASSERT_EQ(assetRef.lock().get(), expectedAsset);
        ASSERT_EQ(assetRef.lock()->getMetadata().location.getFullLocation(), location.getFullLocation());

        // Clean up
        importer.unregisterAllImportersForType<Texture>();
        // Delete the mock importer
        delete mockImporter;
    }

    TEST_F(AssetImporterTest, ImportAssetAutoFailureNoImporters)
    {
        MockAssetImporter importer;
        const AssetLocation location("test::myAsset@path");
        ImporterFileInput input;

        // Call the method
        const auto assetRef = importer.importAssetAuto(location, input);

        // Assertions
        ASSERT_FALSE(assetRef.isValid());
    }

    TEST_F(AssetImporterTest, ImporterPriorityOrder) {
        MockAssetImporter importer;
        const auto bestImporter = new MockImporter();
        const auto wrongImporter = new MockImporter();
        const AssetLocation location("test::myAsset@path");
        ImporterFileInput input;
        const auto expectedAsset = new Texture();

        // Setup expectations
        EXPECT_CALL(*bestImporter, canRead(testing::_)).WillOnce(testing::Return(true));
        EXPECT_CALL(*bestImporter, importImpl(testing::_))
            .WillOnce(testing::Invoke([&](AssetImporterContext& ctx) {
                ctx.setMainAsset(expectedAsset);
            }));

        EXPECT_CALL(*wrongImporter, canRead(testing::_)).Times(0);
        EXPECT_CALL(*wrongImporter, importImpl(testing::_)).Times(0);

        // Register the mock importers with different priorities
        importer.registerImporter<Texture>(wrongImporter, 50);
        importer.registerImporter<Texture>(bestImporter, 100);

        // Call the method
        const auto assetRef = importer.importAssetAuto(location, input);

        // Assertions
        ASSERT_TRUE(assetRef.isValid());
        ASSERT_EQ(assetRef.lock().get(), expectedAsset);
    }

    TEST_F(AssetImporterTest, SetAndGetCustomContext) {
        MockAssetImporter importer;
        AssetImporterContext customContext;

        // Set custom context
        importer.setCustomContext(&customContext);
        ASSERT_EQ(importer.getCustomContext(), &customContext);

        // Clear custom context
        importer.clearCustomContext();
        ASSERT_EQ(importer.getCustomContext(), nullptr);
    }

} // namespace nexo::assets
