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

namespace nexo::renderer {

    /**
    * @class Shader
    * @brief Abstract class representing a shader program in the rendering pipeline.
    *
    * The `Shader` class provides a generic interface for creating and managing shader
    * programs. These programs are used to execute rendering operations on the GPU.
    *
    * Responsibilities:
    * - Create shader programs from source code or files.
    * - Bind and unbind shader programs during rendering.
    * - Set uniform variables to pass data from the CPU to the GPU.
    *
    * Subclasses:
    * - `OpenGlShader`: Implements this interface using OpenGL-specific functionality.
    *
    * Example Usage:
    * ```cpp
    * auto shader = Shader::create("path/to/shader.glsl");
    * shader->bind();
    * shader->setUniformFloat("uTime", 1.0f);
    * ```
    */
    class Shader {
        public:
            virtual ~Shader() = default;

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
            * - `UnknownGraphicsApi` if no graphics API is supported.
            * - `ShaderCreationFailed` if shader compilation fails.
            */
            static std::shared_ptr<Shader> create(const std::string &path);

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
            * - `UnknownGraphicsApi` if no graphics API is supported.
            * - `ShaderCreationFailed` if shader compilation fails.
            */
            static std::shared_ptr<Shader> create(const std::string& name, const std::string &vertexSource, const std::string &fragmentSource);

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

            virtual bool setUniformFloat(const std::string &name, float value) const = 0;
            virtual bool setUniformFloat3(const std::string &name, const glm::vec3 &values) const = 0;
            virtual bool setUniformFloat4(const std::string &name, const glm::vec4 &values) const = 0;
            virtual bool setUniformMatrix(const std::string &name, const glm::mat4 &matrix) const = 0;
            virtual bool setUniformInt(const std::string &name, int value) const = 0;
            virtual bool setUniformIntArray(const std::string &name, const int *values, unsigned int count) const = 0;

            void addStorageBuffer(const std::shared_ptr<ShaderStorageBuffer> &buffer);
            void setStorageBufferData(unsigned int index, void *data, unsigned int size);
            virtual void bindStorageBufferBase(unsigned int index, unsigned int bindingPoint) const = 0;
            virtual void bindStorageBuffer(unsigned int index) const = 0;
            virtual void unbindStorageBuffer(unsigned int index) const = 0;

            [[nodiscard]] virtual const std::string &getName() const = 0;
            virtual unsigned int getProgramId() const = 0;
        protected:
            static std::string readFile(const std::string &filepath);
        	std::vector<std::shared_ptr<ShaderStorageBuffer>> m_storageBuffers;
    };

    class ShaderLibrary {
        public:
            void add(const std::shared_ptr<Shader> &shader);
            void add(const std::string &name, const std::shared_ptr<Shader> &shader);
            std::shared_ptr<Shader> load(const std::string &path);
            std::shared_ptr<Shader> load(const std::string &name, const std::string &path);
            std::shared_ptr<Shader> load(const std::string &name, const std::string &vertexSource, const std::string &fragmentSource);
            std::shared_ptr<Shader> get(const std::string &name) const;
        private:
            std::unordered_map<std::string , std::shared_ptr<Shader>> m_shaders;
    };
}
