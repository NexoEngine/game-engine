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

#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>

namespace nexo::renderer {

    class Shader {
        public:
            virtual ~Shader() = default;

            static std::shared_ptr<Shader> create(const std::string &path);
            static std::shared_ptr<Shader> create(const std::string& name, const std::string &vertexSource, const std::string &fragmentSource);

            virtual void bind() const = 0;
            virtual void unbind() const = 0;

            virtual void setUniformFloat(const std::string &name, const float value) const = 0;
            virtual void setUniformFloat3(const std::string &name, const glm::vec3 &values) const = 0;
            virtual void setUniformFloat4(const std::string &name, const glm::vec4 &values) const = 0;
            virtual void setUniformMatrix(const std::string &name, const glm::mat4 &matrix) const = 0;
            virtual void setUniformInt(const std::string &name, int value) const = 0;
            virtual void setUniformIntArray(const std::string &name, const int *values, unsigned int count) const = 0;

            [[nodiscard]] virtual const std::string &getName() const = 0;
        protected:
            static std::string readFile(const std::string &filepath);
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
