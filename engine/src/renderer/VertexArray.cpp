//// VertexArray.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for vertex array abstract class
//
///////////////////////////////////////////////////////////////////////////////
#include "VertexArray.hpp"
#include "core/exceptions/Exceptions.hpp"
#ifdef GRAPHICS_API_OPENGL
    #include "opengl/OpenGlVertexArray.hpp"
#endif

namespace nexo::renderer {

    std::shared_ptr<VertexArray> createVertexArray()
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlVertexArray>();
        #endif
        throw core::UnknownGraphicsApi("UNKNOWN");
    }

}
