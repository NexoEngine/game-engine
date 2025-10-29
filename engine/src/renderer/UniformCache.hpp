//// UniformCache.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        29/10/2025
//  Description: Header file for the optimized uniform cache
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <glad/glad.h>

namespace nexo::renderer {

    struct CachedUniformInfo {
        size_t offset;      // Offset in the main buffer
        size_t size;        // Size in bytes
        GLenum type;        // OpenGL type (GL_FLOAT, GL_FLOAT_VEC3, etc.)
        int location;       // OpenGL uniform location
        bool isDirty;       // Dirty flag
    };

    // Helper struct to match shader reflection data
    struct ShaderUniformInfo {
        std::string name;
        GLenum type;
        GLint size;
        GLint location;
    };

    class UniformCache {
    public:
        UniformCache();
        ~UniformCache() = default;

        // Initialize the cache with uniform information from shader reflection
        void initialize(const std::unordered_map<std::string, ShaderUniformInfo>& uniformInfos);

        // Uniform setters - return true if value changed and needs GPU upload
        bool setFloat(const std::string& name, float value);
        bool setFloat2(const std::string& name, const glm::vec2& value);
        bool setFloat3(const std::string& name, const glm::vec3& value);
        bool setFloat4(const std::string& name, const glm::vec4& value);
        bool setInt(const std::string& name, int value);
        bool setBool(const std::string& name, bool value);
        bool setMatrix(const std::string& name, const glm::mat4& value);

        // Utility functions
        bool isDirty(const std::string& name) const;
        bool hasValue(const std::string& name) const;
        void clearDirtyFlag(const std::string& name);
        void clearAllDirtyFlags();

        // Debug/stats
        size_t getTotalBufferSize() const { return m_buffer.size(); }
        size_t getUniformCount() const { return m_uniforms.size(); }

    private:
        std::vector<uint8_t> m_buffer;                                  // Single contiguous buffer
        std::unordered_map<std::string, CachedUniformInfo> m_uniforms;  // Name -> uniform info

        // Helper functions
        size_t getTypeSize(GLenum type) const;

        template<typename T>
        bool setUniformValue(const std::string& name, const T& value);
    };
} // namespace nexo::renderer
