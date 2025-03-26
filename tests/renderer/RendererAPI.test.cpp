//// RendererAPI.test.cpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/11/2024
//  Description: Test file for the renderer api class
//
///////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glad/glad.h>
#include "opengl/OpenGlVertexArray.hpp"

#include "contexts/opengl.hpp"
#include "opengl/OpenGlRendererAPI.hpp"
#include "renderer/RendererExceptions.hpp"

namespace nexo::renderer {
    TEST_F(OpenGLTest, InitializationTest) {
        OpenGlRendererApi rendererApi;
        // Validate init
        EXPECT_NO_THROW(rendererApi.init());
    }

    TEST_F(OpenGLTest, ViewportSetup) {
        OpenGlRendererApi rendererApi;
        rendererApi.init();

        // Validate viewport resizing
        EXPECT_NO_THROW(rendererApi.setViewport(0, 0, 800, 600));
        GLint viewport[4] = {0};
        glGetIntegerv(GL_VIEWPORT, viewport);
        EXPECT_EQ(viewport[0], 0);    // x
        EXPECT_EQ(viewport[1], 0);    // y
        EXPECT_EQ(viewport[2], 800);  // width
        EXPECT_EQ(viewport[3], 600);  // height

        // Validate viewport resizing twice
        EXPECT_NO_THROW(rendererApi.setViewport(200, 100, 1920, 1080));
        glGetIntegerv(GL_VIEWPORT, viewport);
        EXPECT_EQ(viewport[0], 200);    // x
        EXPECT_EQ(viewport[1], 100);    // y
        EXPECT_EQ(viewport[2], 1920);  // width
        EXPECT_EQ(viewport[3], 1080);  // height


        // Validate invalid viewport values
        EXPECT_THROW(rendererApi.setViewport(0, 0, 0, 600), GraphicsApiViewportResizingFailure);
        EXPECT_THROW(rendererApi.setViewport(0, 0, 800, 0), GraphicsApiViewportResizingFailure);

        // Validate too big dimensions
        unsigned int width = 0;
        unsigned int height = 0;
        rendererApi.getMaxViewportSize(&width, &height);
        EXPECT_THROW(rendererApi.setViewport(0, 0, width + 1, height),
                     GraphicsApiViewportResizingFailure);
    }

    TEST_F(OpenGLTest, ClearTest) {
        OpenGlRendererApi rendererApi;
        rendererApi.init();

        // Validate clear color via opengl getters
        glm::vec4 clearColor(0.1f, 0.2f, 0.3f, 1.0f);
        rendererApi.setClearColor(clearColor);
        float storedClearColor[4] = {0.0f};
        glGetFloatv(GL_COLOR_CLEAR_VALUE, storedClearColor);
        EXPECT_NEAR(storedClearColor[0], clearColor.r, 0.01f); // R
        EXPECT_NEAR(storedClearColor[1], clearColor.g, 0.01f); // G
        EXPECT_NEAR(storedClearColor[2], clearColor.b, 0.01f); // B
        EXPECT_NEAR(storedClearColor[3], clearColor.a, 0.01f); // A
        EXPECT_NO_THROW(rendererApi.clear());


        // Validate via the viewport color
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport); // Retrieve the viewport size
        int width = viewport[2];
        int height = viewport[3];

        float pixelData[4] = {0.0f}; // RGBA
        glReadPixels(width / 2, height / 2, 1, 1, GL_RGBA, GL_FLOAT, pixelData);

        EXPECT_NEAR(pixelData[0], clearColor.r, 0.01f); // R
        EXPECT_NEAR(pixelData[1], clearColor.g, 0.01f); // G
        EXPECT_NEAR(pixelData[2], clearColor.b, 0.01f); // B
        EXPECT_NEAR(pixelData[3], clearColor.a, 0.01f); // A

        // Validate the depth via the opengl getters
        glEnable(GL_DEPTH_TEST);
        rendererApi.setClearDepth(1.0f);
        float depthValue = 0.0f;
        rendererApi.clear();
        glGetFloatv(GL_DEPTH_CLEAR_VALUE, &depthValue);

        // Validate the depth via the view port values
        glReadPixels(width / 2, height / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValue);
        EXPECT_NEAR(depthValue, 1.0f, 0.01f);
    }

    TEST_F(OpenGLTest, ExceptionOnUninitializedAPI) {
        OpenGlRendererApi rendererApi;

        // Validate exception is thrown for uninitialized API methods
        EXPECT_THROW(rendererApi.setViewport(0, 0, 800, 600), GraphicsApiNotInitialized);
        EXPECT_THROW(rendererApi.clear(), GraphicsApiNotInitialized);
        EXPECT_THROW(rendererApi.setClearColor(glm::vec4(1.0f)), GraphicsApiNotInitialized);

        auto vertexArray = std::make_shared<OpenGlVertexArray>();
        EXPECT_THROW(rendererApi.drawIndexed(vertexArray), GraphicsApiNotInitialized);

        // Validate exception is thrown when passing a null vertex array
        rendererApi.init();
        EXPECT_THROW(rendererApi.drawIndexed(nullptr), InvalidValue);
    }

}
