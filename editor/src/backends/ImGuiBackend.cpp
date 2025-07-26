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
#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/openglImGuiBackend.hpp"
#endif

namespace nexo::editor {

    void ImGuiBackend::init([[maybe_unused]] const std::shared_ptr<renderer::NxWindow>& window)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::init(static_cast<GLFWwindow*>(window->window()));
        #else
            THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
        #endif
    }

    void ImGuiBackend::shutdown()
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::shutdown();
        #else
            THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
        #endif
    }

    void ImGuiBackend::initFontAtlas()
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::initFontAtlas();
        #else
            THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
        #endif
    }

    void ImGuiBackend::begin()
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::begin();
        #else
            THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
        #endif
    }

    void ImGuiBackend::end([[maybe_unused]] const std::shared_ptr<renderer::NxWindow>& window)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::end(static_cast<GLFWwindow*>(window->window()));
        #else
            THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
        #endif
    }

    void ImGuiBackend::setErrorCallback([[maybe_unused]] const std::shared_ptr<renderer::NxWindow> &window)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            const auto callback = OpenGLImGuiBackend::getErrorCallback();
            window->setErrorCallback(callback);
        #else
            THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
        #endif
    }

}
