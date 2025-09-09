//// OpenGlTexture2D.cpp //////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for opengl texture2D class
//
///////////////////////////////////////////////////////////////////////////////

#include "OpenGlTexture2D.hpp"

#include <Exception.hpp>
#include <RendererExceptions.hpp>

#include <stb_image.h>

namespace nexo::renderer {

    NxOpenGlTexture2D::NxOpenGlTexture2D(const unsigned int width, const unsigned int height) : m_width(width), m_height(height)
    {
        createOpenGLTexture(nullptr, width, height, GL_RGBA8, GL_RGBA);
    }

    NxOpenGlTexture2D::NxOpenGlTexture2D(const uint8_t *buffer, const unsigned int width, const unsigned int height,
        const NxTextureFormat format) : m_width(width), m_height(height)
    {
        if (!buffer)
            THROW_EXCEPTION(NxInvalidValue, "OPENGL", "Buffer is null");

        GLint internalFormat = 0;
        GLenum dataFormat = 0;
        switch (format) {
            [[likely]] case NxTextureFormat::RGBA8:
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            [[likely]] case NxTextureFormat::RGB8:
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;
            case NxTextureFormat::RG8:
                internalFormat = GL_RG8;
                dataFormat = GL_RG;
                break;
            case NxTextureFormat::R8:
                internalFormat = GL_R8;
                dataFormat = GL_RED;
                break;

            default:
                THROW_EXCEPTION(NxTextureUnsupportedFormat, "OPENGL", static_cast<int>(format), "");
        }

        createOpenGLTexture(buffer, width, height, internalFormat, dataFormat);
    }

    NxOpenGlTexture2D::NxOpenGlTexture2D(const std::string &path)
        : m_path(path)
    {
        int width = 0;
        int height = 0;
        int channels = 0;
        //TODO: Set this conditionnaly based on the type of texture
        stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data)
            THROW_EXCEPTION(NxFileNotFoundException, path);

        try {
            ingestDataFromStb(data, width, height, channels, path);
        } catch (const Exception&) {
            stbi_image_free(data);
            throw;
        }
        stbi_image_free(data);
    }

    NxOpenGlTexture2D::~NxOpenGlTexture2D()
    {
        glDeleteTextures(1, &m_id);
    }

    NxOpenGlTexture2D::NxOpenGlTexture2D(const uint8_t* buffer, const unsigned int len)
    {
        int width = 0;
        int height = 0;
        int channels = 0;
        //TODO: Set this conditionnaly based on the type of texture
        //stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = stbi_load_from_memory(buffer, static_cast<int>(len), &width, &height, &channels, 0);
        if (!data)
            THROW_EXCEPTION(NxTextureUnsupportedFormat, "OPENGL", channels, "(buffer)");

        try {
            ingestDataFromStb(data, width, height, channels, "(buffer)");
        } catch (const Exception&) {
            stbi_image_free(data);
            throw;
        }
        stbi_image_free(data);
    }

    unsigned int NxOpenGlTexture2D::getMaxTextureSize() const
    {
        int maxTextureSize = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        return static_cast<unsigned int>(maxTextureSize);
    }

    void NxOpenGlTexture2D::setData(void *data, const size_t size)
    {
        if (const size_t expectedSize = static_cast<size_t>(m_width) * m_height * (m_dataFormat == GL_RGBA ? 4 : 3); size != expectedSize)
            THROW_EXCEPTION(NxTextureSizeMismatch, "OPENGL", size, expectedSize);
        glBindTexture(GL_TEXTURE_2D, m_id);
        // Update the entire texture with new data
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<int>(m_width), static_cast<int>(m_height), m_dataFormat, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void NxOpenGlTexture2D::ingestDataFromStb(const uint8_t* data, const int width, const int height, const int channels,
        const std::string& debugPath)
    {
        GLint internalFormat = 0;
        GLenum dataFormat = 0;
        switch (channels) {
            [[likely]] case 4: // red, green, blue, alpha
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            [[likely]] case 3: // red, green, blue
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
                break;
            case 2: // grey, alpha
                internalFormat = GL_RG16;
                dataFormat = GL_RG16;
                break;
            case 1: // grey
                internalFormat = GL_R8;
                dataFormat = GL_RED;
                break;
            default:
                THROW_EXCEPTION(NxTextureUnsupportedFormat, "OPENGL", channels, debugPath);
        }

        createOpenGLTexture(data, static_cast<unsigned int>(width), static_cast<unsigned int>(height), internalFormat, dataFormat);
    }

    void NxOpenGlTexture2D::createOpenGLTexture(const uint8_t* buffer, const unsigned int width, const unsigned int height,
        const GLint internalFormat, const GLenum dataFormat)
    {
        const unsigned int maxTextureSize = getMaxTextureSize();
        if (width > maxTextureSize || height > maxTextureSize)
            THROW_EXCEPTION(NxTextureInvalidSize, "OPENGL", width, height, maxTextureSize);

        const auto glWidth = static_cast<GLsizei>(width);
        const auto glHeight = static_cast<GLsizei>(height);

        m_internalFormat = internalFormat;
        m_dataFormat = dataFormat;
        m_width = width;
        m_height = height;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, glWidth, glHeight, 0, m_dataFormat, GL_UNSIGNED_BYTE, buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void NxOpenGlTexture2D::bind(const unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void NxOpenGlTexture2D::unbind(const unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
