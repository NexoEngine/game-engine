//// Exceptions.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header for the nexo exceptions abstract class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

#include "Exception.hpp"

namespace nexo::core {
    class UnknownGraphicsApi final : public Exception {
        public:
            explicit UnknownGraphicsApi(const std::string &backendApiName, const char *file = __FILE__,
                                        const int line = __LINE__)
                : Exception("Unknown graphics API: " + backendApiName, file, line) {}
    };

    class GraphicsApiInitFailure final : public Exception {
        public:
            explicit GraphicsApiInitFailure(const std::string &backendApiName, const char *file = __FILE__,
                                            const int line = __LINE__)
                : Exception("Failed to initialize graphics api: " + backendApiName, file, line) {}
    };

    class GraphicsApiWindowInitFailure final : public Exception {
        public:
            explicit GraphicsApiWindowInitFailure(const std::string &backendApiName, const char *file = __FILE__,
                                                  const int line = __LINE__)
                : Exception("Failed to initialize graphics api: " + backendApiName, file, line) {}
    };

    class FileNotFoundException final : public Exception {
        public:
            explicit FileNotFoundException(const std::string &filePath, const char *file = __FILE__,
                                           const int line = __LINE__)
                : Exception("File not found: " + filePath, file, line) {}
    };

    class ShaderCreationFailed final : public Exception {
        public:
            explicit ShaderCreationFailed(const std::string &backendApi, const std::string &message,
                                          const std::string &path = "", const char *file = __FILE__,
                                          const int line = __LINE__)
                : Exception("[" + backendApi + "] Failed to create the shader (" + path + "): " + message, file, line)
            {}
    };

    class FramebufferCreationFailed final : public Exception {
        public:
        explicit FramebufferCreationFailed(const std::string &backendApi, const char *file = __FILE__,
                                      const int line = __LINE__)
            : Exception("[" + backendApi + "] Failed to create the framebuffer", file, line)
        {}
    };
}
