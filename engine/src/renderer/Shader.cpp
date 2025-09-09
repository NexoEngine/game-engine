//// Shader.cpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for shader abstract class and shader library
//
///////////////////////////////////////////////////////////////////////////////
#include "Shader.hpp"
#include "Attributes.hpp"
#include "renderer/RendererExceptions.hpp"
#include "Logger.hpp"
#include <variant>
#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/OpenGlShader.hpp"
#endif

#include <fstream>

namespace nexo::renderer {

    std::shared_ptr<NxShader> NxShader::create(const std::string &path)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlShader>(path);
        #else
            THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
        #endif
    }

    std::shared_ptr<NxShader> NxShader::create(const std::string& name, const std::string &vertexSource, const std::string &fragmentSource)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlShader>(name, vertexSource, fragmentSource);
        #else
            THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
        #endif
    }

    std::string NxShader::readFile(const std::string &filepath)
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
        THROW_EXCEPTION(NxFileNotFoundException, filepath);
    }

    void NxShader::addStorageBuffer(const std::shared_ptr<NxShaderStorageBuffer> &buffer)
    {
        m_storageBuffers.push_back(buffer);
    }

    void NxShader::setStorageBufferData(const size_t index, void* data, const size_t size)
    {
        if (index >= m_storageBuffers.size())
            THROW_EXCEPTION(NxOutOfRangeException, index, m_storageBuffers.size());
        m_storageBuffers[index]->setData(data, size);
    }

    bool NxShader::setUniformFloat(const std::string& name, const float value) const
    {
        // Use uniform cache to avoid redundant state changes
        if (!m_uniformCache.isDirty(name)) {
            const auto optionalValue = m_uniformCache.getValue(name);
            if (optionalValue.has_value() &&
                std::holds_alternative<float>(*optionalValue) &&
                std::get<float>(*optionalValue) == value) {
                return true; // Value hasn't changed, skip the update
            }
        }

        m_uniformCache.setFloat(name, value);
        return false;
    }

    bool NxShader::setUniformFloat2(const std::string& name, const glm::vec2& values) const
    {
        if (!m_uniformCache.isDirty(name)) {
            const auto optionalValue = m_uniformCache.getValue(name);
            if (optionalValue.has_value() &&
                std::holds_alternative<glm::vec2>(*optionalValue) &&
                std::get<glm::vec2>(*optionalValue) == values) {
                return true;
            }
        }

        m_uniformCache.setFloat2(name, values);
        return false;
    }

    bool NxShader::setUniformFloat3(const std::string& name, const glm::vec3& values) const
    {
        if (!m_uniformCache.isDirty(name)) {
            const auto optionalValue = m_uniformCache.getValue(name);
            if (optionalValue.has_value() &&
                std::holds_alternative<glm::vec3>(*optionalValue) &&
                std::get<glm::vec3>(*optionalValue) == values) {
                return true;
            }
        }

        m_uniformCache.setFloat3(name, values);
        return false;
    }

    bool NxShader::setUniformFloat4(const std::string& name, const glm::vec4& values) const
    {
        if (!m_uniformCache.isDirty(name)) {
            const auto optionalValue = m_uniformCache.getValue(name);
            if (optionalValue.has_value() &&
                std::holds_alternative<glm::vec4>(*optionalValue) &&
                std::get<glm::vec4>(*optionalValue) == values) {
                return true;
            }
        }

        m_uniformCache.setFloat4(name, values);
        return false;
    }

    bool NxShader::setUniformMatrix(const std::string& name, const glm::mat4& matrix) const
    {
        if (!m_uniformCache.isDirty(name)) {
            const auto optionalValue = m_uniformCache.getValue(name);
            if (optionalValue.has_value() &&
                std::holds_alternative<glm::mat4>(*optionalValue) &&
                std::get<glm::mat4>(*optionalValue) == matrix) {
                return true;
            }
        }

        m_uniformCache.setMatrix(name, matrix);
        return false;
    }

    bool NxShader::setUniformBool(const std::string& name, bool value) const
    {
        if (!m_uniformCache.isDirty(name)) {
            const auto optionalValue = m_uniformCache.getValue(name);
            if (optionalValue.has_value() &&
                std::holds_alternative<bool>(*optionalValue) &&
                std::get<bool>(*optionalValue) == value) {
                return true;
            }
        }

        m_uniformCache.setBool(name, value);
        return false;
    }

    bool NxShader::setUniformInt(const std::string& name, int value) const
    {
        if (!m_uniformCache.isDirty(name)) {
            const auto optionalValue = m_uniformCache.getValue(name);
            if (optionalValue.has_value() &&
                std::holds_alternative<int>(*optionalValue) &&
                std::get<int>(*optionalValue) == value) {
                return true;
            }
        }

        m_uniformCache.setInt(name, value);
        return false;
    }

    bool NxShader::setUniformIntArray(
        [[maybe_unused]] const std::string& name,
        [[maybe_unused]] const int* values,
        [[maybe_unused]] unsigned int count
    ) const {
        // Arrays are more complex for caching, so we'll always update them
        // In a real implementation, you might want to cache arrays too
        return false;
    }

    bool NxShader::setUniform(const std::string &name, UniformValue value) const
    {
        return std::visit([this, &name]<typename T>(T&& arg) -> bool {
            using DecayedT = std::decay_t<T>;
            if constexpr (std::is_same_v<DecayedT, float>)
                return setUniformFloat(name, std::forward<T>(arg));
            else if constexpr (std::is_same_v<DecayedT, glm::vec2>)
                return setUniformFloat2(name, std::forward<T>(arg));
            else if constexpr (std::is_same_v<DecayedT, glm::vec3>)
                return setUniformFloat3(name, std::forward<T>(arg));
            else if constexpr (std::is_same_v<DecayedT, glm::vec4>)
                return setUniformFloat4(name, std::forward<T>(arg));
            else if constexpr (std::is_same_v<DecayedT, int>)
                return setUniformInt(name, std::forward<T>(arg));
            else if constexpr (std::is_same_v<DecayedT, bool>)
                return setUniformBool(name, std::forward<T>(arg));
            else if constexpr (std::is_same_v<DecayedT, glm::mat4>)
                return setUniformMatrix(name, std::forward<T>(arg));
            else
                return false;
        }, value);
    }

    bool NxShader::hasUniform(const std::string& name) const
    {
        return m_uniformInfos.contains(name);
    }

    bool NxShader::hasAttribute(const int location) const
    {
        return m_attributeInfos.contains(location);
    }

    bool NxShader::isCompatibleWithMesh(const RequiredAttributes& attributes) const
    {
        return attributes.compatibleWith(m_requiredAttributes);
    }

    void NxShader::resetCache()
    {
        m_uniformCache.clearAllDirtyFlags();
    }
}
