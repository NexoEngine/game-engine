//// openglImGuiBackend.cpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source for the imgui backend opengl implementation
//
///////////////////////////////////////////////////////////////////////////////

#include "openglImGuiBackend.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <loguru.hpp>

namespace nexo::editor {
    void OpenGLImGuiBackend::init(GLFWwindow *window)
    {
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
    }

    void OpenGLImGuiBackend::shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void OpenGLImGuiBackend::initFontAtlas()
    {
        ImGui_ImplOpenGL3_CreateFontsTexture();
    }

    void OpenGLImGuiBackend::begin()
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void OpenGLImGuiBackend::end(GLFWwindow *window)
    {
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    void* OpenGLImGuiBackend::getErrorCallback()
    {
        // Define the error callback
        static auto errorCallback = [](int error, const char* description) {
            switch (error) {
                case GLFW_NOT_INITIALIZED:
                case GLFW_NO_CURRENT_CONTEXT:
                    LOG_F(FATAL, "[OPENGL ERROR] (%d): %s", error, description);
                break;

                case GLFW_INVALID_ENUM:
                case GLFW_INVALID_VALUE:
                    LOG_F(WARNING, "[OPENGL WARNING] (%d): %s", error, description);
                break;

                case GLFW_OUT_OF_MEMORY:
                    LOG_F(FATAL, "[OPENGL FATAL ERROR] (%d): Out of memory - %s", error, description);
                break;

                case GLFW_API_UNAVAILABLE:
                case GLFW_VERSION_UNAVAILABLE:
                case GLFW_FORMAT_UNAVAILABLE:
                    LOG_F(ERROR, "[OPENGL ERROR] (%d): %s", error, description);
                break;

                case GLFW_PLATFORM_ERROR:
                    LOG_F(ERROR, "[OPENGL PLATFORM ERROR] (%d): %s", error, description);
                break;

                case GLFW_NO_WINDOW_CONTEXT:
                    LOG_F(WARNING, "[OPENGL WARNING] (%d): %s", error, description);
                break;

                default:
                    LOG_F(ERROR, "[OPENGL UNKNOWN ERROR] (%d): %s", error, description);
            }
        };

        return reinterpret_cast<void*>(+errorCallback);
    }

}

