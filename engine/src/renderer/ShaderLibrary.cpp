//// ShaderLibrary.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        19/04/2025
//  Description: Source file for the shader library
//
///////////////////////////////////////////////////////////////////////////////

#include "ShaderLibrary.hpp"
#include "Logger.hpp"

namespace nexo::renderer {
    void ShaderLibrary::add(const std::shared_ptr<NxShader> &shader)
    {
        const std::string &name = shader->getName();
        m_shaders[name] = shader;
    }

    void ShaderLibrary::add(const std::string &name, const std::shared_ptr<NxShader> &shader)
    {
        m_shaders[name] = shader;
    }

    std::shared_ptr<NxShader> ShaderLibrary::load(const std::string &name, const std::string &path)
    {
        auto shader = NxShader::create(path);
        add(name, shader);
        return shader;
    }

    std::shared_ptr<NxShader> ShaderLibrary::load(const std::string &path)
    {
        auto shader = NxShader::create(path);
        add(shader);
        return shader;
    }

    std::shared_ptr<NxShader> ShaderLibrary::load(const std::string &name, const std::string &vertexSource, const std::string &fragmentSource)
    {
        auto shader = NxShader::create(name, vertexSource, fragmentSource);
        add(shader);
        return shader;
    }

    std::shared_ptr<NxShader> ShaderLibrary::get(const std::string &name) const
    {
        if (!m_shaders.contains(name))
        {
            LOG(NEXO_WARN, "ShaderLibrary::get: shader {} not found", name);
            return nullptr;
        }
        return m_shaders.at(name);
    }
}
