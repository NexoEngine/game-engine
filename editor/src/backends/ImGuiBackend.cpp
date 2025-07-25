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
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
    }

    void ImGuiBackend::shutdown()
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::shutdown();
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
    }

    void ImGuiBackend::initFontAtlas()
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::initFontAtlas();
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
    }

    void ImGuiBackend::begin()
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::begin();
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
    }

    void ImGuiBackend::end([[maybe_unused]] const std::shared_ptr<renderer::NxWindow>& window)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            OpenGLImGuiBackend::end(static_cast<GLFWwindow*>(window->window()));
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
    }

    void ImGuiBackend::setErrorCallback([[maybe_unused]] const std::shared_ptr<renderer::NxWindow> &window)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            const auto callback = OpenGLImGuiBackend::getErrorCallback();
            window->setErrorCallback(callback);
            return;
        #endif
        THROW_EXCEPTION(BackendRendererApiNotSupported, "UNKNOWN");
    }

}
