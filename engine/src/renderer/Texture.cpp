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

    /**
     * @brief Creates a 2D texture with the specified width and height.
     *
     * Instantiates a Texture2D object using the provided dimensions. If the OpenGL graphics API is enabled
     * (indicated by the GRAPHICS_API_OPENGL macro), the function returns an instance of OpenGlTexture2D.
     * Otherwise, it throws an UnknownGraphicsApi exception.
     *
     * @param width The width of the texture in pixels.
     * @param height The height of the texture in pixels.
     * @return std::shared_ptr<Texture2D> A shared pointer to the created Texture2D instance.
     *
     * @throws UnknownGraphicsApi If the graphics API is not OpenGL.
     */
    std::shared_ptr<Texture2D> Texture2D::create(unsigned int width, unsigned int height)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlTexture2D>(width, height);
        #endif
        THROW_EXCEPTION(UnknownGraphicsApi, "UNKNOWN");
    }

    /**
     * @brief Creates a Texture2D object from a buffer of pixel data.
     *
     * Instantiates a Texture2D using the provided raw pixel data. When the OpenGL graphics API is available,
     * it returns a shared pointer to an OpenGL texture implementation; otherwise, it throws an UnknownGraphicsApi exception.
     *
     * @param buffer Pointer to the raw texture pixel data.
     * @param len Length of the pixel data in bytes.
     * @return std::shared_ptr<Texture2D> A shared pointer to the created Texture2D object.
     *
     * @throws UnknownGraphicsApi If a supported graphics API is not available.
     */
    std::shared_ptr<Texture2D> Texture2D::create(uint8_t* buffer, unsigned int len)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlTexture2D>(buffer, len);
        #endif
        THROW_EXCEPTION(UnknownGraphicsApi, "UNKNOWN");
    }

    /**
     * @brief Creates a 2D texture from an image file.
     *
     * Instantiates a Texture2D object using the provided file path. When the OpenGL graphics API is enabled,
     * an OpenGlTexture2D is created from the image file. Otherwise, an UnknownGraphicsApi exception is thrown.
     *
     * @param path The file path to the texture image.
     * @return A shared pointer to the created Texture2D object.
     * @throws UnknownGraphicsApi if the OpenGL graphics API is not supported.
     */
    std::shared_ptr<Texture2D> Texture2D::create(const std::string &path)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlTexture2D>(path);
        #endif
        THROW_EXCEPTION(UnknownGraphicsApi, "UNKNOWN");
    }

}
