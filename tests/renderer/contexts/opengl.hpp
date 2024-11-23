//// opengl.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        23/11/2024
//  Description: Header for opengl tests context
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

#include "renderer/Buffer.hpp"

namespace nexo::renderer {

    // Class to set up an opengl context
    class OpenGLTest : public ::testing::Test {
        protected:
        GLFWwindow* window = nullptr;

        void SetUp() override {
            if (!glfwInit()) {
                FAIL() << "Failed to initialize GLFW";
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
            if (!window) {
                glfwTerminate();
                FAIL() << "Failed to create GLFW window";
            }

            glfwMakeContextCurrent(window);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                glfwDestroyWindow(window);
                glfwTerminate();
                FAIL() << "Failed to initialize GLAD";
            }
        }

        void TearDown() override {
            if (window) {
                glfwDestroyWindow(window);
            }
            glfwTerminate();
        }
    };

    class MockVertexBuffer : public VertexBuffer {
        public:
        MOCK_METHOD(void, bind, (), (const, override));
        MOCK_METHOD(void, unbind, (), (const, override));
        MOCK_METHOD(void, setLayout, (const BufferLayout &layout), (override));
        MOCK_METHOD(const BufferLayout, getLayout, (), (const, override));
        MOCK_METHOD(void, setData, (void *data, unsigned int size), (override));
        MOCK_METHOD(unsigned int, getId, (), (const override));
    };

    class MockIndexBuffer : public IndexBuffer {
        public:
        MOCK_METHOD(void, bind, (), (const, override));
        MOCK_METHOD(void, unbind, (), (const, override));
        MOCK_METHOD(void, setData, (unsigned int *data, unsigned int size), (override));
        MOCK_METHOD(unsigned int, getCount, (), (const, override));
        MOCK_METHOD(unsigned int, getId, (), (const, override));
    };
}