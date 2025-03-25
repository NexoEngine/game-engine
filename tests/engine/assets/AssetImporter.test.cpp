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
#include "assets/Assets/Texture/Texture.hpp"
#include "assets/Assets/Model/Model.hpp"

namespace nexo::assets {

    using testing::Expectation;
    using testing::Invoke;
    using testing::Return;

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
        FRIEND_TEST(AssetImporterTest, MultipleImportersType);
        FRIEND_TEST(AssetImporterTest, TryIncompatibleImporters);
        FRIEND_TEST(AssetImporterTest, ImportersTriedNoValidFound);
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
            .WillOnce(Invoke([&](AssetImporterContext& ctx) {
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

        // Setup expectations with ordering
        Expectation canReadCall = EXPECT_CALL(*mockImporter, canRead(testing::_))
            .WillOnce(Return(true));

        EXPECT_CALL(*mockImporter, importImpl(testing::_))
            .After(canReadCall)
            .WillOnce(Invoke([&](AssetImporterContext& ctx) {
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

        // Setup expectations with ordering
        Expectation canReadCall = EXPECT_CALL(*bestImporter, canRead(testing::_))
            .WillOnce(Return(true));

        EXPECT_CALL(*bestImporter, importImpl(testing::_))
            .After(canReadCall)
            .WillOnce(Invoke([&](AssetImporterContext& ctx) {
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

    class MockShaderAsset final : public Asset<void *, AssetType::SHADER> {

    };

    TEST_F(AssetImporterTest, MultipleImportersType)
    {
        MockAssetImporter importer;
        const auto textureImporter = new MockImporter();
        const auto textureImporter2 = new MockImporter();
        const auto validModelImporter = new MockImporter();
        const auto cannotReadModelImporter = new MockImporter();

        importer.registerImporter<Texture>(textureImporter, 100);
        importer.registerImporter<Model>(validModelImporter, 90);
        importer.registerImporter<Texture>(textureImporter2, 50);
        importer.registerImporter<Model>(cannotReadModelImporter, 110);
        importer.registerImporter<Model, MockImporter>(120);

        const auto textureImporters = importer.getImportersForType<Texture>();
        const auto modelImporters = importer.getImportersForType<Model>();

        // Verify that importers are registered correctly
        EXPECT_EQ(textureImporters.size(), 2);
        EXPECT_EQ(modelImporters.size(), 3);

        EXPECT_TRUE(importer.hasImportersForType<Texture>());
        EXPECT_TRUE(importer.hasImportersForType<Model>());

        EXPECT_EQ(textureImporters[0], textureImporter);
        EXPECT_EQ(textureImporters[1], textureImporter2);

        const auto cannotReadModelImporter2 = dynamic_cast<MockImporter*>(modelImporters[0]);
        EXPECT_NE(cannotReadModelImporter2, nullptr);
        EXPECT_EQ(modelImporters[1], cannotReadModelImporter);
        EXPECT_EQ(modelImporters[2], validModelImporter);

        // Setup expectations for each importer
        // Texture importers should NEVER be called
        EXPECT_CALL(*textureImporter, canRead(testing::_)).Times(0);
        EXPECT_CALL(*textureImporter, importImpl(testing::_)).Times(0);
        EXPECT_CALL(*textureImporter2, canRead(testing::_)).Times(0);
        EXPECT_CALL(*textureImporter2, importImpl(testing::_)).Times(0);

        // Setup call expectations with proper ordering
        // First the templated MockImporter will be checked
        Expectation canReadCall2 = EXPECT_CALL(*cannotReadModelImporter2, canRead(testing::_))
            .WillOnce(Return(false));

        // Then the cannotReadModelImporter will be checked
        Expectation canReadCall = EXPECT_CALL(*cannotReadModelImporter, canRead(testing::_))
            .After(canReadCall2)
            .WillOnce(Return(false));

        // Finally the validModelImporter will be checked and used
        Expectation validCanReadCall = EXPECT_CALL(*validModelImporter, canRead(testing::_))
            .After(canReadCall)
            .WillOnce(Return(true));

        EXPECT_CALL(*validModelImporter, importImpl(testing::_))
            .After(validCanReadCall)
            .WillOnce(Invoke([](AssetImporterContext& ctx) {
                ctx.setMainAsset(new Model());
            }));

        EXPECT_CALL(*cannotReadModelImporter, importImpl(testing::_)).Times(0);
        EXPECT_CALL(*cannotReadModelImporter2, importImpl(testing::_)).Times(0);

        const AssetLocation location("test::myAsset@path");
        const ImporterFileInput input;
        // Should import the model using the validModelImporter,
        // even though cannotReadModelImporter is registered as higher priority
        const auto assetRef = importer.importAsset<Model>(location, input);
        EXPECT_TRUE(assetRef);

        const AssetLocation location2("test::myAsset@path2");
        const auto invalidShaderAssetRef = importer.importAsset<MockShaderAsset>(location2, input);
        EXPECT_FALSE(invalidShaderAssetRef);
    }

    /**
     * @brief Test the import and the feature that tries incompatible importers.
     * In the code, as a last resort, importers that previously return canRead -> false
     * are called again to try to import the asset.
     */
    TEST_F(AssetImporterTest, TryIncompatibleImporters) {
        MockAssetImporter importer;
        const auto bestImporter = new MockImporter();
        const auto wrongImporter = new MockImporter();
        const AssetLocation location("test::myAsset@path");
        ImporterFileInput input;
        const auto expectedAsset = new Texture();

        // Setup expectations with ordering
        // First the bestImporter canRead is checked
        Expectation bestCanReadCall = EXPECT_CALL(*bestImporter, canRead(testing::_))
            .WillOnce(Return(false));

        // Then wrongImporter canRead is checked
        Expectation wrongCanReadCall = EXPECT_CALL(*wrongImporter, canRead(testing::_))
            .After(bestCanReadCall)
            .WillOnce(Return(false));

        // Finally bestImporter importImpl is called as last resort
        EXPECT_CALL(*bestImporter, importImpl(testing::_))
            .After(wrongCanReadCall)
            .WillOnce(Invoke([&](AssetImporterContext& ctx) {
                ctx.setMainAsset(expectedAsset);
            }));

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

    TEST_F(AssetImporterTest, ImportersTriedNoValidFound) {
        MockAssetImporter importer;
        const auto wrongImporter1 = new MockImporter();
        const auto wrongImporter2 = new MockImporter();
        const AssetLocation location("test::myAsset@path");
        ImporterFileInput input;

        // Setup expectations with ordering
        // First wrongImporter1 canRead is checked
        Expectation wrong1CanReadCall = EXPECT_CALL(*wrongImporter1, canRead(testing::_))
            .WillOnce(Return(false));

        // Then wrongImporter2 canRead is checked
        Expectation wrong2CanReadCall = EXPECT_CALL(*wrongImporter2, canRead(testing::_))
            .After(wrong1CanReadCall)
            .WillOnce(Return(false));

        // Then wrongImporter1 importImpl is attempted (highest priority first)
        Expectation wrong1ImportCall = EXPECT_CALL(*wrongImporter1, importImpl(testing::_))
            .After(wrong2CanReadCall)
            .WillOnce(Return());

        // Finally wrongImporter2 importImpl is attempted
        EXPECT_CALL(*wrongImporter2, importImpl(testing::_))
            .After(wrong1ImportCall)
            .WillOnce(Return());

        // Register the mock importers with different priorities
        importer.registerImporter<Texture>(wrongImporter2, 50);
        importer.registerImporter<Texture>(wrongImporter1, 100);

        // Call the method
        const auto assetRef = importer.importAssetAuto(location, input);

        // Assertions
        ASSERT_FALSE(assetRef.isValid());
    }

} // namespace nexo::assets
