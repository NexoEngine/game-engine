//// SubTexture2D.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
    class SubTexture2D {
        public:
            SubTexture2D(const std::shared_ptr<Texture2D> &texture, const glm::vec2 &min, const glm::vec2 &max);

            [[nodiscard]] const std::shared_ptr<Texture2D> &getTexture() const { return m_texture; };
            [[nodiscard]] const glm::vec2 *getTextureCoords() const { return m_texCoords; };

            static std::shared_ptr<SubTexture2D> createFromCoords(const std::shared_ptr<Texture2D> &texture, const glm::vec2 &coords, const glm::vec2 &cellSize, const glm::vec2 &spriteSize = {1, 1});
        private:
            std::shared_ptr<Texture2D> m_texture;
            glm::vec2 m_texCoords[4]{};
    };
}