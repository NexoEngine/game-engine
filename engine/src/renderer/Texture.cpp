//// Texture.cpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for texture abstract classes
//
///////////////////////////////////////////////////////////////////////////////

#include "Texture.hpp"
#include "Renderer.hpp"
#include "renderer/RendererExceptions.hpp"
#ifdef GRAPHICS_API_OPENGL
    #include "GraphicsBackends/opengl/OpenGlTexture2D.hpp"
#endif

namespace nexo::renderer {

    std::shared_ptr<Texture2D> Texture2D::create(unsigned int width, unsigned int height)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlTexture2D>(width, height);
        #endif
        THROW_EXCEPTION(UnknownGraphicsApi, "UNKNOWN");
    }

    std::shared_ptr<Texture2D> Texture2D::create(uint8_t* buffer, unsigned int len)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlTexture2D>(buffer, len);
        #endif
        THROW_EXCEPTION(UnknownGraphicsApi, "UNKNOWN");
    }

    std::shared_ptr<Texture2D> Texture2D::create(const std::string &path)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlTexture2D>(path);
        #endif
        THROW_EXCEPTION(UnknownGraphicsApi, "UNKNOWN");
    }

}
