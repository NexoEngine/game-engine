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

    NxOpenGlTexture2D::NxOpenGlTexture2D(const unsigned int width, const unsigned int height) : m_width(width), m_height(height)
    {
        const unsigned int maxTextureSize = getMaxTextureSize();
        if (width > maxTextureSize || height > maxTextureSize)
            THROW_EXCEPTION(NxTextureInvalidSize, "OPENGL", width, height, maxTextureSize);
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

    NxOpenGlTexture2D::NxOpenGlTexture2D(const std::string &path)
    {
        int width = 0;
        int height = 0;
        int channels = 0;
        //TODO: Set this conditionnaly based on the type of texture
        //stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data)
            THROW_EXCEPTION(NxFileNotFoundException, path);
        ingestDataFromStb(data, width, height, channels, path);
    }

    NxOpenGlTexture2D::~NxOpenGlTexture2D()
    {
        glDeleteTextures(1, &m_id);
    }

    NxOpenGlTexture2D::NxOpenGlTexture2D(const uint8_t* buffer, unsigned int len)
    {
        int width = 0;
        int height = 0;
        int channels = 0;
        //TODO: Set this conditionnaly based on the type of texture
        //stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = stbi_load_from_memory(buffer, len, &width, &height, &channels, 0);
        if (!data)
            THROW_EXCEPTION(NxTextureUnsupportedFormat, "OPENGL", channels, "(buffer)");
        ingestDataFromStb(data, width, height, channels, "(buffer)");
    }

    unsigned int NxOpenGlTexture2D::getMaxTextureSize() const
    {
        int maxTextureSize = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        return static_cast<unsigned int>(maxTextureSize);
    }

    void NxOpenGlTexture2D::setData(void *data, const unsigned int size)
    {
        if (const unsigned int expectedSize = m_width * m_height * (m_dataFormat == GL_RGBA ? 4 : 3); size != expectedSize)
            THROW_EXCEPTION(NxTextureSizeMismatch, "OPENGL", size, expectedSize);
        glBindTexture(GL_TEXTURE_2D, m_id);
        // Update the entire texture with new data
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<int>(m_width), static_cast<int>(m_height), m_dataFormat, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void NxOpenGlTexture2D::ingestDataFromStb(uint8_t* data, int width, int height, int channels,
        const std::string& debugPath)
    {
        m_width = width;
        m_height = height;

        GLenum internalFormat = 0;
        GLenum dataFormat = 0;

        switch (channels) {
            [[likely]] case 4:
                m_internalFormat = GL_RGBA8;
                m_dataFormat = GL_RGBA;
                break;
            [[likely]] case 3:
                m_internalFormat = GL_RGB8;
                m_dataFormat = GL_RGB;
                break;
            case 2:
                m_internalFormat = GL_RG8;
                m_dataFormat = GL_RG;
                break;
            case 1:
                m_internalFormat = GL_R8;
                m_dataFormat = GL_RED;
                break;
            default:
                stbi_image_free(data);
                THROW_EXCEPTION(NxTextureUnsupportedFormat, "OPENGL", channels, debugPath);
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
