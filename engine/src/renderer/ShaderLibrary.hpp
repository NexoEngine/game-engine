//// ShaderLibrary.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        19/04/2025
//  Description: Header file for the shader library
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <unordered_map>
#include "Shader.hpp"

namespace nexo::renderer {
    class ShaderLibrary {
        public:
            void add(const std::shared_ptr<NxShader> &shader);
            void add(const std::string &name, const std::shared_ptr<NxShader> &shader);
            std::shared_ptr<NxShader> load(const std::string &path);
            std::shared_ptr<NxShader> load(const std::string &name, const std::string &path);
            std::shared_ptr<NxShader> load(const std::string &name, const std::string &vertexSource, const std::string &fragmentSource);
            std::shared_ptr<NxShader> get(const std::string &name) const;
        private:
            std::unordered_map<std::string , std::shared_ptr<NxShader>> m_shaders;
    };
}
