//// UniformCache.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        29/10/2025
//  Description: Source file for the optimized uniform cache
//
///////////////////////////////////////////////////////////////////////////////

#include "UniformCache.hpp"
#include <algorithm>
#include <cassert>
#include <cstring>

namespace nexo::renderer {

    UniformCache::UniformCache()
    {
        // Reserve initial buffer space to avoid frequent reallocations
        m_buffer.reserve(4096); // 4KB should be enough for most shaders
    }

    void UniformCache::initialize(const std::unordered_map<std::string, ShaderUniformInfo>& uniformInfos)
    {
        // Clear existing data
        m_buffer.clear();
        m_uniforms.clear();

        // Calculate total buffer size needed
        size_t totalSize = 0;
        for (const auto& [name, info] : uniformInfos) {
            size_t typeSize = getTypeSize(info.type);
            if (typeSize > 0) {
                // Align to 16-byte boundaries for better cache performance
                totalSize = (totalSize + 15) & ~15;
                totalSize += typeSize;
            }
        }

        // Reserve the calculated size
        m_buffer.resize(totalSize, 0);

        // Allocate space for each uniform
        size_t currentOffset = 0;
        for (const auto& [name, info] : uniformInfos) {
            size_t typeSize = getTypeSize(info.type);
            if (typeSize > 0) {
                // Align to 16-byte boundaries
                currentOffset = (currentOffset + 15) & ~15;

                CachedUniformInfo uniformInfo;
                uniformInfo.offset = currentOffset;
                uniformInfo.size = typeSize;
                uniformInfo.type = info.type;
                uniformInfo.location = info.location;
                uniformInfo.isDirty = false;

                m_uniforms[name] = uniformInfo;
                currentOffset += typeSize;
            }
        }
    }

    size_t UniformCache::getTypeSize(GLenum type) const
    {
        switch (type) {
            case GL_FLOAT:           return sizeof(float);
            case GL_FLOAT_VEC2:      return sizeof(glm::vec2);
            case GL_FLOAT_VEC3:      return sizeof(glm::vec3);
            case GL_FLOAT_VEC4:      return sizeof(glm::vec4);
            case GL_INT:             return sizeof(int);
            case GL_BOOL:            return sizeof(bool);
            case GL_FLOAT_MAT2:      return sizeof(float) * 4;
            case GL_FLOAT_MAT3:      return sizeof(float) * 9;
            case GL_FLOAT_MAT4:      return sizeof(glm::mat4);
            case GL_SAMPLER_2D:      return sizeof(int);
            case GL_SAMPLER_CUBE:    return sizeof(int);
            // Add more types as needed
            default:
                return 0; // Unknown type
        }
    }

    template<typename T>
    bool UniformCache::setUniformValue(const std::string& name, const T& value)
    {
        auto it = m_uniforms.find(name);
        if (it == m_uniforms.end()) {
            return false; // Uniform not found
        }

        CachedUniformInfo& uniformInfo = it->second;

        // Verify size matches expected type size
        assert(uniformInfo.size == sizeof(T));

        // Get pointer to data location in buffer
        uint8_t* dataPtr = m_buffer.data() + uniformInfo.offset;

        // Check if value has changed using memcmp for performance
        if (std::memcmp(dataPtr, &value, sizeof(T)) != 0) {
            // Value changed, update buffer
            std::memcpy(dataPtr, &value, sizeof(T));
            uniformInfo.isDirty = true;
            return true; // Value changed, needs GPU upload
        }

        return false; // Value unchanged
    }

    bool UniformCache::setFloat(const std::string& name, float value)
    {
        return setUniformValue(name, value);
    }

    bool UniformCache::setFloat2(const std::string& name, const glm::vec2& value)
    {
        return setUniformValue(name, value);
    }

    bool UniformCache::setFloat3(const std::string& name, const glm::vec3& value)
    {
        return setUniformValue(name, value);
    }

    bool UniformCache::setFloat4(const std::string& name, const glm::vec4& value)
    {
        return setUniformValue(name, value);
    }

    bool UniformCache::setInt(const std::string& name, int value)
    {
        return setUniformValue(name, value);
    }

    bool UniformCache::setBool(const std::string& name, bool value)
    {
        return setUniformValue(name, value);
    }

    bool UniformCache::setMatrix(const std::string& name, const glm::mat4& value)
    {
        return setUniformValue(name, value);
    }

    bool UniformCache::isDirty(const std::string& name) const
    {
        auto it = m_uniforms.find(name);
        return it != m_uniforms.end() && it->second.isDirty;
    }

    bool UniformCache::hasValue(const std::string& name) const
    {
        return m_uniforms.find(name) != m_uniforms.end();
    }

    void UniformCache::clearDirtyFlag(const std::string& name)
    {
        auto it = m_uniforms.find(name);
        if (it != m_uniforms.end()) {
            it->second.isDirty = false;
        }
    }

    void UniformCache::clearAllDirtyFlags()
    {
        for (auto& [name, uniform] : m_uniforms) {
            uniform.isDirty = false;
        }
    }
} // namespace nexo::renderer
