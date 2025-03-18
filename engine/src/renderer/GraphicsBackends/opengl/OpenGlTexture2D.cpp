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
     * @brief Constructs an OpenGL 2D texture by loading image data from a file.
     *
     * Loads the image from the specified file path using stb_image, retrieving its width, height, and channel count.
     * If the image data cannot be loaded, a FileNotFoundException is thrown.
     * Upon successful loading, the image data is ingested using ingestDataFromStb(), which sets up the texture.
     *
     * @param path The file path of the texture image.
     *
     * @throws FileNotFoundException Thrown if the image at the given path cannot be loaded.
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
     * Deletes the texture associated with this instance by calling glDeleteTextures 
     * with the texture identifier.
     */
    OpenGlTexture2D::~OpenGlTexture2D()
    {
        glDeleteTextures(1, &m_id);
    }

    /**
     * @brief Constructs a 2D texture from an in-memory image buffer.
     *
     * This constructor loads image data from the provided memory buffer using stb_image's
     * stbi_load_from_memory. On successful loading, it initializes the texture by processing
     * the image data with ingestDataFromStb. If the image data cannot be loaded due to an
     * unsupported format, it throws a TextureUnsupportedFormat exception.
     *
     * @param buffer Pointer to the image data buffer.
     * @param len Size of the image data buffer in bytes.
     *
     * @throws TextureUnsupportedFormat if the image data cannot be loaded from the buffer.
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
     * This function queries OpenGL for the maximum allowed texture size and returns it as an unsigned integer.
     *
     * @return The maximum texture size.
     */
    unsigned int OpenGlTexture2D::getMaxTextureSize() const
    {
        int maxTextureSize = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        return static_cast<unsigned int>(maxTextureSize);
    }

    /**
     * @brief Updates the entire texture with new pixel data.
     *
     * Validates that the provided data size matches the expected texture size, calculated as
     * width * height * (4 for GL_RGBA or 3 otherwise). If the sizes do not match, a
     * TextureSizeMismatch exception is thrown.
     *
     * The function binds the texture, updates its entire content using glTexSubImage2D, and then unbinds it.
     *
     * @param data Pointer to the new texture data.
     * @param size Size, in bytes, of the new data.
     * @throws TextureSizeMismatch if the provided size does not match the expected texture size.
     */
    void OpenGlTexture2D::setData(void *data, const unsigned int size)
    {
        if (const unsigned int expectedSize = m_width * m_height * (m_dataFormat == GL_RGBA ? 4 : 3); size != expectedSize)
            THROW_EXCEPTION(TextureSizeMismatch, "OPENGL", size, expectedSize);
        glBindTexture(GL_TEXTURE_2D, m_id);
        // Update the entire texture with new data
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<int>(m_width), static_cast<int>(m_height), GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    /**
     * @brief Processes STB image data and initializes an OpenGL 2D texture.
     *
     * This method sets the texture's dimensions and determines the appropriate OpenGL internal 
     * and data formats based on the number of color channels (supports 3 for RGB and 4 for RGBA). 
     * It generates an OpenGL texture, binds it, uploads the image data, configures filtering and 
     * wrapping parameters, and then frees the image data. If an unsupported channel count is provided, 
     * the image data is freed and a TextureUnsupportedFormat exception is thrown.
     *
     * @param data Pointer to image data loaded by STB; the data is freed within this method.
     * @param width Width of the image in pixels.
     * @param height Height of the image in pixels.
     * @param channels Number of color channels; must be 3 (RGB) or 4 (RGBA).
     * @param debugPath Debug identifier (e.g., file path) used for error reporting if the format is unsupported.
     *
     * @throws TextureUnsupportedFormat if the image's channel count is not 3 or 4.
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
     * @brief Binds the texture to a specified texture unit.
     *
     * Activates the texture unit using `glActiveTexture` and binds the texture to the `GL_TEXTURE_2D` target.
     *
     * @param slot The texture unit slot to which this texture will be bound.
     */
    void OpenGlTexture2D::bind(const unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    /**
     * @brief Unbinds the texture from the specified texture unit.
     *
     * This method sets the active OpenGL texture unit to the given slot and unbinds any 2D texture
     * by binding texture ID 0.
     *
     * @param slot The texture unit index from which the texture is unbound.
     */
    void OpenGlTexture2D::unbind(const unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
