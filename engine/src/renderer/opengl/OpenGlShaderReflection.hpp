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
#include <unordered_map>
#include <vector>
#include "Shader.hpp"

namespace nexo::renderer {

    class ShaderReflection {
       public:
        /**
         * \brief Reflects all active uniforms in the given OpenGL shader program.
         * \param programId The OpenGL shader program ID.
         * \return A map from uniform name to UniformInfo structure.
         */
        static std::unordered_map<std::string, UniformInfo> reflectUniforms(unsigned int programId);

        /**
         * \brief Reflects all active attributes in the given OpenGL shader program.
         * \param programId The OpenGL shader program ID.
         * \return A map from attribute location to AttributeInfo structure.
         */
        static std::unordered_map<int, AttributeInfo> reflectAttributes(unsigned int programId);

        /**
         * \brief Gets the list of required attribute names for the given OpenGL shader program.
         * \param programId The OpenGL shader program ID.
         * \return A vector of required attribute names.
         */
        static std::vector<std::string> getRequiredAttributes(unsigned int programId);
    };

} // namespace nexo::renderer
