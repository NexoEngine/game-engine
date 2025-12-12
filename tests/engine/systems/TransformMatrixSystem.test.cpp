//// TransformMatrixSystem.test.cpp /////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for TransformMatrixSystem (createTransformMatrix function)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "systems/TransformMatrixSystem.hpp"
#include "components/Transform.hpp"

namespace nexo::system {

class TransformMatrixSystemTest : public ::testing::Test {
protected:
    components::TransformComponent transform;

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

    // Helper to compare vec3 with epsilon
    static bool compareVec3(const glm::vec3& a, const glm::vec3& b, float epsilon = 0.0001f) {
        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }

    // Helper to extract translation from matrix
    static glm::vec3 getTranslation(const glm::mat4& mat) {
        return glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
    }

    // Helper to extract scale from matrix (approximation for uniform scale)
    static glm::vec3 getScale(const glm::mat4& mat) {
        glm::vec3 scale;
        scale.x = glm::length(glm::vec3(mat[0][0], mat[0][1], mat[0][2]));
        scale.y = glm::length(glm::vec3(mat[1][0], mat[1][1], mat[1][2]));
        scale.z = glm::length(glm::vec3(mat[2][0], mat[2][1], mat[2][2]));
        return scale;
    }
};

// =============================================================================
// Identity Transform Tests
// =============================================================================

TEST_F(TransformMatrixSystemTest, IdentityTransform) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);  // Identity quaternion
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);

    EXPECT_TRUE(compareMat4(result, glm::mat4(1.0f)));
}

TEST_F(TransformMatrixSystemTest, DefaultTransformComponent) {
    // TransformComponent defaults: pos=0, quat=identity, size=1
    transform.pos = glm::vec3(0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);

    EXPECT_TRUE(compareMat4(result, glm::mat4(1.0f)));
}

// =============================================================================
// Translation Only Tests
// =============================================================================

TEST_F(TransformMatrixSystemTest, TranslationPositiveValues) {
    transform.pos = glm::vec3(5.0f, 10.0f, 15.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 10.0f, 15.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, TranslationNegativeValues) {
    transform.pos = glm::vec3(-5.0f, -10.0f, -15.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, -10.0f, -15.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, TranslationMixedValues) {
    transform.pos = glm::vec3(-5.0f, 10.0f, -15.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 10.0f, -15.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, TranslationLargeValues) {
    transform.pos = glm::vec3(1000.0f, 2000.0f, 3000.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);

    glm::vec3 resultPos = getTranslation(result);
    EXPECT_TRUE(compareVec3(resultPos, glm::vec3(1000.0f, 2000.0f, 3000.0f)));
}

// =============================================================================
// Rotation Only Tests
// =============================================================================

TEST_F(TransformMatrixSystemTest, RotationAroundXAxis90Degrees) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, RotationAroundYAxis90Degrees) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, RotationAroundZAxis90Degrees) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, RotationAroundXAxis180Degrees) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, Rotation360Degrees) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::angleAxis(glm::radians(360.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);

    // 360 degrees should be equivalent to identity
    EXPECT_TRUE(compareMat4(result, glm::mat4(1.0f)));
}

TEST_F(TransformMatrixSystemTest, RotationArbitraryAngle) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, RotationArbitraryAxis) {
    glm::vec3 axis = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::angleAxis(glm::radians(60.0f), axis);
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::rotate(glm::mat4(1.0f), glm::radians(60.0f), axis);

    EXPECT_TRUE(compareMat4(result, expected));
}

// =============================================================================
// Scale Only Tests
// =============================================================================

TEST_F(TransformMatrixSystemTest, ScaleUniformPositive) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(2.0f, 2.0f, 2.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, ScaleNonUniform) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(2.0f, 3.0f, 4.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 4.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, ScaleSmallValues) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(0.1f, 0.2f, 0.3f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.2f, 0.3f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, ScaleLargeValues) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(100.0f, 200.0f, 300.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);

    glm::vec3 resultScale = getScale(result);
    EXPECT_TRUE(compareVec3(resultScale, glm::vec3(100.0f, 200.0f, 300.0f)));
}

TEST_F(TransformMatrixSystemTest, ScaleNegativeX) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(-1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 1.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, ScaleNegativeY) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f, -1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, ScaleNegativeZ) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f, 1.0f, -1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -1.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, ScaleAllNegative) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(-2.0f, -3.0f, -4.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::scale(glm::mat4(1.0f), glm::vec3(-2.0f, -3.0f, -4.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, ScaleZeroValue) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(0.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::scale(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 1.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

// =============================================================================
// Combined Transformation Tests
// =============================================================================

TEST_F(TransformMatrixSystemTest, TranslationAndScale) {
    transform.pos = glm::vec3(10.0f, 20.0f, 30.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(2.0f, 3.0f, 4.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 20.0f, 30.0f)) *
                         glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 4.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, TranslationAndRotation) {
    transform.pos = glm::vec3(5.0f, 10.0f, 15.0f);
    transform.quat = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 10.0f, 15.0f)) *
                         glm::toMat4(transform.quat);

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, RotationAndScale) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    transform.size = glm::vec3(2.0f, 3.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::toMat4(transform.quat) *
                         glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 1.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, FullTRSTransform) {
    transform.pos = glm::vec3(10.0f, 20.0f, 30.0f);
    transform.quat = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    transform.size = glm::vec3(2.0f, 3.0f, 4.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 20.0f, 30.0f)) *
                         glm::toMat4(transform.quat) *
                         glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 4.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, ComplexTransform) {
    transform.pos = glm::vec3(-15.5f, 22.3f, -8.7f);
    transform.quat = glm::angleAxis(glm::radians(127.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
    transform.size = glm::vec3(0.5f, 1.5f, 2.5f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::translate(glm::mat4(1.0f), transform.pos) *
                         glm::toMat4(transform.quat) *
                         glm::scale(glm::mat4(1.0f), transform.size);

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, NegativeScaleWithRotation) {
    transform.pos = glm::vec3(5.0f, 5.0f, 5.0f);
    transform.quat = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transform.size = glm::vec3(-1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 5.0f)) *
                         glm::toMat4(transform.quat) *
                         glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 1.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, MultipleRotationsComposed) {
    // Compose multiple rotations using quaternion multiplication
    glm::quat rotX = glm::angleAxis(glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat rotY = glm::angleAxis(glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rotZ = glm::angleAxis(glm::radians(60.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = rotZ * rotY * rotX;  // Order matters in quaternion multiplication
    transform.size = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::toMat4(transform.quat);

    EXPECT_TRUE(compareMat4(result, expected));
}

// =============================================================================
// Edge Cases and Special Values
// =============================================================================

TEST_F(TransformMatrixSystemTest, VerySmallScale) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(0.001f, 0.001f, 0.001f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::scale(glm::mat4(1.0f), glm::vec3(0.001f, 0.001f, 0.001f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, VeryLargeScale) {
    transform.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1000.0f, 1000.0f, 1000.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);

    glm::vec3 resultScale = getScale(result);
    EXPECT_TRUE(compareVec3(resultScale, glm::vec3(1000.0f, 1000.0f, 1000.0f)));
}

TEST_F(TransformMatrixSystemTest, NegativeTranslationNegativeScale) {
    transform.pos = glm::vec3(-10.0f, -20.0f, -30.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(-2.0f, -3.0f, -4.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);
    glm::mat4 expected = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -20.0f, -30.0f)) *
                         glm::scale(glm::mat4(1.0f), glm::vec3(-2.0f, -3.0f, -4.0f));

    EXPECT_TRUE(compareMat4(result, expected));
}

TEST_F(TransformMatrixSystemTest, TransformOrderTRS) {
    // Verify that the transform is applied in the correct order: Translation * Rotation * Scale
    transform.pos = glm::vec3(1.0f, 0.0f, 0.0f);
    transform.quat = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    transform.size = glm::vec3(2.0f, 1.0f, 1.0f);

    glm::mat4 result = TransformMatrixSystem::createTransformMatrix(transform);

    // Apply to a test point to verify order
    glm::vec4 point(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 transformed = result * point;

    // Expected: Scale (2,0,0) -> Rotate -> Translate
    // Scale: (1,0,0) * 2 = (2,0,0)
    // Rotate 90deg around Z: (2,0,0) -> (0,2,0)
    // Translate by (1,0,0): (0,2,0) + (1,0,0) = (1,2,0)
    EXPECT_NEAR(transformed.x, 1.0f, 0.0001f);
    EXPECT_NEAR(transformed.y, 2.0f, 0.0001f);
    EXPECT_NEAR(transformed.z, 0.0f, 0.0001f);
}

}  // namespace nexo::system
