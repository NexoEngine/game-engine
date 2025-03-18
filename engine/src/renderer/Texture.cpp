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
     * @brief Creates a 2D texture with the specified dimensions.
     *
     * This static method instantiates and returns a shared pointer to a Texture2D object configured for the OpenGL graphics API when available.
     * If the GRAPHICS_API_OPENGL macro is defined, an OpenGlTexture2D instance is created using the provided width and height.
     * Otherwise, the function throws an UnknownGraphicsApi exception.
     *
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @return std::shared_ptr<Texture2D> A shared pointer to the created texture.
     */
    std::shared_ptr<Texture2D> Texture2D::create(unsigned int width, unsigned int height)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlTexture2D>(width, height);
        #endif
        THROW_EXCEPTION(UnknownGraphicsApi, "UNKNOWN");
    }

    /**
     * @brief Creates a Texture2D instance from a raw data buffer.
     *
     * Constructs and returns a shared pointer to a Texture2D object using the provided
     * raw texture data. When compiled with OpenGL support (i.e., when the GRAPHICS_API_OPENGL
     * macro is defined), it instantiates an OpenGlTexture2D. Otherwise, it throws an
     * UnknownGraphicsApi exception.
     *
     * @param buffer Pointer to the raw texture data.
     * @param len Length of the data buffer in bytes.
     * @return std::shared_ptr<Texture2D> A shared pointer to the created Texture2D object.
     *
     * @throws UnknownGraphicsApi If the underlying graphics API is not supported.
     */
    std::shared_ptr<Texture2D> Texture2D::create(uint8_t* buffer, unsigned int len)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlTexture2D>(buffer, len);
        #endif
        THROW_EXCEPTION(UnknownGraphicsApi, "UNKNOWN");
    }

    /**
     * @brief Creates a Texture2D instance from a file path.
     *
     * This static factory method instantiates and returns a shared pointer to a Texture2D object
     * using the texture file specified by the given path. When compiled with the OpenGL graphics API
     * (enabled via GRAPHICS_API_OPENGL), it creates an OpenGlTexture2D object. Otherwise, it throws
     * an UnknownGraphicsApi exception.
     *
     * @param path The file system path to the texture image.
     *
     * @return A shared pointer to the created Texture2D instance.
     *
     * @throws UnknownGraphicsApi if the graphics API is not supported.
     */
    std::shared_ptr<Texture2D> Texture2D::create(const std::string &path)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlTexture2D>(path);
        #endif
        THROW_EXCEPTION(UnknownGraphicsApi, "UNKNOWN");
    }

}
