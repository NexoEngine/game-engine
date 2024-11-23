//// RendererExceptions.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Header file for renderer exceptions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Exception.hpp"

namespace nexo::renderer {
    class FileNotFoundException final : public Exception {
        public:
            explicit FileNotFoundException(const std::string &filePath, const char *file = __FILE__,
                                           const int line = __LINE__)
                : Exception("File not found: " + filePath, file, line) {}
    };

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

    class InvalidValue final : public Exception {
        public:
            explicit InvalidValue(const std::string &backendApiName, const std::string &msg,
                                  const char *file = __FILE__, const int line = __LINE__) : Exception(
                "[" + backendApiName + "] Invalid value: " + msg, file, line) {};
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

    class FramebufferResizingFailed final : public Exception {
        public:
            explicit FramebufferResizingFailed(const std::string &backendApi, const bool tooBig,
                                               const unsigned int width, const unsigned int height,
                                               const char *file = __FILE__, const int line = __LINE__) : Exception(
                "[" + backendApi + "] Framebuffer resizing failed: " + std::to_string(width) + "x" +
                std::to_string(height) + " is too " + (tooBig ? "big" : "small"), file, line) {};
    };

    class FramebufferUnsupportedColorFormat final : public Exception {
        public:
            explicit FramebufferUnsupportedColorFormat(const std::string &backendApiName, const char *file = __FILE__,
                                                   const int line = __LINE__) : Exception(
            "[" + backendApiName + "] Unsupported framebuffer color attachment format", file, line) {};
    };

    class FramebufferUnsupportedDepthFormat final : public Exception {
        public:
            explicit FramebufferUnsupportedDepthFormat(const std::string &backendApiName, const char *file = __FILE__,
                                                   const int line = __LINE__) : Exception(
            "[" + backendApiName + "] Unsupported framebuffer depth attachment format", file, line) {};
    };

    class BufferLayoutEmpty final : public Exception {
        public:
            explicit BufferLayoutEmpty(const std::string &backendApi, const char *file = __FILE__,
                                       const int line = __LINE__) : Exception(
                "[" + backendApi + "] Vertex buffer layout cannot be empty", file, line) {};
    };
}
