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

    class GraphicsApiNotInitialized final : public Exception {
        public:
            explicit GraphicsApiNotInitialized(const std::string &backendApiName, const char *file = __FILE__,
                                               const int line = __LINE__)
                : Exception("[" + backendApiName + "] Api is not initialized, call the init function first", file, line)
            {}
    };

    class GraphicsApiViewportResizingFailure final : public Exception {
        public:
            explicit GraphicsApiViewportResizingFailure(const std::string &backendApi, const bool tooBig,
                                                        const unsigned int width, const unsigned int height,
                                                        const char *file = __FILE__,
                                                        const int line = __LINE__) : Exception(
                "[" + backendApi + "] Viewport resizing failed: " + std::to_string(width) + "x" +
                std::to_string(height) + " is too " + (tooBig ? "big" : "small"), file, line) {};
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

    class ShaderInvalidUniform final : public Exception {
        public:
            explicit ShaderInvalidUniform(const std::string &backendApi, const std::string &shaderName,
                                          const std::string &uniformName, const char *file = __FILE__,
                                          const int line = __LINE__) : Exception(
                "[" + backendApi + "] Failed to retrieve uniform \"" + uniformName + "\" in shader: " + shaderName,
                file, line) {};
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

    enum class RendererType {
        RENDERER_2D,
        RENDERER_3D
    };

    class RendererNotInitialized final : public Exception {
        public:
            explicit RendererNotInitialized(const RendererType type, const char *file = __FILE__,
                                            const int line = __LINE__) : Exception(
                std::string((type == RendererType::RENDERER_2D ? "[RENDERER 2D]" : "[RENDERER 3D]")) +
                " Renderer not initialized, call the init function first", file, line) {};
    };

    class RendererSceneLifeCycleFailure : public Exception {
        public:
            explicit RendererSceneLifeCycleFailure(const RendererType type, const std::string &msg,
                                                   const char *file = __FILE__, const int line = __LINE__) : Exception(
                std::string((type == RendererType::RENDERER_2D ? "[RENDERER 2D]" : "[RENDERER 3D]")) + " " + msg, file,
                line) {};
    };

    class TextureInvalidSize : public Exception {
        public:
            explicit TextureInvalidSize(const std::string &backendApi,
                                        const unsigned int width, const unsigned int height,
                                        const unsigned int maxTextureSize,
                                        const char *file = __FILE__, const int line = __LINE__) : Exception(
                "[" + backendApi + "] Invalid size for texture: " + std::to_string(width) + "x" +
                std::to_string(height) + " is too big, max texture size is : " + std::to_string(maxTextureSize), file,
                line) {};
    };

    class TextureUnsupportedFormat : public Exception {
        public:
            explicit TextureUnsupportedFormat(const std::string &backendApi, const int channels,
                                              const std::string &path,
                                              const char *file = __FILE__, const int line = __LINE__) : Exception(
                "[" + backendApi + "] Unsupported image format with " + std::to_string(channels) + " channels in " +
                path, file,
                line) {};
    };

    class TextureSizeMismatch : public Exception {
        public:
            explicit TextureSizeMismatch(const std::string &backendApi, const int dataSize, const int expectedSize,
                                         const char *file = __FILE__, const int line = __LINE__) : Exception(
                "[" + backendApi + "] Data size does not match the texture size: " + std::to_string(dataSize) + " != " +
                std::to_string(expectedSize), file, line) {};
    };

    class StbiLoadException : public Exception {
        public:
            explicit StbiLoadException(const std::string &msg, const char *file = __FILE__, const int line = __LINE__)
                : Exception("STBI load failed: " + msg, file, line) {}
    };
}
