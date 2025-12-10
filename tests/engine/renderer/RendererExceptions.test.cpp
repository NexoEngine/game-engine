//// RendererExceptions.test.cpp ///////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for renderer exception classes
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/RendererExceptions.hpp"

namespace nexo::renderer {

// =============================================================================
// NxRendererType Enum Tests
// =============================================================================

class RendererTypeEnumTest : public ::testing::Test {};

TEST_F(RendererTypeEnumTest, Renderer2DValue) {
    EXPECT_EQ(static_cast<int>(NxRendererType::RENDERER_2D), 0);
}

TEST_F(RendererTypeEnumTest, Renderer3DValue) {
    EXPECT_EQ(static_cast<int>(NxRendererType::RENDERER_3D), 1);
}

// =============================================================================
// NxOutOfRangeException Tests
// =============================================================================

class OutOfRangeExceptionTest : public ::testing::Test {};

TEST_F(OutOfRangeExceptionTest, MessageContainsIndex) {
    NxOutOfRangeException ex(5, 3);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("5"), std::string::npos);
}

TEST_F(OutOfRangeExceptionTest, MessageContainsSize) {
    NxOutOfRangeException ex(5, 3);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("3"), std::string::npos);
}

TEST_F(OutOfRangeExceptionTest, IsThrowable) {
    EXPECT_THROW(throw NxOutOfRangeException(10, 5), NxOutOfRangeException);
}

// =============================================================================
// NxFileNotFoundException Tests
// =============================================================================

class FileNotFoundExceptionTest : public ::testing::Test {};

TEST_F(FileNotFoundExceptionTest, MessageContainsPath) {
    NxFileNotFoundException ex("/path/to/file.txt");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("/path/to/file.txt"), std::string::npos);
}

TEST_F(FileNotFoundExceptionTest, IsThrowable) {
    EXPECT_THROW(throw NxFileNotFoundException("missing.txt"), NxFileNotFoundException);
}

// =============================================================================
// NxUnknownGraphicsApi Tests
// =============================================================================

class UnknownGraphicsApiTest : public ::testing::Test {};

TEST_F(UnknownGraphicsApiTest, MessageContainsApiName) {
    NxUnknownGraphicsApi ex("Vulkan");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("Vulkan"), std::string::npos);
}

TEST_F(UnknownGraphicsApiTest, IsThrowable) {
    EXPECT_THROW(throw NxUnknownGraphicsApi("DirectX"), NxUnknownGraphicsApi);
}

// =============================================================================
// NxGraphicsApiInitFailure Tests
// =============================================================================

class GraphicsApiInitFailureTest : public ::testing::Test {};

TEST_F(GraphicsApiInitFailureTest, MessageContainsApiName) {
    NxGraphicsApiInitFailure ex("OpenGL");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("OpenGL"), std::string::npos);
}

// =============================================================================
// NxGraphicsApiNotInitialized Tests
// =============================================================================

class GraphicsApiNotInitializedTest : public ::testing::Test {};

TEST_F(GraphicsApiNotInitializedTest, MessageContainsApiName) {
    NxGraphicsApiNotInitialized ex("OpenGL");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("OpenGL"), std::string::npos);
}

TEST_F(GraphicsApiNotInitializedTest, MessageMentionsInit) {
    NxGraphicsApiNotInitialized ex("OpenGL");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("init"), std::string::npos);
}

// =============================================================================
// NxGraphicsApiViewportResizingFailure Tests
// =============================================================================

class ViewportResizingFailureTest : public ::testing::Test {};

TEST_F(ViewportResizingFailureTest, TooBigMessage) {
    NxGraphicsApiViewportResizingFailure ex("OpenGL", true, 10000, 10000);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("too big"), std::string::npos);
}

TEST_F(ViewportResizingFailureTest, TooSmallMessage) {
    NxGraphicsApiViewportResizingFailure ex("OpenGL", false, 0, 0);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("too small"), std::string::npos);
}

TEST_F(ViewportResizingFailureTest, MessageContainsDimensions) {
    NxGraphicsApiViewportResizingFailure ex("OpenGL", true, 1920, 1080);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("1920"), std::string::npos);
    EXPECT_NE(msg.find("1080"), std::string::npos);
}

// =============================================================================
// NxShaderCreationFailed Tests
// =============================================================================

class ShaderCreationFailedTest : public ::testing::Test {};

TEST_F(ShaderCreationFailedTest, MessageContainsApiAndError) {
    NxShaderCreationFailed ex("OpenGL", "Compilation error");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("OpenGL"), std::string::npos);
    EXPECT_NE(msg.find("Compilation error"), std::string::npos);
}

TEST_F(ShaderCreationFailedTest, MessageContainsPath) {
    NxShaderCreationFailed ex("OpenGL", "error", "/shaders/test.glsl");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("/shaders/test.glsl"), std::string::npos);
}

// =============================================================================
// NxShaderInvalidUniform Tests
// =============================================================================

class ShaderInvalidUniformTest : public ::testing::Test {};

TEST_F(ShaderInvalidUniformTest, MessageContainsUniformName) {
    NxShaderInvalidUniform ex("OpenGL", "BasicShader", "uModelMatrix");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("uModelMatrix"), std::string::npos);
}

TEST_F(ShaderInvalidUniformTest, MessageContainsShaderName) {
    NxShaderInvalidUniform ex("OpenGL", "BasicShader", "uModelMatrix");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("BasicShader"), std::string::npos);
}

// =============================================================================
// NxFramebufferCreationFailed Tests
// =============================================================================

class FramebufferCreationFailedTest : public ::testing::Test {};

TEST_F(FramebufferCreationFailedTest, MessageContainsApi) {
    NxFramebufferCreationFailed ex("OpenGL");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("OpenGL"), std::string::npos);
}

TEST_F(FramebufferCreationFailedTest, MessageMentionsFramebuffer) {
    NxFramebufferCreationFailed ex("OpenGL");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("framebuffer"), std::string::npos);
}

// =============================================================================
// NxFramebufferResizingFailed Tests
// =============================================================================

class FramebufferResizingFailedTest : public ::testing::Test {};

TEST_F(FramebufferResizingFailedTest, TooBigMessage) {
    NxFramebufferResizingFailed ex("OpenGL", true, 8192, 8192);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("too big"), std::string::npos);
}

TEST_F(FramebufferResizingFailedTest, TooSmallMessage) {
    NxFramebufferResizingFailed ex("OpenGL", false, 0, 0);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("too small"), std::string::npos);
}

// =============================================================================
// NxBufferLayoutEmpty Tests
// =============================================================================

class BufferLayoutEmptyTest : public ::testing::Test {};

TEST_F(BufferLayoutEmptyTest, MessageContainsApi) {
    NxBufferLayoutEmpty ex("OpenGL");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("OpenGL"), std::string::npos);
}

TEST_F(BufferLayoutEmptyTest, MessageMentionsEmpty) {
    NxBufferLayoutEmpty ex("OpenGL");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("empty"), std::string::npos);
}

// =============================================================================
// NxRendererNotInitialized Tests
// =============================================================================

class RendererNotInitializedTest : public ::testing::Test {};

TEST_F(RendererNotInitializedTest, Renderer2DMessage) {
    NxRendererNotInitialized ex(NxRendererType::RENDERER_2D);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("2D"), std::string::npos);
}

TEST_F(RendererNotInitializedTest, Renderer3DMessage) {
    NxRendererNotInitialized ex(NxRendererType::RENDERER_3D);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("3D"), std::string::npos);
}

// =============================================================================
// NxRendererSceneLifeCycleFailure Tests
// =============================================================================

class RendererSceneLifeCycleFailureTest : public ::testing::Test {};

TEST_F(RendererSceneLifeCycleFailureTest, Renderer2DMessage) {
    NxRendererSceneLifeCycleFailure ex(NxRendererType::RENDERER_2D, "Scene not started");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("2D"), std::string::npos);
    EXPECT_NE(msg.find("Scene not started"), std::string::npos);
}

TEST_F(RendererSceneLifeCycleFailureTest, Renderer3DMessage) {
    NxRendererSceneLifeCycleFailure ex(NxRendererType::RENDERER_3D, "Scene already ended");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("3D"), std::string::npos);
}

// =============================================================================
// NxTextureInvalidSize Tests
// =============================================================================

class TextureInvalidSizeTest : public ::testing::Test {};

TEST_F(TextureInvalidSizeTest, MessageContainsDimensions) {
    NxTextureInvalidSize ex("OpenGL", 16384, 16384, 8192);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("16384"), std::string::npos);
}

TEST_F(TextureInvalidSizeTest, MessageContainsMaxSize) {
    NxTextureInvalidSize ex("OpenGL", 16384, 16384, 8192);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("8192"), std::string::npos);
}

// =============================================================================
// NxTextureUnsupportedFormat Tests
// =============================================================================

class TextureUnsupportedFormatTest : public ::testing::Test {};

TEST_F(TextureUnsupportedFormatTest, MessageContainsChannels) {
    NxTextureUnsupportedFormat ex("OpenGL", 5, "/textures/test.png");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("5"), std::string::npos);
}

TEST_F(TextureUnsupportedFormatTest, MessageContainsPath) {
    NxTextureUnsupportedFormat ex("OpenGL", 5, "/textures/test.png");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("/textures/test.png"), std::string::npos);
}

// =============================================================================
// NxTextureSizeMismatch Tests
// =============================================================================

class TextureSizeMismatchTest : public ::testing::Test {};

TEST_F(TextureSizeMismatchTest, MessageContainsSizes) {
    NxTextureSizeMismatch ex("OpenGL", 1000, 2000);
    std::string msg = ex.what();
    EXPECT_NE(msg.find("1000"), std::string::npos);
    EXPECT_NE(msg.find("2000"), std::string::npos);
}

// =============================================================================
// NxStbiLoadException Tests
// =============================================================================

class StbiLoadExceptionTest : public ::testing::Test {};

TEST_F(StbiLoadExceptionTest, MessageContainsError) {
    NxStbiLoadException ex("Failed to decode PNG");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("Failed to decode PNG"), std::string::npos);
}

TEST_F(StbiLoadExceptionTest, MessageMentionsStbi) {
    NxStbiLoadException ex("error");
    std::string msg = ex.what();
    EXPECT_NE(msg.find("STBI"), std::string::npos);
}

// =============================================================================
// NxPipelineRenderTargetNotSetException Tests
// =============================================================================

class PipelineRenderTargetNotSetTest : public ::testing::Test {};

TEST_F(PipelineRenderTargetNotSetTest, MessageMentionsRenderTarget) {
    NxPipelineRenderTargetNotSetException ex;
    std::string msg = ex.what();
    EXPECT_NE(msg.find("render target"), std::string::npos);
}

// =============================================================================
// Exception Inheritance Tests
// =============================================================================

class ExceptionInheritanceTest : public ::testing::Test {};

TEST_F(ExceptionInheritanceTest, AllExceptionsInheritFromException) {
    EXPECT_NO_THROW({
        try {
            throw NxOutOfRangeException(0, 0);
        } catch (const Exception&) {}
    });

    EXPECT_NO_THROW({
        try {
            throw NxFileNotFoundException("test");
        } catch (const Exception&) {}
    });

    EXPECT_NO_THROW({
        try {
            throw NxUnknownGraphicsApi("test");
        } catch (const Exception&) {}
    });
}

TEST_F(ExceptionInheritanceTest, AllExceptionsCatchableAsStdException) {
    EXPECT_NO_THROW({
        try {
            throw NxShaderCreationFailed("API", "error");
        } catch (const std::exception&) {}
    });
}

}  // namespace nexo::renderer
