//// Renderer3D.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Header file for the renderer3D class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Shader.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"

#include <array>
#include <glm/glm.hpp>

namespace nexo::renderer {

    struct CubeVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;
        float texIndex;
    };

    struct Renderer3DStats {
        unsigned int drawCalls = 0;
        unsigned int cubeCount = 0;

        [[nodiscard]] unsigned int getTotalVertexCount() const { return cubeCount * 8; }
        [[nodiscard]] unsigned int getTotalIndexCount() const { return cubeCount * 36; }
    };

    struct Renderer3DStorage {
        const unsigned int maxCubes = 10000;
        const unsigned int maxVertices = maxCubes * 8;
        const unsigned int maxIndices = maxCubes * 36;
        static constexpr unsigned int maxTextureSlots = 32;

        std::shared_ptr<Shader> textureShader;
        std::shared_ptr<VertexArray> cubeVertexArray;
        std::shared_ptr<VertexBuffer> cubeVertexBuffer;
        std::shared_ptr<Texture2D> whiteTexture;

        unsigned int cubeIndexCount = 0;
        CubeVertex* cubeVertexBufferBase = nullptr;
        CubeVertex* cubeVertexBufferPtr = nullptr;

        std::array<std::shared_ptr<Texture2D>, maxTextureSlots> textureSlots;
        unsigned int textureSlotIndex = 1;

        glm::vec3 cubeVertexPositions[8];

        Renderer3DStats stats;
    };

    class Renderer3D {
    public:
        void init();
        void shutdown();

        void beginScene(const glm::mat4& viewProjection) const;
        void endScene() const;

        // With color
        void drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color) const;
        //void drawCube(const glm::vec3& position, const glm::vec3& size, float rotation, const glm::vec4& color) const;

        // With texture
        void drawCube(const glm::vec3& position, const glm::vec3& size, const std::shared_ptr<Texture2D>& texture) const;
        //void drawCube(const glm::vec3& position, const glm::vec3& size, float rotation, const std::shared_ptr<Texture2D>& texture) const;

        void resetStats() const;
        [[nodiscard]] Renderer3DStats getStats() const;

    private:
        Renderer3DStorage* m_storage = nullptr;

        void flush() const;
        void flushAndReset() const;
        void generateCubeVertices(const glm::mat4& transform, const glm::vec4& color, float textureIndex, const glm::vec2* textureCoords) const;
        [[nodiscard]] float getTextureIndex(const std::shared_ptr<Texture2D>& texture) const;
    };

}
