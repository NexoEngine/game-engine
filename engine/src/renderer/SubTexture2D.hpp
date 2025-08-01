//// SubTexture2D.hpp /////////////////////////////////////////////////////////
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
//  Description: Header file for subtexture class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/vec2.hpp>

#include "Texture.hpp"

namespace nexo::renderer {
    /**
    * @class NxSubTexture2D
    * @brief Represents a portion of a 2D texture, useful for sprite rendering.
    *
    * The `NxSubTexture2D` class allows defining a sub-region within a larger texture.
    * This is commonly used in sprite sheets where a single texture contains multiple
    * sprites. The class provides texture coordinates to render only the specified region.
    *
    * Responsibilities:
    * - Define a rectangular sub-region of a texture using minimum and maximum coordinates.
    * - Provide texture coordinates for rendering the sub-region.
    * - Create subtextures dynamically from grid coordinates, such as in sprite sheets.
    *
    * Example Usage:
    * ```cpp
    * auto texture = NxTexture2D::create("path/to/texture.png");
    * auto subTexture = NxSubTexture2D::createFromCoords(texture, {1, 1}, {64, 64});
    * ```
    */
    class NxSubTexture2D {
        public:
            /**
            * @brief Constructs a `NxSubTexture2D` from specified texture coordinates.
            *
            * Initializes the subtexture by defining its bounds using normalized minimum
            * and maximum coordinates. The coordinates should be normalized to the texture's size
            * (values between 0 and 1).
            *
            * @param texture A shared pointer to the base `NxTexture2D`.
            * @param min The normalized minimum coordinates (bottom-left corner) of the subtexture.
            * @param max The normalized maximum coordinates (top-right corner) of the subtexture.
            */
            NxSubTexture2D(const std::shared_ptr<NxTexture2D> &texture, const glm::vec2 &min, const glm::vec2 &max);

            [[nodiscard]] const std::shared_ptr<NxTexture2D> &getTexture() const { return m_texture; };
            /**
            * @brief Retrieves the texture coordinates for the subtexture.
            *
            * Returns a pointer to an array of four `glm::vec2` objects representing the
            * texture coordinates of the subtexture's corners. The coordinates are ordered as:
            * - Bottom-left
            * - Bottom-right
            * - Top-right
            * - Top-left
            *
            * @return A constant pointer to the array of texture coordinates.
            */
            [[nodiscard]] const glm::vec2 *getTextureCoords() const { return m_texCoords; };

            /**
            * @brief Creates a `NxSubTexture2D` from grid-based coordinates within a texture.
            *
            * Dynamically calculates the normalized minimum and maximum texture coordinates
            * for a subtexture based on its position and size in a sprite sheet.
            *
            * @param texture A shared pointer to the base `NxTexture2D`.
            * @param coords The grid-based coordinates (e.g., sprite index in a sprite sheet).
            * @param cellSize The size of each cell (sprite) in the sprite sheet, in pixels.
            * @param spriteSize The size of the sprite in grid units, defaulting to {1, 1}.
            * @return A shared pointer to the created `NxSubTexture2D` instance.
            *
            * Example:
            * ```cpp
            * auto texture = NxTexture2D::create("path/to/spritesheet.png");
            * auto subTexture = NxSubTexture2D::createFromCoords(texture, {1, 1}, {64, 64});
            * ```
            *
            * Example Explanation:
            * - The sprite is located at grid position (1, 1) in the sprite sheet.
            * - Each cell in the grid is 64x64 pixels.
            * - The sprite occupies one grid cell by default.
            */
            static std::shared_ptr<NxSubTexture2D> createFromCoords(const std::shared_ptr<NxTexture2D> &texture, const glm::vec2 &coords, const glm::vec2 &cellSize, const glm::vec2 &spriteSize = {1, 1});
        private:
            std::shared_ptr<NxTexture2D> m_texture;
            glm::vec2 m_texCoords[4]{};
    };
}
