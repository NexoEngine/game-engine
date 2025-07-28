//// Texture.hpp //////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for texture abstract classes
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <memory>
#include <string>
#include <string_view>

namespace nexo::renderer {

    /**
    * @class NxTexture
    * @brief Abstract base class for representing textures in a rendering system.
    *
    * The `Texture` class provides a common interface for managing texture resources
    * in a rendering API. It defines the basic operations for texture creation, data management,
    * and binding/unbinding to the graphics pipeline.
    *
    * Responsibilities:
    * - Retrieve texture properties (e.g., width, height, ID).
    * - Manage texture data.
    * - Bind and unbind textures to specific texture slots.
    *
    * Derived classes (e.g., `NxOpenGlTexture2D`) implement platform-specific behavior for
    * managing textures in different rendering backends.
    */
    class NxTexture {
        public:
            virtual ~NxTexture() = default;

            [[nodiscard]] virtual unsigned int getWidth() const = 0;
            [[nodiscard]] virtual unsigned int getHeight() const = 0;
            [[nodiscard]] virtual unsigned int getMaxTextureSize() const = 0;

            [[nodiscard]] virtual unsigned int getId() const = 0;

            virtual void bind(unsigned int slot = 0) const = 0;
            virtual void unbind(unsigned int slot = 0) const = 0;

            virtual void setData(void *data, unsigned int size) = 0;

            bool operator==(const NxTexture &other) const { return this->getId() == other.getId(); };
    };

    /**
     * @enum NxTextureFormat
     * @brief Enumeration of texture formats.
     *
     * This enum defines various texture formats that can be used in the rendering system.
     * Each format corresponds to a specific pixel layout and color depth.
     *
     * For example:
     * - `R8` represents a single-channel texture with 8 bits per channel.
     * - `RG8` represents a two-channel texture with 8 bits per channel.
     * - `RGB8` represents a three-channel texture with 8 bits per channel.
     * - `RGBA8` represents a four-channel texture with 8 bits per channel.
     *
     * @note If a texture format is invalid, it is represented by `INVALID`, which value is 0.
     */
    enum class NxTextureFormat {
        INVALID = 0, // Invalid texture format, used for error reporting

        R8 = 1,      // 1 channel RED, 8 bits per channel
        RG8,         // 2 channels RED GREEN, 8 bits per channel
        RGB8,        // 3 channels RED GREEN BLUE, 8 bits per channel
        RGBA8,       // 4 channels RED GREEN BLUE ALPHA, 8 bits per channel

        _NB_FORMATS_ // Number of texture formats, used for array sizing
    };

    /**
     * @brief Converts a NxTextureFormat enum value to its string representation.
     *
     * This function takes a NxTextureFormat enum value and returns its corresponding
     * string representation. The string is a human-readable format name (e.g., "R8", "RGBA8").
     *
     * @param format The NxTextureFormat enum value to convert.
     * @return A string_view representing the format name.
     */
    [[nodiscard]] constexpr std::string_view NxTextureFormatToString(const NxTextureFormat format)
    {
        switch (format) {
            case NxTextureFormat::R8:    return "R8";
            case NxTextureFormat::RG8:   return "RG8";
            case NxTextureFormat::RGB8:  return "RGB8";
            case NxTextureFormat::RGBA8: return "RGBA8";
            default: return "INVALID";
        }
    }

    /**
     * @brief Converts a string representation of a texture format to its NxTextureFormat enum value.
     *
     * This function takes a string representation of a texture format (e.g., "R8", "RGBA8") and
     * returns the corresponding NxTextureFormat enum value. If the string does not match any
     * known format, it returns NxTextureFormat::INVALID.
     *
     * @param format The string representation of the texture format.
     * @return The corresponding NxTextureFormat enum value.
     */
    NxTextureFormat NxTextureFormatFromString(const std::string_view &format);

    /**
     * @brief Converts ARGB8 format to RGBA8 format in place.
     *
     * This function takes a pointer to a byte array and its size, and converts the
     * pixel data from ARGB8 format to RGBA8 format. The conversion is done in place,
     * meaning the original data will be modified.
     *
     * @param bytes Pointer to the byte array containing ARGB8 pixel data.
     * @param size Size of the byte array in bytes.
     */
    void NxTextureFormatConvertArgb8ToRgba8(uint8_t *bytes, size_t size);

    class NxTexture2D :  public NxTexture {
        public:
            /**
             * @brief Creates a blank 2D texture with the specified dimensions.
             *
             * Allocates a texture resource with the given width and height. The texture
             * will have no initial data and can be updated later with `setData`.
             *
             * @param width The width of the texture in pixels.
             * @param height The height of the texture in pixels.
             * @return A shared pointer to the created `NxTexture2D` instance.
             *
             * Example:
             * ```cpp
             * auto blankTexture = NxTexture2D::create(512, 512);
             * ```
             */
            static std::shared_ptr<NxTexture2D> create(unsigned int width, unsigned int height);


            /**
             * @brief Creates a 2D texture from raw pixel data in memory.
             *
             * Creates a texture from a raw pixel buffer with the specified dimensions and format.
             * This is useful when you have direct access to pixel data that wasn't loaded through
             * compressed images files or when you want to create textures from procedurally generated data.
             *
             * @param buffer Pointer to the raw pixel data. The buffer should contain pixel data
             *        in a format that matches the specified NxTextureFormat. The data consists
             *        of height scanlines of width pixels, with each pixel consisting of N components
             *        (where N depends on the format). The first pixel pointed to is bottom-left-most
             *        in the image. There is no padding between image scanlines or between pixels.
             *        Each component is an 8-bit unsigned value (uint8_t).
             * @param width The width of the texture in pixels.
             * @param height The height of the texture in pixels.
             * @param format The format of the pixel data, which determines the number of components
             *        per pixel.
             * @return A shared pointer to the created NxTexture2D instance.
             *
             * Example:
             * ```cpp
             * // Create a 128x128 RGBA texture with custom data
             * std::vector<uint8_t> pixelData(128 * 128 * 4); // 4 components (RGBA)
             * // Fill pixelData with your custom values...
             * auto texture = NxTexture2D::create(pixelData.data(), 128, 128, NxTextureFormat::RGBA8);
             * ```
             */
            static std::shared_ptr<NxTexture2D> create(const uint8_t *buffer, unsigned int width, unsigned int height, NxTextureFormat format);

            /**
             * @brief Creates a 2D texture from file in memory.
             *
             * Loads the texture data from the specified memory buffer. The buffer must contain
             * image data in a supported format (e.g., PNG, JPG). The texture will be ready
             * for rendering after creation.
             *
             * @param buffer The memory buffer containing the texture image data.
             * @param len The length of the memory buffer in bytes.
             * @return A shared pointer to the created `NxTexture2D` instance.
             *
             * Example:
             * ```cpp
             * std::vector<uint8_t> imageData = ...; // Load image data into a buffer
             * auto texture = NxTexture2D::create(imageData.data(), imageData.size());
             * ```
             */
            static std::shared_ptr<NxTexture2D> create(const uint8_t* buffer, unsigned int len);

            /**
            * @brief Creates a 2D texture from an image file.
            *
            * Loads the texture data from the specified file path. The file must contain
            * image data in a supported format (e.g., PNG, JPG). The texture will be ready
            * for rendering after creation.
            *
            * @param path The file path to the texture image.
            * @return A shared pointer to the created `NxTexture2D` instance.
            *
            * Example:
            * ```cpp
            * auto texture = NxTexture2D::create("assets/textures/brick_wall.png");
            * ```
            */
            static std::shared_ptr<NxTexture2D> create(const std::string &path);
    };

}
