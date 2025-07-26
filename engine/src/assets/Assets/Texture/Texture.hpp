//// Texture.hpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/02/2025
//  Description: Header file for the Texture class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "assets/Asset.hpp"

namespace nexo::assets {

    struct TextureData {
        std::shared_ptr<renderer::NxTexture2D> texture;
    };

    /**
     * @class Texture
     *
     * @brief Represents a texture asset.
     */
    class Texture final : public Asset<TextureData, AssetType::TEXTURE> {
        public:

            /**
             * @brief Default constructor.
             *
             * Creates an empty Texture asset without an underlying texture.
             * Use one of the provided static factory methods to create a fully initialized texture.
             */
            Texture() = default;

            /**
             * @brief Constructs a Texture object from a file path.
             *
             * Creates a texture asset by loading image data from the specified file.
             * Supported formats include PNG, JPEG, BMP, GIF, TGA, and more
             * (any format supported by stb_image).
             *
             * @param path The path to the texture file.
             *
             * @throws NxFileNotFoundException If the file cannot be found or read.
             * @throws NxTextureUnsupportedFormat If the image format is not supported.
             * @throws NxTextureInvalidSize If the image dimensions exceed the maximum texture size.
             */
            explicit Texture(const std::filesystem::path &path)
                : Asset()
            {
                const auto texture = renderer::NxTexture2D::create(path.string());
                auto textureData = std::make_unique<TextureData>();
                textureData->texture = texture;
                setData(std::move(textureData));
            }

            /**
             * @brief Constructs a blank Texture with the specified dimensions.
             *
             * Creates a new empty texture with the given width and height.
             * The texture is initialized with transparent black pixels (all zeros).
             * This is useful for creating render targets or textures that will be
             * filled with data later.
             *
             * @param width The width of the texture in pixels.
             * @param height The height of the texture in pixels.
             *
             * @throws NxTextureInvalidSize If the dimensions exceed the maximum texture size.
             */
            Texture(const unsigned int width, const unsigned int height)
                : Asset()
            {
                const auto texture = renderer::NxTexture2D::create(width, height);
                auto textureData = std::make_unique<TextureData>();
                textureData->texture = texture;
                setData(std::move(textureData));
            }

            /**
             * @brief Constructs a Texture from raw pixel data in memory.
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
             * @param width The width of the texture in pixels.
             * @param height The height of the texture in pixels.
             * @param format The format of the pixel data (R8, RG8, RGB8, or RGBA8).
             *
             * @throws NxInvalidValue If the buffer is null.
             * @throws NxTextureUnsupportedFormat If the specified format is not supported.
             * @throws NxTextureInvalidSize If the dimensions exceed the maximum texture size.
             */
            Texture(const uint8_t *buffer, const unsigned int width, const unsigned int height, const renderer::NxTextureFormat format)
                : Asset()
            {
                const auto texture = renderer::NxTexture2D::create(buffer, width, height, format);
                auto textureData = std::make_unique<TextureData>();
                textureData->texture = texture;
                setData(std::move(textureData));
            }

            /**
             * @brief Constructs a Texture from image data in memory.
             *
             * Creates a texture by loading image data from a memory buffer.
             * The buffer should contain a complete image file (e.g., PNG, JPEG)
             * that will be decoded using stb_image.
             *
             * @param buffer Pointer to the image file data in memory.
             * @param size Size of the buffer in bytes.
             *
             * @throws NxTextureUnsupportedFormat If the image format is not supported.
             * @throws NxTextureInvalidSize If the image dimensions exceed the maximum texture size.
             */
            Texture(const uint8_t* buffer, const unsigned int size)
                : Asset()
            {
                const auto texture = renderer::NxTexture2D::create(buffer, size);
                auto textureData = std::unique_ptr<TextureData>();
                textureData->texture = texture;
                setData(std::move(textureData));
            }

            ~Texture() override = default;
    };

}
