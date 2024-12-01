//// ImGuiBackend.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source file for the imgui backend interface class
//
///////////////////////////////////////////////////////////////////////////////

#include "ImGuiBackend.hpp"
#include "exceptions/Exceptions.hpp"
#ifdef GRAPHICS_API_OPENGL
    #include "opengl/openglImGuiBackend.hpp"
#endif

namespace nexo::editor {

    void ImGuiBackend::init(const std::shared_ptr<renderer::Window>& window)
    {
        #ifdef GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::init(static_cast<GLFWwindow*>(window->window()));
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
    }

    void ImGuiBackend::shutdown()
    {
        #ifdef GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::shutdown();
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
    }

    void ImGuiBackend::initFontAtlas()
    {
        #ifdef GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::initFontAtlas();
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
    }

    void ImGuiBackend::begin()
    {
        #ifdef GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::begin();
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
    }

    void ImGuiBackend::end(const std::shared_ptr<renderer::Window>& window)
    {
        #ifdef GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::end(static_cast<GLFWwindow*>(window->window()));
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
    }

    void ImGuiBackend::setErrorCallback(const std::shared_ptr<renderer::Window> &window)
    {
        #ifdef GRAPHICS_API_OPENGL
            const auto callback = OpenGLImGuiBackend::getErrorCallback();
            window->setErrorCallback(callback);
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");;
    }

}