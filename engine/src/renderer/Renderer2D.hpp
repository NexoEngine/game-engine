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

#include "core/camera/OrthographicCamera.hpp"
#include "Texture.hpp"
#include "SubTexture2D.hpp"

namespace nexo::renderer {

    class Renderer2D {
        public:
            static void init();
            static void shutdown();

            static void beginScene(const camera::OrthographicCamera &camera);
            static void endScene();
            static void flush();

            // Without rotation
            static void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color);
            static void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec4 &color);
            // With texture
            static void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture);
            static void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture);
            // With subtexture (sprites)
            static void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subTexture);
            static void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subTexture);


            // With rotation
            static void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const glm::vec4 &color);
            static void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const glm::vec4 &color);
            // With texture
            static void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<Texture2D> &texture);
            static void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<Texture2D> &texture);
            // With subtexture (sprites)
            static void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<SubTexture2D> &subTexture);
            static void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<SubTexture2D> &subTexture);

            struct RendererStats {
                unsigned int drawCalls = 0;
                unsigned int quadCount = 0;

                [[nodiscard]] unsigned int getTotalVertexCount() const { return quadCount * 4; }
                [[nodiscard]] unsigned int getTotalIndexCount() const { return quadCount * 6; }
            };
            static void resetStats();
            static RendererStats getStats();

        private:
            static void flushAndReset();

            // Helper functions
            static void generateQuadVertices(const glm::mat4 &transform, glm::vec4 color, float textureIndex, const glm::vec2 *textureCoords);
            static float getTextureIndex(const std::shared_ptr<Texture2D> &texture);
    };
}




