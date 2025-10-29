//// Shader.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for shader abstract class and shader library class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Attributes.hpp"
#include "ShaderStorageBuffer.hpp"
#include "UniformCache.hpp"

namespace nexo::renderer {
    enum class NxShaderUniforms {
        VIEW_PROJECTION,
        MODEL_MATRIX,
        CAMERA_POSITION,

        TEXTURE_SAMPLER,

        DIR_LIGHT,
        AMBIENT_LIGHT,
        POINT_LIGHT_ARRAY,
        NB_POINT_LIGHT,
        SPOT_LIGHT_ARRAY,
        NB_SPOT_LIGHT,

        MATERIAL
    };

    inline const std::unordered_map<NxShaderUniforms, std::string> ShaderUniformsName = {
        {NxShaderUniforms::VIEW_PROJECTION, "uViewProjection"},
        {NxShaderUniforms::MODEL_MATRIX, "uMatModel"},
        {NxShaderUniforms::CAMERA_POSITION, "uCamPos"},
        {NxShaderUniforms::TEXTURE_SAMPLER, "uTexture"},
        {NxShaderUniforms::DIR_LIGHT, "uDirLight"},
        {NxShaderUniforms::AMBIENT_LIGHT, "uAmbientLight"},
        {NxShaderUniforms::POINT_LIGHT_ARRAY, "uPointLights"},
        {NxShaderUniforms::NB_POINT_LIGHT, "uNbPointLights"},
        {NxShaderUniforms::SPOT_LIGHT_ARRAY, "uSpotLights"},
        {NxShaderUniforms::NB_SPOT_LIGHT, "uNbSpotLights"},
        {NxShaderUniforms::MATERIAL, "uMaterial"}};

    struct UniformInfo {
        std::string name;  // Name of the uniform
        int location;      // Location in the shader
        unsigned int type; // GL type (e.g., GL_FLOAT, GL_FLOAT_VEC3)
        int size;          // Size (for arrays)
    };

    struct AttributeInfo {
        std::string name;  // Name of the attribute
        int location;      // Location in the shader
        unsigned int type; // GL type
        int size;          // Size
    };

    /**
     * @class NxShader
     * @brief Abstract class representing a shader program in the rendering pipeline.
     *
     * The `NxShader` class provides a generic interface for creating and managing shader
     * programs. These programs are used to execute rendering operations on the GPU.
     *
     * Responsibilities:
     * - Create shader programs from source code or files.
     * - Bind and unbind shader programs during rendering.
     * - Set uniform variables to pass data from the CPU to the GPU.
     *
     * Subclasses:
     * - `NxOpenGlShader`: Implements this interface using OpenGL-specific functionality.
     *
     * Example Usage:
     * ```cpp
     * auto shader = NxShader::create("path/to/shader.glsl");
     * shader->bind();
     * shader->setUniformFloat("uTime", 1.0f);
     * ```
     */
    class NxShader {
       public:
        virtual ~NxShader() = default;

        /**
         * @brief Creates a shader program from a source file.
         *
         * Loads and compiles a shader program from the specified file path. The file
         * should contain shader stages marked with `#type` directives.
         *
         * @param path The file path to the shader source code.
         * @return A shared pointer to the created `Shader` instance.
         *
         * Throws:
         * - `NxUnknownGraphicsApi` if no graphics API is supported.
         * - `NxShaderCreationFailed` if shader compilation fails.
         */
        static std::shared_ptr<NxShader> create(const std::string& path);

        /**
         * @brief Creates a shader program from source code strings.
         *
         * Compiles and links a shader program from provided vertex and fragment shader source code.
         *
         * @param name The name of the shader program.
         * @param vertexSource The source code for the vertex shader.
         * @param fragmentSource The source code for the fragment shader.
         * @return A shared pointer to the created `Shader` instance.
         *
         * Throws:
         * - `NxUnknownGraphicsApi` if no graphics API is supported.
         * - `NxShaderCreationFailed` if shader compilation fails.
         */
        static std::shared_ptr<NxShader> create(const std::string& name, const std::string& vertexSource,
                                                const std::string& fragmentSource);

        /**
         * @brief Binds the shader program for use in the rendering pipeline.
         *
         * Makes the shader program active, so subsequent draw calls use this program.
         *
         * Must be implemented by subclasses.
         */
        virtual void bind() const = 0;

        /**
         * @brief Unbinds the shader program.
         *
         * Deactivates the currently bound shader program.
         *
         * Must be implemented by subclasses.
         */
        virtual void unbind() const = 0;

        /**
         * @brief Sets a float uniform value in the shader by name.
         * @param name The name of the uniform variable.
         * @param value The float value to set.
         * @return true if the uniform was found and set, false otherwise.
         */
        virtual bool setUniformFloat(const std::string& name, float value) const;

        /**
         * @brief Sets a vec2 uniform value in the shader by name.
         * @param name The name of the uniform variable.
         * @param values The 2D vector value to set.
         * @return true if the uniform was found and set, false otherwise.
         */
        virtual bool setUniformFloat2(const std::string& name, const glm::vec2& values) const;

        /**
         * @brief Sets a vec3 uniform value in the shader by name.
         * @param name The name of the uniform variable.
         * @param values The 3D vector value to set.
         * @return true if the uniform was found and set, false otherwise.
         */
        virtual bool setUniformFloat3(const std::string& name, const glm::vec3& values) const;

        /**
         * @brief Sets a vec4 uniform value in the shader by name.
         * @param name The name of the uniform variable.
         * @param values The 4D vector value to set.
         * @return true if the uniform was found and set, false otherwise.
         */
        virtual bool setUniformFloat4(const std::string& name, const glm::vec4& values) const;

        /**
         * @brief Sets a mat4 uniform value in the shader by name.
         * @param name The name of the uniform variable.
         * @param matrix The 4x4 matrix value to set.
         * @return true if the uniform was found and set, false otherwise.
         */
        virtual bool setUniformMatrix(const std::string& name, const glm::mat4& matrix) const;

        /**
         * @brief Sets a boolean uniform value in the shader by name.
         * @param name The name of the uniform variable.
         * @param value The boolean value to set.
         * @return true if the uniform was found and set, false otherwise.
         */
        virtual bool setUniformBool(const std::string& name, bool value) const;

        /**
         * @brief Sets an integer uniform value in the shader by name.
         * @param name The name of the uniform variable.
         * @param value The integer value to set.
         * @return true if the uniform was found and set, false otherwise.
         */
        virtual bool setUniformInt(const std::string& name, int value) const;

        /**
         * @brief Sets an array of integer uniform values in the shader by name.
         * @param name The name of the uniform variable.
         * @param values Pointer to the array of integer values.
         * @param count Number of elements in the array.
         * @return true if the uniform was found and set, false otherwise.
         */
        virtual bool setUniformIntArray(const std::string& name, const int* values, unsigned int count) const;

        /**
         * @brief Sets a float uniform value in the shader using enum identifier.
         * @param uniform The uniform enum identifier.
         * @param value The float value to set.
         * @return true if the uniform was found and set.
         */
        virtual bool setUniformFloat(NxShaderUniforms uniform, float value) const = 0;

        /**
         * @brief Sets a vec3 uniform value using enum identifier.
         * @param uniform The uniform enum identifier.
         * @param values The 3D vector value to set.
         * @return true if the uniform was found and set.
         */
        virtual bool setUniformFloat3(NxShaderUniforms uniform, const glm::vec3& values) const = 0;

        /**
         * @brief Sets a vec4 uniform value using enum identifier.
         * @param uniform The uniform enum identifier.
         * @param values The 4D vector value to set.
         * @return true if the uniform was found and set.
         */
        virtual bool setUniformFloat4(NxShaderUniforms uniform, const glm::vec4& values) const = 0;

        /**
         * @brief Sets a mat4 uniform value using enum identifier.
         * @param uniform The uniform enum identifier.
         * @param matrix The 4x4 matrix value to set.
         * @return true if the uniform was found and set.
         */
        virtual bool setUniformMatrix(NxShaderUniforms uniform, const glm::mat4& matrix) const = 0;

        /**
         * @brief Sets an integer uniform value using enum identifier.
         * @param uniform The uniform enum identifier.
         * @param value The integer value to set.
         * @return true if the uniform was found and set.
         */
        virtual bool setUniformInt(NxShaderUniforms uniform, int value) const = 0;

        /**
         * @brief Sets an array of integer values using enum identifier.
         * @param uniform The uniform enum identifier.
         * @param values Pointer to the integer array.
         * @param count Number of elements.
         * @return true if the uniform was found and set.
         */
        virtual bool setUniformIntArray(NxShaderUniforms uniform, const int* values, unsigned int count) const = 0;

        /**
         * @brief Adds a shader storage buffer to the shader.
         * @param buffer The buffer to add.
         */
        void addStorageBuffer(const std::shared_ptr<NxShaderStorageBuffer>& buffer);

        /**
         * @brief Updates data in a storage buffer at given index.
         * @param index Buffer index.
         * @param data Pointer to the data.
         * @param size Size of the data in bytes.
         */
        void setStorageBufferData(size_t index, void* data, size_t size) const;

        /**
         * @brief Binds a storage buffer to a binding point.
         * @param index Buffer index.
         * @param bindingPoint Target binding point.
         */
        virtual void bindStorageBufferBase(unsigned int index, unsigned int bindingPoint) const = 0;

        /**
         * @brief Binds a storage buffer.
         * @param index Buffer index to bind.
         */
        virtual void bindStorageBuffer(unsigned int index) const = 0;

        /**
         * @brief Unbinds a storage buffer.
         * @param index Buffer index to unbind.
         */
        virtual void unbindStorageBuffer(unsigned int index) const = 0;

        /**
         * @brief Checks if a uniform exists in the shader.
         * @param name Name of the uniform.
         * @return true if the uniform exists.
         */
        bool hasUniform(const std::string& name) const;

        /**
         * @brief Checks if an attribute exists at given location.
         * @param location Attribute location.
         * @return true if the attribute exists.
         */
        bool hasAttribute(int location) const;

        /**
         * @brief Checks if shader is compatible with mesh attributes.
         * @param meshAttributes Required attributes.
         * @return true if compatible.
         */
        bool isCompatibleWithMesh(const RequiredAttributes& meshAttributes) const;

        /**
         * @brief Resets the uniform cache.
         */
        void resetCache() const;

        /**
         * @brief Gets the shader program name.
         * @return The name of the shader.
         */
        [[nodiscard]] virtual const std::string& getName() const = 0;

        /**
         * @brief Gets the shader program ID.
         * @return The program ID.
         */
        virtual unsigned int getProgramId() const = 0;

       protected:
        /**
         * @brief Reads the contents of a file into a string.
         * @param filepath The path to the file.
         * @return The file contents as a string.
         *
         * Throws:
         * - `NxFileNotFoundException` if the file cannot be opened.
         */
        static std::string readFile(const std::string& filepath);
        std::vector<std::shared_ptr<NxShaderStorageBuffer>> m_storageBuffers;
        RequiredAttributes m_requiredAttributes;
        std::unordered_map<std::string, UniformInfo> m_uniformInfos;
        std::unordered_map<int, AttributeInfo> m_attributeInfos;
        mutable UniformCache m_uniformCache;
    };
} // namespace nexo::renderer
