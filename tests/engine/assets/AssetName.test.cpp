//// AssetName.test.cpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        07/12/2024
//  Description: Test file for the AssetName class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "assets/AssetName.hpp"

namespace nexo::assets {

    TEST(AssetNameTest, ValidAssetName)
    {
        // Valid names
        EXPECT_NO_THROW(AssetName("ValidName"));
        EXPECT_NO_THROW(AssetName("Valid_Name"));
        EXPECT_NO_THROW(AssetName("Valid123"));
        EXPECT_NO_THROW(AssetName("Valid-Name"));
    }

    TEST(AssetNameTest, InvalidAssetNameEmpty)
    {
        // Empty name should throw an exception
        EXPECT_THROW(AssetName(""), InvalidAssetName);
    }

    TEST(AssetNameTest, InvalidAssetNameTooLong)
    {
        // Name exceeding 255 characters should throw an exception
        std::string longName(256, 'a');
        EXPECT_THROW({
            AssetName assetName(longName);
        }, InvalidAssetName);
    }

    TEST(AssetNameTest, InvalidAssetNameContainsInvalidCharacters)
    {
        // Names with invalid characters should throw an exception
        EXPECT_THROW(AssetName("Invalid@Name"), InvalidAssetName);
        EXPECT_THROW(AssetName("Invalid/Name"), InvalidAssetName);
        EXPECT_THROW(AssetName("Invalid\\Name"), InvalidAssetName);
    }

    TEST(AssetNameTest, TestAllInvalidChars)
    {
        // Test all invalid characters
        for (int i = 0; AssetName::InvalidCharList[i] != '\0'; ++i) {
            std::string name = "InvalidXName";
            name[7] = AssetName::InvalidCharList[i];
            EXPECT_THROW({
                AssetName testName(name);
            }, InvalidAssetName);
            EXPECT_THROW(AssetName::checkName(name), InvalidAssetName);
            EXPECT_FALSE(AssetName::isValid(name));
        }
    }

    TEST(AssetNameTest, ImplicitConversionToString)
    {
        // Test implicit conversion to std::string
        const AssetName name("ValidName");
        const std::string str = name;
        EXPECT_EQ(str, "ValidName");
    }

    TEST(AssetNameTest, EqualityOperators)
    {
        // Test equality and inequality operators
        AssetName name1("Name1");
        const AssetName name2("Name1");
        AssetName name3("Name3");

        EXPECT_TRUE(name1 == name2);
        EXPECT_FALSE(name1 != name2);
        EXPECT_FALSE(name1 == name3);
        EXPECT_TRUE(name1 != name3);
    }

    TEST(AssetNameTest, CheckNameStaticFunctionValid)
    {
        // Static checkName function for valid names
        EXPECT_NO_THROW(AssetName::checkName("ValidName"));
    }

    TEST(AssetNameTest, CheckNameStaticFunctionInvalid)
    {
        // Static checkName function for invalid names
        EXPECT_THROW(AssetName::checkName(""), InvalidAssetName);
        EXPECT_THROW(AssetName::checkName("Invalid@Name"), InvalidAssetName);
        EXPECT_THROW(AssetName::checkName("Invalid/Name"), InvalidAssetName);
    }

    TEST(AssetNameTest, IsValidStaticFunction)
    {
        // Static isValid function
        EXPECT_TRUE(AssetName::isValid("ValidName"));
        EXPECT_FALSE(AssetName::isValid(""));
        EXPECT_FALSE(AssetName::isValid("Invalid@Name"));
        EXPECT_FALSE(AssetName::isValid("Invalid/Name"));
        EXPECT_FALSE(AssetName::isValid(std::string(256, 'a'))); // Too long
    }

    TEST(AssetNameTest, ImplicitConstruction)
    {
        // Test implicit construction
        EXPECT_NO_THROW(AssetName name = std::string("ValidName"));
        EXPECT_NO_THROW(AssetName name = "ValidName");
        EXPECT_NO_THROW(AssetName name = std::string_view("ValidName"));
    }

} // namespace nexo::assets
