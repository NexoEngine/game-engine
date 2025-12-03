//// TransparentStringHash.test.cpp ///////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        03/12/2025
//  Description: Test file for TransparentStringHash utility
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "utils/TransparentStringHash.hpp"
#include <unordered_map>
#include <string_view>

namespace nexo::editor {

    class TransparentStringHashTest : public ::testing::Test {
    protected:
        TransparentStringHash hasher;
    };

    // ==========================================================================
    // Basic Hash Consistency Tests
    // ==========================================================================

    TEST_F(TransparentStringHashTest, HashStringConsistent) {
        std::string s = "test";
        size_t hash1 = hasher(s);
        size_t hash2 = hasher(s);
        EXPECT_EQ(hash1, hash2);
    }

    TEST_F(TransparentStringHashTest, HashStringViewConsistent) {
        std::string_view sv = "test";
        size_t hash1 = hasher(sv);
        size_t hash2 = hasher(sv);
        EXPECT_EQ(hash1, hash2);
    }

    TEST_F(TransparentStringHashTest, HashCStringConsistent) {
        const char* cs = "test";
        size_t hash1 = hasher(cs);
        size_t hash2 = hasher(cs);
        EXPECT_EQ(hash1, hash2);
    }

    // ==========================================================================
    // Cross-type Hash Equality Tests
    // ==========================================================================

    TEST_F(TransparentStringHashTest, SameContentSameHashAcrossTypes) {
        std::string s = "hello";
        std::string_view sv = "hello";
        const char* cs = "hello";

        size_t hashString = hasher(s);
        size_t hashStringView = hasher(sv);
        size_t hashCString = hasher(cs);

        // All should produce the same hash for the same content
        EXPECT_EQ(hashString, hashStringView);
        EXPECT_EQ(hashStringView, hashCString);
    }

    TEST_F(TransparentStringHashTest, DifferentContentDifferentHash) {
        std::string s1 = "hello";
        std::string s2 = "world";

        EXPECT_NE(hasher(s1), hasher(s2));
    }

    // ==========================================================================
    // Edge Cases
    // ==========================================================================

    TEST_F(TransparentStringHashTest, EmptyStringHash) {
        std::string empty = "";
        std::string_view emptySv = "";
        const char* emptyCs = "";

        size_t hashString = hasher(empty);
        size_t hashStringView = hasher(emptySv);
        size_t hashCString = hasher(emptyCs);

        EXPECT_EQ(hashString, hashStringView);
        EXPECT_EQ(hashStringView, hashCString);
    }

    TEST_F(TransparentStringHashTest, NullptrCStringReturnsZero) {
        const char* nullStr = nullptr;
        EXPECT_EQ(hasher(nullStr), 0u);
    }

    TEST_F(TransparentStringHashTest, SingleCharacterHash) {
        std::string s = "a";
        std::string_view sv = "a";
        const char* cs = "a";

        EXPECT_EQ(hasher(s), hasher(sv));
        EXPECT_EQ(hasher(sv), hasher(cs));
    }

    TEST_F(TransparentStringHashTest, LongStringHash) {
        std::string longStr(1000, 'x');
        std::string_view longSv = longStr;

        EXPECT_EQ(hasher(longStr), hasher(longSv));
    }

    TEST_F(TransparentStringHashTest, StringWithSpaces) {
        std::string s = "hello world";
        std::string_view sv = "hello world";

        EXPECT_EQ(hasher(s), hasher(sv));
    }

    TEST_F(TransparentStringHashTest, SpecialCharacters) {
        std::string s = "test!@#$%^&*()";
        std::string_view sv = "test!@#$%^&*()";

        EXPECT_EQ(hasher(s), hasher(sv));
    }

    // ==========================================================================
    // Heterogeneous Lookup Integration Test
    // ==========================================================================

    TEST_F(TransparentStringHashTest, HeterogeneousLookupWorksInUnorderedMap) {
        std::unordered_map<std::string, int, TransparentStringHash, std::equal_to<>> map;

        map["key1"] = 1;
        map["key2"] = 2;
        map["key3"] = 3;

        // Lookup using string_view (heterogeneous lookup)
        std::string_view sv = "key1";
        EXPECT_NE(map.find(sv), map.end());
        EXPECT_EQ(map.find(sv)->second, 1);

        // Lookup using const char* (heterogeneous lookup)
        const char* cs = "key2";
        EXPECT_NE(map.find(cs), map.end());
        EXPECT_EQ(map.find(cs)->second, 2);

        // Lookup for non-existent key
        EXPECT_EQ(map.find("nonexistent"), map.end());
    }

    TEST_F(TransparentStringHashTest, IsTransparentTypeExists) {
        // Verify that is_transparent type alias is defined
        static_assert(std::is_same_v<TransparentStringHash::is_transparent, void>,
                      "is_transparent should be void");
    }

}
