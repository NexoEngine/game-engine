//// UniformCache.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        07/05/2025
//  Description: Header file for the uniform cache
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

namespace nexo::renderer {

    using UniformValue = std::variant<float, glm::vec2, glm::vec3, glm::vec4, int, bool, glm::mat4>;

    class UniformCache {
       public:
        /**
         * @brief Sets a float uniform value.
         *
         * @param name The name of the uniform variable.
         * @param value The float value to set.
         *
         * If the value is different from the current cached value, it updates the cache and marks it as dirty.
         */
        void setFloat(const std::string& name, float value);

        /**
         * @brief Sets a 2D vector uniform value.
         *
         * @param name The name of the uniform variable.
         * @param value The vec2 value to set.
         *
         * If the value is different from the current cached value, it updates the cache and marks it as dirty.
         */
        void setFloat2(const std::string& name, const glm::vec2& value);

        /**
         * @brief Sets a 3D vector uniform value.
         *
         * @param name The name of the uniform variable.
         * @param value The vec3 value to set.
         *
         * If the value is different from the current cached value, it updates the cache and marks it as dirty.
         */
        void setFloat3(const std::string& name, const glm::vec3& value);

        /**
         * @brief Sets a 4D vector uniform value.
         *
         * @param name The name of the uniform variable.
         * @param value The vec4 value to set.
         *
         * If the value is different from the current cached value, it updates the cache and marks it as dirty.
         */
        void setFloat4(const std::string& name, const glm::vec4& value);

        /**
         * @brief Sets an integer uniform value.
         *
         * @param name The name of the uniform variable.
         * @param value The integer value to set.
         *
         * If the value is different from the current cached value, it updates the cache and marks it as dirty.
         */
        void setInt(const std::string& name, int value);

        /**
         * @brief Sets a boolean uniform value.
         *
         * @param name The name of the uniform variable.
         * @param value The boolean value to set.
         *
         * If the value is different from the current cached value, it updates the cache and marks it as dirty.
         */
        void setBool(const std::string& name, bool value);

        /**
         * @brief Sets a 4x4 matrix uniform value.
         *
         * @param name The name of the uniform variable.
         * @param value The mat4 value to set.
         *
         * If the value is different from the current cached value, it updates the cache and marks it as dirty.
         */
        void setMatrix(const std::string& name, const glm::mat4& value);

        /**
         * @brief Checks if a uniform value is marked as dirty.
         *
         * @param name The name of the uniform variable.
         * @return true if the uniform is dirty, false otherwise.
         */
        [[nodiscard]] bool isDirty(const std::string& name) const;

        /**
         * @brief Checks if a uniform value exists in the cache.
         *
         * @param name The name of the uniform variable.
         * @return true if the uniform exists in the cache, false otherwise.
         */
        [[nodiscard]] bool hasValue(const std::string& name) const;

        /**
         * @brief Retrieves a uniform value from the cache.
         *
         * @param name The name of the uniform variable.
         * @return The uniform value if it exists, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<UniformValue> getValue(const std::string& name) const;

        /**
         * @brief Clears the dirty flag for a specific uniform.
         *
         * @param name The name of the uniform variable.
         */
        void clearDirtyFlag(const std::string& name);

        /**
         * @brief Clears all dirty flags in the cache.
         */
        void clearAllDirtyFlags();

       private:
        std::unordered_map<std::string, UniformValue> m_values;
        std::unordered_map<std::string, bool> m_dirtyFlags;
    };

} // namespace nexo::renderer
