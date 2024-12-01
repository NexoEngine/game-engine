//// Matrix.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/11/2024
//  Description: Header file for the matrixes utilitary functions
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>

namespace nexo::math {
    void decomposeTransformEuler(const glm::mat4 &mat, glm::vec3 &outTranslation, glm::vec3 &outRotation, glm::vec3 &outScale);
}