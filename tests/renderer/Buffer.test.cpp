//// Buffer.test.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        22/11/2024
//  Description: Test file for the buffers class
//
///////////////////////////////////////////////////////////////////////////////


#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Buffer.hpp"
#include "GraphicsBackends/opengl/OpenGlBuffer.hpp"
#include "contexts/opengl.hpp"

namespace nexo::renderer {
    TEST(BufferLayout, BasicConstruction)
    {
        BufferLayout layout = {
            {ShaderDataType::FLOAT3, "Position"},
            {ShaderDataType::FLOAT4, "Color"}
        };

        EXPECT_EQ(layout.getStride(), 28); // FLOAT3 (12 bytes) + FLOAT4 (16 bytes)
        EXPECT_EQ(layout.getElements().size(), 2);

        const auto &elements = layout.getElements();
        EXPECT_EQ(elements[0].offset, 0);
        EXPECT_EQ(elements[1].offset, 12);
    }

    TEST(BufferLayout, EmptyLayout)
    {
        BufferLayout layout;
        EXPECT_EQ(layout.getStride(), 0);
        EXPECT_TRUE(layout.getElements().empty());
    }

    TEST(BufferLayout, ComponentCount)
    {
        BufferElements element(ShaderDataType::FLOAT3, "Position");
        EXPECT_EQ(element.getComponentCount(), 3);

        element = BufferElements(ShaderDataType::MAT4, "Matrix");
        EXPECT_EQ(element.getComponentCount(), 16);
    }

    TEST(BufferLayout, LargeLayout)
    {
        BufferLayout layout = {
            {ShaderDataType::FLOAT3, "Position"},
            {ShaderDataType::FLOAT4, "Color"},
            {ShaderDataType::FLOAT2, "UV"},
            {ShaderDataType::MAT4, "ModelMatrix"},
            {ShaderDataType::INT, "ID"}
        };

        EXPECT_EQ(layout.getStride(), 104); // 12 + 16 + 8 + 64 + 4
        EXPECT_EQ(layout.getElements().size(), 5);

        const auto &elements = layout.getElements();
        EXPECT_EQ(elements[0].offset, 0); // Position
        EXPECT_EQ(elements[1].offset, 12); // Color
        EXPECT_EQ(elements[2].offset, 28); // UV
        EXPECT_EQ(elements[3].offset, 36); // ModelMatrix
        EXPECT_EQ(elements[4].offset, 100); // ID
    }

    TEST(BufferLayout, MixedNormalization)
    {
        BufferLayout layout = {
            {ShaderDataType::FLOAT4, "Position", true},
            {ShaderDataType::INT3, "BoneIDs"},
            {ShaderDataType::FLOAT4, "Weights", true}
        };

        EXPECT_EQ(layout.getStride(), 44); // 16 + 12 + 16
        EXPECT_EQ(layout.getElements().size(), 3);

        const auto &elements = layout.getElements();
        EXPECT_TRUE(elements[0].normalized);
        EXPECT_FALSE(elements[1].normalized);
        EXPECT_TRUE(elements[2].normalized);
    }

    TEST_F(OpenGLTest, VertexBufferCreationAndBinding)
    {
        OpenGlVertexBuffer buffer1(nullptr, 100);
        OpenGlVertexBuffer buffer2(nullptr, 100);

        // Validate buffer IDs are unique
        EXPECT_NE(buffer1.getId(), buffer2.getId());

        // Check binding state for buffer1
        buffer1.bind();
        GLint boundBuffer;
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBuffer);
        EXPECT_EQ(boundBuffer, buffer1.getId());

        // Unbind buffer1 and validate
        buffer1.unbind();
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBuffer);
        EXPECT_EQ(boundBuffer, 0);

        // Check binding state for buffer2
        buffer2.bind();
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBuffer);
        EXPECT_EQ(boundBuffer, buffer2.getId());

        buffer2.unbind();
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBuffer);
        EXPECT_EQ(boundBuffer, 0);
    }

    TEST_F(OpenGLTest, VertexBufferDataUpdate)
    {
        float vertices[] = {0.0f, 1.0f, 2.0f};
        OpenGlVertexBuffer buffer(vertices, sizeof(vertices));

        buffer.bind();
        // Validate the data provided
        float bufferData[3];
        glGetNamedBufferSubData(buffer.getId(), 0, sizeof(vertices), bufferData);
        for (int i = 0; i < 3; i++)
        {
            EXPECT_EQ(bufferData[i], vertices[i]);
        }

        // Modify the values and do the same
        float updatedVertices[] = {3.0f, 4.0f, 5.0f};
        buffer.setData(updatedVertices, sizeof(updatedVertices));
        glGetNamedBufferSubData(buffer.getId(), 0, sizeof(vertices), bufferData);
        for (int i = 0; i < 3; i++)
        {
            EXPECT_EQ(bufferData[i], updatedVertices[i]);
        }

        buffer.unbind();
    }

    TEST_F(OpenGLTest, IndexBufferCreationAndBinding)
    {
        OpenGlIndexBuffer buffer1;
        OpenGlIndexBuffer buffer2;

        // Validate buffer IDs are unique
        EXPECT_NE(buffer1.getId(), buffer2.getId());

        // Check binding state for buffer1
        buffer1.bind();
        GLint boundBuffer;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundBuffer);
        EXPECT_EQ(boundBuffer, buffer1.getId());

        // Unbind buffer1 and validate
        buffer1.unbind();
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundBuffer);
        EXPECT_EQ(boundBuffer, 0);

        // Check binding state for buffer2
        buffer2.bind();
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundBuffer);
        EXPECT_EQ(boundBuffer, buffer2.getId());

        buffer2.unbind();
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundBuffer);
        EXPECT_EQ(boundBuffer, 0);
    }

    TEST_F(OpenGLTest, IndexBufferDataUpdate)
    {
        OpenGlIndexBuffer buffer;

        unsigned int indices[] = {0, 1, 2, 2, 3, 0};
        // Set data binds the buffer
        buffer.setData(indices, 6);
        // Verify that the buffer got updated
        EXPECT_EQ(buffer.getCount(), 6);

        GLint bufferSize;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
        // Validate bounded buffer size
        EXPECT_EQ(bufferSize, sizeof(indices));
        // Validate the data provided
        unsigned int bufferData[6];
        glGetNamedBufferSubData(buffer.getId(), 0, sizeof(indices), bufferData);
        for (int i = 0; i < 6; i++)
        {
            EXPECT_EQ(bufferData[i], indices[i]);
        }

        // Same thing with different size and different values
        unsigned int updatedIndices[] = {0, 3, 2, 2, 1, 0, 7, 8};
        buffer.setData(updatedIndices, 8);
        EXPECT_EQ(buffer.getCount(), 8);
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
        EXPECT_EQ(bufferSize, sizeof(updatedIndices));
        unsigned int updatedBufferData[8];
        glGetNamedBufferSubData(buffer.getId(), 0, sizeof(updatedIndices), updatedBufferData);
        for (int i = 0; i < 8; i++)
        {
            EXPECT_EQ(updatedBufferData[i], updatedIndices[i]);
        }
        buffer.unbind();
    }
}
