///////////////////////////////////////////////////////////////////////////////
//
//  Math function stubs for testing
//  These are minimal implementations to resolve linking issues
//
///////////////////////////////////////////////////////////////////////////////

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace nexo::math {

// Stub implementation for missing math function
void decomposeTransformQuat(const glm::mat4& transform, 
                           glm::vec3& translation, 
                           glm::quat& rotation, 
                           glm::vec3& scale) {
    // Simple decomposition - this is a stub for testing purposes
    translation = glm::vec3(transform[3]);
    scale = glm::vec3(
        glm::length(glm::vec3(transform[0])),
        glm::length(glm::vec3(transform[1])),
        glm::length(glm::vec3(transform[2]))
    );
    
    // Extract rotation matrix by normalizing columns
    glm::mat3 rotMatrix;
    rotMatrix[0] = glm::vec3(transform[0]) / scale.x;
    rotMatrix[1] = glm::vec3(transform[1]) / scale.y;
    rotMatrix[2] = glm::vec3(transform[2]) / scale.z;
    
    rotation = glm::quat_cast(rotMatrix);
}

}