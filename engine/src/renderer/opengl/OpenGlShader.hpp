//// OpenGlShader.hpp /////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for opengl shader class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glad/glad.h>
#include "renderer/Shader.hpp"

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

        /**
         * @brief Constructs a shader program from vertex and fragment source code.
         *
         * Compiles and links a shader program using the provided vertex and fragment shader
         * source code strings.
         *
         * @param name A name identifier for the shader program.
         * @param vertexSource The source code for the vertex shader.
         * @param fragmentSource The source code for the fragment shader.
         *
         * Throws:
         * - `NxShaderCreationFailed` if shader compilation fails.
         */
        NxOpenGlShader(std::string name, const std::string_view &vertexSource, const std::string_view &fragmentSource);

        ~NxOpenGlShader() override;

        /**
         * @brief Activates the shader program in OpenGL.
         *
         * Binds the shader program for use in subsequent rendering operations.
         */
        void bind() const override;
        /**
         * @brief Deactivates the shader program in OpenGL.
         *
         * Unbinds the shader program, reverting to fixed-function pipeline or no shader.
         */
        void unbind() const override;

        /**
         * @brief Sets a float uniform variable in the shader program.
         * @param name The name of the uniform variable.
         * @param value The float value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformFloat(const std::string &name, float value) const override;

        /**
         * @brief Sets a vec2 (2-component float vector) uniform variable in the shader program.
         * @param name The name of the uniform variable.
         * @param values The glm::vec2 value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformFloat2(const std::string &name, const glm::vec2 &values) const override;

        /**
         * @brief Sets a vec3 (3-component float vector) uniform variable in the shader program.
         * @param name The name of the uniform variable.
         * @param values The glm::vec3 value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformFloat3(const std::string &name, const glm::vec3 &values) const override;

        /**
         * @brief Sets a vec4 (4-component float vector) uniform variable in the shader program.
         * @param name The name of the uniform variable.
         * @param values The glm::vec4 value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformFloat4(const std::string &name, const glm::vec4 &values) const override;

        /**
         * @brief Sets a mat4 (4x4 float matrix) uniform variable in the shader program.
         * @param name The name of the uniform variable.
         * @param matrix The glm::mat4 value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformMatrix(const std::string &name, const glm::mat4 &matrix) const override;

        /**
         * @brief Sets a boolean uniform variable in the shader program.
         * @param name The name of the uniform variable.
         * @param value The boolean value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformBool(const std::string &name, bool value) const override;

        /**
         * @brief Sets an integer uniform variable in the shader program.
         * @param name The name of the uniform variable.
         * @param value The integer value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformInt(const std::string &name, int value) const override;

        /**
         * @brief Sets an array of integer uniform variables in the shader program.
         * @param name The name of the uniform variable.
         * @param values Pointer to the array of integer values.
         * @param count The number of elements in the array.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformIntArray(const std::string &name, const int *values, unsigned int count) const override;

        /**
         * @brief Sets a float uniform variable using a predefined uniform enum.
         * @param uniform The predefined uniform identifier.
         * @param value The float value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformFloat(NxShaderUniforms uniform, float value) const override;

        /**
         * @brief Sets a vec3 (3-component float vector) uniform variable using a predefined uniform enum.
         * @param uniform The predefined uniform identifier.
         * @param values The glm::vec3 value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformFloat3(NxShaderUniforms uniform, const glm::vec3 &values) const override;

        /**
         * @brief Sets a vec4 (4-component float vector) uniform variable using a predefined uniform enum.
         * @param uniform The predefined uniform identifier.
         * @param values The glm::vec4 value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformFloat4(NxShaderUniforms uniform, const glm::vec4 &values) const override;

        /**
         * @brief Sets a mat4 (4x4 float matrix) uniform variable using a predefined uniform enum.
         * @param uniform The predefined uniform identifier.
         * @param matrix The glm::mat4 value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformMatrix(NxShaderUniforms uniform, const glm::mat4 &matrix) const override;

        /**
         * @brief Sets an integer uniform variable using a predefined uniform enum.
         * @param uniform The predefined uniform identifier.
         * @param value The integer value to set.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformInt(NxShaderUniforms uniform, int value) const override;

        /**
         * @brief Sets an array of integer uniform variables using a predefined uniform enum.
         * @param uniform The predefined uniform identifier.
         * @param values Pointer to the array of integer values.
         * @param count The number of elements in the array.
         * @return True if the uniform was set successfully, false otherwise.
         */
        bool setUniformIntArray(NxShaderUniforms uniform, const int *values, unsigned int count) const override;

        /**
         * @brief Binds a storage buffer to the shader program at the specified index.
         * @param index The index of the storage buffer to bind.
         */
        void bindStorageBuffer(unsigned int index) const override;

        /**
         * @brief Binds a storage buffer to a specific binding location in the shader program.
         * @param index The index of the storage buffer.
         * @param bindingLocation The binding location in the shader.
         */
        void bindStorageBufferBase(unsigned int index, unsigned int bindingLocation) const override;

        /**
         * @brief Unbinds a storage buffer from the shader program at the specified index.
         * @param index The index of the storage buffer to unbind.
         */
        void unbindStorageBuffer(unsigned int index) const override;

        /**
         * @brief Retrieves the name of the shader program.
         * @return The name of the shader program as a constant reference to a string.
         */
        [[nodiscard]] const std::string &getName() const override
        {
            return m_name;
        }

        /**
         * @brief Retrieves the OpenGL program ID of the shader.
         * @return The OpenGL program ID as an unsigned integer.
         */
        unsigned int getProgramId() const override
        {
            return m_id;
        }

       private:
        std::string m_name;
        unsigned int m_id = 0;

        /**
         * @brief Preprocesses shader source code to separate different shader stages.
         *
         * Parses the provided shader source code string to identify and extract
         * individual shader stages (e.g., vertex, fragment) based on `#type` directives.
         *
         * @param src The complete shader source code as a string view.
         * @param filePath The file path of the shader source (for error reporting).
         * @return A map associating OpenGL shader types (GLenum) with their corresponding source code strings.
         *
         * Throws:
         * - `NxShaderCreationFailed` if there are syntax errors or invalid shader types.
         */
        static std::unordered_map<GLenum, std::string> preProcess(const std::string_view &src,
                                                                  const std::string &filePath);

        /**
         * @brief Compiles and links OpenGL shaders from the provided sources.
         *
         * @param shaderSources Map associating OpenGL shader types (GLenum) with their source code.
         * @throws NxShaderCreationFailed if compilation or linking fails.
         */
        void compile(const std::unordered_map<GLenum, std::string> &shaderSources);

        /**
         * @brief Prepares and caches the locations of shader uniforms.
         *
         * This method should be called after the shader program has been compiled.
         */
        void setupUniformLocations();

        /**
         * @brief Retrieves the location of a uniform variable in the shader program.
         *
         * @param name The name of the uniform variable.
         * @return The location of the uniform, or -1 if not found.
         */
        int getUniformLocation(const std::string &name) const;
    };

} // namespace nexo::renderer
