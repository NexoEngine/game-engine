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
#include "Logger.hpp"
#include <stb_image.h>

namespace nexo::renderer {

    OpenGlTexture2D::OpenGlTexture2D(const unsigned int width, const unsigned int height) : m_width(width), m_height(height)
    {
        m_internalFormat = GL_RGBA8;
        m_dataFormat = GL_RGBA;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<int>(m_internalFormat), static_cast<int>(width), static_cast<int>(height), 0, m_dataFormat, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    OpenGlTexture2D::OpenGlTexture2D(const std::string &path)
    {
        int width, height, channels = 0;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            LOG(NEXO_ERROR, "[OPENGL] File not found: {}", path);
            return;
        }
        m_width = width;
        m_height = height;

        GLenum internalFormat = 0;
        GLenum dataFormat = 0;

        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        else
        {
            LOG(NEXO_ERROR, "[OPENGL] Unsupported image format with {} channels in {}", channels, path);
            stbi_image_free(data);
            return;
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

    OpenGlTexture2D::~OpenGlTexture2D()
    {
        glDeleteTextures(1, &m_id);
    }

    void OpenGlTexture2D::setData(void *data, const unsigned int size)
    {
        if (const unsigned int expectedSize = m_width * m_height * (m_dataFormat == GL_RGBA ? 4 : 3); size != expectedSize) {
            LOG(NEXO_ERROR, "[OPENGL] Data size does not match the texture size: {} != {}", size, expectedSize);
            return;
        }
        glBindTexture(GL_TEXTURE_2D, m_id);
        // Update the entire texture with new data
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<int>(m_width), static_cast<int>(m_height), GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGlTexture2D::bind(const unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}