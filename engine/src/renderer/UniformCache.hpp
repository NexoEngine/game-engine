//// UniformCache.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/05/2025
//  Description: Header file for the uniform cache
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <glm/glm.hpp>
#include <optional>

namespace nexo::renderer {

    using UniformValue = std::variant<
        float,
        glm::vec2,
        glm::vec3,
        glm::vec4,
        int,
        bool,
        glm::mat4
    >;

    class UniformCache {
    public:
        void setFloat(const std::string& name, float value);
        void setFloat2(const std::string& name, const glm::vec2& value);
        void setFloat3(const std::string& name, const glm::vec3& value);
        void setFloat4(const std::string& name, const glm::vec4& value);
        void setInt(const std::string& name, int value);
        void setBool(const std::string& name, bool value);
        void setMatrix(const std::string& name, const glm::mat4& value);

        bool isDirty(const std::string& name) const;
        bool hasValue(const std::string& name) const;
        std::optional<UniformValue> getValue(const std::string& name) const;
        void clearDirtyFlag(const std::string& name);
        void clearAllDirtyFlags();

    private:
        std::unordered_map<std::string, UniformValue> m_values;
        std::unordered_map<std::string, bool> m_dirtyFlags;
    };

}
