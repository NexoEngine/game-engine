///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Test file for Buffer classes
//
///////////////////////////////////////////////////////////////////////////////
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "renderer/Buffer.hpp"
#include "opengl/OpenGlBuffer.hpp"
#include "renderer/RendererExceptions.hpp"
#include <array>

namespace nexo::renderer {

    // Mock for testing abstract classes
    class MockVertexBuffer : public NxVertexBuffer {
    public:
        MOCK_METHOD(void, bind, (), (const, override));
        MOCK_METHOD(void, unbind, (), (const, override));
        MOCK_METHOD(void, setLayout, (const NxBufferLayout&), (override));
        MOCK_METHOD(NxBufferLayout, getLayout, (), (const, override));
        MOCK_METHOD(void, setData, (void*, size_t), (override));
        MOCK_METHOD(unsigned int, getId, (), (const, override));
    };

    class MockIndexBuffer : public NxIndexBuffer {
    public:
        MOCK_METHOD(void, bind, (), (const, override));
        MOCK_METHOD(void, unbind, (), (const, override));
        MOCK_METHOD(void, setData, (unsigned int*, size_t), (override));
        MOCK_METHOD(size_t, getCount, (), (const, override));
        MOCK_METHOD(unsigned int, getId, (), (const, override));
    };

    // Test fixture for OpenGL-based tests
    class OpenGLBufferTest : public ::testing::Test {
    protected:
    	GLFWwindow *window = nullptr;

        void SetUp() override {
	        if (!glfwInit())
	        {
	            GTEST_SKIP() << "GLFW initialization failed. Skipping OpenGL tests.";
	        }

	        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	        window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
	        if (!window)
	        {
	            glfwTerminate();
	            GTEST_SKIP() << "Failed to create GLFW window. Skipping OpenGL tests.";
	        }

	        glfwMakeContextCurrent(window);

	        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	        {
	            glfwDestroyWindow(window);
	            glfwTerminate();
	            GTEST_SKIP() << "Failed to initialize GLAD. Skipping OpenGL tests.";
	        }

	        GLint major = 0, minor = 0;
	        glGetIntegerv(GL_MAJOR_VERSION, &major);
	        glGetIntegerv(GL_MINOR_VERSION, &minor);
	        if (major < 4 || (major == 4 && minor < 5))
	        {
	            glfwDestroyWindow(window);
	            glfwTerminate();
	            GTEST_SKIP() << "OpenGL 4.5 is required. Skipping OpenGL tests.";
	        }
        }

        void TearDown() override {
            // Clean up if needed
        }
    };

    // Test fixture for factory function tests
    class BufferFactoryTest : public ::testing::Test {
    protected:
   		GLFWwindow *window = nullptr;

        void SetUp() override {
	        if (!glfwInit())
	        {
	            GTEST_SKIP() << "GLFW initialization failed. Skipping OpenGL tests.";
	        }

	        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	        window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
	        if (!window)
	        {
	            glfwTerminate();
	            GTEST_SKIP() << "Failed to create GLFW window. Skipping OpenGL tests.";
	        }

	        glfwMakeContextCurrent(window);

	        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	        {
	            glfwDestroyWindow(window);
	            glfwTerminate();
	            GTEST_SKIP() << "Failed to initialize GLAD. Skipping OpenGL tests.";
	        }

	        GLint major = 0, minor = 0;
	        glGetIntegerv(GL_MAJOR_VERSION, &major);
	        glGetIntegerv(GL_MINOR_VERSION, &minor);
	        if (major < 4 || (major == 4 && minor < 5))
	        {
	            glfwDestroyWindow(window);
	            glfwTerminate();
	            GTEST_SKIP() << "OpenGL 4.5 is required. Skipping OpenGL tests.";
	        }
        }
    };

    // Tests for ShaderDataType operations
    TEST(ShaderDataTypeTest, ShaderDataTypeSizeReturnsCorrectSizes) {
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::FLOAT), 4);
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::FLOAT2), 8);
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::FLOAT3), 12);
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::FLOAT4), 16);
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::MAT3), 36);
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::MAT4), 64);
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT), 4);
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT2), 8);
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT3), 12);
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::INT4), 16);
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::BOOL), 1);
        EXPECT_EQ(shaderDataTypeSize(NxShaderDataType::NONE), 0);
    }

    // Tests for BufferElements
    TEST(BufferElementsTest, ConstructorSetsProperties) {
        NxBufferElements element(NxShaderDataType::FLOAT3, "Position", false);

        EXPECT_EQ(element.name, "Position");
        EXPECT_EQ(element.type, NxShaderDataType::FLOAT3);
        EXPECT_EQ(element.size, 12); // FLOAT3 = 3 * 4 bytes
        EXPECT_EQ(element.offset, 0); // Initial offset is 0
        EXPECT_FALSE(element.normalized);
    }

    TEST(BufferElementsTest, GetComponentCountReturnsCorrectCount) {
        EXPECT_EQ(NxBufferElements(NxShaderDataType::FLOAT, "").getComponentCount(), 1);
        EXPECT_EQ(NxBufferElements(NxShaderDataType::FLOAT2, "").getComponentCount(), 2);
        EXPECT_EQ(NxBufferElements(NxShaderDataType::FLOAT3, "").getComponentCount(), 3);
        EXPECT_EQ(NxBufferElements(NxShaderDataType::FLOAT4, "").getComponentCount(), 4);
        EXPECT_EQ(NxBufferElements(NxShaderDataType::INT, "").getComponentCount(), 1);
        EXPECT_EQ(NxBufferElements(NxShaderDataType::INT2, "").getComponentCount(), 2);
        EXPECT_EQ(NxBufferElements(NxShaderDataType::INT3, "").getComponentCount(), 3);
        EXPECT_EQ(NxBufferElements(NxShaderDataType::INT4, "").getComponentCount(), 4);
        EXPECT_EQ(NxBufferElements(NxShaderDataType::MAT3, "").getComponentCount(), 9);
        EXPECT_EQ(NxBufferElements(NxShaderDataType::MAT4, "").getComponentCount(), 16);
        EXPECT_EQ(NxBufferElements(NxShaderDataType::BOOL, "").getComponentCount(), 1);
        EXPECT_EQ(NxBufferElements(NxShaderDataType::NONE, "").getComponentCount(), -1);
    }

    // Tests for BufferLayout
    TEST(BufferLayoutTest, ConstructorWithInitializerListCalculatesOffsetsAndStride) {
        NxBufferLayout layout = {
            {NxShaderDataType::FLOAT3, "Position"},
            {NxShaderDataType::FLOAT4, "Color"},
            {NxShaderDataType::FLOAT2, "TexCoord"}
        };

        EXPECT_EQ(layout.getStride(), 36); // 12 + 16 + 8

        auto elements = layout.getElements();
        EXPECT_EQ(elements.size(), 3);

        EXPECT_EQ(elements[0].offset, 0);
        EXPECT_EQ(elements[1].offset, 12);
        EXPECT_EQ(elements[2].offset, 28);
    }

    TEST(BufferLayoutTest, EmptyConstructorCreatesEmptyLayout) {
        NxBufferLayout layout;
        EXPECT_EQ(layout.getStride(), 0);
        EXPECT_TRUE(layout.getElements().empty());
    }

    TEST(BufferLayoutTest, IteratorFunctionsWork) {
        NxBufferLayout layout = {
            {NxShaderDataType::FLOAT3, "Position"},
            {NxShaderDataType::FLOAT4, "Color"}
        };

        int count = 0;
        for ([[maybe_unused]] auto& element : layout) {
            count++;
        }
        EXPECT_EQ(count, 2);
    }

    // Mocking tests for abstract classes
    TEST(BufferMockTest, MockVertexBufferCanCallMethods) {
        MockVertexBuffer buffer;
        NxBufferLayout layout = {
            {NxShaderDataType::FLOAT3, "Position"}
        };

        EXPECT_CALL(buffer, setLayout(testing::_)).Times(1);
        EXPECT_CALL(buffer, getLayout()).WillOnce(testing::Return(layout));
        EXPECT_CALL(buffer, bind()).Times(1);
        EXPECT_CALL(buffer, unbind()).Times(1);
        EXPECT_CALL(buffer, setData(testing::_, testing::_)).Times(1);
        EXPECT_CALL(buffer, getId()).WillOnce(testing::Return(1));

        buffer.setLayout(layout);
        NxBufferLayout retrievedLayout = buffer.getLayout();
        buffer.bind();
        buffer.unbind();
        float data[] = {1.0f, 2.0f, 3.0f};
        buffer.setData(data, sizeof(data));
        unsigned int id = buffer.getId();

        EXPECT_EQ(retrievedLayout.getStride(), layout.getStride());
        EXPECT_EQ(id, 1);
    }

    TEST(BufferMockTest, MockIndexBufferCanCallMethods) {
        MockIndexBuffer buffer;

        EXPECT_CALL(buffer, bind()).Times(1);
        EXPECT_CALL(buffer, unbind()).Times(1);
        EXPECT_CALL(buffer, setData(testing::_, testing::_)).Times(1);
        EXPECT_CALL(buffer, getCount()).WillOnce(testing::Return(6));
        EXPECT_CALL(buffer, getId()).WillOnce(testing::Return(2));

        buffer.bind();
        buffer.unbind();
        unsigned int indices[] = {0, 1, 2, 2, 3, 0};
        buffer.setData(indices, 6);
        size_t count = buffer.getCount();
        unsigned int id = buffer.getId();

        EXPECT_EQ(count, 6);
        EXPECT_EQ(id, 2);
    }

    // Factory function tests - using TEST_F with BufferFactoryTest fixture
    TEST_F(BufferFactoryTest, CreateVertexBufferWithDataReturnsValidBuffer) {
        float vertices[] = {1.0f, 2.0f, 3.0f};

        #ifdef NX_GRAPHICS_API_OPENGL
            auto buffer = createVertexBuffer(vertices, sizeof(vertices));
            ASSERT_NE(buffer, nullptr);
            EXPECT_NE(buffer->getId(), 0);
        #else
            EXPECT_THROW(createVertexBuffer(vertices, sizeof(vertices)), UnknownGraphicsApi);
        #endif
    }

    TEST_F(BufferFactoryTest, CreateVertexBufferWithSizeReturnsValidBuffer) {
        #ifdef NX_GRAPHICS_API_OPENGL
            auto buffer = createVertexBuffer(1024);
            ASSERT_NE(buffer, nullptr);
            EXPECT_NE(buffer->getId(), 0);
        #else
            EXPECT_THROW(createVertexBuffer(1024), UnknownGraphicsApi);
        #endif
    }

    TEST_F(BufferFactoryTest, CreateIndexBufferReturnsValidBuffer) {
        #ifdef NX_GRAPHICS_API_OPENGL
            auto buffer = createIndexBuffer();
            ASSERT_NE(buffer, nullptr);
            EXPECT_NE(buffer->getId(), 0);
        #else
            EXPECT_THROW(createIndexBuffer(), UnknownGraphicsApi);
        #endif
    }

    // OpenGL specific implementation tests - using TEST_F with OpenGLBufferTest fixture
    #ifdef NX_GRAPHICS_API_OPENGL
    TEST_F(OpenGLBufferTest, OpenGlVertexBufferWithDataWorksCorrectly) {

        float vertices[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
        NxOpenGlVertexBuffer buffer(vertices, sizeof(vertices));

        EXPECT_NE(buffer.getId(), 0);

        // Test bind/unbind without crashes
        EXPECT_NO_THROW(buffer.bind());
        EXPECT_NO_THROW(buffer.unbind());

        // Test layout setting and retrieval
        NxBufferLayout layout = {
            {NxShaderDataType::FLOAT3, "Position"}
        };
        EXPECT_NO_THROW(buffer.setLayout(layout));
        NxBufferLayout retrievedLayout = buffer.getLayout();
        EXPECT_EQ(retrievedLayout.getStride(), layout.getStride());

        // Test setting new data
        float newData[] = {7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f};
        EXPECT_NO_THROW(buffer.setData(newData, sizeof(newData)));
    }

    TEST_F(OpenGLBufferTest, OpenGlVertexBufferEmptyConstructorWorksCorrectly) {

        NxOpenGlVertexBuffer buffer(1024);

        EXPECT_NE(buffer.getId(), 0);

        // Test bind/unbind without crashes
        EXPECT_NO_THROW(buffer.bind());
        EXPECT_NO_THROW(buffer.unbind());

        // Test setting data after creation
        float data[] = {1.0f, 2.0f, 3.0f, 4.0f};
        EXPECT_NO_THROW(buffer.setData(data, sizeof(data)));
    }

    TEST_F(OpenGLBufferTest, OpenGlIndexBufferWorksCorrectly) {

        NxOpenGlIndexBuffer buffer;

        EXPECT_NE(buffer.getId(), 0);

        // Test bind/unbind without crashes
        EXPECT_NO_THROW(buffer.bind());
        EXPECT_NO_THROW(buffer.unbind());

        // Test setting indices and count retrieval
        unsigned int indices[] = {0, 1, 2, 2, 3, 0};
        EXPECT_NO_THROW(buffer.setData(indices, 6));
        EXPECT_EQ(buffer.getCount(), 6);
    }
    #endif // NX_GRAPHICS_API_OPENGL
}
