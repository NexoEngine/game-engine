//// OpenGlShader.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for opengl shader class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "renderer/Shader.hpp"
#include <glad/glad.h>

namespace nexo::renderer {

    /**
    * @class OpenGlShader
    * @brief Implementation of the Shader interface using OpenGL.
    *
    * The `OpenGlShader` class provides OpenGL-specific functionality for creating
    * and managing shader programs. It supports setting uniform variables and compiling
    * shaders from source code or files.
    *
    * Responsibilities:
    * - Compile and link shader programs using OpenGL.
    * - Set uniform variables for rendering operations.
    * - Manage the lifecycle of OpenGL shader programs.
    */
    class OpenGlShader final : public Shader {
        public:
            /**
            * @brief Constructs a shader program from a source file.
            *
            * Reads and compiles a shader program from the specified file path. The file should
            * contain `#type` directives to separate shader stages.
            *
            * @param path The file path to the shader source code.
            *
            * Throws:
            * - `FileNotFoundException` if the file cannot be found.
            * - `ShaderCreationFailed` if shader compilation fails.
            */
            OpenGlShader(const std::string &path);
            OpenGlShader(std::string name, const std::string_view &vertexSource, const std::string_view &fragmentSource);
            ~OpenGlShader() override;

            /**
            * @brief Activates the shader program in OpenGL.
            *
            * Binds the shader program for use in subsequent rendering operations.
            */
            void bind() const override;
            void unbind() const override;

            void setUniformFloat(const std::string &name, const float value) const override;
            void setUniformFloat3(const std::string &name, const glm::vec3 &values) const override;
            void setUniformFloat4(const std::string &name, const glm::vec4 &values) const override;
            void setUniformMatrix(const std::string &name, const glm::mat4 &matrix) const override;
            void setUniformInt(const std::string &name, int value) const override;
            void setUniformIntArray(const std::string &name, const int *values, unsigned int count) const override;

            [[nodiscard]] const std::string &getName() const override { return m_name; };
            unsigned int getProgramId() const override { return m_id; };
        private:
            std::string m_name;
            unsigned int m_id = 0;
            static std::unordered_map<GLenum, std::string> preProcess(const std::string_view &src, const std::string &filePath);
            void compile(const std::unordered_map<GLenum, std::string> &shaderSources);
    };

}