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

    struct TransparentStringHasher {
        using is_transparent = void;  // enable heterogeneous lookup

        size_t operator()(std::string_view sv) const noexcept {
            return std::hash<std::string_view>{}(sv);
        }
        size_t operator()(const std::string &s) const noexcept {
            return operator()(std::string_view(s));
        }
    };

    class ShaderLibrary {
        public:
            ShaderLibrary();
            void add(const std::shared_ptr<NxShader> &shader);
            void add(const std::string &name, const std::shared_ptr<NxShader> &shader);
            std::shared_ptr<NxShader> load(const std::string &path);
            std::shared_ptr<NxShader> load(const std::string &name, const std::string &path);
            std::shared_ptr<NxShader> load(const std::string &name, const std::string &vertexSource, const std::string &fragmentSource);
            std::shared_ptr<NxShader> get(const std::string &name) const;

            static ShaderLibrary& getInstance() {
                static ShaderLibrary instance;
                return instance;
            }
        private:
            std::unordered_map<
                std::string,
                std::shared_ptr<NxShader>,
                TransparentStringHasher,
                std::equal_to<>
            > m_shaders;
    };
}
