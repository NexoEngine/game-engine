//// RendererExceptions.test.cpp //////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        02/12/2024
//  Description: Test file for the RendererExceptions class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/RendererExceptions.hpp"

namespace nexo::renderer {

    TEST(RendererExceptionsTest, FileNotFoundException) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2; // Account for the next line

        FileNotFoundException ex("test_file.txt");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("File not found: test_file.txt"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, UnknownGraphicsApi) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        UnknownGraphicsApi ex("Vulkan");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("Unknown graphics API: Vulkan"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, GraphicsApiInitFailure) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        GraphicsApiInitFailure ex("OpenGL");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("Failed to initialize graphics API: OpenGL"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, ShaderCreationFailed) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        ShaderCreationFailed ex("OpenGL", "Compilation error", "shader.glsl");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Failed to create the shader (shader.glsl): Compilation error"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, FramebufferResizingFailed) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        FramebufferResizingFailed ex("Vulkan", false, 800, 600);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[Vulkan] Framebuffer resizing failed: 800x600 is too small"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, RendererNotInitialized) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        RendererNotInitialized ex(RendererType::RENDERER_3D);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[RENDERER 3D] Renderer not initialized, call the init function first"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, TextureInvalidSize) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        TextureInvalidSize ex("OpenGL", 4096, 4096, 2048);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Invalid size for texture: 4096x4096 is too big, max texture size is : 2048"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, StbiLoadException) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        StbiLoadException ex("Invalid PNG file");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("STBI load failed: Invalid PNG file"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, OutOfRangeException) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        OutOfRangeException ex(10, 5);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("Index 10 is out of range [0, 5)"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, GraphicsApiNotInitialized) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        GraphicsApiNotInitialized ex("OpenGL");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] API is not initialized, call the init function first"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, GraphicsApiViewportResizingFailure) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        GraphicsApiViewportResizingFailure ex("OpenGL", true, 4096, 4096);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Viewport resizing failed: 4096x4096 is too big"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, GraphicsApiWindowInitFailure) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        GraphicsApiWindowInitFailure ex("OpenGL");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("Failed to initialize graphics API: OpenGL"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, InvalidValue) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        InvalidValue ex("OpenGL", "Negative width value");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Invalid value: Negative width value"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, ShaderInvalidUniform) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        ShaderInvalidUniform ex("OpenGL", "main.glsl", "u_ViewProjection");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Failed to retrieve uniform \"u_ViewProjection\" in shader: main.glsl"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, FramebufferCreationFailed) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        FramebufferCreationFailed ex("OpenGL");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Failed to create the framebuffer"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, FramebufferUnsupportedColorFormat) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        FramebufferUnsupportedColorFormat ex("OpenGL");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Unsupported framebuffer color attachment format"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, FramebufferUnsupportedDepthFormat) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        FramebufferUnsupportedDepthFormat ex("OpenGL");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Unsupported framebuffer depth attachment format"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, FramebufferReadFailure) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        FramebufferReadFailure ex("OpenGL", 0, 100, 200);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Unable to read framebuffer with index 0 at coordinate (100, 200)"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, FramebufferInvalidIndex) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        FramebufferInvalidIndex ex("OpenGL", 5);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Invalid attachment index : 5"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, BufferLayoutEmpty) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        BufferLayoutEmpty ex("OpenGL");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Vertex buffer layout cannot be empty"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, TextureUnsupportedFormat) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        TextureUnsupportedFormat ex("OpenGL", 5, "texture.exr");
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Unsupported image format with 5 channels in texture.exr"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }

    TEST(RendererExceptionsTest, TextureSizeMismatch) {
        constexpr const char* expectedFile = __FILE__;
        constexpr unsigned int expectedLine = __LINE__ + 2;

        TextureSizeMismatch ex("OpenGL", 1024, 2048);
        std::string formattedMessage = ex.what();

        EXPECT_NE(formattedMessage.find("[OpenGL] Data size does not match the texture size: 1024 != 2048"), std::string::npos);
        EXPECT_NE(formattedMessage.find(expectedFile), std::string::npos);
        EXPECT_NE(formattedMessage.find(std::to_string(expectedLine)), std::string::npos);
    }
}
