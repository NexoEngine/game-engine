//// SubTexture2D.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for subtexture class
//
///////////////////////////////////////////////////////////////////////////////
#include "SubTexture2D.hpp"

namespace nexo::renderer {
    NxSubTexture2D::NxSubTexture2D(const std::shared_ptr<NxTexture2D> &texture, const glm::vec2 &min, const glm::vec2 &max)
        : m_texture(texture)
    {
        m_texCoords[0] = {min.x, min.y};
        m_texCoords[1] = {max.x, min.y};
        m_texCoords[2] = {max.x, max.y};
        m_texCoords[3] = {min.x, max.y};
    }

    std::shared_ptr<NxSubTexture2D> NxSubTexture2D::createFromCoords(const std::shared_ptr<NxTexture2D> &texture, const glm::vec2 &coords, const glm::vec2 &cellSize, const glm::vec2 &spriteSize)
    {
        glm::vec2 min = {(coords.x * cellSize.x) / static_cast<float>(texture->getWidth()) , (coords.y * cellSize.y) / static_cast<float>(texture->getHeight())};
        glm::vec2 max = {((coords.x + spriteSize.x) * cellSize.x) / static_cast<float>(texture->getWidth()), ((coords.y + spriteSize.y) * cellSize.y) / static_cast<float>(texture->getHeight())};

        return std::make_shared<NxSubTexture2D>(texture, min, max);
    }
}