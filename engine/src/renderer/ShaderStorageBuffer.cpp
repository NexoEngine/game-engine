//// ShaderStorageBuffer.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Source file for the SSBOs
//
///////////////////////////////////////////////////////////////////////////////

#include "ShaderStorageBuffer.hpp"
#include "renderer/RendererExceptions.hpp"
#include <memory>
#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/OpenGlShaderStorageBuffer.hpp"
#endif

namespace nexo::renderer {
	std::shared_ptr<NxShaderStorageBuffer> NxShaderStorageBuffer::create(unsigned int size)
	{
  		#ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlShaderStorageBuffer>(size);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
	}
}
