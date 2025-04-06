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
    #include "opengl/OpenGlTexture2D.hpp"
#endif

namespace nexo::renderer {

    /**
     * @brief Creates a Texture2D instance with the specified dimensions.
     *
     * This factory method instantiates a Texture2D object configured with the given width and height.
     * When the OpenGL graphics API is enabled, it returns an instance of OpenGlTexture2D; otherwise,
     * it throws an UnknownGraphicsApi exception.
     *
     * @param width The texture's width in pixels.
     * @param height The texture's height in pixels.
     * @return A shared pointer to the created Texture2D object.
     *
     * @throws UnknownGraphicsApi if the graphics API is not recognized.
     */
    std::shared_ptr<Texture2D> Texture2D::create(unsigned int width, unsigned int height)
    {
        #ifdef GRAPHICS_API_OPENGL
            return std::make_shared<OpenGlTexture2D>(width, height);
        #endif
        THROW_EXCEPTION(UnknownGraphicsApi, "UNKNOWN");
    }

    /**
     * @brief Creates a Texture2D object from raw pixel data.
     *
     * Constructs a Texture2D using a provided buffer of pixel data and its length.
     * When compiled with the OpenGL graphics API (GRAPHICS_API_OPENGL defined), an OpenGlTexture2D is instantiated with the data.
     * Otherwise, an UnknownGraphicsApi exception is thrown.
     *
     * @param buffer Pointer to the raw pixel data.
     * @param len Length of the pixel data buffer in bytes.
     * @return std::shared_ptr<Texture2D> A shared pointer to the created Texture2D object.
     *
     * @throws UnknownGraphicsApi if the graphics API is not supported.
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
     * Constructs a Texture2D object by loading an image from the specified file path.
     * When compiled with OpenGL support (i.e., GRAPHICS_API_OPENGL is defined), it returns
     * an instance of OpenGlTexture2D. Otherwise, it throws an UnknownGraphicsApi exception.
     *
     * @param path The file path to the texture image.
     * @return std::shared_ptr<Texture2D> A shared pointer to the newly created Texture2D object.
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
