//// Framebuffer.test.cpp /////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        23/11/2024
//  Description: Test file for the framebuffer
//
///////////////////////////////////////////////////////////////////////////////

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "opengl/OpenGlVertexArray.hpp"
#include "opengl/OpenGlBuffer.hpp"
#include "opengl/OpenGlFramebuffer.hpp"
#include "contexts/opengl.hpp"
#include "RendererExceptions.hpp"

namespace nexo::renderer {

    TEST_F(OpenGLTest, FramebufferCreationAndBinding)
    {
        NxFramebufferSpecs specs;
        specs.width = 800;
        specs.height = 600;
        specs.samples = 1;
        specs.attachments.attachments = {
            {NxFrameBufferTextureFormats::RGBA8},
            {NxFrameBufferTextureFormats::DEPTH24STENCIL8}
        };
        NxOpenGlFramebuffer framebuffer(specs);

        // Validate framebuffer id
        EXPECT_NE(framebuffer.getFramebufferId(), 0);

        // Validate specifications
        const auto &retrievedSpecs = framebuffer.getSpecs();
        EXPECT_EQ(retrievedSpecs.width, specs.width);
        EXPECT_EQ(retrievedSpecs.height, specs.height);
        EXPECT_EQ(retrievedSpecs.samples, specs.samples);

        // Validate color attachment ID
        EXPECT_NE(framebuffer.getColorAttachmentId(0), 0);

        // Validate binding
        framebuffer.bind();
        GLint boundFramebuffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFramebuffer);
        EXPECT_EQ(boundFramebuffer, framebuffer.getFramebufferId());


        // Check framebuffer size properties
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        EXPECT_EQ(viewport[2], specs.width);
        EXPECT_EQ(viewport[3], specs.height);

        // Validate unbinding
        framebuffer.unbind();
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFramebuffer);
        EXPECT_EQ(boundFramebuffer, 0);
    }

    TEST_F(OpenGLTest, FramebufferResize)
    {
        NxFramebufferSpecs specs;
        specs.width = 800;
        specs.height = 600;
        specs.samples = 1;
        specs.attachments.attachments = {
            {NxFrameBufferTextureFormats::RGBA8},
            {NxFrameBufferTextureFormats::DEPTH24STENCIL8}
        };
        NxOpenGlFramebuffer framebuffer(specs);

        framebuffer.resize(1024, 768);

        // Validate new dimensions
        const auto &retrievedSpecs = framebuffer.getSpecs();
        EXPECT_EQ(retrievedSpecs.width, 1024);
        EXPECT_EQ(retrievedSpecs.height, 768);
    }

    TEST_F(OpenGLTest, ResizeWithInvalidDimensions)
    {
        NxFramebufferSpecs specs;
        specs.width = 800;
        specs.height = 600;
        specs.samples = 1;
        specs.attachments.attachments = {
            {NxFrameBufferTextureFormats::RGBA8},
            {NxFrameBufferTextureFormats::DEPTH24STENCIL8}
        };
        NxOpenGlFramebuffer framebuffer(specs);

        EXPECT_THROW(framebuffer.resize(0, 600), NxFramebufferResizingFailed);
        EXPECT_THROW(framebuffer.resize(800, 0), NxFramebufferResizingFailed);
        EXPECT_THROW(framebuffer.resize(9000, 600), NxFramebufferResizingFailed);
        EXPECT_THROW(framebuffer.resize(800, 9000), NxFramebufferResizingFailed);
    }

    TEST_F(OpenGLTest, InvalidFramebufferCreation)
    {
        NxFramebufferSpecs specs;
        specs.width = 0;
        specs.height = 600;

        EXPECT_THROW(NxOpenGlFramebuffer framebuffer(specs), NxFramebufferResizingFailed);
        specs.width = 800;
        specs.height = 0;
        EXPECT_THROW(NxOpenGlFramebuffer framebuffer(specs), NxFramebufferResizingFailed);
        specs.width = 9000;
        specs.height = 600;
        EXPECT_THROW(NxOpenGlFramebuffer framebuffer(specs), NxFramebufferResizingFailed);
        specs.width = 800;
        specs.height = 9000;
        EXPECT_THROW(NxOpenGlFramebuffer framebuffer(specs), NxFramebufferResizingFailed);
    }

    TEST_F(OpenGLTest, MultipleColorAttachments)
    {
        NxFramebufferSpecs specs;
        specs.width = 800;
        specs.height = 600;
        specs.samples = 1;
        specs.attachments.attachments = {
            {NxFrameBufferTextureFormats::RGBA8},
            {NxFrameBufferTextureFormats::RGBA16},
            {NxFrameBufferTextureFormats::DEPTH24STENCIL8}
        };

        // Check if the hardware supports at least the required number of attachments
        GLint maxAttachments;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttachments);
        EXPECT_GE(maxAttachments, static_cast<GLint>(specs.attachments.attachments.size()));

        NxOpenGlFramebuffer framebuffer(specs);

        // Verify the IDs of the color attachments
        EXPECT_NE(framebuffer.getColorAttachmentId(0), 0);
        EXPECT_NE(framebuffer.getColorAttachmentId(1), 0);

        framebuffer.bind();

        // Validate framebuffer completeness
        GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        EXPECT_EQ(framebufferStatus, GL_FRAMEBUFFER_COMPLETE) << "Framebuffer is not complete";

        // Check that all attachments are correctly bound
        for (unsigned int i = 0; i < specs.attachments.attachments.size(); ++i)
        {
            GLint boundTexture;
            GLenum attachmentType = (i < 2) ? GL_COLOR_ATTACHMENT0 + i : GL_DEPTH_STENCIL_ATTACHMENT;
            glGetFramebufferAttachmentParameteriv(
                GL_FRAMEBUFFER,
                attachmentType,
                GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                &boundTexture
            );

            if (i < 2)
            {
                EXPECT_EQ(static_cast<unsigned int>(boundTexture), framebuffer.getColorAttachmentId(i));
            } else if (framebuffer.getSpecs().attachments.attachments[i].textureFormat ==
                       NxFrameBufferTextureFormats::DEPTH24STENCIL8)
            {
                EXPECT_EQ(static_cast<unsigned int>(boundTexture),
                          framebuffer.getDepthAttachmentId());
            } else
            {
                EXPECT_EQ(static_cast<unsigned int>(boundTexture), 0);
            }
        }

        // Validate RGBA8 size
        GLint r, g, b, a;
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
            &r
        );
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
            &g
        );
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
            &b
        );
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
            &a
        );
        EXPECT_EQ(r, 8);
        EXPECT_EQ(g, 8);
        EXPECT_EQ(b, 8);
        EXPECT_EQ(a, 8);

        // Validate RGBA16 size
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT1,
            GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE,
            &r
        );
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT1,
            GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
            &g
        );
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT1,
            GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
            &b
        );
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT1,
            GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
            &a
        );
        EXPECT_EQ(r, 16);
        EXPECT_EQ(g, 16);
        EXPECT_EQ(b, 16);
        EXPECT_EQ(a, 16);

        // Validate depth attachment size
        GLint depth;
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT,
            GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
            &depth
        );
        EXPECT_EQ(depth, 24);

        // Validate stencil attachment size
        GLint stencil;
        glGetFramebufferAttachmentParameteriv(
            GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT,
            GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
            &stencil
        );
        EXPECT_EQ(stencil, 8);

        framebuffer.unbind();
    }

    TEST_F(OpenGLTest, InvalidFormat)
    {
        NxFramebufferSpecs specs;
        specs.width = 800;
        specs.height = 600;
        specs.samples = 1;

        // Test unsupported color format
        specs.attachments.attachments = {
            {static_cast<NxFrameBufferTextureFormats>(999)} // Invalid format
        };

        EXPECT_THROW(
            NxOpenGlFramebuffer framebuffer(specs);
            , NxFramebufferUnsupportedColorFormat);
    }

    TEST_F(OpenGLTest, GetPixelWrapperValid)
    {
#ifdef _WIN32
        // TODO: fix test (see #99)
        GTEST_SKIP() << "This test infinitely loops on the CI on Windows, skipping for now.";
#else
        NxFramebufferSpecs specs;
        specs.width = 100;
        specs.height = 100;
        specs.samples = 1;
        specs.attachments.attachments = {
            {NxFrameBufferTextureFormats::RGBA8}
        };

        NxOpenGlFramebuffer framebuffer(specs);
        framebuffer.bind();

        int pixelValue = 0;
        EXPECT_NO_THROW(framebuffer.getPixelWrapper(0, 50, 50, &pixelValue, typeid(int)));

        framebuffer.unbind();
#endif
    }

    TEST_F(OpenGLTest, GetPixelWrapperUnsupportedType)
    {
        // Verify that getPixelWrapper throws when provided with a type other than int.
        NxFramebufferSpecs specs;
        specs.width = 100;
        specs.height = 100;
        specs.samples = 1;
        specs.attachments.attachments = {
            {NxFrameBufferTextureFormats::RGBA8}
        };

        NxOpenGlFramebuffer framebuffer(specs);
        int dummy = 0;
        EXPECT_THROW(
            framebuffer.getPixelWrapper(0, 50, 50, &dummy, typeid(float)),
            NxFramebufferUnsupportedColorFormat
        );
    }

    TEST_F(OpenGLTest, GetPixelWrapperInvalidAttachmentIndex)
    {
        // Verify that getPixelWrapper throws if the attachment index is out of bounds.
        NxFramebufferSpecs specs;
        specs.width = 100;
        specs.height = 100;
        specs.samples = 1;
        // Only one color attachment.
        specs.attachments.attachments = {
            {NxFrameBufferTextureFormats::RGBA8}
        };

        NxOpenGlFramebuffer framebuffer(specs);
        int dummy = 0;
        // Attachment index 1 is invalid because only index 0 exists.
        EXPECT_THROW(
            framebuffer.getPixelWrapper(1, 50, 50, &dummy, typeid(int)),
            NxFramebufferInvalidIndex
        );
    }

    TEST_F(OpenGLTest, ClearAttachmentWrapperValid)
    {
        // Test that clearAttachmentWrapper does not throw when clearing a valid attachment with a supported type.
        NxFramebufferSpecs specs;
        specs.width = 100;
        specs.height = 100;
        specs.samples = 1;
        specs.attachments.attachments = {
            {NxFrameBufferTextureFormats::RGBA8}
        };

        NxOpenGlFramebuffer framebuffer(specs);
        int clearValue = 0;
        EXPECT_NO_THROW(framebuffer.clearAttachmentWrapper(0, &clearValue, typeid(int)));
    }

    TEST_F(OpenGLTest, ClearAttachmentWrapperUnsupportedType)
    {
        // Test that clearAttachmentWrapper throws if called with an unsupported type.
        NxFramebufferSpecs specs;
        specs.width = 100;
        specs.height = 100;
        specs.samples = 1;
        specs.attachments.attachments = {
            {NxFrameBufferTextureFormats::RGBA8}
        };

        NxOpenGlFramebuffer framebuffer(specs);
        int clearValue = 0;
        EXPECT_THROW(
            framebuffer.clearAttachmentWrapper(0, &clearValue, typeid(float)),
            NxFramebufferUnsupportedColorFormat
        );
    }

    TEST_F(OpenGLTest, ClearAttachmentWrapperInvalidAttachmentIndex)
    {
        // Test that clearAttachmentWrapper throws when the attachment index is out of range.
        NxFramebufferSpecs specs;
        specs.width = 100;
        specs.height = 100;
        specs.samples = 1;
        // Only one color attachment exists.
        specs.attachments.attachments = {
            {NxFrameBufferTextureFormats::RGBA8}
        };

        NxOpenGlFramebuffer framebuffer(specs);
        int clearValue = 0;
        // Attachment index 1 is invalid.
        EXPECT_THROW(
            framebuffer.clearAttachmentWrapper(1, &clearValue, typeid(int)),
            NxFramebufferInvalidIndex
        );
    }

    TEST_F(OpenGLTest, ClearAndGetPixelRedIntegerAttachment) {
        NxFramebufferSpecs specs;
        specs.width = 100;
        specs.height = 100;
        specs.samples = 1;
        specs.attachments.attachments = { NxFrameBufferTextureFormats::RED_INTEGER };

        NxOpenGlFramebuffer framebuffer(specs);
        framebuffer.bind();
        int clearValue = 123;
        EXPECT_NO_THROW(framebuffer.clearAttachmentWrapper(0, &clearValue, typeid(int)));

        int pixelValue = 0;
        EXPECT_NO_THROW(framebuffer.getPixelWrapper(0, 50, 50, &pixelValue, typeid(int)));
        EXPECT_EQ(pixelValue, clearValue);
        framebuffer.unbind();
    }

    TEST_F(OpenGLTest, ClearAndGetPixelMultipleAttachments) {
        NxFramebufferSpecs specs;
        specs.width = 100;
        specs.height = 100;
        specs.samples = 1;
        specs.attachments.attachments = {
            { NxFrameBufferTextureFormats::RGBA8 },
            { NxFrameBufferTextureFormats::RED_INTEGER }
        };

        NxOpenGlFramebuffer framebuffer(specs);
        framebuffer.bind();

        // Clear the second (red integer) attachment to a known value.
        int clearValue = 77;
        EXPECT_NO_THROW(framebuffer.clearAttachmentWrapper(1, &clearValue, typeid(int)));

        int pixelValue = 0;
        EXPECT_NO_THROW(framebuffer.getPixelWrapper(1, 50, 50, &pixelValue, typeid(int)));
        EXPECT_EQ(pixelValue, clearValue);
        framebuffer.unbind();
    }

    // While OpenGL's glReadPixels does not throw exceptions for out–of–bounds reads, we ensure
    // that our wrapper does not crash. (The returned value may be undefined.)
    TEST_F(OpenGLTest, GetPixelOutOfBoundsRedIntegerAttachment) {
        NxFramebufferSpecs specs;
        specs.width = 50;
        specs.height = 50;
        specs.samples = 1;
        specs.attachments.attachments = { { static_cast<NxFrameBufferTextureFormats>(3) } };

        NxOpenGlFramebuffer framebuffer(specs);
        framebuffer.bind();
        int pixelValue = 0;
        // Attempt to read a pixel well outside the 50x50 region.
        EXPECT_NO_THROW(framebuffer.getPixelWrapper(0, 100, 100, &pixelValue, typeid(int)));
        framebuffer.unbind();
    }

}
