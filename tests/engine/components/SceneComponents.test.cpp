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
//  Description: Test file for the Scene components
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "components/SceneComponents.hpp"
#include <limits>
#include <vector>

namespace nexo::components {

class SceneComponentsTest : public ::testing::Test {
protected:
    void SetUp() override {
        sceneTag = SceneTag{};
    }
    
    SceneTag sceneTag;
};

// SceneTag Tests

TEST_F(SceneComponentsTest, DefaultConstruction) {
    SceneTag tag;
    
    EXPECT_EQ(tag.id, 0);
    EXPECT_TRUE(tag.isActive);
    EXPECT_TRUE(tag.isRendered);
}

TEST_F(SceneComponentsTest, ValueConstruction) {
    SceneTag tag{42, false, true};
    
    EXPECT_EQ(tag.id, 42);
    EXPECT_FALSE(tag.isActive);
    EXPECT_TRUE(tag.isRendered);
}

TEST_F(SceneComponentsTest, SceneIdAssignment) {
    sceneTag.id = 123;
    EXPECT_EQ(sceneTag.id, 123);
    
    sceneTag.id = 0;
    EXPECT_EQ(sceneTag.id, 0);
    
    sceneTag.id = std::numeric_limits<unsigned int>::max();
    EXPECT_EQ(sceneTag.id, std::numeric_limits<unsigned int>::max());
}

TEST_F(SceneComponentsTest, ActiveFlagToggle) {
    EXPECT_TRUE(sceneTag.isActive);
    
    sceneTag.isActive = false;
    EXPECT_FALSE(sceneTag.isActive);
    
    sceneTag.isActive = true;
    EXPECT_TRUE(sceneTag.isActive);
}

TEST_F(SceneComponentsTest, RenderedFlagToggle) {
    EXPECT_TRUE(sceneTag.isRendered);
    
    sceneTag.isRendered = false;
    EXPECT_FALSE(sceneTag.isRendered);
    
    sceneTag.isRendered = true;
    EXPECT_TRUE(sceneTag.isRendered);
}

TEST_F(SceneComponentsTest, MementoSave) {
    sceneTag.id = 456;
    sceneTag.isActive = false;
    sceneTag.isRendered = true;
    
    auto memento = sceneTag.save();
    
    EXPECT_EQ(memento.id, 456);
    EXPECT_FALSE(memento.isActive);
    EXPECT_TRUE(memento.isRendered);
}

TEST_F(SceneComponentsTest, MementoRestore) {
    SceneTag::Memento memento{789, true, false};
    
    sceneTag.restore(memento);
    
    EXPECT_EQ(sceneTag.id, 789);
    EXPECT_TRUE(sceneTag.isActive);
    EXPECT_FALSE(sceneTag.isRendered);
}

TEST_F(SceneComponentsTest, MementoRoundTrip) {
    sceneTag.id = 999;
    sceneTag.isActive = false;
    sceneTag.isRendered = false;
    
    auto memento = sceneTag.save();
    
    // Modify values
    sceneTag.id = 0;
    sceneTag.isActive = true;
    sceneTag.isRendered = true;
    
    // Restore
    sceneTag.restore(memento);
    
    EXPECT_EQ(sceneTag.id, 999);
    EXPECT_FALSE(sceneTag.isActive);
    EXPECT_FALSE(sceneTag.isRendered);
}

TEST_F(SceneComponentsTest, CopyConstruction) {
    sceneTag.id = 100;
    sceneTag.isActive = false;
    sceneTag.isRendered = true;
    
    SceneTag copy(sceneTag);
    
    EXPECT_EQ(copy.id, 100);
    EXPECT_FALSE(copy.isActive);
    EXPECT_TRUE(copy.isRendered);
    
    // Verify independence
    copy.id = 200;
    EXPECT_EQ(sceneTag.id, 100);
    EXPECT_EQ(copy.id, 200);
}

TEST_F(SceneComponentsTest, CopyAssignment) {
    SceneTag first{10, true, false};
    SceneTag second{20, false, true};
    
    second = first;
    
    EXPECT_EQ(second.id, 10);
    EXPECT_TRUE(second.isActive);
    EXPECT_FALSE(second.isRendered);
}

TEST_F(SceneComponentsTest, MoveConstruction) {
    sceneTag.id = 300;
    sceneTag.isActive = false;
    sceneTag.isRendered = false;
    
    SceneTag moved(std::move(sceneTag));
    
    EXPECT_EQ(moved.id, 300);
    EXPECT_FALSE(moved.isActive);
    EXPECT_FALSE(moved.isRendered);
}

TEST_F(SceneComponentsTest, MoveAssignment) {
    SceneTag first{30, false, false};
    SceneTag second{40, true, true};
    
    second = std::move(first);
    
    EXPECT_EQ(second.id, 30);
    EXPECT_FALSE(second.isActive);
    EXPECT_FALSE(second.isRendered);
}

TEST_F(SceneComponentsTest, MultipleSceneTags) {
    std::vector<SceneTag> tags;
    
    // Create tags for multiple scenes
    for (unsigned int i = 0; i < 10; ++i) {
        SceneTag tag;
        tag.id = i;
        tag.isActive = (i % 2 == 0);
        tag.isRendered = (i % 3 != 0);
        tags.push_back(tag);
    }
    
    // Verify all tags maintain their values
    for (unsigned int i = 0; i < tags.size(); ++i) {
        EXPECT_EQ(tags[i].id, i);
        EXPECT_EQ(tags[i].isActive, (i % 2 == 0));
        EXPECT_EQ(tags[i].isRendered, (i % 3 != 0));
    }
}

TEST_F(SceneComponentsTest, SceneIdUniqueness) {
    std::vector<SceneTag> tags;
    std::set<unsigned int> uniqueIds;
    
    // Create many scene tags
    for (unsigned int i = 0; i < 1000; ++i) {
        SceneTag tag;
        tag.id = i;
        tags.push_back(tag);
        uniqueIds.insert(i);
    }
    
    // All IDs should be unique
    EXPECT_EQ(uniqueIds.size(), tags.size());
}

TEST_F(SceneComponentsTest, SceneStatesCombinations) {
    // Test all possible state combinations
    struct TestCase {
        bool active;
        bool rendered;
        const char* description;
    };
    
    std::vector<TestCase> testCases = {
        {true, true, "Active and Rendered"},
        {true, false, "Active but Not Rendered"},
        {false, true, "Inactive but Rendered"},
        {false, false, "Inactive and Not Rendered"}
    };
    
    for (const auto& testCase : testCases) {
        SceneTag tag;
        tag.isActive = testCase.active;
        tag.isRendered = testCase.rendered;
        
        EXPECT_EQ(tag.isActive, testCase.active) << testCase.description;
        EXPECT_EQ(tag.isRendered, testCase.rendered) << testCase.description;
        
        // Test memento preserves states
        auto memento = tag.save();
        EXPECT_EQ(memento.isActive, testCase.active) << testCase.description;
        EXPECT_EQ(memento.isRendered, testCase.rendered) << testCase.description;
    }
}

TEST_F(SceneComponentsTest, AggregateInitialization) {
    // Default aggregate initialization
    SceneTag tag1{};
    EXPECT_EQ(tag1.id, 0);
    EXPECT_TRUE(tag1.isActive);
    EXPECT_TRUE(tag1.isRendered);
    
    // Partial initialization
    SceneTag tag2{5};
    EXPECT_EQ(tag2.id, 5);
    EXPECT_TRUE(tag2.isActive);
    EXPECT_TRUE(tag2.isRendered);
    
    // Full initialization
    SceneTag tag3{10, false, false};
    EXPECT_EQ(tag3.id, 10);
    EXPECT_FALSE(tag3.isActive);
    EXPECT_FALSE(tag3.isRendered);
}

TEST_F(SceneComponentsTest, SceneTransitions) {
    // Simulate scene transitions
    sceneTag.id = 0;
    sceneTag.isActive = true;
    sceneTag.isRendered = true;
    
    // Save current scene state
    auto scene0State = sceneTag.save();
    
    // Transition to scene 1
    sceneTag.id = 1;
    sceneTag.isActive = false;
    sceneTag.isRendered = true;
    
    // Save scene 1 state
    auto scene1State = sceneTag.save();
    
    // Go back to scene 0
    sceneTag.restore(scene0State);
    EXPECT_EQ(sceneTag.id, 0);
    EXPECT_TRUE(sceneTag.isActive);
    EXPECT_TRUE(sceneTag.isRendered);
    
    // Go to scene 1 again
    sceneTag.restore(scene1State);
    EXPECT_EQ(sceneTag.id, 1);
    EXPECT_FALSE(sceneTag.isActive);
    EXPECT_TRUE(sceneTag.isRendered);
}

TEST_F(SceneComponentsTest, EdgeCaseSceneIds) {
    std::vector<unsigned int> edgeCaseIds = {
        0,                                          // Minimum
        1,                                          // Common case
        100,                                        // Typical value
        65535,                                      // 16-bit max
        std::numeric_limits<unsigned int>::max() / 2,  // Half max
        std::numeric_limits<unsigned int>::max() - 1,  // Near max
        std::numeric_limits<unsigned int>::max()       // Maximum
    };
    
    for (auto id : edgeCaseIds) {
        sceneTag.id = id;
        EXPECT_EQ(sceneTag.id, id);
        
        // Test memento with edge case IDs
        auto memento = sceneTag.save();
        EXPECT_EQ(memento.id, id);
        
        SceneTag newTag;
        newTag.restore(memento);
        EXPECT_EQ(newTag.id, id);
    }
}

} // namespace nexo::components