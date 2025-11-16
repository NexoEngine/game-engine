//// VersionedJson.test.cpp ////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢀⢀⢀⢀⢀⢀
//
//  Author:      GitHub Copilot
//  Date:        14/11/2025
//  Description: Test file for VersionedJson serialization/deserialization utilities
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>

import nexo.json;
import nexo.save.serialization;

using nexo::json;

// ============================================================================
// VersionedJson Tests - Basic Functionality
// ============================================================================

TEST(VersionedJsonBasic, DefaultConstruction) {
    nexo::save::detail::VersionedJson vj;
    EXPECT_EQ(vj.version, 0);
    EXPECT_TRUE(vj.data.is_null());
}

TEST(VersionedJsonBasic, ConstructionWithValues) {
    nexo::save::detail::VersionedJson vj;
    vj.version = 1;
    vj.data = json{{"key", "value"}};

    EXPECT_EQ(vj.version, 1);
    EXPECT_EQ(vj.data.at("key").get<std::string>(), "value");
}

// ============================================================================
// VersionedJson to_json Tests
// ============================================================================

TEST(VersionedJsonToJson, Version0WithPrimitiveData) {
    // Version 0 should serialize data directly without version marker
    nexo::save::detail::VersionedJson vj;
    vj.version = 0;
    vj.data = 42;

    json j = vj;

    EXPECT_EQ(j.get<int>(), 42);
    EXPECT_FALSE(j.contains("_version"));
}

TEST(VersionedJsonToJson, Version0WithObjectData) {
    // Version 0 with object data should still be written directly
    nexo::save::detail::VersionedJson vj;
    vj.version = 0;
    vj.data = json{{"name", "test"}};

    json j = vj;

    EXPECT_EQ(j.at("name").get<std::string>(), "test");
    EXPECT_FALSE(j.contains("_version"));
}

TEST(VersionedJsonToJson, VersionWithObjectData) {
    // Non-zero version with object data should add _version key
    nexo::save::detail::VersionedJson vj;
    vj.version = 2;
    vj.data = json{{"name", "test"}, {"value", 123}};

    json j = vj;

    EXPECT_EQ(j.at("_version").get<uint32_t>(), 2);
    EXPECT_EQ(j.at("name").get<std::string>(), "test");
    EXPECT_EQ(j.at("value").get<int>(), 123);
}

TEST(VersionedJsonToJson, VersionWithPrimitiveData) {
    // Non-zero version with primitive data should wrap in _data key
    nexo::save::detail::VersionedJson vj;
    vj.version = 3;
    vj.data = "primitive_string";

    json j = vj;

    EXPECT_EQ(j.at("_version").get<uint32_t>(), 3);
    EXPECT_EQ(j.at("_data").get<std::string>(), "primitive_string");
}

TEST(VersionedJsonToJson, VersionWithArrayData) {
    // Non-zero version with array data should wrap in _data key
    nexo::save::detail::VersionedJson vj;
    vj.version = 1;
    vj.data = json::array({1, 2, 3});

    json j = vj;

    EXPECT_EQ(j.at("_version").get<uint32_t>(), 1);
    EXPECT_TRUE(j.at("_data").is_array());
    EXPECT_EQ(j.at("_data").at(0).get<int>(), 1);
}

TEST(VersionedJsonToJson, VersionWithNullData) {
    // Non-zero version with null data should wrap in _data key
    nexo::save::detail::VersionedJson vj;
    vj.version = 1;
    vj.data = nullptr;

    json j = vj;

    EXPECT_EQ(j.at("_version").get<uint32_t>(), 1);
    EXPECT_TRUE(j.at("_data").is_null());
}

// ============================================================================
// VersionedJson from_json Tests
// ============================================================================

TEST(VersionedJsonFromJson, PrimitiveWithoutVersion) {
    // Primitive without version key should be version 0
    json j = 42;

    nexo::save::detail::VersionedJson vj;
    vj = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(vj.version, 0);
    EXPECT_EQ(vj.data.get<int>(), 42);
}

TEST(VersionedJsonFromJson, ObjectWithoutVersion) {
    // Object without version key should be version 0
    json j = json{{"name", "test"}, {"value", 456}};

    nexo::save::detail::VersionedJson vj;
    vj = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(vj.version, 0);
    EXPECT_EQ(vj.data.at("name").get<std::string>(), "test");
    EXPECT_EQ(vj.data.at("value").get<int>(), 456);
}

TEST(VersionedJsonFromJson, ArrayWithoutVersion) {
    // Array without version key should be version 0
    json j = json::array({1, 2, 3});

    nexo::save::detail::VersionedJson vj;
    vj = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(vj.version, 0);
    EXPECT_TRUE(vj.data.is_array());
    EXPECT_EQ(vj.data.at(0).get<int>(), 1);
}

TEST(VersionedJsonFromJson, NullWithoutVersion) {
    // Null without version key should be version 0
    json j = nullptr;

    nexo::save::detail::VersionedJson vj;
    vj = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(vj.version, 0);
    EXPECT_TRUE(vj.data.is_null());
}

TEST(VersionedJsonFromJson, ObjectWithVersionAndData) {
    // Object with _version but no _data key should extract version and use rest as data
    json j = json{{"_version", 2}, {"name", "test"}};

    nexo::save::detail::VersionedJson vj;
    vj = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(vj.version, 2);
    EXPECT_EQ(vj.data.at("name").get<std::string>(), "test");
    EXPECT_FALSE(vj.data.contains("_version"));
}

TEST(VersionedJsonFromJson, ObjectWithVersionAndDataKey) {
    // Object with _version and _data key should extract both
    json j = json{{"_version", 3}, {"_data", json{{"inner", "value"}}}};

    nexo::save::detail::VersionedJson vj;
    vj = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(vj.version, 3);
    EXPECT_EQ(vj.data.at("inner").get<std::string>(), "value");
}

TEST(VersionedJsonFromJson, ObjectWithVersionAndPrimitiveData) {
    // Object with _version and _data containing primitive
    json j = json{{"_version", 1}, {"_data", "string_value"}};

    nexo::save::detail::VersionedJson vj;
    vj = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(vj.version, 1);
    EXPECT_EQ(vj.data.get<std::string>(), "string_value");
}

TEST(VersionedJsonFromJson, ObjectWithVersionAndArrayData) {
    // Object with _version and _data containing array
    json j = json{{"_version", 2}, {"_data", json::array({10, 20, 30})}};

    nexo::save::detail::VersionedJson vj;
    vj = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(vj.version, 2);
    EXPECT_TRUE(vj.data.is_array());
    EXPECT_EQ(vj.data.at(1).get<int>(), 20);
}

// ============================================================================
// VersionedJson RoundTrip Tests
// ============================================================================

TEST(VersionedJsonRoundTrip, Version0Primitive) {
    // Version 0 with primitive data
    nexo::save::detail::VersionedJson original;
    original.version = 0;
    original.data = 123;

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, original.version);
    EXPECT_EQ(restored.data, original.data);
}

TEST(VersionedJsonRoundTrip, Version0Object) {
    // Version 0 with object data
    nexo::save::detail::VersionedJson original;
    original.version = 0;
    original.data = json{{"name", "test"}, {"count", 42}};

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, original.version);
    EXPECT_EQ(restored.data, original.data);
}

TEST(VersionedJsonRoundTrip, Version0Array) {
    // Version 0 with array data
    nexo::save::detail::VersionedJson original;
    original.version = 0;
    original.data = json::array({1.5f, 2.5f, 3.5f});

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, original.version);
    EXPECT_EQ(restored.data, original.data);
}

TEST(VersionedJsonRoundTrip, VersionedObjectData) {
    // Versioned with object data
    nexo::save::detail::VersionedJson original;
    original.version = 3;
    original.data = json{{"field1", "value1"}, {"field2", 999}};

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, original.version);
    EXPECT_EQ(restored.data, original.data);
}

TEST(VersionedJsonRoundTrip, VersionedPrimitiveData) {
    // Versioned with primitive data
    nexo::save::detail::VersionedJson original;
    original.version = 5;
    original.data = "some string";

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, original.version);
    EXPECT_EQ(restored.data, original.data);
}

TEST(VersionedJsonRoundTrip, VersionedArrayData) {
    // Versioned with array data
    nexo::save::detail::VersionedJson original;
    original.version = 2;
    original.data = json::array({"a", "b", "c"});

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, original.version);
    EXPECT_EQ(restored.data, original.data);
}

TEST(VersionedJsonRoundTrip, ComplexNestedStructure) {
    // Complex nested structure
    nexo::save::detail::VersionedJson original;
    original.version = 4;
    original.data = json{
        {"name", "complex"},
        {"nested", json{{"inner", "value"}, {"count", 42}}},
        {"array", json::array({1, 2, 3})}
    };

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, original.version);
    EXPECT_EQ(restored.data, original.data);
    EXPECT_EQ(restored.data.at("nested").at("count").get<int>(), 42);
}

// ============================================================================
// VersionedJson Edge Cases
// ============================================================================

TEST(VersionedJsonEdgeCase, MaxUint32Version) {
    // Very large version number
    nexo::save::detail::VersionedJson original;
    original.version = std::numeric_limits<uint32_t>::max();
    original.data = json{{"test", "data"}};

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, std::numeric_limits<uint32_t>::max());
}

TEST(VersionedJsonEdgeCase, EmptyObject) {
    // Empty object data
    nexo::save::detail::VersionedJson original;
    original.version = 1;
    original.data = json::object();

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, 1);
    EXPECT_TRUE(restored.data.is_object());
    EXPECT_TRUE(restored.data.empty());
}

TEST(VersionedJsonEdgeCase, EmptyArray) {
    // Empty array data
    nexo::save::detail::VersionedJson original;
    original.version = 1;
    original.data = json::array();

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, 1);
    EXPECT_TRUE(restored.data.is_array());
    EXPECT_TRUE(restored.data.empty());
}

TEST(VersionedJsonEdgeCase, SpecialStringCharacters) {
    // String with special characters
    nexo::save::detail::VersionedJson original;
    original.version = 1;
    original.data = "String with \"quotes\", \\backslash, \n newline";

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, 1);
    EXPECT_EQ(restored.data, original.data);
}

TEST(VersionedJsonEdgeCase, FloatingPointPrecision) {
    // Floating point values
    nexo::save::detail::VersionedJson original;
    original.version = 1;
    original.data = json{{"pi", 3.14159265359}, {"e", 2.71828182846}};

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, 1);
    EXPECT_DOUBLE_EQ(restored.data.at("pi").get<double>(), 3.14159265359);
}

TEST(VersionedJsonEdgeCase, LargeNestedStructure) {
    // Large nested structure
    nexo::save::detail::VersionedJson original;
    original.version = 1;
    json large_data = json::object();
    for (int i = 0; i < 100; ++i) {
        large_data["item_" + std::to_string(i)] = i * 2;
    }
    original.data = large_data;

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, 1);
    EXPECT_EQ(restored.data.size(), 100);
    EXPECT_EQ(restored.data.at("item_50").get<int>(), 100);
}

TEST(VersionedJsonEdgeCase, UnicodeString) {
    // Unicode characters
    nexo::save::detail::VersionedJson original;
    original.version = 1;
    original.data = "Unicode: 你好世界 🌍 مرحبا";

    json j = original;
    nexo::save::detail::VersionedJson restored = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, 1);
    EXPECT_EQ(restored.data, original.data);
}

// ============================================================================
// VersionedJson JSON String Parsing Tests
// ============================================================================

TEST(VersionedJsonParsing, ParseVersion0String) {
    // Parse JSON string directly (version 0)
    std::string json_str = R"({"name": "test", "value": 123})";
    json j = json::parse(json_str);

    nexo::save::detail::VersionedJson vj = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(vj.version, 0);
    EXPECT_EQ(vj.data.at("name").get<std::string>(), "test");
}

TEST(VersionedJsonParsing, ParseVersionedString) {
    // Parse JSON string with version
    std::string json_str = R"({"_version": 2, "name": "test", "value": 123})";
    json j = json::parse(json_str);

    nexo::save::detail::VersionedJson vj = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(vj.version, 2);
    EXPECT_EQ(vj.data.at("name").get<std::string>(), "test");
    EXPECT_FALSE(vj.data.contains("_version"));
}

TEST(VersionedJsonParsing, ParseVersionedWithDataKey) {
    // Parse JSON string with _version and _data keys
    std::string json_str = R"({"_version": 3, "_data": {"inner": "value"}})";
    json j = json::parse(json_str);

    nexo::save::detail::VersionedJson vj = j.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(vj.version, 3);
    EXPECT_EQ(vj.data.at("inner").get<std::string>(), "value");
}

TEST(VersionedJsonParsing, DumpAndParse) {
    // Round trip through string
    nexo::save::detail::VersionedJson original;
    original.version = 2;
    original.data = json{{"key1", "val1"}, {"key2", 42}};

    json j = original;
    std::string json_str = j.dump();

    json j2 = json::parse(json_str);
    nexo::save::detail::VersionedJson restored = j2.get<nexo::save::detail::VersionedJson>();

    EXPECT_EQ(restored.version, original.version);
    EXPECT_EQ(restored.data, original.data);
}



