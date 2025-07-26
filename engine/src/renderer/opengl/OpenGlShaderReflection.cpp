//// OpenGlShaderReflection.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/05/2025
//  Description: Source file for the opengl shader reflection
//
///////////////////////////////////////////////////////////////////////////////

#include "OpenGlShaderReflection.hpp"
#include "Logger.hpp"
#include <glad/glad.h>

namespace nexo::renderer {

    std::unordered_map<std::string, UniformInfo> ShaderReflection::reflectUniforms(unsigned int programId)
    {
        std::unordered_map<std::string, UniformInfo> uniforms;

        GLint linkStatus;
        glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            LOG(NEXO_ERROR, "Cannot reflect uniforms from unlinked program {}", programId);
            return uniforms;
        }

        int uniformCount;
        glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &uniformCount);

        int maxNameLength;
        glGetProgramiv(programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

        std::vector<char> nameBuffer(maxNameLength);

        for (int i = 0; i < uniformCount; i++) {
            UniformInfo info;
            int nameLength;
            glGetActiveUniform(programId, i, maxNameLength, &nameLength, &info.size, &info.type, nameBuffer.data());

            info.name = std::string(nameBuffer.data(), nameLength);
            info.location = glGetUniformLocation(programId, info.name.c_str());

            // Store the uniform with original name
            uniforms[info.name] = info;

            // For array uniforms, also store the base name (without [0])
            std::string baseName = info.name;
            const size_t bracketPos = baseName.find('[');
            if (bracketPos != std::string::npos) {
                baseName = baseName.substr(0, bracketPos);

                // Only add if we haven't encountered this base name yet
                if (!uniforms.contains(baseName)) {
                    // For arrays, the base name location is the same as the first element
                    UniformInfo baseInfo = info;
                    baseInfo.name = baseName;
                    uniforms[baseName] = baseInfo;
                }
            }
        }

        return uniforms;
    }

    std::unordered_map<int, AttributeInfo> ShaderReflection::reflectAttributes(const unsigned int programId)
    {
        std::unordered_map<int, AttributeInfo> attributes;

        int attributeCount;
        glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &attributeCount);

        int maxNameLength;
        glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);

        std::vector<char> nameBuffer(maxNameLength);

        for (int i = 0; i < attributeCount; i++) {
            AttributeInfo info;
            int nameLength;
            glGetActiveAttrib(programId, i, maxNameLength, &nameLength, &info.size, &info.type, nameBuffer.data());

            info.name = std::string(nameBuffer.data(), nameLength);
            info.location = glGetAttribLocation(programId, info.name.c_str());

            attributes[info.location] = info;
        }

        return attributes;
    }

    std::vector<std::string> ShaderReflection::getRequiredAttributes(const unsigned int programId)
    {
        auto attributes = reflectAttributes(programId);
        std::vector<std::string> required;

        for (const auto& [location, info] : attributes) {
            required.push_back(info.name);
        }

        return required;
    }

}
