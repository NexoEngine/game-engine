//// Renderer2D.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header for renderer 2D class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Texture.hpp"
#include "SubTexture2D.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

#include <array>

namespace nexo::renderer {

    struct QuadVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;

        float texIndex;
    };

    struct RendererStats {
        unsigned int drawCalls = 0;
        unsigned int quadCount = 0;

        [[nodiscard]] unsigned int getTotalVertexCount() const { return quadCount * 4; }
        [[nodiscard]] unsigned int getTotalIndexCount() const { return quadCount * 6; }
    };

    struct Renderer2DStorage {
        const unsigned int maxQuads = 10000;
        const unsigned int maxVertices = maxQuads * 4;
        const unsigned int maxIndices = maxQuads * 6;
        static constexpr unsigned int maxTextureSlots = 32;

        std::shared_ptr<Shader> textureShader;
        std::shared_ptr<VertexArray> quadVertexArray;
        std::shared_ptr<VertexBuffer> quadVertexBuffer;
        std::shared_ptr<Texture2D> whiteTexture;

        unsigned int quadIndexCount = 0;
        QuadVertex *quadVertexBufferBase = nullptr;
        QuadVertex *quadVertexBufferPtr = nullptr;

        std::array<std::shared_ptr<Texture2D>, maxTextureSlots> textureSlots;
        unsigned int textureSlotIndex = 1;

        glm::vec4 quadVertexPositions[4];

        RendererStats stats;
    };

    class Renderer2D {
        public:
            void init();
            void shutdown();

            void beginScene(const glm::mat4 &viewProjection) const;
            void endScene() const;
            void flush() const;

            // Without rotation
            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec4 &color) const;
            // With texture
            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture) const;
            // With subtexture (sprites)
            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subTexture) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subTexture) const;


            // With rotation
            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const glm::vec4 &color) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const glm::vec4 &color) const;
            // With texture
            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<Texture2D> &texture) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<Texture2D> &texture) const;
            // With subtexture (sprites)
            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<SubTexture2D> &subTexture) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<SubTexture2D> &subTexture) const;


            void resetStats() const;
            [[nodiscard]] RendererStats getStats() const;

        private:
            Renderer2DStorage *m_storage = nullptr;

            void flushAndReset() const;

            // Helper functions
            void generateQuadVertices(const glm::mat4 &transform, glm::vec4 color, float textureIndex, const glm::vec2 *textureCoords) const;
            [[nodiscard]] float getTextureIndex(const std::shared_ptr<Texture2D> &texture) const;
    };

    class RendererContext {
        public:
            RendererContext() = default;
            Renderer2D renderer2D;
    };
}




