//// Input.cpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the input class
//
///////////////////////////////////////////////////////////////////////////////
#include "Input.hpp"
#include "core/exceptions/Exceptions.hpp"
#ifdef GRAPHICS_API_OPENGL
    #include "opengl/InputOpenGl.hpp"
#endif


namespace nexo::event {

    std::shared_ptr<Input> Input::_instance = nullptr;

    void Input::init(const std::shared_ptr<renderer::Window>& window)
    {
        if (!_instance)
        {
            #ifdef GRAPHICS_API_OPENGL
                _instance = std::make_shared<InputOpenGl>(window);
                return;
            #endif
            THROW_EXCEPTION(core::UnknownGraphicsApi, "UNKNOWN");
        }
    }


}