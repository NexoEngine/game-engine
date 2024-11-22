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

    class OpenGlShader final : public Shader {
        public:
            explicit OpenGlShader(const std::string &path);
            OpenGlShader(std::string name, const std::string &vertexSource, const std::string &fragmentSource);
            ~OpenGlShader() override;

            void bind() const override;
            void unbind() const override;

            void setUniformFloat(const std::string &name, const float value) const override;
            void setUniformFloat3(const std::string &name, const glm::vec3 &values) const override;
            void setUniformFloat4(const std::string &name, const glm::vec4 &values) const override;
            void setUniformMatrix(const std::string &name, const glm::mat4 &matrix) const override;
            void setUniformInt(const std::string &name, int value) const override;
            void setUniformIntArray(const std::string &name, const int *values, unsigned int count) const override;

            [[nodiscard]] const std::string &getName() const override { return m_name; };
        private:
            std::string m_name;
            unsigned int m_id = 0;
            static std::unordered_map<GLenum, std::string> preProcess(const std::string &src, const std::string &filePath);
            void compile(const std::unordered_map<GLenum, std::string> &shaderSources);
    };

}