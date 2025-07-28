//// Vector.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/03/2025
//  Description: Source file for the vectors utilities functions
//
///////////////////////////////////////////////////////////////////////////////

#include "Vector.hpp"

namespace nexo::math {
	void extractCameraComponents(const glm::vec3 &rotation, glm::vec3 &front, glm::vec3 &right, glm::vec3 &up)
	{
		const float pitch = rotation.x - 180.0f;
		const float yaw = rotation.y + 90.0f;

		front.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
		front.y = sinf(glm::radians(pitch));
		front.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
		front = glm::normalize(front);

		right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
		up = glm::normalize(glm::cross(right, front));
	}

	glm::vec3 customQuatToEuler(const glm::quat &q)
	{
        glm::vec3 euler;

        if (float sinp = 2.0f * (q.w * q.y - q.z * q.x); std::abs(sinp) >= 1.0f)
            euler.x = std::copysign(glm::half_pi<float>(), sinp);
        else
            euler.x = std::asin(sinp);

        euler.y = std::atan2(2.0f * (q.w * q.z + q.x * q.y),
                             1.0f - 2.0f * (q.y * q.y + q.z * q.z));

        euler.z = std::atan2(2.0f * (q.w * q.x + q.y * q.z),
                             1.0f - 2.0f * (q.x * q.x + q.y * q.y));

        return glm::degrees(euler);
   	}

    bool isPosInBounds(const glm::vec2 pos, const glm::vec2 &min, const glm::vec2 &max)
    {
        return pos.x >= min.x && pos.x <= max.x && pos.y >= min.y && pos.y <= max.y;
    }
}
