//// Shader.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for shader abstract class and shader library
//
///////////////////////////////////////////////////////////////////////////////
#include "Shader.hpp"
#include "core/exceptions/Exceptions.hpp"
#ifdef GRAPHICS_API_OPENGL
    #include "opengl/OpenGlShader.hpp"
#endif

#include <fstream>

namespace nexo::renderer {

    std::shared_ptr<Shader> Shader::create(const std::string &path)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlShader>(path);
        #endif
        THROW_EXCEPTION(core::UnknownGraphicsApi, "UNKNOWN");

    }

    std::shared_ptr<Shader> Shader::create(std::string name, const std::string &vertexSource, const std::string &fragmentSource)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlShader>(name, vertexSource, fragmentSource);
        #endif
        THROW_EXCEPTION(core::UnknownGraphicsApi, "UNKNOWN");

    }

    std::string Shader::readFile(const std::string &filepath)
    {
        std::string result;
        if (std::ifstream in(filepath, std::ios::in | std::ios::binary); in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
            return result;
        }
        std::cerr << "Failed to open file " << filepath << std::endl;
        return result;
    }

    void ShaderLibrary::add(const std::shared_ptr<Shader> &shader)
    {
        const std::string &name = shader->getName();
        m_shaders[name] = shader;
    }

    void ShaderLibrary::add(const std::string &name, const std::shared_ptr<Shader> &shader)
    {
        m_shaders[name] = shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::load(const std::string &name, const std::string &path)
    {
        auto shader = Shader::create(path);
        add(name, shader);
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::load(const std::string &path)
    {
        auto shader = Shader::create(path);
        add(shader);
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::load(const std::string &name, const std::string &vertexSource, const std::string &fragmentSource)
    {
        auto shader = Shader::create(name, vertexSource, fragmentSource);
        add(shader);
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::get(const std::string &name) const
    {
        if (m_shaders.find(name) == m_shaders.end())
        {
            std::cerr << "Shader not found: " << name << std::endl;
            return nullptr;
        }
        return m_shaders.at(name);
    }

}
