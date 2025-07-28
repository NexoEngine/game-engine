//// UniformCache.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/05/2025
//  Description: Source file for the open gl uniform cache
//
///////////////////////////////////////////////////////////////////////////////

#include "UniformCache.hpp"

namespace nexo::renderer {

    void UniformCache::setFloat(const std::string& name, float value)
    {
        const auto it = m_values.find(name);
        if (it == m_values.end() || !std::holds_alternative<float>(it->second) || std::get<float>(it->second) != value) {
            m_values[name] = value;
            m_dirtyFlags[name] = true;
        }
    }

    void UniformCache::setFloat2(const std::string& name, const glm::vec2& value)
    {
        const auto it = m_values.find(name);
        if (it == m_values.end() || !std::holds_alternative<glm::vec2>(it->second) || std::get<glm::vec2>(it->second) != value) {
            m_values[name] = value;
            m_dirtyFlags[name] = true;
        }
    }

    void UniformCache::setFloat3(const std::string& name, const glm::vec3& value)
    {
        const auto it = m_values.find(name);
        if (it == m_values.end() || !std::holds_alternative<glm::vec3>(it->second) || std::get<glm::vec3>(it->second) != value) {
            m_values[name] = value;
            m_dirtyFlags[name] = true;
        }
    }

    void UniformCache::setFloat4(const std::string& name, const glm::vec4& value)
    {
        const auto it = m_values.find(name);
        if (it == m_values.end() || !std::holds_alternative<glm::vec4>(it->second) || std::get<glm::vec4>(it->second) != value) {
            m_values[name] = value;
            m_dirtyFlags[name] = true;
        }
    }

    void UniformCache::setInt(const std::string& name, int value)
    {
        const auto it = m_values.find(name);
        if (it == m_values.end() || !std::holds_alternative<int>(it->second) || std::get<int>(it->second) != value) {
            m_values[name] = value;
            m_dirtyFlags[name] = true;
        }
    }

    void UniformCache::setBool(const std::string& name, bool value)
    {
        const auto it = m_values.find(name);
        if (it == m_values.end() || !std::holds_alternative<bool>(it->second) || std::get<bool>(it->second) != value) {
            m_values[name] = value;
            m_dirtyFlags[name] = true;
        }
    }

    void UniformCache::setMatrix(const std::string& name, const glm::mat4& value)
    {
        const auto it = m_values.find(name);
        if (it == m_values.end() || !std::holds_alternative<glm::mat4>(it->second) || std::get<glm::mat4>(it->second) != value) {
            m_values[name] = value;
            m_dirtyFlags[name] = true;
        }
    }

    bool UniformCache::isDirty(const std::string& name) const
    {
        const auto it = m_dirtyFlags.find(name);
        return it != m_dirtyFlags.end() && it->second;
    }

    bool UniformCache::hasValue(const std::string& name) const
    {
        return m_values.contains(name);
    }

    std::optional<UniformValue> UniformCache::getValue(const std::string& name) const
    {
        const auto it = m_values.find(name);
        return (it != m_values.end()) ? std::optional<UniformValue>(it->second) : std::nullopt;
    }

    void UniformCache::clearDirtyFlag(const std::string& name)
    {
        m_dirtyFlags[name] = false;
    }

    void UniformCache::clearAllDirtyFlags()
    {
        for (auto& [name, dirty] : m_dirtyFlags) {
            dirty = false;
        }
    }
}
