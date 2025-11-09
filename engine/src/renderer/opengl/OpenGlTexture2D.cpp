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

#include <stb_image.h>

namespace nexo::renderer {

    NxOpenGlTexture2D::NxOpenGlTexture2D(const unsigned int width, const unsigned int height)
        : m_width(width), m_height(height)
    {
        createOpenGLTexture(nullptr, width, height, GL_RGBA8, GL_RGBA);
    }

    NxOpenGlTexture2D::NxOpenGlTexture2D(const uint8_t *buffer, const unsigned int width, const unsigned int height,
                                         const NxTextureFormat format)
        : m_width(width), m_height(height)
    {
        if (!buffer) THROW_EXCEPTION(NxInvalidValue, "OPENGL", "Buffer is null");

        GLint internalFormat = 0;
        GLenum dataFormat    = 0;
        switch (format) {
            [[likely]] case NxTextureFormat::RGBA8:
                internalFormat = GL_RGBA8;
                dataFormat     = GL_RGBA;
                break;
            [[likely]] case NxTextureFormat::RGB8:
                internalFormat = GL_RGB8;
                dataFormat     = GL_RGB;
                break;
            case NxTextureFormat::RG8:
                internalFormat = GL_RG8;
                dataFormat     = GL_RG;
                break;
            case NxTextureFormat::R8:
                internalFormat = GL_R8;
                dataFormat     = GL_RED;
                break;


            default:
                THROW_EXCEPTION(NxTextureUnsupportedFormat, "OPENGL", static_cast<int>(format), "");
        }

        createOpenGLTexture(buffer, width, height, internalFormat, dataFormat);
    }

    NxOpenGlTexture2D::NxOpenGlTexture2D(const std::string &path) : m_path(path)
    {
        int width    = 0;
        int height   = 0;
        int channels = 0;
        // TODO: Set this conditionnaly based on the type of texture
        stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) THROW_EXCEPTION(NxFileNotFoundException, path);

        try {
            ingestDataFromStb(data, width, height, channels, path);
        } catch (const Exception &) {
            stbi_image_free(data);
            throw;
        }
        stbi_image_free(data);
    }

    NxOpenGlTexture2D::~NxOpenGlTexture2D()
    {
        glDeleteTextures(1, &m_id);
    }

    NxOpenGlTexture2D::NxOpenGlTexture2D(const uint8_t *buffer, const unsigned int len)
    {
        int width    = 0;
        int height   = 0;
        int channels = 0;
        // TODO: Set this conditionnaly based on the type of texture
        // stbi_set_flip_vertically_on_load(1);
        stbi_uc *data = stbi_load_from_memory(buffer, static_cast<int>(len), &width, &height, &channels, 0);
        if (!data) THROW_EXCEPTION(NxTextureUnsupportedFormat, "OPENGL", channels, "(buffer)");

        try {
            ingestDataFromStb(data, width, height, channels, "(buffer)");
        } catch (const Exception &) {
            stbi_image_free(data);
            throw;
        }
        stbi_image_free(data);
    }

    NxOpenGlTexture2D::NxOpenGlTexture2D(const uint8_t *buffer, unsigned int len, TextureType type, bool generateMipmaps)
        : m_textureType(type)
    {
        int width = 0, height = 0, channels = 0;

        // Configure STB based on texture type
        configureSTBForTextureType(type);

        stbi_uc *data = nullptr;

        // Check for HDR textures (for emissive maps)
        if ((type == TextureType::EMISSIVE || type == TextureType::ALBEDO) && stbi_is_hdr_from_memory(buffer, static_cast<int>(len))) {
            float *hdrData = stbi_loadf_from_memory(buffer, static_cast<int>(len), &width, &height, &channels, 0);
            if (!hdrData) THROW_EXCEPTION(NxTextureUnsupportedFormat, "OPENGL", channels, "(buffer)");

            try {
                ingestHDRDataFromStb(hdrData, width, height, channels, "(buffer)", type);
            } catch (const Exception &) {
                stbi_image_free(hdrData);
                throw;
            }
            stbi_image_free(hdrData);
        } else {
            data = stbi_load_from_memory(buffer, static_cast<int>(len), &width, &height, &channels, 0);
            if (!data) THROW_EXCEPTION(NxTextureUnsupportedFormat, "OPENGL", channels, "(buffer)");

            try {
                ingestDataFromStb(data, width, height, channels, "(buffer)", type);
            } catch (const Exception &) {
                stbi_image_free(data);
                throw;
            }
            stbi_image_free(data);
        }

        if (generateMipmaps) {
            configureForTextureType(type);
        }
    }

    NxOpenGlTexture2D::NxOpenGlTexture2D(const uint8_t *buffer, unsigned int width, unsigned int height,
                                         NxTextureFormat format, TextureType type, bool generateMipmaps)
        : m_width(width), m_height(height), m_textureType(type)
    {
        if (!buffer) THROW_EXCEPTION(NxInvalidValue, "OPENGL", "Buffer is null");

        GLint internalFormat = 0;
        GLenum dataFormat = 0;
        bool useSRGB = (type == TextureType::ALBEDO || type == TextureType::EMISSIVE);

        switch (format) {
            [[likely]] case NxTextureFormat::RGBA8:
                internalFormat = useSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            [[likely]] case NxTextureFormat::RGB8:
                internalFormat = useSRGB ? GL_SRGB8 : GL_RGB8;
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

        createOpenGLTexture(buffer, width, height, internalFormat, dataFormat, false, type);  // sRGB handled above

        if (generateMipmaps) {
            configureForTextureType(type);
        }
    }

    void NxOpenGlTexture2D::configureSTBForTextureType(TextureType type)
    {
        switch (type) {
            case TextureType::NORMAL:
                // Don't flip normal maps - they need special handling
                stbi_set_flip_vertically_on_load(0);
                break;
            case TextureType::HEIGHT:
                // Height maps might not need flipping depending on your pipeline
                stbi_set_flip_vertically_on_load(0);
                break;
            default:
                // Most textures need flipping for OpenGL
                stbi_set_flip_vertically_on_load(1);
                break;
        }
    }

    void NxOpenGlTexture2D::ingestHDRDataFromStb(const float *data, int width, int height,
                                                 int channels, const std::string &debugPath, TextureType type)
    {
        GLint internalFormat = 0;
        GLenum dataFormat = 0;

        switch (channels) {
            case 4:
                internalFormat = GL_RGBA16F;
                dataFormat = GL_RGBA;
                break;
            case 3:
                internalFormat = GL_RGB16F;
                dataFormat = GL_RGB;
                break;
            case 1:
                internalFormat = GL_R16F;
                dataFormat = GL_RED;
                break;
            default:
                THROW_EXCEPTION(NxTextureUnsupportedFormat, "OPENGL", channels, debugPath);
        }

        createHDROpenGLTexture(data, static_cast<unsigned int>(width), static_cast<unsigned int>(height),
                              internalFormat, dataFormat, type);
    }

    void NxOpenGlTexture2D::createHDROpenGLTexture(const float *buffer, unsigned int width, unsigned int height,
                                                   GLint internalFormat, GLenum dataFormat, TextureType type)
    {
        // Similar to createOpenGLTexture but for float data
        const auto glWidth = static_cast<GLsizei>(width);
        const auto glHeight = static_cast<GLsizei>(height);

        m_internalFormat = internalFormat;
        m_dataFormat = dataFormat;
        m_width = width;
        m_height = height;
        m_textureType = type;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, glWidth, glHeight, 0, m_dataFormat, GL_FLOAT, buffer);

        // Default parameters - will be overridden by configureForTextureType
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    NxOpenGlTexture2D::NxOpenGlTexture2D(const std::string &path, TextureType type, bool generateMipmaps)
        : m_path(path), m_textureType(type)
    {
        int width = 0, height = 0, channels = 0;

        // Configure STB based on texture type
        configureSTBForTextureType(type);

        stbi_uc *data = nullptr;

        // Check for HDR textures (for emissive maps)
        if ((type == TextureType::EMISSIVE || type == TextureType::ALBEDO) && stbi_is_hdr(path.c_str())) {
            float *hdrData = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
            if (!hdrData) THROW_EXCEPTION(NxFileNotFoundException, path);

            try {
                ingestHDRDataFromStb(hdrData, width, height, channels, path, type);
            } catch (const Exception &) {
                stbi_image_free(hdrData);
                throw;
            }
            stbi_image_free(hdrData);
        } else {
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
            if (!data) THROW_EXCEPTION(NxFileNotFoundException, path);

            try {
                ingestDataFromStb(data, width, height, channels, path, type);
            } catch (const Exception &) {
                stbi_image_free(data);
                throw;
            }
            stbi_image_free(data);
        }

        if (generateMipmaps) {
            configureForTextureType(type);
        }
    }

    unsigned int NxOpenGlTexture2D::getMaxTextureSize() const
    {
        int maxTextureSize = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
        return static_cast<unsigned int>(maxTextureSize);
    }

    void NxOpenGlTexture2D::setData(void *data, const size_t size)
    {
        // Calculate bytes per pixel based on internal format
        int bytesPerPixel = 4; // default for RGBA8

        switch (m_dataFormat) {
            case GL_RGBA: bytesPerPixel = 4; break;
            case GL_RGB:  bytesPerPixel = 3; break;
            case GL_RG:   bytesPerPixel = 2; break;
            case GL_RED:  bytesPerPixel = 1; break;
            default: break;
        }

        // For HDR textures, multiply by appropriate size
        GLenum dataType = GL_UNSIGNED_BYTE;
        if (m_internalFormat == GL_RGB16F || m_internalFormat == GL_RGBA16F || m_internalFormat == GL_R16F) {
            bytesPerPixel *= 2; // 16-bit floats
            dataType = GL_HALF_FLOAT;
        } else if (m_internalFormat == GL_RGB32F || m_internalFormat == GL_RGBA32F) {
            bytesPerPixel *= 4; // 32-bit floats
            dataType = GL_FLOAT;
        }

        const size_t expectedSize = static_cast<size_t>(m_width) * m_height * bytesPerPixel;
        if (size != expectedSize) {
            THROW_EXCEPTION(NxTextureSizeMismatch, "OPENGL", size, expectedSize);
        }

        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<int>(m_width), static_cast<int>(m_height),
                       m_dataFormat, dataType, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void NxOpenGlTexture2D::ingestDataFromStb(const uint8_t *data, int width, int height,
                                              int channels, const std::string &debugPath, TextureType type)
    {
        GLint internalFormat = 0;
        GLenum dataFormat = 0;
        bool useSRGB = (type == TextureType::ALBEDO || type == TextureType::EMISSIVE);

        switch (channels) {
            case 4:
                internalFormat = useSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
                dataFormat = GL_RGBA;
                break;
            case 3:
                internalFormat = useSRGB ? GL_SRGB8 : GL_RGB8;
                dataFormat = GL_RGB;
                break;
            case 2:
                internalFormat = GL_RG8;  // Fixed from RG16
                dataFormat = GL_RG;       // Fixed from RG16
                break;
            case 1:
                internalFormat = GL_R8;
                dataFormat = GL_RED;
                break;
            default:
                THROW_EXCEPTION(NxTextureUnsupportedFormat, "OPENGL", channels, debugPath);
        }

        createOpenGLTexture(data, static_cast<unsigned int>(width), static_cast<unsigned int>(height),
                           internalFormat, dataFormat, false, type);  // sRGB handled above
    }

    void NxOpenGlTexture2D::configureForTextureType(TextureType type) {
        glBindTexture(GL_TEXTURE_2D, m_id);

        switch (type) {
            case TextureType::ALBEDO:
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;

            case TextureType::NORMAL:
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                // Optional: Add anisotropic filtering for normal maps
                break;

            case TextureType::METALLIC:
            case TextureType::ROUGHNESS:
            case TextureType::AO:
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;

            case TextureType::EMISSIVE:
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;

            case TextureType::HEIGHT:
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;

            case TextureType::COMBINED_MR:
                // Metallic in B, Roughness in G
                setChannelSwizzle(ChannelSwizzle::ZERO, ChannelSwizzle::GREEN, ChannelSwizzle::BLUE, ChannelSwizzle::ONE);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;

            case TextureType::COMBINED_ORM:
                // AO in R, Roughness in G, Metallic in B
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void NxOpenGlTexture2D::setChannelSwizzle(ChannelSwizzle r, ChannelSwizzle g, ChannelSwizzle b, ChannelSwizzle a)
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
        GLint swizzle[] = {static_cast<GLint>(r), static_cast<GLint>(g), static_cast<GLint>(b), static_cast<GLint>(a)};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void NxOpenGlTexture2D::createOpenGLTexture(const uint8_t *buffer, const unsigned int width,
                                                const unsigned int height, const GLint internalFormat,
                                                const GLenum dataFormat, bool isSRGB, TextureType type)
    {
        const unsigned int maxTextureSize = getMaxTextureSize();
        if (width > maxTextureSize || height > maxTextureSize)
            THROW_EXCEPTION(NxTextureInvalidSize, "OPENGL", width, height, maxTextureSize);

        const auto glWidth  = static_cast<GLsizei>(width);
        const auto glHeight = static_cast<GLsizei>(height);

        m_internalFormat = internalFormat;
        m_dataFormat = dataFormat;
        m_width = width;
        m_height = height;
        m_textureType = type;

        GLint actualInternalFormat = internalFormat;
        if (isSRGB && (internalFormat == GL_RGBA8 || internalFormat == GL_RGB8)) {
            switch (internalFormat) {
                case GL_RGBA8: actualInternalFormat = GL_SRGB8_ALPHA8; break;
                case GL_RGB8:  actualInternalFormat = GL_SRGB8; break;
                default: break;
            }
        }
        m_internalFormat = actualInternalFormat;

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

} // namespace nexo::renderer
