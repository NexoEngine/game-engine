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
}
