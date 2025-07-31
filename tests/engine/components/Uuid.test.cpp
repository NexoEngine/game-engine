///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date: Today lol
//  Description: Test file for the UuidComponent
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "components/Uuid.hpp"
#include <set>
#include <regex>
#include <thread>
#include <vector>
#include <algorithm>
#include <chrono>

namespace nexo::components {

class UuidComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a default UUID component
        uuidComp = UuidComponent{};
    }
    
    UuidComponent uuidComp;
};

// Test UUID generation function format
TEST_F(UuidComponentTest, UuidFormat) {
    std::string uuid = genUuid();
    
    // Expected format: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    // where x is a hexadecimal character (0-9, a-f)
    std::regex uuidRegex("^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{8}$");
    
    EXPECT_TRUE(std::regex_match(uuid, uuidRegex)) 
        << "UUID does not match expected format: " << uuid;
    
    // Check length (32 hex chars + 4 dashes = 36)
    EXPECT_EQ(uuid.length(), 36);
    
    // Check dash positions
    EXPECT_EQ(uuid[8], '-');
    EXPECT_EQ(uuid[13], '-');
    EXPECT_EQ(uuid[18], '-');
    EXPECT_EQ(uuid[23], '-');
}

// Test that generated UUIDs are unique
TEST_F(UuidComponentTest, UuidUniqueness) {
    std::set<std::string> uuids;
    const int numUuids = 10000;
    
    for (int i = 0; i < numUuids; ++i) {
        std::string uuid = genUuid();
        auto [iter, inserted] = uuids.insert(uuid);
        EXPECT_TRUE(inserted) << "Duplicate UUID generated: " << uuid;
    }
    
    EXPECT_EQ(uuids.size(), numUuids);
}

// Test default construction
TEST_F(UuidComponentTest, DefaultConstruction) {
    UuidComponent comp;
    
    // Should have a non-empty UUID
    EXPECT_FALSE(comp.uuid.empty());
    EXPECT_EQ(comp.uuid.length(), 36);
    
    // Should match UUID format
    std::regex uuidRegex("^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{8}$");
    EXPECT_TRUE(std::regex_match(comp.uuid, uuidRegex));
}

// Test that each component gets a unique UUID
TEST_F(UuidComponentTest, ComponentUniqueness) {
    std::vector<UuidComponent> components;
    const int numComponents = 1000;
    
    // Create many components
    for (int i = 0; i < numComponents; ++i) {
        components.emplace_back();
    }
    
    // Check all UUIDs are unique
    std::set<std::string> uniqueUuids;
    for (const auto& comp : components) {
        uniqueUuids.insert(comp.uuid);
    }
    
    EXPECT_EQ(uniqueUuids.size(), numComponents);
}

// Test memento save
TEST_F(UuidComponentTest, SaveMemento) {
    // Get the original UUID
    std::string originalUuid = uuidComp.uuid;
    
    // Save memento
    auto memento = uuidComp.save();
    
    // Verify memento contains the UUID
    EXPECT_EQ(memento.uuid, originalUuid);
}

// Test memento restore
TEST_F(UuidComponentTest, RestoreMemento) {
    // Create a specific memento
    UuidComponent::Memento memento{"12345678-1234-5678-1234-567812345678"};
    
    // Restore from memento
    uuidComp.restore(memento);
    
    // Verify UUID was restored
    EXPECT_EQ(uuidComp.uuid, "12345678-1234-5678-1234-567812345678");
}

// Test save and restore round trip
TEST_F(UuidComponentTest, SaveRestoreRoundTrip) {
    // Get original UUID
    std::string originalUuid = uuidComp.uuid;
    
    // Save state
    auto memento = uuidComp.save();
    
    // Change UUID
    uuidComp.uuid = "00000000-0000-0000-0000-000000000000";
    EXPECT_NE(uuidComp.uuid, originalUuid);
    
    // Restore state
    uuidComp.restore(memento);
    
    // Verify original UUID is restored
    EXPECT_EQ(uuidComp.uuid, originalUuid);
}

// Test copy construction
TEST_F(UuidComponentTest, CopyConstruction) {
    UuidComponent original;
    std::string originalUuid = original.uuid;
    
    UuidComponent copy(original);
    
    // Copy should have the same UUID
    EXPECT_EQ(copy.uuid, originalUuid);
}

// Test copy assignment
TEST_F(UuidComponentTest, CopyAssignment) {
    UuidComponent first;
    UuidComponent second;
    
    std::string firstUuid = first.uuid;
    std::string secondUuid = second.uuid;
    
    // UUIDs should be different initially
    EXPECT_NE(firstUuid, secondUuid);
    
    // Copy assignment
    second = first;
    
    // Now they should have the same UUID
    EXPECT_EQ(second.uuid, firstUuid);
}

// Test move construction
TEST_F(UuidComponentTest, MoveConstruction) {
    UuidComponent original;
    std::string originalUuid = original.uuid;
    
    UuidComponent moved(std::move(original));
    
    // Moved component should have the original UUID
    EXPECT_EQ(moved.uuid, originalUuid);
}

// Test move assignment
TEST_F(UuidComponentTest, MoveAssignment) {
    UuidComponent first;
    UuidComponent second;
    
    std::string firstUuid = first.uuid;
    
    second = std::move(first);
    
    // Second should now have first's UUID
    EXPECT_EQ(second.uuid, firstUuid);
}

// Test manual UUID assignment
TEST_F(UuidComponentTest, ManualUuidAssignment) {
    // Assign a custom UUID
    uuidComp.uuid = "custom-uuid-1234-5678-9abc-def012345678";
    
    EXPECT_EQ(uuidComp.uuid, "custom-uuid-1234-5678-9abc-def012345678");
    
    // Save and restore should preserve custom UUID
    auto memento = uuidComp.save();
    UuidComponent newComp;
    newComp.restore(memento);
    
    EXPECT_EQ(newComp.uuid, "custom-uuid-1234-5678-9abc-def012345678");
}

// Test empty UUID handling
TEST_F(UuidComponentTest, EmptyUuid) {
    // Set empty UUID
    uuidComp.uuid = "";
    
    EXPECT_TRUE(uuidComp.uuid.empty());
    
    // Can still save/restore empty UUID
    auto memento = uuidComp.save();
    EXPECT_TRUE(memento.uuid.empty());
    
    UuidComponent newComp;
    newComp.restore(memento);
    EXPECT_TRUE(newComp.uuid.empty());
}

// Test invalid UUID formats (component should accept any string)
TEST_F(UuidComponentTest, InvalidUuidFormats) {
    std::vector<std::string> invalidUuids = {
        "not-a-uuid",
        "12345",
        "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
        "UPPERCASE-UUID-1234-5678-9ABC-DEF012345678",
        "12345678-1234-5678-1234-56781234567", // One char short
        "12345678-1234-5678-1234-5678123456789", // One char long
        "12345678_1234_5678_1234_567812345678", // Underscores instead of dashes
        "g2345678-1234-5678-1234-567812345678", // Invalid hex char 'g'
    };
    
    // Component should accept any string as UUID
    for (const auto& invalidUuid : invalidUuids) {
        uuidComp.uuid = invalidUuid;
        EXPECT_EQ(uuidComp.uuid, invalidUuid);
    }
}

// Test UUID generation performance
TEST_F(UuidComponentTest, GenerationPerformance) {
    const int iterations = 10000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        genUuid();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should be able to generate 10000 UUIDs in reasonable time (< 1 second)
    EXPECT_LT(duration.count(), 1000) 
        << "UUID generation too slow: " << duration.count() << "ms for " 
        << iterations << " UUIDs";
}

// Test thread safety of UUID generation
TEST_F(UuidComponentTest, ThreadSafetyGeneration) {
    const int numThreads = 10;
    const int uuidsPerThread = 1000;
    std::vector<std::thread> threads;
    std::vector<std::vector<std::string>> threadUuids(numThreads);
    
    // Generate UUIDs in multiple threads
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([&threadUuids, i, uuidsPerThread]() {
            for (int j = 0; j < uuidsPerThread; ++j) {
                threadUuids[i].push_back(genUuid());
            }
        });
    }
    
    // Wait for all threads
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Collect all UUIDs and check for uniqueness
    std::set<std::string> allUuids;
    for (const auto& vec : threadUuids) {
        for (const auto& uuid : vec) {
            auto [iter, inserted] = allUuids.insert(uuid);
            EXPECT_TRUE(inserted) << "Duplicate UUID in multi-threaded generation: " << uuid;
        }
    }
    
    EXPECT_EQ(allUuids.size(), numThreads * uuidsPerThread);
}

// Test hexadecimal character distribution
TEST_F(UuidComponentTest, HexCharacterDistribution) {
    std::map<char, int> charCount;
    const int numUuids = 10000;
    
    // Initialize counts
    for (char c : "0123456789abcdef") {
        if (c != '\0') charCount[c] = 0;
    }
    
    // Generate UUIDs and count characters
    for (int i = 0; i < numUuids; ++i) {
        std::string uuid = genUuid();
        for (char c : uuid) {
            if (c != '-') {
                charCount[c]++;
            }
        }
    }
    
    // Each UUID has 32 hex characters
    int totalChars = numUuids * 32;
    int expectedPerChar = totalChars / 16; // 16 possible hex chars
    
    // Check distribution (allow 20% deviation)
    for (const auto& [c, count] : charCount) {
        double deviation = std::abs(count - expectedPerChar) / static_cast<double>(expectedPerChar);
        EXPECT_LT(deviation, 0.2) 
            << "Character '" << c << "' has skewed distribution: " 
            << count << " occurrences (expected ~" << expectedPerChar << ")";
    }
}

// Test aggregate initialization
TEST_F(UuidComponentTest, AggregateInitialization) {
    // Default aggregate initialization
    UuidComponent comp1{};
    EXPECT_FALSE(comp1.uuid.empty());
    
    // Cannot use aggregate initialization with specific UUID due to default member initializer
    // But we can test that the default-generated UUID works
    std::regex uuidRegex("^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{8}$");
    EXPECT_TRUE(std::regex_match(comp1.uuid, uuidRegex));
}

// Test comparison of components
TEST_F(UuidComponentTest, ComponentComparison) {
    UuidComponent comp1;
    UuidComponent comp2;
    UuidComponent comp3;
    
    // Each should have a different UUID
    EXPECT_NE(comp1.uuid, comp2.uuid);
    EXPECT_NE(comp1.uuid, comp3.uuid);
    EXPECT_NE(comp2.uuid, comp3.uuid);
    
    // Make two components have the same UUID
    comp2.uuid = comp1.uuid;
    EXPECT_EQ(comp1.uuid, comp2.uuid);
}

// Test special UUID values
TEST_F(UuidComponentTest, SpecialUuidValues) {
    // Nil UUID (all zeros)
    uuidComp.uuid = "00000000-0000-0000-0000-000000000000";
    EXPECT_EQ(uuidComp.uuid, "00000000-0000-0000-0000-000000000000");
    
    // Max UUID (all f's)
    uuidComp.uuid = "ffffffff-ffff-ffff-ffff-ffffffffffff";
    EXPECT_EQ(uuidComp.uuid, "ffffffff-ffff-ffff-ffff-ffffffffffff");
}

} // namespace nexo::components