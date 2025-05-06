//// OpenGlShaderReflection.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/05/2025
//  Description: Header file for the opengl shader reflection
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include "Shader.hpp"

namespace nexo::renderer {

    class ShaderReflection {
    public:
        static std::unordered_map<std::string, UniformInfo> reflectUniforms(unsigned int programId);
        static std::unordered_map<int, AttributeInfo> reflectAttributes(unsigned int programId);
        static std::vector<std::string> getRequiredAttributes(unsigned int programId);
    };

}
