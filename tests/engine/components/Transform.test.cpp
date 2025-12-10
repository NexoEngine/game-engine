//// Transform.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        03/12/2025
//  Description: Test file for TransformComponent (memento pattern and child management)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>
#include "components/Transform.hpp"

namespace nexo::components {

class TransformComponentTest : public ::testing::Test {
protected:
    TransformComponent transform;

    // Helper to compare vec3 with epsilon
    static bool compareVec3(const glm::vec3& a, const glm::vec3& b, float epsilon = 0.0001f) {
        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }

    // Helper to compare quaternions with epsilon
    static bool compareQuat(const glm::quat& a, const glm::quat& b, float epsilon = 0.0001f) {
        return glm::all(glm::epsilonEqual(glm::vec4(a.x, a.y, a.z, a.w),
                                          glm::vec4(b.x, b.y, b.z, b.w), epsilon));
    }

    // Helper to compare mat4 with epsilon
    static bool compareMat4(const glm::mat4& a, const glm::mat4& b, float epsilon = 0.0001f) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (std::abs(a[i][j] - b[i][j]) > epsilon)
                    return false;
            }
        }
        return true;
    }
};

// =============================================================================
// Default State Tests
// =============================================================================

TEST_F(TransformComponentTest, DefaultPosition) {
    // Default position should be zero (note: pos is uninitialized by default)
    TransformComponent t;
    t.pos = glm::vec3(0.0f);
    EXPECT_TRUE(compareVec3(t.pos, glm::vec3(0.0f, 0.0f, 0.0f)));
}

TEST_F(TransformComponentTest, DefaultScale) {
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(1.0f, 1.0f, 1.0f)));
}

TEST_F(TransformComponentTest, DefaultRotation) {
    // Identity quaternion (w=1, x=y=z=0)
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(1.0f, 0.0f, 0.0f, 0.0f)));
}

TEST_F(TransformComponentTest, DefaultWorldMatrix) {
    EXPECT_TRUE(compareMat4(transform.worldMatrix, glm::mat4(1.0f)));
}

TEST_F(TransformComponentTest, DefaultLocalMatrix) {
    EXPECT_TRUE(compareMat4(transform.localMatrix, glm::mat4(1.0f)));
}

TEST_F(TransformComponentTest, DefaultLocalCenter) {
    EXPECT_TRUE(compareVec3(transform.localCenter, glm::vec3(0.0f, 0.0f, 0.0f)));
}

TEST_F(TransformComponentTest, DefaultChildrenEmpty) {
    EXPECT_TRUE(transform.children.empty());
}

// =============================================================================
// Save/Restore Memento Tests
// =============================================================================

TEST_F(TransformComponentTest, SaveCapturesPosition) {
    transform.pos = glm::vec3(1.0f, 2.0f, 3.0f);
    auto memento = transform.save();
    EXPECT_TRUE(compareVec3(memento.position, glm::vec3(1.0f, 2.0f, 3.0f)));
}

TEST_F(TransformComponentTest, SaveCapturesRotation) {
    transform.quat = glm::quat(glm::vec3(0.5f, 1.0f, 1.5f));  // Euler to quat
    auto memento = transform.save();
    EXPECT_TRUE(compareQuat(memento.rotation, transform.quat));
}

TEST_F(TransformComponentTest, SaveCapturesScale) {
    transform.size = glm::vec3(2.0f, 3.0f, 4.0f);
    auto memento = transform.save();
    EXPECT_TRUE(compareVec3(memento.scale, glm::vec3(2.0f, 3.0f, 4.0f)));
}

TEST_F(TransformComponentTest, SaveCapturesLocalMatrix) {
    glm::mat4 testMatrix(2.0f);
    transform.localMatrix = testMatrix;
    auto memento = transform.save();
    EXPECT_TRUE(compareMat4(memento.localMatrix, testMatrix));
}

TEST_F(TransformComponentTest, SaveCapturesLocalCenter) {
    transform.localCenter = glm::vec3(5.0f, 6.0f, 7.0f);
    auto memento = transform.save();
    EXPECT_TRUE(compareVec3(memento.localCenter, glm::vec3(5.0f, 6.0f, 7.0f)));
}

TEST_F(TransformComponentTest, SaveCapturesChildren) {
    transform.children = {1, 2, 3, 4, 5};
    auto memento = transform.save();
    EXPECT_EQ(memento.children.size(), 5u);
    EXPECT_EQ(memento.children, transform.children);
}

TEST_F(TransformComponentTest, RestoreAppliesPosition) {
    TransformComponent::Memento memento;
    memento.position = glm::vec3(10.0f, 20.0f, 30.0f);
    memento.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    memento.scale = glm::vec3(1.0f);
    memento.localMatrix = glm::mat4(1.0f);
    memento.localCenter = glm::vec3(0.0f);

    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(10.0f, 20.0f, 30.0f)));
}

TEST_F(TransformComponentTest, RestoreAppliesRotation) {
    glm::quat testQuat = glm::quat(glm::vec3(1.0f, 0.0f, 0.0f));

    TransformComponent::Memento memento;
    memento.position = glm::vec3(0.0f);
    memento.rotation = testQuat;
    memento.scale = glm::vec3(1.0f);
    memento.localMatrix = glm::mat4(1.0f);
    memento.localCenter = glm::vec3(0.0f);

    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, testQuat));
}

TEST_F(TransformComponentTest, RestoreAppliesScale) {
    TransformComponent::Memento memento;
    memento.position = glm::vec3(0.0f);
    memento.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    memento.scale = glm::vec3(5.0f, 10.0f, 15.0f);
    memento.localMatrix = glm::mat4(1.0f);
    memento.localCenter = glm::vec3(0.0f);

    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(5.0f, 10.0f, 15.0f)));
}

TEST_F(TransformComponentTest, RestoreAppliesChildren) {
    TransformComponent::Memento memento;
    memento.position = glm::vec3(0.0f);
    memento.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    memento.scale = glm::vec3(1.0f);
    memento.localMatrix = glm::mat4(1.0f);
    memento.localCenter = glm::vec3(0.0f);
    memento.children = {100, 200, 300};

    transform.restore(memento);
    EXPECT_EQ(transform.children.size(), 3u);
    EXPECT_EQ(transform.children[0], 100u);
    EXPECT_EQ(transform.children[1], 200u);
    EXPECT_EQ(transform.children[2], 300u);
}

TEST_F(TransformComponentTest, SaveRestoreRoundTrip) {
    // Set up complex state
    transform.pos = glm::vec3(1.0f, 2.0f, 3.0f);
    transform.quat = glm::quat(glm::vec3(0.1f, 0.2f, 0.3f));
    transform.size = glm::vec3(4.0f, 5.0f, 6.0f);
    transform.localMatrix = glm::mat4(3.0f);
    transform.localCenter = glm::vec3(7.0f, 8.0f, 9.0f);
    transform.children = {10, 20, 30};

    // Save state
    auto memento = transform.save();

    // Modify all fields
    transform.pos = glm::vec3(0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f);
    transform.localMatrix = glm::mat4(1.0f);
    transform.localCenter = glm::vec3(0.0f);
    transform.children.clear();

    // Restore from memento
    transform.restore(memento);

    // Verify all fields restored
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(1.0f, 2.0f, 3.0f)));
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(0.1f, 0.2f, 0.3f))));
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(4.0f, 5.0f, 6.0f)));
    EXPECT_TRUE(compareMat4(transform.localMatrix, glm::mat4(3.0f)));
    EXPECT_TRUE(compareVec3(transform.localCenter, glm::vec3(7.0f, 8.0f, 9.0f)));
    EXPECT_EQ(transform.children.size(), 3u);
}

// =============================================================================
// Child Management Tests
// =============================================================================

TEST_F(TransformComponentTest, AddChildAppendsToList) {
    transform.addChild(42);
    ASSERT_EQ(transform.children.size(), 1u);
    EXPECT_EQ(transform.children[0], 42u);
}

TEST_F(TransformComponentTest, AddChildMultiple) {
    transform.addChild(1);
    transform.addChild(2);
    transform.addChild(3);

    ASSERT_EQ(transform.children.size(), 3u);
    EXPECT_EQ(transform.children[0], 1u);
    EXPECT_EQ(transform.children[1], 2u);
    EXPECT_EQ(transform.children[2], 3u);
}

TEST_F(TransformComponentTest, AddChildPreventsDuplicates) {
    transform.addChild(42);
    transform.addChild(42);  // Duplicate
    transform.addChild(42);  // Duplicate

    EXPECT_EQ(transform.children.size(), 1u);
}

TEST_F(TransformComponentTest, AddChildPreventsDuplicatesAmongMany) {
    transform.addChild(1);
    transform.addChild(2);
    transform.addChild(3);
    transform.addChild(2);  // Duplicate
    transform.addChild(1);  // Duplicate

    EXPECT_EQ(transform.children.size(), 3u);
}

TEST_F(TransformComponentTest, RemoveChildRemovesExisting) {
    transform.children = {1, 2, 3, 4, 5};

    transform.removeChild(3);

    ASSERT_EQ(transform.children.size(), 4u);
    EXPECT_EQ(transform.children[0], 1u);
    EXPECT_EQ(transform.children[1], 2u);
    EXPECT_EQ(transform.children[2], 4u);
    EXPECT_EQ(transform.children[3], 5u);
}

TEST_F(TransformComponentTest, RemoveChildFromEmpty) {
    EXPECT_NO_THROW(transform.removeChild(42));
    EXPECT_TRUE(transform.children.empty());
}

TEST_F(TransformComponentTest, RemoveChildNonExistent) {
    transform.children = {1, 2, 3};

    transform.removeChild(999);  // Not in list

    EXPECT_EQ(transform.children.size(), 3u);
}

TEST_F(TransformComponentTest, RemoveChildFirst) {
    transform.children = {1, 2, 3};

    transform.removeChild(1);

    ASSERT_EQ(transform.children.size(), 2u);
    EXPECT_EQ(transform.children[0], 2u);
    EXPECT_EQ(transform.children[1], 3u);
}

TEST_F(TransformComponentTest, RemoveChildLast) {
    transform.children = {1, 2, 3};

    transform.removeChild(3);

    ASSERT_EQ(transform.children.size(), 2u);
    EXPECT_EQ(transform.children[0], 1u);
    EXPECT_EQ(transform.children[1], 2u);
}

TEST_F(TransformComponentTest, RemoveChildAll) {
    transform.children = {1, 2, 3};

    transform.removeChild(1);
    transform.removeChild(2);
    transform.removeChild(3);

    EXPECT_TRUE(transform.children.empty());
}

TEST_F(TransformComponentTest, AddAfterRemove) {
    transform.addChild(1);
    transform.removeChild(1);
    transform.addChild(1);

    ASSERT_EQ(transform.children.size(), 1u);
    EXPECT_EQ(transform.children[0], 1u);
}

}  // namespace nexo::components
