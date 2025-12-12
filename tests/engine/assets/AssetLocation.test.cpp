//// AssetLocation.test.cpp ///////////////////////////////////////////////////
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
        #if defined(GTEST_SKIP)
            GTEST_SKIP() << "TODO: Implement validation for path, this test should pass later";
        #else
            const std::string invalidLocation = "myPack::my@Asset@path/to/asset";
            EXPECT_THROW({
                AssetLocation cannotCreate(invalidLocation);
            }, InvalidAssetLocation);
        #endif
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

    // ============================================================================
    // Path Normalization Edge Cases
    // ============================================================================

    TEST(AssetLocationTest, PathNormalizationLeadingSlash)
    {
        AssetLocation location("myAsset@/path/to/asset");
        EXPECT_EQ(location.getPath(), "path/to/asset");
        EXPECT_EQ(location.getFullLocation(), "myAsset@path/to/asset");
    }

    TEST(AssetLocationTest, PathNormalizationMultipleLeadingSlashes)
    {
        AssetLocation location("myAsset@///path/to/asset");
        EXPECT_EQ(location.getPath(), "path/to/asset");
        EXPECT_EQ(location.getFullLocation(), "myAsset@path/to/asset");
    }

    TEST(AssetLocationTest, PathNormalizationBackslashes)
    {
        AssetLocation location("myAsset@path\\to\\asset");
        // On Windows, backslashes are normalized to forward slashes
        // On Linux, backslashes are treated as literal characters in filenames
        #ifdef _WIN32
            EXPECT_EQ(location.getPath(), "path/to/asset");
            EXPECT_EQ(location.getFullLocation(), "myAsset@path/to/asset");
        #else
            // On Linux, backslashes are valid filename characters
            EXPECT_EQ(location.getPath(), "path\\to\\asset");
            EXPECT_EQ(location.getFullLocation(), "myAsset@path\\to\\asset");
        #endif
    }

    TEST(AssetLocationTest, PathNormalizationMixedSlashes)
    {
        AssetLocation location("myAsset@path/to\\asset");
        // On Windows, backslashes are normalized to forward slashes
        // On Linux, backslashes are treated as literal characters
        #ifdef _WIN32
            EXPECT_EQ(location.getPath(), "path/to/asset");
            EXPECT_EQ(location.getFullLocation(), "myAsset@path/to/asset");
        #else
            // On Linux, backslashes are valid filename characters
            EXPECT_EQ(location.getPath(), "path/to\\asset");
            EXPECT_EQ(location.getFullLocation(), "myAsset@path/to\\asset");
        #endif
    }

    TEST(AssetLocationTest, SetPathWithLeadingSlash)
    {
        AssetLocation location("myAsset");
        location.setPath("/new/path");
        EXPECT_EQ(location.getPath(), "new/path");
        EXPECT_EQ(location.getFullLocation(), "myAsset@new/path");
    }

    // ============================================================================
    // Setter Method Chaining
    // ============================================================================

    TEST(AssetLocationTest, SetterMethodChainingAll)
    {
        AssetLocation location("initial");
        AssetLocation& result = location.setName("chainedAsset")
                                       .setPath("chained/path")
                                       .setPackName("chainedPack");

        EXPECT_EQ(&result, &location);
        EXPECT_EQ(location.getName(), "chainedAsset");
        EXPECT_EQ(location.getPath(), "chained/path");
        ASSERT_TRUE(location.getPackName().has_value());
        EXPECT_EQ(location.getPackName()->get(), "chainedPack");
        EXPECT_EQ(location.getFullLocation(), "chainedPack::chainedAsset@chained/path");
    }

    TEST(AssetLocationTest, SetterMethodChainingNameAndPath)
    {
        AssetLocation location("initial");
        location.setName("asset1").setPath("path1");

        EXPECT_EQ(location.getName(), "asset1");
        EXPECT_EQ(location.getPath(), "path1");
        EXPECT_EQ(location.getFullLocation(), "asset1@path1");
    }

    TEST(AssetLocationTest, SetterMethodChainingWithClear)
    {
        AssetLocation location("pack::asset@path");
        location.clearPackName().setPath("newpath");

        EXPECT_FALSE(location.getPackName().has_value());
        EXPECT_EQ(location.getPath(), "newpath");
        EXPECT_EQ(location.getFullLocation(), "asset@newpath");
    }

    // ============================================================================
    // Copy and Move Semantics
    // ============================================================================

    TEST(AssetLocationTest, CopyConstructorPreservesAllFields)
    {
        AssetLocation original("myPack::myAsset@path/to/asset");
        AssetLocation copy(original);

        EXPECT_EQ(copy.getName(), original.getName());
        EXPECT_EQ(copy.getPath(), original.getPath());
        ASSERT_TRUE(copy.getPackName().has_value());
        ASSERT_TRUE(original.getPackName().has_value());
        EXPECT_EQ(copy.getPackName()->get(), original.getPackName()->get());
        EXPECT_EQ(copy.getFullLocation(), original.getFullLocation());
    }

    TEST(AssetLocationTest, CopyConstructorWithoutPack)
    {
        AssetLocation original("myAsset@path/to/asset");
        AssetLocation copy(original);

        EXPECT_EQ(copy.getName(), original.getName());
        EXPECT_EQ(copy.getPath(), original.getPath());
        EXPECT_FALSE(copy.getPackName().has_value());
        EXPECT_EQ(copy.getFullLocation(), original.getFullLocation());
    }

    TEST(AssetLocationTest, MoveConstructorWorks)
    {
        AssetLocation original("myPack::myAsset@path/to/asset");
        std::string originalFullLocation = original.getFullLocation();

        AssetLocation moved(std::move(original));

        EXPECT_EQ(moved.getFullLocation(), originalFullLocation);
        EXPECT_EQ(moved.getName(), "myAsset");
        EXPECT_EQ(moved.getPath(), "path/to/asset");
        ASSERT_TRUE(moved.getPackName().has_value());
        EXPECT_EQ(moved.getPackName()->get(), "myPack");
    }

    TEST(AssetLocationTest, CopyAssignmentWorks)
    {
        AssetLocation original("myPack::myAsset@path/to/asset");
        AssetLocation copy("different");

        copy = original;

        EXPECT_EQ(copy.getName(), original.getName());
        EXPECT_EQ(copy.getPath(), original.getPath());
        EXPECT_EQ(copy.getFullLocation(), original.getFullLocation());
    }

    TEST(AssetLocationTest, MoveAssignmentWorks)
    {
        AssetLocation original("myPack::myAsset@path/to/asset");
        std::string originalFullLocation = original.getFullLocation();
        AssetLocation moved("different");

        moved = std::move(original);

        EXPECT_EQ(moved.getFullLocation(), originalFullLocation);
        EXPECT_EQ(moved.getName(), "myAsset");
    }

    // ============================================================================
    // InvalidAssetLocation Exception Details
    // ============================================================================

    TEST(AssetLocationTest, InvalidAssetLocationContainsOriginalString)
    {
        const std::string invalidLocation = "pack::@invalid";
        try {
            AssetLocation location(invalidLocation);
            FAIL() << "Expected InvalidAssetLocation exception";
        } catch (const InvalidAssetLocation& e) {
            std::string message = e.getMessage();
            EXPECT_NE(message.find(invalidLocation), std::string::npos)
                << "Exception message should contain original location string";
        }
    }

    TEST(AssetLocationTest, InvalidAssetLocationContainsErrorDescription)
    {
        const std::string invalidLocation = "pack::@invalid";
        try {
            AssetLocation location(invalidLocation);
            FAIL() << "Expected InvalidAssetLocation exception";
        } catch (const InvalidAssetLocation& e) {
            std::string message = e.getMessage();
            EXPECT_FALSE(message.empty());
            EXPECT_NE(message.find("Invalid"), std::string::npos)
                << "Exception message should describe the error";
        }
    }

    // ============================================================================
    // getFullLocation() Format Tests
    // ============================================================================

    TEST(AssetLocationTest, GetFullLocationOnlyName)
    {
        AssetLocation location("myAsset");
        EXPECT_EQ(location.getFullLocation(), "myAsset");
        EXPECT_EQ(location.getName(), "myAsset");
        EXPECT_EQ(location.getPath(), "");
        EXPECT_FALSE(location.getPackName().has_value());
    }

    TEST(AssetLocationTest, GetFullLocationNameAndPath)
    {
        AssetLocation location("myAsset@some/path");
        EXPECT_EQ(location.getFullLocation(), "myAsset@some/path");
        EXPECT_EQ(location.getName(), "myAsset");
        EXPECT_EQ(location.getPath(), "some/path");
        EXPECT_FALSE(location.getPackName().has_value());
    }

    TEST(AssetLocationTest, GetFullLocationNameAndPack)
    {
        AssetLocation location("myPack::myAsset");
        EXPECT_EQ(location.getFullLocation(), "myPack::myAsset");
        EXPECT_EQ(location.getName(), "myAsset");
        EXPECT_EQ(location.getPath(), "");
        ASSERT_TRUE(location.getPackName().has_value());
        EXPECT_EQ(location.getPackName()->get(), "myPack");
    }

    TEST(AssetLocationTest, GetFullLocationAllComponents)
    {
        AssetLocation location("myPack::myAsset@some/path");
        EXPECT_EQ(location.getFullLocation(), "myPack::myAsset@some/path");
    }

    TEST(AssetLocationTest, GetFullLocationAfterClearingPack)
    {
        AssetLocation location("myPack::myAsset@path");
        location.clearPackName();
        EXPECT_EQ(location.getFullLocation(), "myAsset@path");
    }

} // namespace nexo::assets
