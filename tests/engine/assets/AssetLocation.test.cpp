//// AssetLocation.test.cpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        07/12/2024
//  Description: Unit tests for the AssetLocation class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/AssetLocation.hpp"
#include "assets/AssetName.hpp"
#include "assets/AssetPackName.hpp"

namespace nexo::assets {

    TEST(AssetLocationTest, ValidFullLocationWithPackAndPath)
    {
        std::string fullLocation = "myPack::myAsset@path/to/asset";
        AssetLocation location(fullLocation);

        ASSERT_TRUE(location.getPackName().has_value());
        EXPECT_EQ(location.getPackName()->get(), "myPack");
        EXPECT_EQ(location.getName(), "myAsset");
        EXPECT_EQ(location.getPath(), "path/to/asset");
        EXPECT_EQ(location.getFullLocation(), fullLocation);
    }

    TEST(AssetLocationTest, ValidFullLocationWithoutPack)
    {
        std::string fullLocation = "myAsset@path/to/asset";
        AssetLocation location(fullLocation);

        EXPECT_FALSE(location.getPackName().has_value());
        EXPECT_EQ(location.getName().data(), "myAsset");
        EXPECT_EQ(location.getPath(), "path/to/asset");
        EXPECT_EQ(location.getFullLocation(), fullLocation);
    }

    TEST(AssetLocationTest, ValidFullLocationWithoutPath)
    {
        std::string fullLocation = "myPack::myAsset";
        AssetLocation location(fullLocation);

        ASSERT_TRUE(location.getPackName().has_value());
        EXPECT_EQ(location.getPackName()->get(), "myPack");
        EXPECT_EQ(location.getName(), "myAsset");
        EXPECT_EQ(location.getPath(), "");
        EXPECT_EQ(location.getFullLocation(), fullLocation);
    }

    TEST(AssetLocationTest, InvalidLocationEmpty)
    {
        EXPECT_THROW(AssetLocation(""), InvalidAssetLocation);
    }

    TEST(AssetLocationTest, InvalidLocationWithInvalidName)
    {
        GTEST_SKIP() << "TODO: Implement validation for path, this test should pass later";
        const std::string invalidLocation = "myPack::my@Asset@path/to/asset";
        EXPECT_THROW({
            AssetLocation cannotCreate(invalidLocation);
        }, InvalidAssetLocation);
    }

    TEST(AssetLocationTest, ParseFullLocationStaticFunctionValid)
    {
        const std::string fullLocation = "myPack::myAsset@path/to/asset";
        std::string extractedAssetName, extractedPath, extractedPackName;

        AssetLocation::parseFullLocation(fullLocation, extractedAssetName, extractedPath, extractedPackName);

        EXPECT_EQ(extractedPackName, "myPack");
        EXPECT_EQ(extractedAssetName, "myAsset");
        EXPECT_EQ(extractedPath, "path/to/asset");
    }

    TEST(AssetLocationTest, ParseFullLocationStaticFunctionWithoutPack)
    {
        const std::string fullLocation = "myAsset@path/to/asset";
        std::string extractedAssetName, extractedPath, extractedPackName;

        AssetLocation::parseFullLocation(fullLocation, extractedAssetName, extractedPath, extractedPackName);

        EXPECT_EQ(extractedPackName, "");
        EXPECT_EQ(extractedAssetName, "myAsset");
        EXPECT_EQ(extractedPath, "path/to/asset");
    }

    TEST(AssetLocationTest, ParseFullLocationStaticFunctionWithoutPath)
    {
        const std::string fullLocation = "myPack::myAsset";
        std::string extractedAssetName, extractedPath, extractedPackName;

        AssetLocation::parseFullLocation(fullLocation, extractedAssetName, extractedPath, extractedPackName);

        EXPECT_EQ(extractedPackName, "myPack");
        EXPECT_EQ(extractedAssetName, "myAsset");
        EXPECT_EQ(extractedPath, "");
    }

    TEST(AssetLocationTest, SetLocationExplicitly)
    {
        const AssetName name("myAsset");
        const std::string path = "path/to/asset";
        AssetPackName packName("myPack");

        AssetLocation location("test");
        EXPECT_EQ(location.getFullLocation(), "test");
        location.setLocation(name, path, packName);

        ASSERT_TRUE(location.getPackName().has_value());
        EXPECT_EQ(location.getPackName()->get(), "myPack");
        EXPECT_EQ(location.getName(), "myAsset");
        EXPECT_EQ(location.getPath(), "path/to/asset");
        EXPECT_EQ(location.getFullLocation(), "myPack::myAsset@path/to/asset");
    }

    TEST(AssetLocationTest, SetLocationExplicitlyWithoutPack)
    {
        const AssetName name("myAsset");
        const std::string path = "path/to/asset";

        AssetLocation location("test");
        EXPECT_EQ(location.getFullLocation(), "test");
        location.setLocation(name, path);

        EXPECT_FALSE(location.getPackName().has_value());
        EXPECT_EQ(location.getName(), "myAsset");
        EXPECT_EQ(location.getPath(), "path/to/asset");
        EXPECT_EQ(location.getFullLocation(), "myAsset@path/to/asset");
    }

    TEST(AssetLocationTest, SetLocationExplicitlyWithoutPath)
    {
        const AssetName name("myAsset");
        AssetPackName packName("myPack");

        AssetLocation location("test");
        EXPECT_EQ(location.getFullLocation(), "test");
        location.setLocation(name, "", packName);

        ASSERT_TRUE(location.getPackName().has_value());
        EXPECT_EQ(location.getPackName()->get(), "myPack");
        EXPECT_EQ(location.getName(), "myAsset");
        EXPECT_EQ(location.getPath(), "");
        EXPECT_EQ(location.getFullLocation(), "myPack::myAsset");
    }

    TEST(AssetLocationTest, SetName)
    {
        AssetLocation location("myPack::myAsset@path/to/asset");
        EXPECT_EQ(location.getFullLocation(), "myPack::myAsset@path/to/asset");


        const std::string newName = "newAssetName";
        location.setName(newName);
        EXPECT_EQ(location.getFullLocation(), std::format("myPack::{}@path/to/asset", newName));
    }

    TEST(AssetLocationTest, InvalidSetName)
    {
        AssetLocation location("myPack::myAsset@path/to/asset");
        EXPECT_EQ(location.getFullLocation(), "myPack::myAsset@path/to/asset");

        EXPECT_THROW({
            location.setName("");
        }, InvalidName);

        EXPECT_THROW({
            location.setName("newAssetName@");
        }, InvalidName);
    }

    TEST(AssetLocationTest, SetPath)
    {
        AssetLocation location("myPack::myAsset@path/to/asset");
        EXPECT_EQ(location.getFullLocation(), "myPack::myAsset@path/to/asset");

        const std::string newPath = "new/path/to/asset";
        location.setPath(newPath);
        EXPECT_EQ(location.getFullLocation(), std::format("myPack::myAsset@{}", newPath));
    }

    TEST(AssetLocationTest, SetPackName)
    {
        AssetLocation location("myPack::myAsset@path/to/asset");
        EXPECT_EQ(location.getFullLocation(), "myPack::myAsset@path/to/asset");

        const std::string newPackName = "newPackName";
        location.setPackName(newPackName);
        EXPECT_EQ(location.getFullLocation(), std::format("{}::myAsset@path/to/asset", newPackName));
    }

    TEST(AssetLocationTest, ClearPackName)
    {
        AssetLocation location("myPack::myAsset@path/to/asset");
        EXPECT_EQ(location.getFullLocation(), "myPack::myAsset@path/to/asset");

        location.clearPackName();
        EXPECT_EQ(location.getFullLocation(), "myAsset@path/to/asset");
    }

    TEST(AssetLocationTest, InvalidSetPackNameEmpty)
    {
        AssetLocation location("myPack::myAsset@path/to/asset");
        EXPECT_EQ(location.getFullLocation(), "myPack::myAsset@path/to/asset");

        EXPECT_THROW({
            location.setPackName("");
        }, InvalidName);

        EXPECT_THROW({
            location.setPackName("myPack::");
        }, InvalidName);
    }

    TEST(AssetLocationTest, EqualityOperators)
    {
        const std::string fullLocationEq1 = "myPack::myAsset@path/to/asset";
        const std::string fullLocationEq2 = "myPack::myAsset@path/to/asset";
        const std::string fullLocationNeq = "myPack::myAsset@path/to/otherAsset";

        const AssetLocation locationEq1(fullLocationEq1);
        const AssetLocation locationEq2(fullLocationEq2);
        const AssetLocation locationNeq(fullLocationNeq);

        EXPECT_EQ(locationEq1, locationEq2);
        EXPECT_EQ(locationEq1, fullLocationEq2);

        EXPECT_EQ(locationEq2, locationEq1);
        EXPECT_EQ(locationEq2, fullLocationEq1);

        EXPECT_NE(locationEq1, locationNeq);
        EXPECT_NE(locationEq2, locationNeq);
        EXPECT_NE(locationNeq, locationEq1);
        EXPECT_NE(locationNeq, locationEq2);
        EXPECT_NE(locationNeq, fullLocationEq1);
        EXPECT_NE(fullLocationNeq, locationEq1);
        EXPECT_NE(fullLocationNeq, locationEq2);

        EXPECT_EQ(locationNeq, fullLocationNeq);
    }

} // namespace nexo::assets
