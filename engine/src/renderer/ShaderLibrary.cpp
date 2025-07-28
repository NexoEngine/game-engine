//// ShaderLibrary.cpp ////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
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
        // Helper lambda to safely load a shader with proper error handling
        auto safeLoadShader = [this](const std::string& name, const std::string& relativePath) {
            try {
                // Resolve the absolute path
                const std::filesystem::path absPath = Path::resolvePathRelativeToExe(relativePath);

                // Check if the shader file exists
                if (!std::filesystem::exists(absPath)) {
                    LOG(NEXO_ERROR, "Shader file not found: {}", absPath.string());
                    return false;
                }

                // Try to load the shader
                load(name, absPath.string());
                LOG(NEXO_INFO, "Shader '{}' loaded successfully", name);
                return true;
            } catch (const std::exception& e) {
                LOG(NEXO_ERROR, "Failed to load shader '{}': {}", name, e.what());
                return false;
            } catch (...) {
                LOG(NEXO_ERROR, "Unknown error loading shader '{}'", name);
                return false;
            }
        };

        // Load all required shaders with error handling
        safeLoadShader("Phong", "../resources/shaders/phong.glsl");
        safeLoadShader("Outline pulse flat", "../resources/shaders/outline_pulse_flat.glsl");
        safeLoadShader("Outline pulse transparent flat", "../resources/shaders/outline_pulse_transparent_flat.glsl");
        safeLoadShader("Albedo unshaded transparent", "../resources/shaders/albedo_unshaded_transparent.glsl");
        safeLoadShader("Grid shader", "../resources/shaders/grid_shader.glsl");
        safeLoadShader("Flat color", "../resources/shaders/flat_color.glsl");
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
