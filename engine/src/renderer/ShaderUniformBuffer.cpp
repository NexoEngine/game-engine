//// ShaderUniformBuffer.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Source file for the Uniform Buffer (UBO)
//
///////////////////////////////////////////////////////////////////////////////////////////

#include "ShaderUniformBuffer.hpp"

#include <memory>
#include "renderer/RendererExceptions.hpp"

#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/OpenGlShaderUniformBuffer.hpp"
#endif

namespace nexo::renderer {

    std::shared_ptr<NxShaderUniformBuffer> NxShaderUniformBuffer::create(unsigned int size)
    {
#ifdef NX_GRAPHICS_API_OPENGL
        return std::make_shared<NxOpenGlShaderUniformBuffer>(size);
#else
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
#endif
    }

} // namespace nexo::renderer
