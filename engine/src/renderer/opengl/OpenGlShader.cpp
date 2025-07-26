//// OpenGlShader.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for opengl shader class
//
///////////////////////////////////////////////////////////////////////////////

#include "OpenGlShader.hpp"
#include "Exception.hpp"
#include "Logger.hpp"
#include "Shader.hpp"
#include "renderer/RendererExceptions.hpp"
#include "OpenGlShaderReflection.hpp"

#include <array>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

namespace nexo::renderer {

    static GLenum shaderTypeFromString(const std::string_view &type)
    {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment")
            return GL_FRAGMENT_SHADER;

        return 0;
    }

    NxOpenGlShader::NxOpenGlShader(const std::string &path)
    {
        const std::string src = readFile(path);
        const auto shaderSources = preProcess(src, path);
        compile(shaderSources);

        auto lastSlash = path.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        const auto lastDot = path.rfind('.');
        const auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
        m_name = path.substr(lastSlash, count);
        setupUniformLocations();
    }

    NxOpenGlShader::NxOpenGlShader(std::string name, const std::string_view &vertexSource,
                               const std::string_view &fragmentSource) : m_name(std::move(name))
    {
        std::unordered_map<GLenum, std::string> preProcessedSource;
        preProcessedSource[GL_VERTEX_SHADER] = vertexSource;
        preProcessedSource[GL_FRAGMENT_SHADER] = fragmentSource;
        compile(preProcessedSource);
        setupUniformLocations();
    }

    NxOpenGlShader::~NxOpenGlShader()
    {
        glDeleteProgram(m_id);
    }

    std::unordered_map<GLenum, std::string> NxOpenGlShader::preProcess(const std::string_view &src,
                                                                     const std::string &filePath)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char *typeToken = "#type";
        size_t pos = src.find(typeToken, 0);
        unsigned int currentLine = 1;
        while (pos != std::string::npos)
        {
            constexpr size_t typeTokenLength = 5;
            const size_t eol = src.find_first_of("\r\n", pos);
            if (eol == std::string::npos)
                THROW_EXCEPTION(NxShaderCreationFailed, "OPENGL",
                            "Syntax error at line: " + std::to_string(currentLine), filePath);

            const size_t begin = pos + typeTokenLength + 1;
            std::string_view type = src.substr(begin, eol - begin);
            if (!shaderTypeFromString(type))
                THROW_EXCEPTION(NxShaderCreationFailed, "OPENGL",
                            "Invalid shader type encountered at line: " + std::to_string(currentLine), filePath);

            const size_t nextLinePos = src.find_first_not_of("\r\n", eol);
            if (nextLinePos == std::string::npos)
                THROW_EXCEPTION(NxShaderCreationFailed, "OPENGL",
                            "Syntax error at line: " + std::to_string(currentLine), filePath);

            pos = src.find(typeToken, nextLinePos);

            shaderSources[shaderTypeFromString(type)] = (pos == std::string::npos)
                                                            ? src.substr(nextLinePos)
                                                            : src.substr(nextLinePos, pos - nextLinePos);
            currentLine += std::count(src.begin() + nextLinePos,
                                      src.begin() + (pos == std::string::npos ? src.size() : pos), '\n');
        }

        return shaderSources;
    }

    void NxOpenGlShader::compile(const std::unordered_map<GLenum, std::string> &shaderSources)
    {
        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        if (shaderSources.size() > 2)
            THROW_EXCEPTION(NxShaderCreationFailed, "OPENGL",
                        "Only two shader type (vertex/fragment) are supported for now", "");
        const GLuint program = glCreateProgram();
        std::array<GLenum, 2> glShaderIds{};
        unsigned int shaderIndex = 0;
        for (const auto &[fst, snd]: shaderSources)
        {
            const GLenum type = fst;
            const std::string src = snd;
            // Create an empty vertex shader handle
            const GLuint shader = glCreateShader(type);

            // Send the vertex shader source code to GL
            // Note that std::string's .c_str is NULL character terminated.
            const GLchar *source = src.c_str();
            glShaderSource(shader, 1, &source, nullptr);

            // Compile the vertex shader
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                // We don't need the shader anymore.
                glDeleteShader(shader);
                THROW_EXCEPTION(NxShaderCreationFailed, "OPENGL",
                                "Opengl failed to compile the shader: " + std::string(infoLog.data()), "");
            }
            glAttachShader(program, shader);
            glShaderIds[shaderIndex++] = shader;
        }
        m_id = program;

        // Link our program
        glLinkProgram(m_id);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_id, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_id, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_id);
            // Don't leak shaders either.
            for (const auto id: glShaderIds)
                glDeleteShader(id);

            THROW_EXCEPTION(NxShaderCreationFailed, "OPENGL",
                                "Opengl failed to compile the shader: " + std::string(infoLog.data()), "");
        }

        // Always detach shaders after a successful link.
        for (const auto id: glShaderIds)
            glDetachShader(program, id);
    }

    void NxOpenGlShader::setupUniformLocations()
    {
        m_uniformInfos = ShaderReflection::reflectUniforms(m_id);
        m_attributeInfos = ShaderReflection::reflectAttributes(m_id);

        static const std::unordered_map<std::string, std::function<void(RequiredAttributes&)>> attributeMappers = {
            {"aPos", [](RequiredAttributes& attrs) { attrs.bitsUnion.flags.position = true; }},
            {"aNormal", [](RequiredAttributes& attrs) { attrs.bitsUnion.flags.normal = true; }},
            {"aTangent", [](RequiredAttributes& attrs) { attrs.bitsUnion.flags.tangent = true; }},
            {"aBiTangent", [](RequiredAttributes& attrs) { attrs.bitsUnion.flags.bitangent = true; }},
            {"aTexCoord", [](RequiredAttributes& attrs) { attrs.bitsUnion.flags.uv0 = true; }}
        };

        for (const auto& [location, info] : m_attributeInfos) {
            const auto it = attributeMappers.find(info.name);
            if (it != attributeMappers.end()) {
                it->second(m_requiredAttributes);
            }
        }
    }

    void NxOpenGlShader::bind() const
    {
        glUseProgram(m_id);
    }

    void NxOpenGlShader::unbind() const
    {
        glUseProgram(0);
    }

    int NxOpenGlShader::getUniformLocation(const std::string& name) const {
        const auto it = m_uniformInfos.find(name);
        if (it != m_uniformInfos.end()) {
            return it->second.location;
        }
        return glGetUniformLocation(m_id, name.c_str());
    }

    bool NxOpenGlShader::setUniformFloat(const std::string& name, const float value) const
    {
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformFloat(name, value))
            return true; // Value was cached, no need to update

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniform1f(location, value);
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformFloat(const NxShaderUniforms uniform, const float value) const
    {
        const std::string &name = ShaderUniformsName.at(uniform);
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformFloat(name, value))
            return true;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniform1f(location, value);
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformFloat2(const std::string& name, const glm::vec2& values) const
    {
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformFloat2(name, values))
            return true;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniform2f(location, values.x, values.y);
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformFloat3(const std::string& name, const glm::vec3& values) const
    {
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformFloat3(name, values))
            return true;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniform3f(location, values.x, values.y, values.z);
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformFloat3(const NxShaderUniforms uniform, const glm::vec3 &values) const
    {
        const std::string &name = ShaderUniformsName.at(uniform);
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformFloat3(name, values))
            return true;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniform3f(location, values.x, values.y, values.z);
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformFloat4(const std::string& name, const glm::vec4& values) const
    {
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformFloat4(name, values))
            return true;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniform4f(location, values.x, values.y, values.z, values.w);
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformFloat4(const NxShaderUniforms uniform, const glm::vec4 &values) const
    {
        const std::string &name = ShaderUniformsName.at(uniform);
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformFloat4(name, values))
            return true;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniform4f(location, values.x, values.y, values.z, values.w);
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformMatrix(const std::string& name, const glm::mat4& matrix) const
    {
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformMatrix(name, matrix))
            return true;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformMatrix(const NxShaderUniforms uniform, const glm::mat4 &matrix) const
    {
        const std::string &name = ShaderUniformsName.at(uniform);
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformMatrix(name, matrix))
            return true;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformInt(const std::string& name, int value) const
    {
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformInt(name, value))
            return true;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniform1i(location, value);
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformBool(const std::string& name, bool value) const
    {
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformBool(name, value))
            return true;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniform1i(location, static_cast<int>(value));
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformInt(const NxShaderUniforms uniform, const int value) const
    {
        const std::string &name = ShaderUniformsName.at(uniform);
        if (!NxShader::hasUniform(name))
            return false;
        if (NxShader::setUniformInt(name, value))
            return true;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }
        glUniform1i(location, value);
        m_uniformCache.clearDirtyFlag(name);
        return true;
    }

    bool NxOpenGlShader::setUniformIntArray(const std::string &name, const int *values, const unsigned int count) const
    {
        if (!NxShader::hasUniform(name))
            return false;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }

        glUniform1iv(location, static_cast<int>(count), values);
        return true;
    }

    bool NxOpenGlShader::setUniformIntArray(const NxShaderUniforms uniform, const int *values, const unsigned int count) const
    {
        const std::string &name = ShaderUniformsName.at(uniform);
        if (!NxShader::hasUniform(name))
            return false;

        const int location = getUniformLocation(name);
        if (location == -1) {
            LOG(NEXO_WARN, "For shader {}, uniform {} not found", m_name, name);
            return false;
        }

        glUniform1iv(location, static_cast<int>(count), values);
        return true;
    }

    void NxOpenGlShader::bindStorageBuffer(const unsigned int index) const
    {
    	if (index > m_storageBuffers.size())
     		THROW_EXCEPTION(NxOutOfRangeException, index, m_storageBuffers.size());
    	m_storageBuffers[index]->bind();
    }

    void NxOpenGlShader::unbindStorageBuffer(const unsigned int index) const
    {
    	if (index > m_storageBuffers.size())
     		THROW_EXCEPTION(NxOutOfRangeException, index, m_storageBuffers.size());
    	m_storageBuffers[index]->unbind();
    }

    void NxOpenGlShader::bindStorageBufferBase(const unsigned int index, const unsigned int bindingLocation) const
    {
   		if (index > m_storageBuffers.size())
    		THROW_EXCEPTION(NxOutOfRangeException, index, m_storageBuffers.size());
      	m_storageBuffers[index]->bindBase(bindingLocation);
    }
}
