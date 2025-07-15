//// ShaderLibrary.cpp ////////////////////////////////////////////////////////
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
#include "Path.hpp"

namespace nexo::renderer {

    ShaderLibrary::ShaderLibrary()
    {
        load("Phong", Path::resolvePathRelativeToExe(
            "../resources/shaders/phong.glsl").string());
        load("Outline pulse flat", Path::resolvePathRelativeToExe(
            "../resources/shaders/outline_pulse_flat.glsl").string());
        load("Outline pulse transparent flat", Path::resolvePathRelativeToExe(
            "../resources/shaders/outline_pulse_transparent_flat.glsl").string());
        load("Albedo unshaded transparent", Path::resolvePathRelativeToExe(
            "../resources/shaders/albedo_unshaded_transparent.glsl").string());
        load("Grid shader", Path::resolvePathRelativeToExe(
            "../resources/shaders/grid_shader.glsl").string());
        load("Flat color", Path::resolvePathRelativeToExe(
            "../resources/shaders/flat_color.glsl").string());
    }

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
