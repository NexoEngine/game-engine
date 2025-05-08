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
#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

#include "ShaderStorageBuffer.hpp"
#include "Attributes.hpp"
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
        {NxShaderUniforms::MATERIAL, "uMaterial"}
    };

    struct UniformInfo {
        std::string name;      // Name of the uniform
        int location;          // Location in the shader
        unsigned int type;     // GL type (e.g., GL_FLOAT, GL_FLOAT_VEC3)
        int size;              // Size (for arrays)
    };

    struct AttributeInfo {
        std::string name;      // Name of the attribute
        int location;          // Location in the shader
        unsigned int type;     // GL type
        int size;              // Size
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
            static std::shared_ptr<NxShader> create(const std::string &path);

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
            static std::shared_ptr<NxShader> create(const std::string& name, const std::string &vertexSource, const std::string &fragmentSource);

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

            virtual bool setUniformFloat(const std::string &name, float value) const;
            virtual bool setUniformFloat2(const std::string &name, const glm::vec2 &values) const;
            virtual bool setUniformFloat3(const std::string &name, const glm::vec3 &values) const;
            virtual bool setUniformFloat4(const std::string &name, const glm::vec4 &values) const;
            virtual bool setUniformMatrix(const std::string &name, const glm::mat4 &matrix) const;
            virtual bool setUniformBool(const std::string &name, bool value) const;
            virtual bool setUniformInt(const std::string &name, int value) const;
            virtual bool setUniformIntArray(const std::string &name, const int *values, unsigned int count) const;

            virtual bool setUniformFloat(NxShaderUniforms uniform, float value) const = 0;
            virtual bool setUniformFloat3(NxShaderUniforms uniform, const glm::vec3 &values) const = 0;
            virtual bool setUniformFloat4(NxShaderUniforms uniform, const glm::vec4 &values) const = 0;
            virtual bool setUniformMatrix(NxShaderUniforms uniform, const glm::mat4 &matrix) const = 0;
            virtual bool setUniformInt(NxShaderUniforms uniform, int value) const = 0;
            virtual bool setUniformIntArray(NxShaderUniforms uniform, const int *values, unsigned int count) const = 0;

            bool setUniform(const std::string &name, UniformValue value);

            void addStorageBuffer(const std::shared_ptr<NxShaderStorageBuffer> &buffer);
            void setStorageBufferData(unsigned int index, void *data, unsigned int size);
            virtual void bindStorageBufferBase(unsigned int index, unsigned int bindingPoint) const = 0;
            virtual void bindStorageBuffer(unsigned int index) const = 0;
            virtual void unbindStorageBuffer(unsigned int index) const = 0;

            bool hasUniform(const std::string& name) const;
            bool hasAttribute(int location) const;

            bool isCompatibleWithMesh(const RequiredAttributes& meshAttributes) const;

            void resetCache();

            [[nodiscard]] virtual const std::string &getName() const = 0;
            virtual unsigned int getProgramId() const = 0;
        protected:
            static std::string readFile(const std::string &filepath);
        	std::vector<std::shared_ptr<NxShaderStorageBuffer>> m_storageBuffers;
            RequiredAttributes m_requiredAttributes;
            std::unordered_map<std::string, UniformInfo> m_uniformInfos;
            std::unordered_map<int, AttributeInfo> m_attributeInfos;
            mutable UniformCache m_uniformCache;
    };


}
