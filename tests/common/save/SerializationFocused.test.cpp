//// SerializationContext.test.cpp ////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢀⢀⢀⢀⢀⢀
//
//  Author:      Guillaume HEIN
//  Date:        07/11/2025
//  Description: Focused serialization tests: ADL integration, versioning,
//               error handling, components
//
///////////////////////////////////////////////////////////////////////////////

#include <glm/glm.hpp>
#include <gtest/gtest.h>

#include "Json.hpp"
#include "save/Serialization.hpp"
#include "save/glm/glm.hpp"

using json = nlohmann::json;

// 1) ADL integration test: ensure nlohmann::json uses adl_serializer -> nexo::save::serialize
TEST(SerializationFocused, ADLIntegration_GLMVec3) {
    glm::vec3 original(1.0f, 2.0f, 3.0f);

    // Use nlohmann::json's ADL-based API
    json j = original; // should call nexo::save::serialize via adl_serializer
    ASSERT_TRUE(j.is_object());
    EXPECT_EQ(j["x"], 1.0f);

    glm::vec3 restored = j.get<glm::vec3>(); // should call nexo::save::deserialize
    EXPECT_FLOAT_EQ(restored.x, original.x);
    EXPECT_FLOAT_EQ(restored.y, original.y);
    EXPECT_FLOAT_EQ(restored.z, original.z);
}

// 2) Versioning + migration: declare a small type and provide two-version serializers inside this test translation unit
namespace nexo::save {
    struct VersionedTestType {
        int value = 0;
    };

    // Current version is 1
    template<>
    struct CurrentVersion<VersionedTestType> { static constexpr uint32_t value = 1; };

    // Version 0 serializer: field name "a"
    template<>
    struct Serializer<VersionedTestType, 0> {
        static void serialize(nexo::json& j, const VersionedTestType& v, const SerializationContext& /*ctx*/ = {}) {
            j = json::object();
            j["a"] = v.value;
            j["_version"] = 0;
        }
        static void deserialize(const json& j, VersionedTestType& v, const SerializationContext& /*ctx*/ = {}) {
            v.value = j.at("a");
        }
        static void migrate_from_previous(const json& /*j*/) {
            // nothing to do for v0
        }
    };

    // Version 1 serializer: field name "b"
    template<>
    struct Serializer<VersionedTestType, 1> {
        static void serialize(nexo::json& j, const VersionedTestType& v, const SerializationContext& /*ctx*/ = {}) {
            j = json::object();
            j["b"] = v.value;
            j["_version"] = 1;
        }
        static void deserialize(const json& j, VersionedTestType& v, const SerializationContext& /*ctx*/ = {}) {
            v.value = j.at("b");
        }
        static void migrate_from_previous(const json& j) {
            // Move field "a" -> "b"
            json& mutable_j = const_cast<json&>(j);
            if (mutable_j.contains("a")) {
                mutable_j["b"] = mutable_j["a"];
                mutable_j.erase("a");
            }
        }
    };
}

TEST(SerializationFocused, Versioning_Migration) {
    using nexo::save::VersionedTestType;

    // Create JSON that represents version 0
    json j;
    j["a"] = 42;
    j["_version"] = 0;

    VersionedTestType dest;
    nexo::save::deserialize(j, dest); // should migrate to v1 and then deserialize

    EXPECT_EQ(dest.value, 42);
}

TEST(SerializationFocused, Versioning_FutureVersionThrows) {
    using nexo::save::VersionedTestType;

    json j;
    j["b"] = 7;
    j["_version"] = 9999; // future

    VersionedTestType dest;
    EXPECT_THROW(nexo::save::deserialize(j, dest), std::runtime_error);
}

// 4) Edge case: wrong field types lead to nlohmann::json exception
TEST(SerializationFocused, ErrorHandling_WrongFieldType) {
    json j;
    j["x"] = "not a float";
    j["y"] = 2.0f;
    j["z"] = 3.0f;

    glm::vec3 v;
    EXPECT_THROW(nexo::save::deserialize(j, v), nlohmann::json::exception);
}

// 5) Real component test + ADL integration for a user type: define a simple GameObject and a serializer
namespace nexo::save {
    struct SimpleTransform {
        glm::vec3 position;
    };

    template<>
    struct CurrentVersion<SimpleTransform> { static constexpr uint32_t value = 0; };

    template<>
    struct Serializer<SimpleTransform, 0> {
        static void serialize(nexo::json& j, const SimpleTransform& t, const SerializationContext& /*ctx*/ = {}) {
            j = json::object();
            j["position"] = t.position; // uses adl for glm::vec3
            j["_version"] = 0;
        }
        static void deserialize(const json& j, SimpleTransform& t, const SerializationContext& /*ctx*/ = {}) {
            nexo::save::deserialize(j.at("position"), t.position);
        }
        static void migrate_from_previous(const json& /*j*/) {}
    };

    struct GameObject {
        std::string name;
        SimpleTransform transform{};
        std::vector<std::string> components;
    };

    template<>
    struct CurrentVersion<GameObject> { static constexpr uint32_t value = 0; };

    template<>
    struct Serializer<GameObject, 0> {
        static void serialize(nexo::json& j, const GameObject& g, const SerializationContext& /*ctx*/ = {}) {
            j = json::object();
            j["name"] = g.name;
            j["transform"] = g.transform; // ADL: should call Serializer<SimpleTransform,0>
            j["components"] = g.components;
            j["_version"] = 0;
        }
        static void deserialize(const json& j, GameObject& g, const SerializationContext& /*ctx*/ = {}) {
            g.name = j.at("name").get<std::string>();
            g.transform = j.at("transform").get<SimpleTransform>();
            g.components = j.at("components").get<std::vector<std::string>>();
        }
        static void migrate_from_previous(const json& /*j*/) {}
    };
}

TEST(SerializationFocused, RealComponent_GameObjectRoundTrip_ADL) {
    nexo::save::GameObject go;
    go.name = "Player";
    go.transform.position = glm::vec3(10.0f, 5.0f, 0.0f);
    go.components = {"Transform", "Collider", "Rigidbody"};

    // Use nlohmann::json ADL API to serialize the GameObject
    json j = go; // should use nexo::save::adl_serializer via CurrentVersion/Serializer

    EXPECT_TRUE(j.is_object());
    EXPECT_EQ(j["name"].get<std::string>(), "Player");
    EXPECT_TRUE(j.contains("transform"));

    nexo::save::serialize(j, go); // alternatively, explicit serialize call
    EXPECT_TRUE(j.is_object());
    EXPECT_EQ(j["name"].get<std::string>(), "Player");
    EXPECT_TRUE(j.contains("transform"));


    nexo::save::GameObject restored = j.get<nexo::save::GameObject>();
    EXPECT_EQ(restored.name, go.name);
    EXPECT_EQ(restored.components.size(), go.components.size());
    EXPECT_FLOAT_EQ(restored.transform.position.x, go.transform.position.x);
}
