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

#include <source_location>

namespace nexo::renderer {
    class FileNotFoundException final : public Exception {
        public:
            explicit FileNotFoundException(const std::string &filePath,
                                           const std::source_location loc = std::source_location::current())
                : Exception("File not found: " + filePath, loc) {}
    };

    class UnknownGraphicsApi final : public Exception {
        public:
            explicit UnknownGraphicsApi(const std::string &backendApiName,
                                        const std::source_location loc = std::source_location::current())
                : Exception("Unknown graphics API: " + backendApiName, loc) {}
    };

    class GraphicsApiInitFailure final : public Exception {
        public:
            explicit GraphicsApiInitFailure(const std::string &backendApiName,
                                            const std::source_location loc = std::source_location::current())
                : Exception("Failed to initialize graphics api: " + backendApiName, loc) {}
    };

    class GraphicsApiNotInitialized final : public Exception {
        public:
            explicit GraphicsApiNotInitialized(const std::string &backendApiName,
                                               const std::source_location loc = std::source_location::current())
                : Exception("[" + backendApiName + "] Api is not initialized, call the init function first", loc)
            {}
    };

    class GraphicsApiViewportResizingFailure final : public Exception {
        public:
            explicit GraphicsApiViewportResizingFailure(const std::string &backendApi, const bool tooBig,
                                                        const unsigned int width, const unsigned int height,
                                                        const std::source_location loc =
                                                                std::source_location::current()) : Exception(
                "[" + backendApi + "] Viewport resizing failed: " + std::to_string(width) + "x" +
                std::to_string(height) + " is too " + (tooBig ? "big" : "small"), loc) {};
    };

    class GraphicsApiWindowInitFailure final : public Exception {
        public:
            explicit GraphicsApiWindowInitFailure(const std::string &backendApiName,
                                                  const std::source_location loc = std::source_location::current())
                : Exception("Failed to initialize graphics api: " + backendApiName, loc) {}
    };

    class InvalidValue final : public Exception {
        public:
            explicit InvalidValue(const std::string &backendApiName, const std::string &msg,
                                  const std::source_location loc = std::source_location::current()) : Exception(
                "[" + backendApiName + "] Invalid value: " + msg, loc) {};
    };

    class ShaderCreationFailed final : public Exception {
        public:
            explicit ShaderCreationFailed(const std::string &backendApi, const std::string &message,
                                          const std::string &path = "",
                                          const std::source_location loc = std::source_location::current())
                : Exception("[" + backendApi + "] Failed to create the shader (" + path + "): " + message, loc)
            {}
    };

    class ShaderInvalidUniform final : public Exception {
        public:
            explicit ShaderInvalidUniform(const std::string &backendApi, const std::string &shaderName,
                                          const std::string &uniformName,
                                          const std::source_location loc = std::source_location::current()) : Exception(
                "[" + backendApi + "] Failed to retrieve uniform \"" + uniformName + "\" in shader: " + shaderName,
                loc) {};
    };

    class FramebufferCreationFailed final : public Exception {
        public:
            explicit FramebufferCreationFailed(const std::string &backendApi,
                                               const std::source_location loc = std::source_location::current())
                : Exception("[" + backendApi + "] Failed to create the framebuffer", loc)
            {}
    };

    class FramebufferResizingFailed final : public Exception {
        public:
            explicit FramebufferResizingFailed(const std::string &backendApi, const bool tooBig,
                                               const unsigned int width, const unsigned int height,
                                               const std::source_location loc =
                                                       std::source_location::current()) : Exception(
                "[" + backendApi + "] Framebuffer resizing failed: " + std::to_string(width) + "x" +
                std::to_string(height) + " is too " + (tooBig ? "big" : "small"), loc) {};
    };

    class FramebufferUnsupportedColorFormat final : public Exception {
        public:
            explicit FramebufferUnsupportedColorFormat(const std::string &backendApiName,
                                                       const std::source_location loc =
                                                               std::source_location::current()) : Exception(
                "[" + backendApiName + "] Unsupported framebuffer color attachment format", loc) {};
    };

    class FramebufferUnsupportedDepthFormat final : public Exception {
        public:
            explicit FramebufferUnsupportedDepthFormat(const std::string &backendApiName,
                                                       const std::source_location loc =
                                                               std::source_location::current()) : Exception(
                "[" + backendApiName + "] Unsupported framebuffer depth attachment format", loc) {};
    };

    class BufferLayoutEmpty final : public Exception {
        public:
            explicit BufferLayoutEmpty(const std::string &backendApi,
                                       const std::source_location loc = std::source_location::current()) : Exception(
                "[" + backendApi + "] Vertex buffer layout cannot be empty", loc) {};
    };

    enum class RendererType {
        RENDERER_2D,
        RENDERER_3D
    };

    class RendererNotInitialized final : public Exception {
        public:
            explicit RendererNotInitialized(const RendererType type,
                                            const std::source_location loc =
                                                    std::source_location::current()) : Exception(
                std::string((type == RendererType::RENDERER_2D ? "[RENDERER 2D]" : "[RENDERER 3D]")) +
                " Renderer not initialized, call the init function first", loc) {};
    };

    class RendererSceneLifeCycleFailure : public Exception {
        public:
            explicit RendererSceneLifeCycleFailure(const RendererType type, const std::string &msg,
                                                   const std::source_location loc =
                                                           std::source_location::current()) : Exception(
                std::string((type == RendererType::RENDERER_2D ? "[RENDERER 2D]" : "[RENDERER 3D]")) + " " + msg, loc)
            {};
    };

    class TextureInvalidSize : public Exception {
        public:
            explicit TextureInvalidSize(const std::string &backendApi,
                                        const unsigned int width, const unsigned int height,
                                        const unsigned int maxTextureSize,
                                        const std::source_location loc = std::source_location::current()) : Exception(
                "[" + backendApi + "] Invalid size for texture: " + std::to_string(width) + "x" +
                std::to_string(height) + " is too big, max texture size is : " + std::to_string(maxTextureSize),
                loc) {};
    };

    class TextureUnsupportedFormat : public Exception {
        public:
            explicit TextureUnsupportedFormat(const std::string &backendApi, const int channels,
                                              const std::string &path,
                                              const std::source_location loc =
                                                      std::source_location::current()) : Exception(
                "[" + backendApi + "] Unsupported image format with " + std::to_string(channels) + " channels in " +
                path, loc) {};
    };

    class TextureSizeMismatch : public Exception {
        public:
            explicit TextureSizeMismatch(const std::string &backendApi, const int dataSize, const int expectedSize,
                                         const std::source_location loc = std::source_location::current()) : Exception(
                "[" + backendApi + "] Data size does not match the texture size: " + std::to_string(dataSize) + " != " +
                std::to_string(expectedSize), loc) {};
    };

    class StbiLoadException : public Exception {
        public:
            explicit StbiLoadException(const std::string &msg,
                                       const std::source_location loc = std::source_location::current())
                : Exception("STBI load failed: " + msg, loc) {}
    };
}
