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
     * @brief Constructs a 2D texture by loading image data from a file.
     *
     * This constructor attempts to load the image specified by @p path using stb_image.
     * If the loading fails, it throws a FileNotFoundException. On success, the loaded image data
     * is processed to initialize the texture dimensions, channels, and format.
     *
     * @param path The file system path to the texture image.
     *
     * @throws FileNotFoundException if the texture image cannot be loaded.
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
     * @brief Destructor that frees the OpenGL texture resource.
     *
     * Invokes glDeleteTextures to delete the associated texture from the GPU, ensuring proper resource cleanup.
     */
    OpenGlTexture2D::~OpenGlTexture2D()
    {
        glDeleteTextures(1, &m_id);
    }

    /**
     * @brief Creates an OpenGL 2D texture from an image stored in a memory buffer.
     *
     * Loads image data from the provided buffer using stb_image and initializes the texture by determining its width, height, and channel count.
     * If the image cannot be loaded due to an unsupported format, a TextureUnsupportedFormat exception is thrown.
     *
     * @param buffer Pointer to the in-memory image data.
     * @param len Length of the image data buffer in bytes.
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
     * @brief Retrieves the maximum texture size supported by the current OpenGL context.
     *
     * Queries OpenGL for the GL_MAX_TEXTURE_SIZE value, which defines the maximum allowed dimension
     * for textures, and returns it as an unsigned integer.
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
     * @brief Updates the entire texture with new data.
     *
     * Validates that the size of the provided data matches the texture's expected size,
     * calculated as width × height multiplied by 4 if the texture is in RGBA format or 3 otherwise.
     * If the size does not match, a TextureSizeMismatch exception is thrown.
     *
     * The function binds the texture, updates its contents using glTexSubImage2D, and unbinds it.
     *
     * @param data Pointer to the new texture data.
     * @param size Total size, in bytes, of the texture data buffer.
     *
     * @throws TextureSizeMismatch if the provided size does not correspond to the expected texture size.
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
     * @brief Processes image data from stb_image to initialize an OpenGL texture.
     *
     * This method sets the texture dimensions and determines the appropriate internal and data
     * formats based on the number of channels (3 for RGB, 4 for RGBA). It then generates an OpenGL
     * texture, binds it, allocates storage with the chosen formats, and configures filtering and wrapping
     * parameters. If the channel count is unsupported, the provided image data is freed and a
     * TextureUnsupportedFormat exception is thrown with the given debug path.
     *
     * After texture creation, the image data is freed.
     *
     * @param data Pointer to the raw image data loaded via stb_image.
     * @param width Image width in pixels.
     * @param height Image height in pixels.
     * @param channels Number of color channels in the image (supported values: 3 or 4).
     * @param debugPath Path used for error debugging if an unsupported format is encountered.
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
     * Activates the texture unit (GL_TEXTURE0 + slot) and binds the texture to GL_TEXTURE_2D.
     *
     * @param slot The texture unit slot to which the texture is bound.
     */
    void OpenGlTexture2D::bind(const unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    /**
     * @brief Unbinds the 2D texture from the specified texture unit.
     *
     * Sets the active texture unit to the given slot and unbinds any 2D texture currently bound to it.
     *
     * @param slot The index of the texture unit from which to unbind the texture.
     */
    void OpenGlTexture2D::unbind(const unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
