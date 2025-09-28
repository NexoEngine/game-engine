//// ShaderLibrary.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        19/04/2025
//  Description: Header file for the shader library
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <unordered_map>
#include "Shader.hpp"

namespace nexo::renderer {

    struct TransparentStringHasher {
        using is_transparent = void; // enable heterogeneous lookup

        size_t operator()(const std::string_view sv) const noexcept
        {
            return std::hash<std::string_view>{}(sv);
        }
        size_t operator()(const std::string &s) const noexcept
        {
            return operator()(std::string_view(s));
        }
    };

    class ShaderLibrary {
       private:
        // Singleton: private constructor and destructor
        ShaderLibrary();
        ~ShaderLibrary() = default;

       public:
        /**
         * @brief Adds a shader to the library.
         * @param shader Shared pointer to the shader to add.
         */
        void add(const std::shared_ptr<NxShader> &shader);

        /** @brief Adds a shader with a specific name to the library.
         * @param name Name to associate with the shader.
         * @param shader Shared pointer to the shader to add.
         */
        void add(const std::string &name, const std::shared_ptr<NxShader> &shader);

        /**
         * @brief Loads a shader from a file path and adds it to the library.
         * The shader name is derived from the file name.
         * @param path File path to load the shader from.
         * @return Shared pointer to the loaded shader.
         */
        std::shared_ptr<NxShader> load(const std::string &path);

        /**
         * @brief Loads a shader from a file path with a specific name.
         * @param name Custom name to associate with the shader.
         * @param path File path to load the shader from.
         * @return Shared pointer to the loaded shader.
         */
        std::shared_ptr<NxShader> load(const std::string &name, const std::string &path);

        /**
         * @brief Creates a shader from source code strings.
         * @param name Name to associate with the shader.
         * @param vertexSource Source code for the vertex shader.
         * @param fragmentSource Source code for the fragment shader.
         * @return Shared pointer to the created shader.
         */
        std::shared_ptr<NxShader> load(const std::string &name, const std::string &vertexSource,
                                       const std::string &fragmentSource);

        /**
         * @brief Retrieves a shader from the library by name.
         * @param name Name of the shader to retrieve.
         * @return Shared pointer to the requested shader.
         * @throw std::runtime_error if shader not found.
         */
        [[nodiscard]] std::shared_ptr<NxShader> get(const std::string &name) const;

        /**
         * @brief Gets the singleton instance of the shader library.
         * @return Reference to the shader library instance.
         */
        static ShaderLibrary &getInstance()
        {
            static ShaderLibrary instance;
            return instance;
        }

        // Delete copy constructor and assignment operator to ensure singleton pattern
        ShaderLibrary(ShaderLibrary const &)  = delete;
        void operator=(ShaderLibrary const &) = delete;

       private:
        std::unordered_map<std::string, std::shared_ptr<NxShader>, TransparentStringHasher, std::equal_to<> > m_shaders;
    };
} // namespace nexo::renderer
