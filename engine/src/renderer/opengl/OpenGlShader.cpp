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

#include <cstring>
#include <array>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

namespace nexo::renderer {

    static GLenum shaderTypeFromString(const std::string &type)
    {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment")
            return GL_FRAGMENT_SHADER;
        std::cerr << "Unknown shader type: " << type << std::endl;
        return 0;
    }

    OpenGlShader::OpenGlShader(const std::string &path)
    {
        const std::string src = readFile(path);
        auto shaderSources = preProcess(src);
        compile(shaderSources);

        auto lastSlash = path.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        const auto lastDot = path.rfind('.');
        const auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
        m_name = path.substr(lastSlash, count);
    }

    OpenGlShader::OpenGlShader(std::string name, const std::string &vertexSource, const std::string &fragmentSource) : m_name(std::move(name))
    {
        std::unordered_map<GLenum, std::string> preProcessedSource;
        preProcessedSource[GL_VERTEX_SHADER] = vertexSource;
        preProcessedSource[GL_FRAGMENT_SHADER] = fragmentSource;
        compile(preProcessedSource);
    }

    OpenGlShader::~OpenGlShader()
    {
        glDeleteProgram(m_id);
    }

    std::unordered_map<GLenum, std::string> OpenGlShader::preProcess(const std::string &src)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char *typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = src.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = src.find_first_of("\r\n", pos);
            if (eol == std::string::npos)
            {
                std::cerr << "Error parsing shader: " << src << std::endl;
                return shaderSources;
            }
            size_t begin = pos + typeTokenLength + 1;
            std::string type = src.substr(begin, eol - begin);
            if (!shaderTypeFromString(type))
            {
                std::cerr << "Invalid type" << type << std::endl;
                return shaderSources;
            }

            size_t nextLinePos = src.find_first_not_of("\r\n", eol);
            if (nextLinePos == std::string::npos)
            {
                std::cerr << "Error parsing shader: " << src << std::endl;
                return shaderSources;
            }
            pos = src.find(typeToken, nextLinePos);

            shaderSources[shaderTypeFromString(type)] = (pos == std::string::npos)
                                                            ? src.substr(nextLinePos)
                                                            : src.substr(nextLinePos, pos - nextLinePos);
        }

        return shaderSources;
    }

    void OpenGlShader::compile(const std::unordered_map<GLenum, std::string> &shaderSources)
    {
        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        if (shaderSources.size() > 2)
        {
            std::cerr << "Only 2 shaders are supported for now" << std::endl;
            return;
        }
        GLuint program = glCreateProgram();
        std::array<GLenum, 2> glShaderIds{};
        unsigned int shaderIndex = 0;
        for (auto &kv: shaderSources)
        {
            const GLenum type = kv.first;
            const std::string src = kv.second;
            // Create an empty vertex shader handle
            GLuint shader = glCreateShader(type);

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
                std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog.data() << std::endl;


                // Use the infoLog as you see fit.

                // In this simple program, we'll just leave
                return;
            }
            glAttachShader(program, shader);
            glShaderIds[shaderIndex++] = shader;;
        }
        m_id = program;

        // Link our program
        glLinkProgram(m_id);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_id, GL_LINK_STATUS, (int *) &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_id, maxLength, &maxLength, &infoLog[0]);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog.data() << std::endl;

            // We don't need the program anymore.
            glDeleteProgram(m_id);
            // Don't leak shaders either.
            for (auto id: glShaderIds)
                glDeleteShader(id);

            // In this simple program, we'll just leave
            return;
        }

        // Always detach shaders after a successful link.
        for (auto id: glShaderIds)
            glDetachShader(program, id);
    }


    void OpenGlShader::bind() const
    {
        glUseProgram(m_id);
    }

    void OpenGlShader::unbind() const
    {
        glUseProgram(0);
    }

    void OpenGlShader::setUniformFloat4(const std::string &name, const glm::vec4 &values) const
    {
        const int loc = glGetUniformLocation(m_id, name.c_str());
        glUniform4f(loc, values.x, values.y, values.z, values.w);
    }

    void OpenGlShader::setUniformMatrix(const std::string &name, const glm::mat4 &matrix) const
    {
        const int loc = glGetUniformLocation(m_id, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGlShader::setUniformInt(const std::string &name, int value) const
    {
        const int loc = glGetUniformLocation(m_id, name.c_str());
        glUniform1i(loc, value);
    }

    void OpenGlShader::setUniformIntArray(const std::string &name, const int *values, const unsigned int count) const
    {
        const int loc = glGetUniformLocation(m_id, name.c_str());
        glUniform1iv(loc, count, values);
    }
}
