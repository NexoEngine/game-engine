//// OpenGlTexture2D.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for open gl texture class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderer/Texture.hpp"
#include <glad/glad.h>

namespace nexo::renderer {
    /**
    * @class NxOpenGlTexture2D
    * @brief OpenGL-specific implementation of the `NxTexture2D` class.
    *
    * The `NxOpenGlTexture2D` class manages 2D textures in an OpenGL rendering context.
    * It supports texture creation, data uploading, and binding/unbinding operations.
    *
    * Responsibilities:
    * - Create textures using OpenGL APIs.
    * - Load texture data from files or raw memory.
    * - Provide texture binding and unbinding functionality.
    */
    class NxOpenGlTexture2D final : public NxTexture2D {
        public:
            ~NxOpenGlTexture2D() override;

            /**
            * @brief Loads an OpenGL 2D texture from an image file.
            *
            * Reads texture data from the specified file path and uploads it to the GPU.
            * Supports common image formats (e.g., PNG, JPG). Automatically detects the
            * internal and data formats based on the number of channels in the image.
            *
            * @param path The file path to the texture image.
            * @throw NxFileNotFoundException If the file cannot be found.
            * @throw NxTextureUnsupportedFormat If the image format is unsupported.
            *
            * Example:
            * ```cpp
            * auto texture = std::make_shared<NxOpenGlTexture2D>("textures/wood.jpg");
            * ```
            */
            explicit NxOpenGlTexture2D(const std::string &path);

            /**
            * @brief Creates a blank OpenGL 2D texture with the specified dimensions.
            *
            * Initializes a texture with the given width and height, using default OpenGL
            * parameters for filtering and wrapping.
            *
            * @param width The width of the texture in pixels.
            * @param height The height of the texture in pixels.
            *
            * Example:
            * ```cpp
            * auto texture = std::make_shared<NxOpenGlTexture2D>(256, 256);
            * ```
            */
            NxOpenGlTexture2D(unsigned int width, unsigned int height);

            /**
             * @brief Creates an OpenGL 2D texture from raw pixel data.
             *
             * Creates a texture from a raw pixel buffer with the specified dimensions and format.
             * This is useful for creating textures from procedurally generated data or when working
             * with raw pixel data from other sources.
             *
             * @param buffer Pointer to the raw pixel data. The buffer should contain pixel data
             *        in a format that matches the specified NxTextureFormat. The data consists
             *        of height scanlines of width pixels, with each pixel consisting of N components
             *        (where N depends on the format). The first pixel pointed to is bottom-left-most
             *        in the image. There is no padding between image scanlines or between pixels.
             *        Each component is an 8-bit unsigned value (uint8_t).
             * @param width The width of the texture in pixels. Must not exceed the maximum texture
             *        size supported by the GPU.
             * @param height The height of the texture in pixels. Must not exceed the maximum texture
             *        size supported by the GPU.
             * @param format The format of the pixel data, which determines the number of components
             *        per pixel and the internal OpenGL representation:
             *        - NxTextureFormat::R8: Single channel (GL_R8/GL_RED)
             *        - NxTextureFormat::RG8: Two channels (GL_RG8/GL_RG)
             *        - NxTextureFormat::RGB8: Three channels (GL_RGB8/GL_RGB)
             *        - NxTextureFormat::RGBA8: Four channels (GL_RGBA8/GL_RGBA)
             * @return A shared pointer to the created NxTexture2D instance.
             *
             * @throws NxInvalidValue If the buffer is null.
             * @throws NxTextureUnsupportedFormat If the specified format is not supported.
             * @throws NxTextureInvalidSize If the dimensions exceed the maximum texture size.
             *
             * Example:
             * ```cpp
             * // Create a 256x256 RGBA texture with custom data
             * std::vector<uint8_t> pixelData(256 * 256 * 4, 255); // 4 components (RGBA)
             * auto texture =  std::make_shared<NxOpenGlTexture2D>(pixelData.data(), 256, 256, NxTextureFormat::RGBA8);
             * ```
             */
            NxOpenGlTexture2D(const uint8_t *buffer, unsigned int width, unsigned int height, NxTextureFormat format);

            /**
             * @brief Creates an OpenGL 2D texture from a file in memory (raw content).
             *
             * Loads the texture data from the specified memory buffer. The buffer must contain
             * image data in a supported format (e.g., PNG, JPG). The texture will be ready
             * for rendering after creation.
             *
             * @param buffer The memory buffer containing the texture image data.
             * @param len The length of the memory buffer in bytes.
             *
             * Example:
             * ```cpp
             * std::vector<uint8_t> imageData = ...; // Load image data into a buffer
             * auto texture = std::make_shared<NxOpenGlTexture2D>(imageData.data(), imageData.size());
             * ```
             */
            NxOpenGlTexture2D(const uint8_t *buffer, unsigned int len);

            [[nodiscard]] unsigned int getWidth() const override {return m_width;};
            [[nodiscard]] unsigned int getHeight() const override {return m_height;};

            /**
            * @brief Retrieves the maximum texture size supported by the OpenGL context.
            *
            * Queries the OpenGL context to determine the largest supported dimension for
            * a texture. Useful for ensuring texture dimensions are within supported limits.
            *
            * @return The maximum texture size in pixels.
            */
            [[nodiscard]] unsigned int getMaxTextureSize() const override;

            [[nodiscard]] unsigned int getId() const override {return m_id;};

            /**
            * @brief Binds the texture to a specified texture slot.
            *
            * Activates the given texture slot and binds this texture to it. This makes the
            * texture available for use in subsequent rendering operations.
            *
            * @param slot The texture slot to bind to (default is 0).
            *
            * Example:
            * ```cpp
            * texture->bind(1); // Bind to texture slot 1
            * ```
            */
            void bind(unsigned int slot = 0) const override;

            /**
            * @brief Unbinds the texture from a specified texture slot.
            *
            * Deactivates the given texture slot, unbinding any texture associated with it.
            *
            * @param slot The texture slot to unbind from (default is 0).
            *
            * Example:
            * ```cpp
            * texture->unbind(1); // Unbind from texture slot 1
            * ```
            */
            void unbind(unsigned int slot = 0) const override;

            /**
            * @brief Updates the texture data.
            *
            * Uploads new pixel data to the GPU for the texture. The data must match the
            * texture's dimensions and format.
            *
            * @param data A pointer to the pixel data.
            * @param size The size of the data in bytes.
            * @throw NxTextureSizeMismatch If the size of the data does not match the texture's dimensions.
            *
            * Example:
            * ```cpp
            * unsigned char pixels[256 * 256 * 4] = { ... }; // RGBA data
            * texture->setData(pixels, sizeof(pixels));
            * ```
            */
            void setData(void *data, unsigned int size) override;
        private:
            /**
             * @brief Ingest and load texture data from stb_image buffer.
             *
             * @param data Pointer to the stb_image buffer.
             * @param width Width of the texture.
             * @param height Height of the texture.
             * @param channels Number of channels in the image data.
             * @param debugPath Path (of potential file) for error reporting. (Default: "(buffer)")
             *
             * @warning data MUST be a valid pointer to a buffer allocated by stb_image.
             *
             * Example:
             * ```cpp
             * int width, height, channels;
             * stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
             * ingestDataFromStb(data, width, height, channels, path);
             * ```
             */
            void ingestDataFromStb(const uint8_t *data, int width, int height, int channels, const std::string& debugPath = "(buffer)");

            /**
             * @brief Creates an OpenGL texture with the specified parameters.
             *
             * @param buffer Pointer to the texture data (can be nullptr, as per OpenGL spec).
             * @param width Width of the texture.
             * @param height Height of the texture.
             * @param internalFormat Internal format of the texture.
             * @param dataFormat Data format of the texture.
             *
             * Example:
             * ```cpp
             * createOpenGLTexture(buffer, width, height, GL_RGBA8, GL_RGBA);
             * ```
             */
            void createOpenGLTexture(const uint8_t* buffer, unsigned int width, unsigned int height, GLint internalFormat, GLenum dataFormat);

            std::string m_path;
            unsigned int m_width{};
            unsigned int m_height{};
            unsigned int m_id{};
            GLint m_internalFormat{};
            GLenum m_dataFormat{};
    };
}
