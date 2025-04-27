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
#include "String.hpp"

#ifdef NX_GRAPHICS_API_OPENGL
    #include "opengl/OpenGlTexture2D.hpp"
#endif

namespace nexo::renderer {

    NxTextureFormat NxTextureFormatFromString(const std::string_view& format)
    {
        if (iequals(format, "R8"))    return NxTextureFormat::R8;
        if (iequals(format, "RG8"))   return NxTextureFormat::RG8;
        if (iequals(format, "RGB8"))  return NxTextureFormat::RGB8;
        if (iequals(format, "RGBA8")) return NxTextureFormat::RGBA8;
        return NxTextureFormat::INVALID;
    }

    std::shared_ptr<NxTexture2D> NxTexture2D::create(unsigned int width, unsigned int height)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlTexture2D>(width, height);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }

    std::shared_ptr<NxTexture2D> NxTexture2D::create(const uint8_t *buffer, unsigned int width, unsigned int height,
        NxTextureFormat format)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlTexture2D>(buffer, width, height, format);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }

    std::shared_ptr<NxTexture2D> NxTexture2D::create(const uint8_t* buffer, unsigned int len)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlTexture2D>(buffer, len);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }

    std::shared_ptr<NxTexture2D> NxTexture2D::create(const std::string &path)
    {
        #ifdef NX_GRAPHICS_API_OPENGL
            return std::make_shared<NxOpenGlTexture2D>(path);
        #endif
        THROW_EXCEPTION(NxUnknownGraphicsApi, "UNKNOWN");
    }

}
