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

    TEST(AssetNameTest, ValidAssetNames)
    {
        EXPECT_NO_THROW(AssetName("ValidName"));
        EXPECT_NO_THROW(AssetName("Valid_Name"));
        EXPECT_NO_THROW(AssetName("Valid123"));
        EXPECT_NO_THROW(AssetName("Valid-Name"));
        EXPECT_NO_THROW(AssetName("Valid.Name"));
    }

    TEST(AssetNameTest, InvalidEmptyName)
    {
        EXPECT_THROW(AssetName(""), InvalidName);
    }

    TEST(AssetNameTest, InvalidTooLongName)
    {
        const std::string longName(AssetNameValidator::MaxLength + 1, 'a');
        EXPECT_THROW({
            AssetName test(longName);
        }, InvalidName);
    }

    TEST(AssetNameTest, InvalidCharactersInName)
    {
        EXPECT_THROW(AssetName("Invalid@Name"), InvalidName);
        EXPECT_THROW(AssetName("Invalid/Name"), InvalidName);
        EXPECT_THROW(AssetName("Invalid:Name"), InvalidName);
        EXPECT_THROW(AssetName("Invalid?Name"), InvalidName);
    }

    TEST(AssetNameTest, ReservedKeywords)
    {
        for (const auto& keyword : AssetNameValidator::ForbiddenKeywords) {
            EXPECT_THROW({
                AssetName test(keyword);
            }, InvalidName);
        }
    }

    TEST(AssetNameTest, StaticValidateNameValidCases)
    {
        EXPECT_EQ(AssetNameValidator::validate("ValidName"), std::nullopt);
        EXPECT_EQ(AssetNameValidator::validate("Valid_Name"), std::nullopt);
        EXPECT_EQ(AssetNameValidator::validate("Valid123"), std::nullopt);
    }

    TEST(AssetNameTest, StaticValidateNameInvalidCases)
    {
        EXPECT_EQ(AssetNameValidator::validate(""), "Cannot be empty.");
        EXPECT_EQ(AssetNameValidator::validate("Invalid@Name"), "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
        EXPECT_EQ(AssetNameValidator::validate(std::string(AssetNameValidator::MaxLength + 1, 'a')), "Cannot exceed 255 characters.");
    }

    TEST(AssetNameTest, StaticIsValidNameFunction)
    {
        EXPECT_TRUE(AssetNameValidator::validate("ValidName") == std::nullopt);
        EXPECT_FALSE(AssetNameValidator::validate("") == std::nullopt);
        EXPECT_FALSE(AssetNameValidator::validate("Invalid@Name") == std::nullopt);
        EXPECT_FALSE(AssetNameValidator::validate(std::string(256, 'a')) == std::nullopt);
    }

    TEST(AssetNameTest, StringConversion)
    {
        const AssetName name("ValidName");

        const std::string str = std::string(name);
        const std::string_view sv = std::string_view(name);
        const char* cstr = name.c_str();
        EXPECT_EQ(str, "ValidName");
        EXPECT_EQ(sv, "ValidName");
        EXPECT_STREQ(cstr, "ValidName");
    }

    TEST(AssetNameTest, EqualityAndInequalityOperators)
    {
        AssetName name1("Name1");
        AssetName name2("Name1");
        AssetName name3("Name3");

        EXPECT_TRUE(name1 == name2);
        EXPECT_FALSE(name1 != name2);
        EXPECT_FALSE(name1 == name3);
        EXPECT_TRUE(name1 != name3);
    }

    TEST(AssetNameTest, TestDataAndCstr)
    {
        AssetName name("Name");
        EXPECT_EQ(name, "Name");
        EXPECT_STREQ(name.c_str(), "Name");
        EXPECT_EQ(std::string(name), "Name");
        EXPECT_EQ(name.data(), "Name");
    }

    TEST(AssetNameTest, Renaming)
    {
        AssetName name("InitialName");
        EXPECT_EQ(name, "InitialName");
        EXPECT_NO_THROW(name.rename("NewName"));
        EXPECT_EQ(name, "NewName");

        EXPECT_EQ(name.rename("Invalid@Name"), "Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
        EXPECT_EQ(name, "NewName");  // Should remain unchanged
    }

    TEST(AssetNameTest, InvalidNameExceptionMessage)
    {
        try {
            AssetName name("Invalid@Name");
        } catch (const InvalidName& e) {
            EXPECT_EQ(e.getMessage(), "Invalid name 'Invalid@Name': Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
        }
    }

    static void testException(const std::string_view name, const std::string& expectedMessage)
    {
        try {
            AssetName assetName(name);
        } catch (const InvalidName& e) {
            EXPECT_EQ(e.getMessage(), expectedMessage);
        }
    }

    TEST(AssetNameTest, AllInvalidNameExceptions)
    {
        testException("", "Invalid name '': Cannot be empty.");
        testException("Invalid@Name", "Invalid name 'Invalid@Name': Allowed characters are 0-9, a-z, A-Z, '.', '_', and '-'.");
        testException(std::string(AssetNameValidator::MaxLength + 1, 'a'), "Invalid name '" + std::string(AssetNameValidator::MaxLength + 1, 'a') + "': Cannot exceed 255 characters.");
        for (const auto& keyword : AssetNameValidator::ForbiddenKeywords) {
            testException(keyword, "Invalid name '" + std::string(keyword) + "': Cannot be a reserved keyword.");
        }
    }

} // namespace nexo::assets
