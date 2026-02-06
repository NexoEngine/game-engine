//// TransparentStringHasher.test.cpp /////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for TransparentStringHasher structure
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include "renderer/ShaderLibrary.hpp"

namespace nexo::renderer {

// =============================================================================
// TransparentStringHasher Tests
// =============================================================================

class TransparentStringHasherTest : public ::testing::Test {
protected:
    TransparentStringHasher hasher;
};

// Test: Hash of std::string matches hash of equivalent std::string_view
TEST_F(TransparentStringHasherTest, StringAndStringViewHashMatch) {
    std::string str = "test_shader";
    std::string_view sv = str;

    size_t hash_str = hasher(str);
    size_t hash_sv = hasher(sv);

    EXPECT_EQ(hash_str, hash_sv);
}

// Test: Different strings produce different hashes
TEST_F(TransparentStringHasherTest, DifferentStringsProduceDifferentHashes) {
    std::string str1 = "shader_one";
    std::string str2 = "shader_two";

    size_t hash1 = hasher(str1);
    size_t hash2 = hasher(str2);

    EXPECT_NE(hash1, hash2);
}

// Test: Empty strings hash consistently
TEST_F(TransparentStringHasherTest, EmptyStringsHashConsistently) {
    std::string empty_str = "";
    std::string_view empty_sv = "";

    size_t hash_str = hasher(empty_str);
    size_t hash_sv = hasher(empty_sv);

    EXPECT_EQ(hash_str, hash_sv);
}

// Test: Same string produces same hash (deterministic)
TEST_F(TransparentStringHasherTest, SameStringProducesSameHash) {
    std::string str = "consistent_shader";

    size_t hash1 = hasher(str);
    size_t hash2 = hasher(str);
    size_t hash3 = hasher(str);

    EXPECT_EQ(hash1, hash2);
    EXPECT_EQ(hash2, hash3);
}

// Test: Long strings work correctly
TEST_F(TransparentStringHasherTest, LongStringsWorkCorrectly) {
    std::string long_str = "this_is_a_very_long_shader_name_that_might_be_used_in_a_real_world_application_with_descriptive_naming_conventions";
    std::string_view long_sv = long_str;

    size_t hash_str = hasher(long_str);
    size_t hash_sv = hasher(long_sv);

    EXPECT_EQ(hash_str, hash_sv);
}

// Test: Strings differing by one character produce different hashes
TEST_F(TransparentStringHasherTest, SimilarStringsProduceDifferentHashes) {
    std::string str1 = "shader_namea";
    std::string str2 = "shader_nameb";

    size_t hash1 = hasher(str1);
    size_t hash2 = hasher(str2);

    EXPECT_NE(hash1, hash2);
}

// Test: String view substring hashes correctly
TEST_F(TransparentStringHasherTest, StringViewSubstringHashesCorrectly) {
    std::string full_str = "prefix_shader_suffix";
    std::string_view sv = full_str;
    std::string_view substr = sv.substr(7, 6); // "shader"
    std::string direct_str = "shader";

    size_t hash_substr = hasher(substr);
    size_t hash_direct = hasher(direct_str);

    EXPECT_EQ(hash_substr, hash_direct);
}

// Test: Case sensitivity
TEST_F(TransparentStringHasherTest, CaseSensitiveHashing) {
    std::string lower = "shader";
    std::string upper = "SHADER";

    size_t hash_lower = hasher(lower);
    size_t hash_upper = hasher(upper);

    EXPECT_NE(hash_lower, hash_upper);
}

// Test: Special characters in strings
TEST_F(TransparentStringHasherTest, SpecialCharactersHandled) {
    std::string special_str = "shader_@#$%^&*()";
    std::string_view special_sv = special_str;

    size_t hash_str = hasher(special_str);
    size_t hash_sv = hasher(special_sv);

    EXPECT_EQ(hash_str, hash_sv);
}

// Test: Numbers in strings
TEST_F(TransparentStringHasherTest, NumericStringsHandled) {
    std::string numeric_str = "shader_12345";
    std::string_view numeric_sv = numeric_str;

    size_t hash_str = hasher(numeric_str);
    size_t hash_sv = hasher(numeric_sv);

    EXPECT_EQ(hash_str, hash_sv);
}

// =============================================================================
// TransparentStringHasher Integration Tests
// =============================================================================

class TransparentStringHasherIntegrationTest : public ::testing::Test {
protected:
    std::unordered_map<
        std::string,
        int,
        TransparentStringHasher,
        std::equal_to<>
    > test_map;
};

// Test: Heterogeneous lookup with string_view
TEST_F(TransparentStringHasherIntegrationTest, HeterogeneousLookupWithStringView) {
    test_map["basic_shader"] = 1;
    test_map["advanced_shader"] = 2;

    std::string_view sv = "basic_shader";
    auto it = test_map.find(sv);

    ASSERT_NE(it, test_map.end());
    EXPECT_EQ(it->second, 1);
}

// Test: Insert with string, lookup with string_view
TEST_F(TransparentStringHasherIntegrationTest, InsertStringLookupStringView) {
    std::string key = "vertex_shader";
    test_map[key] = 42;

    std::string_view sv_key = "vertex_shader";
    EXPECT_EQ(test_map.count(sv_key), 1);
    EXPECT_EQ(test_map[key], 42);
}

// Test: String view lookup avoids string construction
TEST_F(TransparentStringHasherIntegrationTest, StringViewLookupAvoidsConstruction) {
    test_map["fragment_shader"] = 100;

    // Using string_view directly without constructing std::string
    const char* c_str = "fragment_shader";
    std::string_view sv(c_str);

    auto it = test_map.find(sv);
    ASSERT_NE(it, test_map.end());
    EXPECT_EQ(it->second, 100);
}

// Test: Multiple insertions and lookups
TEST_F(TransparentStringHasherIntegrationTest, MultipleInsertionsAndLookups) {
    test_map["shader_a"] = 1;
    test_map["shader_b"] = 2;
    test_map["shader_c"] = 3;

    EXPECT_EQ(test_map.size(), 3);

    std::string_view sv_a = "shader_a";
    std::string_view sv_b = "shader_b";
    std::string_view sv_c = "shader_c";

    auto it_a = test_map.find(sv_a);
    auto it_b = test_map.find(sv_b);
    auto it_c = test_map.find(sv_c);

    ASSERT_NE(it_a, test_map.end());
    ASSERT_NE(it_b, test_map.end());
    ASSERT_NE(it_c, test_map.end());

    EXPECT_EQ(it_a->second, 1);
    EXPECT_EQ(it_b->second, 2);
    EXPECT_EQ(it_c->second, 3);
}

// Test: Lookup non-existent key with string_view
TEST_F(TransparentStringHasherIntegrationTest, LookupNonExistentKeyWithStringView) {
    test_map["existing_shader"] = 1;

    std::string_view sv = "non_existent_shader";
    auto it = test_map.find(sv);

    EXPECT_EQ(it, test_map.end());
}

// Test: is_transparent typedef exists
TEST_F(TransparentStringHasherIntegrationTest, HasIsTransparentTypedef) {
    // This test verifies that the hasher has the is_transparent typedef
    // which is required for heterogeneous lookup
    using HasIsTransparent = typename TransparentStringHasher::is_transparent;
    SUCCEED(); // If compilation succeeds, the typedef exists
}

}  // namespace nexo::renderer
