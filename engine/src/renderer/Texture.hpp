//// Texture.hpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for texture abstract classes
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <memory>

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
            virtual unsigned int getMaxTextureSize() const = 0;

            [[nodiscard]] virtual unsigned int getId() const = 0;

            virtual void bind(unsigned int slot = 0) const = 0;
            virtual void unbind(unsigned int slot = 0) const = 0;

            virtual void setData(void *data, unsigned int size) = 0;

            bool operator==(const NxTexture &other) const { return this->getId() == other.getId(); };
    };

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
            static std::shared_ptr<NxTexture2D> create(uint8_t *buffer, unsigned int len);

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
