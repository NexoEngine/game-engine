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

#include "Logger.hpp"
#include "exceptions/Exceptions.hpp"
#include "openglImGuiBackend.hpp"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <loguru/loguru.hpp>

namespace nexo::editor {
    void OpenGLImGuiBackend::init(GLFWwindow *window)
    {
        if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init("#version 330"))
            THROW_EXCEPTION(BackendRendererApiInitFailed, "OPENGL");
    }

    void OpenGLImGuiBackend::shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void OpenGLImGuiBackend::initFontAtlas()
    {
        if (!ImGui_ImplOpenGL3_CreateFontsTexture())
            THROW_EXCEPTION(BackendRendererApiFontInitFailed, "OPENGL");
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
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    void* OpenGLImGuiBackend::getErrorCallback()
    {
        static auto errorCallback = [](int error, const char* description) {
            switch (error) {
                case GLFW_NOT_INITIALIZED:
                    case GLFW_NO_CURRENT_CONTEXT:
                    THROW_EXCEPTION(BackendRendererApiFatalFailure, "OPENGL", "(" + std::to_string(error) + "): " + description);

                case GLFW_INVALID_ENUM:
                case GLFW_INVALID_VALUE:
                    LOG(NEXO_WARN, "[OPENGL WARNING] ({}): {}", error, description);
                break;

                case GLFW_OUT_OF_MEMORY:
                    THROW_EXCEPTION(BackendRendererApiFatalFailure, "OPENGL", "(" + std::to_string(error) + "): Out of memory - " + description);

                case GLFW_API_UNAVAILABLE:
                case GLFW_VERSION_UNAVAILABLE:
                case GLFW_FORMAT_UNAVAILABLE:
                    LOG(NEXO_ERROR, "[OPENGL ERROR] ({}): {}", error, description);
                break;

                case GLFW_PLATFORM_ERROR:
                    LOG(NEXO_ERROR, "[OPENGL PLATFORM ERROR] ({}): {}", error, description);
                break;

                case GLFW_NO_WINDOW_CONTEXT:
                    LOG(NEXO_WARN, "[OPENGL WARNING] ({}): {}", error, description);
                break;

                default:
                    LOG(NEXO_ERROR, "[OPENGL UNKNOWN ERROR] ({}): {}", error, description);
            }
        };

        return reinterpret_cast<void*>(+errorCallback);
    }
}

