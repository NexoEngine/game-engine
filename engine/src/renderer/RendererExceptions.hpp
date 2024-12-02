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
#include <format>

namespace nexo::renderer {
    class FileNotFoundException final : public Exception {
        public:
            explicit FileNotFoundException(const std::string &filePath,
                                           const std::source_location loc = std::source_location::current())
                : Exception(std::format("File not found: {}", filePath), loc) {}
    };

    class UnknownGraphicsApi final : public Exception {
        public:
            explicit UnknownGraphicsApi(const std::string &backendApiName,
                                        const std::source_location loc = std::source_location::current())
                : Exception(std::format("Unknown graphics API: {}", backendApiName), loc) {}
    };

    class GraphicsApiInitFailure final : public Exception {
        public:
            explicit GraphicsApiInitFailure(const std::string &backendApiName,
                                            const std::source_location loc = std::source_location::current())
                : Exception(std::format("Failed to initialize graphics API: {}", backendApiName), loc) {}
    };

    class GraphicsApiNotInitialized final : public Exception {
        public:
            explicit GraphicsApiNotInitialized(const std::string &backendApiName,
                                               const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] API is not initialized, call the init function first", backendApiName),
                            loc)
            {}
    };

    class GraphicsApiViewportResizingFailure final : public Exception {
        public:
            explicit GraphicsApiViewportResizingFailure(const std::string &backendApi, const bool tooBig,
                                                        const unsigned int width, const unsigned int height,
                                                        const std::source_location loc =
                                                                std::source_location::current())
                : Exception(std::format("[{}] Viewport resizing failed: {}x{} is too {}",
                                        backendApi, width, height, (tooBig ? "big" : "small")), loc) {}
    };

    class GraphicsApiWindowInitFailure final : public Exception {
        public:
            explicit GraphicsApiWindowInitFailure(const std::string &backendApiName,
                                                  const std::source_location loc = std::source_location::current())
                : Exception(std::format("Failed to initialize graphics API: {}", backendApiName), loc) {}
    };

    class InvalidValue final : public Exception {
        public:
            explicit InvalidValue(const std::string &backendApiName, const std::string &msg,
                                  const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Invalid value: {}", backendApiName, msg), loc) {}
    };

    class ShaderCreationFailed final : public Exception {
        public:
            explicit ShaderCreationFailed(const std::string &backendApi, const std::string &message,
                                          const std::string &path = "",
                                          const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Failed to create the shader ({}): {}", backendApi, path, message), loc)
            {}
    };

    class ShaderInvalidUniform final : public Exception {
        public:
            explicit ShaderInvalidUniform(const std::string &backendApi, const std::string &shaderName,
                                          const std::string &uniformName,
                                          const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Failed to retrieve uniform \"{}\" in shader: {}", backendApi, uniformName,
                                        shaderName), loc) {}
    };

    class FramebufferCreationFailed final : public Exception {
        public:
            explicit FramebufferCreationFailed(const std::string &backendApi,
                                               const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Failed to create the framebuffer", backendApi), loc)
            {}
    };

    class FramebufferResizingFailed final : public Exception {
        public:
            explicit FramebufferResizingFailed(const std::string &backendApi, const bool tooBig,
                                               const unsigned int width, const unsigned int height,
                                               const std::source_location loc =
                                                       std::source_location::current())
                : Exception(std::format("[{}] Framebuffer resizing failed: {}x{} is too {}",
                                        backendApi, width, height, (tooBig ? "big" : "small")), loc) {}
    };

    class FramebufferUnsupportedColorFormat final : public Exception {
        public:
            explicit FramebufferUnsupportedColorFormat(const std::string &backendApiName,
                                                       const std::source_location loc =
                                                               std::source_location::current())
                : Exception(std::format("[{}] Unsupported framebuffer color attachment format", backendApiName), loc) {}
    };

    class FramebufferUnsupportedDepthFormat final : public Exception {
        public:
            explicit FramebufferUnsupportedDepthFormat(const std::string &backendApiName,
                                                       const std::source_location loc =
                                                               std::source_location::current())
                : Exception(std::format("[{}] Unsupported framebuffer depth attachment format", backendApiName), loc) {}
    };

    class BufferLayoutEmpty final : public Exception {
        public:
            explicit BufferLayoutEmpty(const std::string &backendApi,
                                       const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Vertex buffer layout cannot be empty", backendApi), loc) {}
    };

    enum class RendererType {
        RENDERER_2D,
        RENDERER_3D
    };

    class RendererNotInitialized final : public Exception {
        public:
            explicit RendererNotInitialized(const RendererType type,
                                            const std::source_location loc =
                                                    std::source_location::current())
                : Exception(std::format("{} Renderer not initialized, call the init function first",
                                        (type == RendererType::RENDERER_2D ? "[RENDERER 2D]" : "[RENDERER 3D]")), loc)
            {}
    };

    class RendererSceneLifeCycleFailure : public Exception {
        public:
            explicit RendererSceneLifeCycleFailure(const RendererType type, const std::string &msg,
                                                   const std::source_location loc =
                                                           std::source_location::current())
                : Exception(std::format("{} {}",
                                        (type == RendererType::RENDERER_2D ? "[RENDERER 2D]" : "[RENDERER 3D]"), msg),
                            loc) {}
    };

    class TextureInvalidSize final : public Exception {
        public:
            explicit TextureInvalidSize(const std::string &backendApi,
                                        const unsigned int width, const unsigned int height,
                                        const unsigned int maxTextureSize,
                                        const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Invalid size for texture: {}x{} is too big, max texture size is : {}",
                                        backendApi, width, height, maxTextureSize), loc) {}
    };

    class TextureUnsupportedFormat final : public Exception {
        public:
            explicit TextureUnsupportedFormat(const std::string &backendApi, const int channels,
                                              const std::string &path,
                                              const std::source_location loc =
                                                      std::source_location::current())
                : Exception(std::format("[{}] Unsupported image format with {} channels in {}",
                                        backendApi, channels, path), loc) {}
    };

    class TextureSizeMismatch final : public Exception {
        public:
            explicit TextureSizeMismatch(const std::string &backendApi, const int dataSize, const int expectedSize,
                                         const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Data size does not match the texture size: {} != {}",
                                        backendApi, dataSize, expectedSize), loc) {}
    };

    class StbiLoadException final : public Exception {
        public:
            explicit StbiLoadException(const std::string &msg,
                                       const std::source_location loc = std::source_location::current())
                : Exception(std::format("STBI load failed: {}", msg), loc) {}
    };
}
