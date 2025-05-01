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
    * @class NxOpenGlShader
    * @brief Implementation of the Shader interface using OpenGL.
    *
    * The `NxOpenGlShader` class provides OpenGL-specific functionality for creating
    * and managing shader programs. It supports setting uniform variables and compiling
    * shaders from source code or files.
    *
    * Responsibilities:
    * - Compile and link shader programs using OpenGL.
    * - Set uniform variables for rendering operations.
    * - Manage the lifecycle of OpenGL shader programs.
    */
    class NxOpenGlShader final : public NxShader {
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
            * - `NxFileNotFoundException` if the file cannot be found.
            * - `NxShaderCreationFailed` if shader compilation fails.
            */
            explicit NxOpenGlShader(const std::string &path);
            NxOpenGlShader(std::string name, const std::string_view &vertexSource, const std::string_view &fragmentSource);
            ~NxOpenGlShader() override;

            /**
            * @brief Activates the shader program in OpenGL.
            *
            * Binds the shader program for use in subsequent rendering operations.
            */
            void bind() const override;
            void unbind() const override;

            bool setUniformFloat(const std::string &name, float value) const override;
            bool setUniformFloat2(const std::string &name, const glm::vec2 &values) const override;
            bool setUniformFloat3(const std::string &name, const glm::vec3 &values) const override;
            bool setUniformFloat4(const std::string &name, const glm::vec4 &values) const override;
            bool setUniformMatrix(const std::string &name, const glm::mat4 &matrix) const override;
            bool setUniformBool(const std::string &name, bool value) const override;
            bool setUniformInt(const std::string &name, int value) const override;
            bool setUniformIntArray(const std::string &name, const int *values, unsigned int count) const override;

            bool setUniformFloat(NxShaderUniforms uniform, float value) const override;
            bool setUniformFloat3(NxShaderUniforms uniform, const glm::vec3 &values) const override;
            bool setUniformFloat4(NxShaderUniforms uniform, const glm::vec4 &values) const override;
            bool setUniformMatrix(NxShaderUniforms uniform, const glm::mat4 &matrix) const override;
            bool setUniformInt(NxShaderUniforms uniform, int value) const override;
            bool setUniformIntArray(NxShaderUniforms uniform, const int *values, unsigned int count) const override;

            void bindStorageBuffer(unsigned int index) const override;
            void bindStorageBufferBase(unsigned int index, unsigned int bindingLocation) const override;
            void unbindStorageBuffer(unsigned int index) const override;

            [[nodiscard]] const std::string &getName() const override { return m_name; };
            unsigned int getProgramId() const override { return m_id; };
        private:
            std::string m_name;
            unsigned int m_id = 0;
            static std::unordered_map<GLenum, std::string> preProcess(const std::string_view &src, const std::string &filePath);
            void compile(const std::unordered_map<GLenum, std::string> &shaderSources);
            void setupUniformLocations();
    };

}
