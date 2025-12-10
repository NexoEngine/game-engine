//// Uuid.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for UUID generation and UuidComponent (memento pattern)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <regex>
#include <set>
#include "components/Uuid.hpp"

namespace nexo::components {

// =============================================================================
// UUID Generator Function Tests
// =============================================================================

class UuidGeneratorTest : public ::testing::Test {
protected:
    // UUID format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx (8-4-4-4-12)
    static bool isValidUuidFormat(const std::string& uuid) {
        // Check length (36 chars: 32 hex + 4 dashes)
        if (uuid.length() != 36) return false;

        // Check dash positions (8, 13, 18, 23)
        if (uuid[8] != '-' || uuid[13] != '-' || uuid[18] != '-' || uuid[23] != '-')
            return false;

        // Check all other characters are hex
        for (size_t i = 0; i < uuid.length(); ++i) {
            if (i == 8 || i == 13 || i == 18 || i == 23) continue;
            if (!std::isxdigit(uuid[i])) return false;
        }

        return true;
    }
};

TEST_F(UuidGeneratorTest, GeneratesCorrectLength) {
    std::string uuid = genUuid();
    EXPECT_EQ(uuid.length(), 36u);
}

TEST_F(UuidGeneratorTest, HasCorrectFormat) {
    std::string uuid = genUuid();
    EXPECT_TRUE(isValidUuidFormat(uuid)) << "Invalid UUID format: " << uuid;
}

TEST_F(UuidGeneratorTest, HasDashesAtCorrectPositions) {
    std::string uuid = genUuid();
    EXPECT_EQ(uuid[8], '-');
    EXPECT_EQ(uuid[13], '-');
    EXPECT_EQ(uuid[18], '-');
    EXPECT_EQ(uuid[23], '-');
}

TEST_F(UuidGeneratorTest, ContainsOnlyHexAndDashes) {
    std::string uuid = genUuid();

    for (size_t i = 0; i < uuid.length(); ++i) {
        char c = uuid[i];
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            EXPECT_EQ(c, '-') << "Expected dash at position " << i;
        } else {
            EXPECT_TRUE(std::isxdigit(c))
                << "Non-hex character '" << c << "' at position " << i;
        }
    }
}

TEST_F(UuidGeneratorTest, UsesLowercaseHex) {
    // Generate multiple UUIDs to have a good sample
    for (int i = 0; i < 100; ++i) {
        std::string uuid = genUuid();
        for (char c : uuid) {
            if (c != '-') {
                // Should be lowercase hex (0-9 or a-f)
                EXPECT_TRUE((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))
                    << "Character '" << c << "' is not lowercase hex";
            }
        }
    }
}

TEST_F(UuidGeneratorTest, GeneratesUniqueValues) {
    std::set<std::string> uuids;
    const int count = 1000;

    for (int i = 0; i < count; ++i) {
        std::string uuid = genUuid();
        EXPECT_TRUE(uuids.insert(uuid).second)
            << "Duplicate UUID generated: " << uuid;
    }

    EXPECT_EQ(uuids.size(), static_cast<size_t>(count));
}

TEST_F(UuidGeneratorTest, MultipleCallsProduceDifferentResults) {
    std::string uuid1 = genUuid();
    std::string uuid2 = genUuid();
    std::string uuid3 = genUuid();

    EXPECT_NE(uuid1, uuid2);
    EXPECT_NE(uuid2, uuid3);
    EXPECT_NE(uuid1, uuid3);
}

TEST_F(UuidGeneratorTest, MatchesRegexPattern) {
    std::regex uuidRegex("^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$");

    for (int i = 0; i < 100; ++i) {
        std::string uuid = genUuid();
        EXPECT_TRUE(std::regex_match(uuid, uuidRegex))
            << "UUID doesn't match regex: " << uuid;
    }
}

// =============================================================================
// UuidComponent Tests
// =============================================================================

class UuidComponentTest : public ::testing::Test {
protected:
    // Helper to check if format is valid
    static bool isValidUuid(const std::string& uuid) {
        if (uuid.length() != 36) return false;
        if (uuid[8] != '-' || uuid[13] != '-' || uuid[18] != '-' || uuid[23] != '-')
            return false;
        return true;
    }
};

TEST_F(UuidComponentTest, DefaultConstructorGeneratesUuid) {
    UuidComponent component;
    EXPECT_FALSE(component.uuid.empty());
    EXPECT_TRUE(isValidUuid(component.uuid));
}

TEST_F(UuidComponentTest, EachComponentGetsUniqueUuid) {
    UuidComponent comp1;
    UuidComponent comp2;
    UuidComponent comp3;

    EXPECT_NE(comp1.uuid, comp2.uuid);
    EXPECT_NE(comp2.uuid, comp3.uuid);
    EXPECT_NE(comp1.uuid, comp3.uuid);
}

TEST_F(UuidComponentTest, SaveCapturesUuid) {
    UuidComponent component;
    std::string originalUuid = component.uuid;

    auto memento = component.save();
    EXPECT_EQ(memento.uuid, originalUuid);
}

TEST_F(UuidComponentTest, RestoreAppliesUuid) {
    UuidComponent component;

    UuidComponent::Memento memento;
    memento.uuid = "custom-uuid-for-testing";

    component.restore(memento);
    EXPECT_EQ(component.uuid, "custom-uuid-for-testing");
}

TEST_F(UuidComponentTest, SaveRestoreRoundTrip) {
    UuidComponent component;
    std::string originalUuid = component.uuid;

    auto memento = component.save();

    // Modify the component
    component.uuid = "modified-uuid";
    EXPECT_NE(component.uuid, originalUuid);

    // Restore from memento
    component.restore(memento);
    EXPECT_EQ(component.uuid, originalUuid);
}

TEST_F(UuidComponentTest, MementoIndependence) {
    UuidComponent component;

    component.uuid = "state1";
    auto memento1 = component.save();

    component.uuid = "state2";
    auto memento2 = component.save();

    EXPECT_EQ(memento1.uuid, "state1");
    EXPECT_EQ(memento2.uuid, "state2");
}

TEST_F(UuidComponentTest, RestorePreservesOtherMementos) {
    UuidComponent component;

    component.uuid = "first";
    auto firstMemento = component.save();

    component.uuid = "second";
    auto secondMemento = component.save();

    component.restore(firstMemento);
    EXPECT_EQ(component.uuid, "first");

    // Second memento should still work
    component.restore(secondMemento);
    EXPECT_EQ(component.uuid, "second");
}

}  // namespace nexo::components
