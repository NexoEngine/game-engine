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
    * @class OpenGlTexture2D
    * @brief OpenGL-specific implementation of the `Texture2D` class.
    *
    * The `OpenGlTexture2D` class manages 2D textures in an OpenGL rendering context.
    * It supports texture creation, data uploading, and binding/unbinding operations.
    *
    * Responsibilities:
    * - Create textures using OpenGL APIs.
    * - Load texture data from files or raw memory.
    * - Provide texture binding and unbinding functionality.
    */
    class OpenGlTexture2D final : public Texture2D {
        public:
            ~OpenGlTexture2D() override;

            /**
            * @brief Loads an OpenGL 2D texture from an image file.
            *
            * Reads texture data from the specified file path and uploads it to the GPU.
            * Supports common image formats (e.g., PNG, JPG). Automatically detects the
            * internal and data formats based on the number of channels in the image.
            *
            * @param path The file path to the texture image.
            * @throw FileNotFoundException If the file cannot be found.
            * @throw TextureUnsupportedFormat If the image format is unsupported.
            *
            * Example:
            * ```cpp
            * auto texture = std::make_shared<OpenGlTexture2D>("textures/wood.jpg");
            * ```
            */
            explicit OpenGlTexture2D(const std::string &path);

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
            * auto texture = std::make_shared<OpenGlTexture2D>(256, 256);
            * ```
            */
            OpenGlTexture2D(unsigned int width, unsigned int height);

            /**
             * @brief Creates a OpenGL 2D texture from file in memory.
             *
             * Loads the texture data from the specified memory buffer. The buffer must contain
             * image data in a supported format (e.g., PNG, JPG). The texture will be ready
             * for rendering after creation.
             *
             * @param buffer The memory buffer containing the texture image data.
             * @param len The length of the memory buffer in bytes.
             * @return A shared pointer to the created `Texture2D` instance.
             *
             * Example:
             * ```cpp
             * std::vector<uint8_t> imageData = ...; // Load image data into a buffer
             * auto texture = std::make_shared<OpenGlTexture2D>(imageData.data(), imageData.size());
             * ```
             */
            OpenGlTexture2D(const uint8_t *buffer, unsigned int len);

            /**
 * @brief Retrieves the width of the texture.
 *
 * @return The width of the texture in pixels.
 */
[[nodiscard]] unsigned int getWidth() const override {return m_width;};
            /**
 * @brief Retrieves the height of the texture.
 *
 * @return The texture's height in pixels.
 */
[[nodiscard]] unsigned int getHeight() const override {return m_height;};

            /**
            * @brief Retrieves the maximum texture size supported by the OpenGL context.
            *
            * Queries the OpenGL context to determine the largest supported dimension for
            * a texture. Useful for ensuring texture dimensions are within supported limits.
            *
            * @return The maximum texture size in pixels.
            */
            unsigned int getMaxTextureSize() const override;

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
            * @throw TextureSizeMismatch If the size of the data does not match the texture's dimensions.
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
            void ingestDataFromStb(uint8_t *data, int width, int height, int channels, const std::string& debugPath = "(buffer)");

            std::string m_path;
            unsigned int m_width;
            unsigned int m_height;
            unsigned int m_id{};
            GLenum m_internalFormat;
            GLenum m_dataFormat;
    };
}
