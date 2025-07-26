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

	class NxOutOfRangeException final : public Exception {
        public:
            explicit NxOutOfRangeException(size_t index, size_t size,
                                         const std::source_location loc = std::source_location::current())
                : Exception(std::format("Index {} is out of range [0, {})", index, size), loc) {}
    };

    class NxFileNotFoundException final : public Exception {
        public:
            explicit NxFileNotFoundException(const std::string &filePath,
                                           const std::source_location loc = std::source_location::current())
                : Exception(std::format("File not found: {}", filePath), loc) {}
    };

    class NxUnknownGraphicsApi final : public Exception {
        public:
            explicit NxUnknownGraphicsApi(const std::string &backendApiName,
                                        const std::source_location loc = std::source_location::current())
                : Exception(std::format("Unknown graphics API: {}", backendApiName), loc) {}
    };

    class NxGraphicsApiInitFailure final : public Exception {
        public:
            explicit NxGraphicsApiInitFailure(const std::string &backendApiName,
                                            const std::source_location loc = std::source_location::current())
                : Exception(std::format("Failed to initialize graphics API: {}", backendApiName), loc) {}
    };

    class NxGraphicsApiNotInitialized final : public Exception {
        public:
            explicit NxGraphicsApiNotInitialized(const std::string &backendApiName,
                                               const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] API is not initialized, call the init function first", backendApiName),
                            loc)
            {}
    };

    class NxGraphicsApiViewportResizingFailure final : public Exception {
        public:
            explicit NxGraphicsApiViewportResizingFailure(const std::string &backendApi, const bool tooBig,
                                                        const unsigned int width, const unsigned int height,
                                                        const std::source_location loc =
                                                                std::source_location::current())
                : Exception(std::format("[{}] Viewport resizing failed: {}x{} is too {}",
                                        backendApi, width, height, (tooBig ? "big" : "small")), loc) {}
    };

    class NxGraphicsApiWindowInitFailure final : public Exception {
        public:
            explicit NxGraphicsApiWindowInitFailure(const std::string &backendApiName,
                                                  const std::source_location loc = std::source_location::current())
                : Exception(std::format("Failed to initialize graphics API: {}", backendApiName), loc) {}
    };

    class NxInvalidValue final : public Exception {
        public:
            explicit NxInvalidValue(const std::string &backendApiName, const std::string &msg,
                                  const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Invalid value: {}", backendApiName, msg), loc) {}
    };

    class NxShaderCreationFailed final : public Exception {
        public:
            explicit NxShaderCreationFailed(const std::string &backendApi, const std::string &message,
                                          const std::string &path = "",
                                          const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Failed to create the shader ({}): {}", backendApi, path, message), loc)
            {}
    };

    class NxShaderInvalidUniform final : public Exception {
        public:
            explicit NxShaderInvalidUniform(const std::string &backendApi, const std::string &shaderName,
                                          const std::string &uniformName,
                                          const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Failed to retrieve uniform \"{}\" in shader: {}", backendApi, uniformName,
                                        shaderName), loc) {}
    };

    class NxFramebufferCreationFailed final : public Exception {
        public:
            explicit NxFramebufferCreationFailed(const std::string &backendApi,
                                               const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Failed to create the framebuffer", backendApi), loc)
            {}
    };

    class NxFramebufferResizingFailed final : public Exception {
        public:
            explicit NxFramebufferResizingFailed(const std::string &backendApi, const bool tooBig,
                                               const unsigned int width, const unsigned int height,
                                               const std::source_location loc =
                                                       std::source_location::current())
                : Exception(std::format("[{}] Framebuffer resizing failed: {}x{} is too {}",
                                        backendApi, width, height, (tooBig ? "big" : "small")), loc) {}
    };

    class NxFramebufferUnsupportedColorFormat final : public Exception {
        public:
            explicit NxFramebufferUnsupportedColorFormat(const std::string &backendApiName,
                                                       const std::source_location loc =
                                                               std::source_location::current())
                : Exception(std::format("[{}] Unsupported framebuffer color attachment format", backendApiName), loc) {}
    };

    class NxFramebufferUnsupportedDepthFormat final : public Exception {
        public:
            explicit NxFramebufferUnsupportedDepthFormat(const std::string &backendApiName,
                                                       const std::source_location loc =
                                                               std::source_location::current())
                : Exception(std::format("[{}] Unsupported framebuffer depth attachment format", backendApiName), loc) {}
    };

    class NxFramebufferReadFailure final : public Exception {
        public:
            explicit NxFramebufferReadFailure(const std::string &backendApiName, int index, int x, int y, const std::source_location loc = std::source_location::current()) : Exception(std::format("[{}] Unable to read framebuffer with index {} at coordinate ({}, {})", backendApiName, index, x, y), loc) {}
    };

    class NxFramebufferInvalidIndex final : public Exception {
        public:
            explicit NxFramebufferInvalidIndex(const std::string &backendApiName, int index, const std::source_location loc = std::source_location::current()) : Exception(std::format("[{}] Invalid attachment index : {}", backendApiName, index), loc) {};
    };

    class NxBufferLayoutEmpty final : public Exception {
        public:
            explicit NxBufferLayoutEmpty(const std::string &backendApi,
                                       const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Vertex buffer layout cannot be empty", backendApi), loc) {}
    };

    enum class NxRendererType {
        RENDERER_2D,
        RENDERER_3D
    };

    class NxRendererNotInitialized final : public Exception {
        public:
            explicit NxRendererNotInitialized(const NxRendererType type,
                                            const std::source_location loc =
                                                    std::source_location::current())
                : Exception(std::format("{} Renderer not initialized, call the init function first",
                                        (type == NxRendererType::RENDERER_2D ? "[RENDERER 2D]" : "[RENDERER 3D]")), loc)
            {}
    };

    class NxRendererSceneLifeCycleFailure final : public Exception {
        public:
            explicit NxRendererSceneLifeCycleFailure(const NxRendererType type, const std::string &msg,
                                                   const std::source_location loc =
                                                           std::source_location::current())
                : Exception(std::format("{} {}",
                                        (type == NxRendererType::RENDERER_2D ? "[RENDERER 2D]" : "[RENDERER 3D]"), msg),
                            loc) {}
    };

    class NxTextureInvalidSize final : public Exception {
        public:
            explicit NxTextureInvalidSize(const std::string &backendApi,
                                        const unsigned int width, const unsigned int height,
                                        const unsigned int maxTextureSize,
                                        const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Invalid size for texture: {}x{} is too big, max texture size is : {}",
                                        backendApi, width, height, maxTextureSize), loc) {}
    };

    class NxTextureUnsupportedFormat final : public Exception {
        public:
            explicit NxTextureUnsupportedFormat(const std::string &backendApi, const int channels,
                                              const std::string &path,
                                              const std::source_location loc =
                                                      std::source_location::current())
                : Exception(std::format("[{}] Unsupported image format with {} channels in {}",
                                        backendApi, channels, path), loc) {}
    };

    class NxTextureSizeMismatch final : public Exception {
        public:
            explicit NxTextureSizeMismatch(const std::string &backendApi, const int dataSize, const int expectedSize,
                                         const std::source_location loc = std::source_location::current())
                : Exception(std::format("[{}] Data size does not match the texture size: {} != {}",
                                        backendApi, dataSize, expectedSize), loc) {}
    };

    class NxStbiLoadException final : public Exception {
        public:
            explicit NxStbiLoadException(const std::string &msg,
                                       const std::source_location loc = std::source_location::current())
                : Exception(std::format("STBI load failed: {}", msg), loc) {}
    };

    class NxPipelineRenderTargetNotSetException final : public Exception {
        public:
            explicit NxPipelineRenderTargetNotSetException(const std::source_location loc =
                                                                  std::source_location::current())
                : Exception(std::format("Pipeline render target not set"), loc) {}
    };
}
