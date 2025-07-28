//// Input.cpp ////////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the input class
//
///////////////////////////////////////////////////////////////////////////////
#include "Input.hpp"
#include "renderer/RendererExceptions.hpp"
#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/InputOpenGl.hpp"
#endif


namespace nexo::event {

    std::shared_ptr<Input> Input::_instance = nullptr;

    void Input::init(const std::shared_ptr<renderer::NxWindow>& window)
    {
        if (!_instance)
        {
            #ifdef NX_GRAPHICS_API_OPENGL
                _instance = std::make_shared<InputOpenGl>(window);
                return;
            #endif
            THROW_EXCEPTION(renderer::NxUnknownGraphicsApi, "UNKNOWN");
        }
    }


}
