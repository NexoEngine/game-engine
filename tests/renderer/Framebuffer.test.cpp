//// Framebuffer.test.cpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
        FramebufferSpecs specs;
        specs.width = 800;
        specs.height = 600;
        specs.samples = 1;
        specs.attachments.attachments = {
            {FrameBufferTextureFormats::RGBA8},
            {FrameBufferTextureFormats::DEPTH24STENCIL8}
        };
        OpenGlFramebuffer framebuffer(specs);

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
        FramebufferSpecs specs;
        specs.width = 800;
        specs.height = 600;
        specs.samples = 1;
        specs.attachments.attachments = {
            {FrameBufferTextureFormats::RGBA8},
            {FrameBufferTextureFormats::DEPTH24STENCIL8}
        };
        OpenGlFramebuffer framebuffer(specs);

        framebuffer.resize(1024, 768);

        // Validate new dimensions
        const auto &retrievedSpecs = framebuffer.getSpecs();
        EXPECT_EQ(retrievedSpecs.width, 1024);
        EXPECT_EQ(retrievedSpecs.height, 768);
    }

    TEST_F(OpenGLTest, ResizeWithInvalidDimensions)
    {
        FramebufferSpecs specs;
        specs.width = 800;
        specs.height = 600;
        specs.samples = 1;
        specs.attachments.attachments = {
            {FrameBufferTextureFormats::RGBA8},
            {FrameBufferTextureFormats::DEPTH24STENCIL8}
        };
        OpenGlFramebuffer framebuffer(specs);

        EXPECT_THROW(framebuffer.resize(0, 600), FramebufferResizingFailed);
        EXPECT_THROW(framebuffer.resize(800, 0), FramebufferResizingFailed);
        EXPECT_THROW(framebuffer.resize(9000, 600), FramebufferResizingFailed);
        EXPECT_THROW(framebuffer.resize(800, 9000), FramebufferResizingFailed);
    }

    TEST_F(OpenGLTest, InvalidFramebufferCreation)
    {
        FramebufferSpecs specs;
        specs.width = 0;
        specs.height = 600;

        EXPECT_THROW(OpenGlFramebuffer framebuffer(specs), FramebufferResizingFailed);
        specs.width = 800;
        specs.height = 0;
        EXPECT_THROW(OpenGlFramebuffer framebuffer(specs), FramebufferResizingFailed);
        specs.width = 9000;
        specs.height = 600;
        EXPECT_THROW(OpenGlFramebuffer framebuffer(specs), FramebufferResizingFailed);
        specs.width = 800;
        specs.height = 9000;
        EXPECT_THROW(OpenGlFramebuffer framebuffer(specs), FramebufferResizingFailed);
    }

    TEST_F(OpenGLTest, MultipleColorAttachments)
    {
        FramebufferSpecs specs;
        specs.width = 800;
        specs.height = 600;
        specs.samples = 1;
        specs.attachments.attachments = {
            {FrameBufferTextureFormats::RGBA8},
            {FrameBufferTextureFormats::RGBA16},
            {FrameBufferTextureFormats::DEPTH24STENCIL8}
        };

        // Check if the hardware supports at least the required number of attachments
        GLint maxAttachments;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttachments);
        EXPECT_GE(maxAttachments, static_cast<GLint>(specs.attachments.attachments.size()));

        OpenGlFramebuffer framebuffer(specs);

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
                       FrameBufferTextureFormats::DEPTH24STENCIL8)
            {
                EXPECT_EQ(static_cast<unsigned int>(boundTexture),
                          static_cast<unsigned int>(framebuffer.getSpecs().attachments.attachments[i].textureFormat));
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
        FramebufferSpecs specs;
        specs.width = 800;
        specs.height = 600;
        specs.samples = 1;

        // Test unsupported color format
        specs.attachments.attachments = {
            {static_cast<FrameBufferTextureFormats>(999)} // Invalid format
        };

        EXPECT_THROW(
            OpenGlFramebuffer framebuffer(specs);
            , FramebufferUnsupportedColorFormat);
    }
}
