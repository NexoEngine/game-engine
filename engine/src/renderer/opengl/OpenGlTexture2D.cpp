//// OpenGlTexture2D.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for opengl texture2D class
//
///////////////////////////////////////////////////////////////////////////////

#include "OpenGlTexture2D.hpp"

#include <Exception.hpp>
#include <RendererExceptions.hpp>

#include "Logger.hpp"
#include <stb_image.h>

namespace nexo::renderer {

    OpenGlTexture2D::OpenGlTexture2D(const unsigned int width, const unsigned int height) : m_width(width), m_height(height)
    {
        const unsigned int maxTextureSize = getMaxTextureSize();
        if (width > maxTextureSize || height > maxTextureSize)
            THROW_EXCEPTION(TextureInvalidSize, "OPENGL", width, height, maxTextureSize);
        m_internalFormat = GL_RGBA8;
        m_dataFormat = GL_RGBA;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(m_internalFormat), static_cast<int>(width), static_cast<int>(height), 0, m_dataFormat, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    /**
     * @brief Constructs a 2D texture from an image file.
     *
     * Loads the image data using the STB image library from the specified file path and initializes the texture.
     * The texture's properties, such as dimensions and formats, are configured by calling ingestDataFromStb.
     *
     * @param path The filesystem path to the image to load.
     *
     * @throws FileNotFoundException If the image cannot be loaded from the provided path.
     */
    OpenGlTexture2D::OpenGlTexture2D(const std::string &path)
    {
        int width = 0;
        int height = 0;
        int channels = 0;
        //TODO: Set this conditionnaly based on the type of texture
        //stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data)
            THROW_EXCEPTION(FileNotFoundException, path);
        ingestDataFromStb(data, width, height, channels, path);
    }

    /**
     * @brief Releases the OpenGL texture resource.
     *
     * Deletes the texture object by calling glDeleteTextures, ensuring the GPU memory associated with the texture is freed.
     */
    OpenGlTexture2D::~OpenGlTexture2D()
    {
        glDeleteTextures(1, &m_id);
    }

    /**
     * @brief Constructs an OpenGlTexture2D from image data in a memory buffer.
     *
     * This constructor loads texture image data using stb_image's stbi_load_from_memory function.
     * If loading fails, it throws a TextureUnsupportedFormat exception with a debug message indicating the source as "(buffer)".
     * On success, it processes the loaded data via ingestDataFromStb to set up the texture.
     *
     * @param buffer Pointer to the memory buffer containing the image data.
     * @param len Size in bytes of the memory buffer.
     */
    OpenGlTexture2D::OpenGlTexture2D(const uint8_t* buffer, unsigned int len)
    {
        int width = 0;
        int height = 0;
        int channels = 0;
        //TODO: Set this conditionnaly based on the type of texture
        //stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = stbi_load_from_memory(buffer, len, &width, &height, &channels, 0);
        if (!data)
            THROW_EXCEPTION(TextureUnsupportedFormat, "OPENGL", channels, "(buffer)");
        ingestDataFromStb(data, width, height, channels, "(buffer)");
    }

    /**
     * @brief Retrieves the maximum texture size supported by the OpenGL implementation.
     *
     * This method calls glGetIntegerv with GL_MAX_TEXTURE_SIZE to obtain the maximum
     * dimension (in pixels) that an OpenGL texture can have.
     *
     * @return unsigned int The maximum texture size available.
     */
    unsigned int OpenGlTexture2D::getMaxTextureSize() const
    {
        int maxTextureSize = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        return static_cast<unsigned int>(maxTextureSize);
    }

    /**
     * @brief Updates the texture with new data.
     *
     * Validates that the provided data size matches the expected texture size, computed as m_width * m_height *
     * (4 when m_dataFormat is GL_RGBA, otherwise 3). If the sizes do not match, a TextureSizeMismatch exception is thrown.
     * Otherwise, the texture is bound, the full texture is updated with the new data using glTexSubImage2D, and then unbound.
     *
     * @param data Pointer to the new texture data.
     * @param size Size in bytes of the supplied data; must equal the expected texture size.
     *
     * @throws TextureSizeMismatch if the supplied size does not match the expected texture size.
     */
    void OpenGlTexture2D::setData(void *data, const unsigned int size)
    {
        if (const unsigned int expectedSize = m_width * m_height * (m_dataFormat == GL_RGBA ? 4 : 3); size != expectedSize)
            THROW_EXCEPTION(TextureSizeMismatch, "OPENGL", size, expectedSize);
        glBindTexture(GL_TEXTURE_2D, m_id);
        // Update the entire texture with new data
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<int>(m_width), static_cast<int>(m_height), m_dataFormat, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    /**
     * @brief Configures the 2D texture using image data loaded by STB.
     *
     * Sets the texture's dimensions and chooses the appropriate internal and data formats based
     * on the number of channels (3 for RGB, 4 for RGBA). Generates a new texture ID, uploads the
     * image data to OpenGL, and applies common texture parameters for filtering and wrapping.
     * If the number of channels is unsupported, frees the image data and throws a TextureUnsupportedFormat exception.
     *
     * @param data Pointer to the image data loaded by the STB library.
     * @param width Image width in pixels.
     * @param height Image height in pixels.
     * @param channels Number of channels in the image (expected to be 3 or 4).
     * @param debugPath Debug path or identifier for improved error reporting.
     */
    void OpenGlTexture2D::ingestDataFromStb(uint8_t* data, int width, int height, int channels,
        const std::string& debugPath)
    {
        m_width = width;
        m_height = height;

        GLenum internalFormat = 0;
        GLenum dataFormat = 0;

        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        } else {
            stbi_image_free(data);
            THROW_EXCEPTION(TextureUnsupportedFormat, "OPENGL", channels, debugPath);
        }

        m_internalFormat = internalFormat;
        m_dataFormat = dataFormat;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(internalFormat), width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
    }


    /**
     * @brief Binds the texture to the specified texture unit.
     *
     * Activates the texture unit corresponding to (GL_TEXTURE0 + slot) and binds this texture to the GL_TEXTURE_2D target.
     *
     * @param slot The texture unit slot to bind the texture to.
     */
    void OpenGlTexture2D::bind(const unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    /**
     * @brief Unbinds the texture from a specified texture unit.
     *
     * Sets the active texture unit to the given slot and unbinds any texture bound to the GL_TEXTURE_2D target.
     *
     * @param slot The texture unit index from which the texture will be unbound.
     */
    void OpenGlTexture2D::unbind(const unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
