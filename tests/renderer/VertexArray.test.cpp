//// VertexArray.test.cpp //////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//
//  Author:      Mehdy MORVAN
//  Date:        22/11/2024
//  Description: Test file for the VertexArray class
//
///////////////////////////////////////////////////////////////////////////////

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "opengl/OpenGlVertexArray.hpp"
#include "opengl/OpenGlBuffer.hpp"
#include "contexts/opengl.hpp"
#include "RendererExceptions.hpp"

namespace nexo::renderer {

    TEST_F(OpenGLTest, VertexArrayCreationAndBinding)
    {
        auto vertexArray1 = std::make_shared<NxOpenGlVertexArray>();
        auto vertexArray2 = std::make_shared<NxOpenGlVertexArray>();

        EXPECT_NE(vertexArray1->getId(), vertexArray2->getId());

        vertexArray1->bind();
        GLint boundVertexArray;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVertexArray);
        EXPECT_EQ(boundVertexArray, vertexArray1->getId());
        vertexArray1->unbind();
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVertexArray);
        EXPECT_EQ(boundVertexArray, 0);

        vertexArray2->bind();
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVertexArray);
        EXPECT_EQ(boundVertexArray, vertexArray2->getId());
        vertexArray2->unbind();
    }

    TEST_F(OpenGLTest, AddVertexBuffer)
    {
        auto vertexArray = std::make_shared<NxOpenGlVertexArray>();

        float vertices[] = {
            0.0f, 0.0f, 0.0f, // Position
            1.0f, 1.0f, 1.0f, 1.0f, // Color
            3, // Texture index
        };
        auto vertexBuffer = std::make_shared<NxOpenGlVertexBuffer>(
            vertices,
            static_cast<unsigned int>(sizeof(vertices))
        );
        NxBufferLayout layout = {
            {NxShaderDataType::FLOAT3, "Position"},
            {NxShaderDataType::FLOAT4, "Color", true},
            {NxShaderDataType::INT, "TextureIndex"},
        };
        vertexBuffer->setLayout(layout);

        vertexArray->addVertexBuffer(vertexBuffer);

        const auto &buffers = vertexArray->getVertexBuffers();
        ASSERT_EQ(buffers.size(), 1);
        EXPECT_EQ(buffers[0], vertexBuffer);

        vertexArray->bind();
        vertexBuffer->bind();
        GLint value;
        // Validate vertex array binding
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &value);
        EXPECT_EQ(value, GL_TRUE);

        // Validate components size (pos, color and texture index)
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_SIZE, &value);
        EXPECT_EQ(value, 3);
        glGetVertexAttribiv(1, GL_VERTEX_ATTRIB_ARRAY_SIZE, &value);
        EXPECT_EQ(value, 4);
        glGetVertexAttribiv(2, GL_VERTEX_ATTRIB_ARRAY_SIZE, &value);
        EXPECT_EQ(value, 1);

        // Validate stride
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &value);
        EXPECT_EQ(value, layout.getStride());

        // Validate type
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_TYPE, &value);
        EXPECT_EQ(value, GL_FLOAT);
        glGetVertexAttribiv(2, GL_VERTEX_ATTRIB_ARRAY_TYPE, &value);
        EXPECT_EQ(value, GL_INT);

        // Validate normalization
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &value);
        EXPECT_EQ(value, GL_FALSE);
        glGetVertexAttribiv(1, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &value);
        EXPECT_EQ(value, GL_TRUE);
    }

    TEST_F(OpenGLTest, InvalidVertexBuffer)
    {
        auto vertexArray = std::make_shared<NxOpenGlVertexArray>();

        float vertices[] = {
            0.0f, 0.0f, 0.0f, // Position
            1.0f, 1.0f, 1.0f, 1.0f, // Color
            3, // Texture index
        };
        auto vertexBuffer = std::make_shared<NxOpenGlVertexBuffer>(
            vertices,
            static_cast<unsigned int>(sizeof(vertices))
        );

        // Empty layout
        EXPECT_THROW(
            vertexArray->addVertexBuffer(vertexBuffer),
            NxBufferLayoutEmpty
        );

        // Null vertex buffer
        EXPECT_THROW(
            vertexArray->addVertexBuffer(nullptr),
            NxInvalidValue
        );
    }

    TEST_F(OpenGLTest, MultipleVertexBuffers)
    {
        auto vertexArray = std::make_shared<NxOpenGlVertexArray>();

        float positions[] = {0.0f, 1.0f, 2.0f};
        auto positionBuffer = std::make_shared<NxOpenGlVertexBuffer>(positions, static_cast<unsigned int>(sizeof(positions)));
        positionBuffer->setLayout({
            {NxShaderDataType::FLOAT3, "Position"}
        });

        float colors[] = {1.0f, 0.0f, 0.0f};
        auto colorBuffer = std::make_shared<NxOpenGlVertexBuffer>(colors, static_cast<unsigned int>(sizeof(colors)));
        colorBuffer->setLayout({
            {NxShaderDataType::FLOAT3, "Color"}
        });

        vertexArray->addVertexBuffer(positionBuffer);
        vertexArray->addVertexBuffer(colorBuffer);

        // Validate vertex buffers are correctly stored
        const auto &buffers = vertexArray->getVertexBuffers();
        ASSERT_EQ(buffers.size(), 2);
        EXPECT_EQ(buffers[0], positionBuffer);
        EXPECT_EQ(buffers[1], colorBuffer);

        vertexArray->bind();
        GLint enabledPosition, enabledColor;
        // Validate vertex buffers bindings
        glGetVertexAttribiv(0, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabledPosition);
        glGetVertexAttribiv(1, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabledColor);

        EXPECT_EQ(enabledPosition, GL_TRUE);
        EXPECT_EQ(enabledColor, GL_TRUE);
    }

    TEST_F(OpenGLTest, SetIndexBuffer)
    {
        auto vertexArray = std::make_shared<NxOpenGlVertexArray>();

        unsigned int indices[] = {0, 1, 2};
        auto indexBuffer = std::make_shared<NxOpenGlIndexBuffer>();
        indexBuffer->setData(indices, 3);

        vertexArray->setIndexBuffer(indexBuffer);

        // Validate index buffer is correctly stored
        const auto &boundIndexBuffer = vertexArray->getIndexBuffer();
        ASSERT_NE(boundIndexBuffer, nullptr);
        EXPECT_EQ(boundIndexBuffer, indexBuffer);

        vertexArray->bind();
        GLint elementBuffer;
        // Validate index buffer is bound
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementBuffer);
        EXPECT_NE(elementBuffer, 0);
    }

    TEST_F(OpenGLTest, InvalidIndexBuffer)
    {
        auto vertexArray = std::make_shared<NxOpenGlVertexArray>();

        EXPECT_THROW(
            vertexArray->setIndexBuffer(nullptr),
            NxInvalidValue
        );
    }
}
