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

    struct NxQuadVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;

        float texIndex;

        int entityID;
    };

    struct NxRendererStats {
        unsigned int drawCalls = 0;
        unsigned int quadCount = 0;

        [[nodiscard]] unsigned int getTotalVertexCount() const { return quadCount * 4; }
        [[nodiscard]] unsigned int getTotalIndexCount() const { return quadCount * 6; }
    };

    struct NxRenderer2DStorage {
        const unsigned int maxQuads = 10000;
        const unsigned int maxVertices = maxQuads * 4;
        const unsigned int maxIndices = maxQuads * 6;
        static constexpr unsigned int maxTextureSlots = 32;

        std::shared_ptr<NxShader> textureShader;
        std::shared_ptr<NxVertexArray> vertexArray;
        std::shared_ptr<NxVertexBuffer> vertexBuffer;
        std::shared_ptr<NxIndexBuffer> indexBuffer;
        std::shared_ptr<NxTexture2D> whiteTexture;

        unsigned int indexCount = 0;
        std::array<NxQuadVertex, 40000> vertexBufferBase;
        std::array<unsigned int, 60000> indexBufferBase;
        NxQuadVertex* vertexBufferPtr = nullptr;
        unsigned int *indexBufferPtr = nullptr;

        std::array<std::shared_ptr<NxTexture2D>, maxTextureSlots> textureSlots;
        unsigned int textureSlotIndex = 1;

        glm::vec4 quadVertexPositions[4];

        NxRendererStats stats;
    };

    /**
     * @class NxRenderer2D
     * @brief Provides a 2D rendering system for drawing quads, textures, and sprites.
     *
     * The `NxRenderer2D` class is a high-performance 2D rendering engine that supports
     * batching, texture binding, and transformation.
     *
     * Features:
     * - Efficient batching of quads to minimize draw calls.
     * - Support for textured and colored quads, including sprites (subtextures).
     * - Dynamic resizing of vertex and index buffers as needed.
     * - Integration with shaders for advanced rendering effects.
     *
     * Responsibilities:
     * - Handles the lifecycle of rendering scenes (begin, end, flush).
     * - Manages internal rendering storage for textures, vertices, and indices.
     * - Provides high-level API for drawing quads with various configurations.
     *
     * Usage:
     * 1. Call `init()` to initialize the renderer.
     * 2. Begin a scene using `beginScene()` with a view-projection matrix.
     * 3. Use `drawQuad()` methods to draw quads with or without textures.
     * 4. Call `endScene()` to finalize the rendering and issue draw calls.
     * 5. Call `shutdown()` to release resources when the renderer is no longer needed.
     */
    class NxRenderer2D {
        public:
            /**
            * @brief Destroys the NxRenderer2D instance and releases resources.
            *
            * Ensures proper cleanup of the internal storage and associated buffers.
            */
            ~NxRenderer2D() = default;

            /**
            * @brief Initializes the NxRenderer2D and allocates required resources.
            *
            * This method sets up the internal storage, including vertex arrays, buffers,
            * textures, and shaders. It also predefines the vertex positions for quads.
            *
            * Responsibilities:
            * - Creates and configures vertex and index buffers.
            * - Allocates memory for vertex and index data.
            * - Sets up a default white texture for rendering quads without textures.
            * - Configures the texture shader and binds texture samplers.
            *
            * Throws:
            * - Exceptions if shader creation or buffer allocation fails.
            *
            * Notes:
            * - Must be called before any rendering operations.
            */
            void init();

            /**
             * @brief Shuts down the NxRenderer2D and releases allocated resources.
             *
             * This method deletes internal storage, including vertex and index buffers,
             * and resets the internal storage pointer.
             *
             * Throws:
             * - NxRendererNotInitialized if the renderer is not initialized.
             */
            void shutdown();

            /**
             * @brief Begins a new rendering scene.
             *
             * Sets up the view-projection matrix for rendering and resets internal storage
             * pointers for batching vertices and indices.
             *
             * @param viewProjection The combined view and projection matrix for the scene.
             *
             * Throws:
             * - NxRendererNotInitialized if the renderer is not initialized.
             * - NxRendererSceneLifeCycleFailure if called without proper initialization.
             */
            void beginScene(const glm::mat4 &viewProjection);

            /**
             * @brief Ends the current rendering scene.
             *
             * Uploads vertex and index data to the GPU, flushes the rendering pipeline,
             * and resets internal buffers for the next frame.
             *
             * Throws:
             * - NxRendererNotInitialized if the renderer is not initialized.
             * - NxRendererSceneLifeCycleFailure if no scene was started with `beginScene()`.
             */
            void endScene() const;
            void flush() const;

            /**
             * @brief Draws a colored quad at the specified position and size.
             *
             * @param pos The position of the quad (2D or 3D).
             * @param size The size of the quad.
             * @param color The color of the quad (RGBA format).
             *
             * Overloaded for:
             * - 2D position (`glm::vec2`) and 3D position (`glm::vec3`).
             */
            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color, int entityID = -1) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec4 &color, int entityID = -1) const;

            /**
             * @brief Draws a textured quad at the specified position and size.
             *
             * @param pos The position of the quad (2D or 3D).
             * @param size The size of the quad.
             * @param texture The texture to apply to the quad.
             *
             * Overloaded for:
             * - 2D position (`glm::vec2`) and 3D position (`glm::vec3`).
             */
            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<NxTexture2D> &texture, int entityID = -1) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<NxTexture2D> &texture, int entityID = -1) const;

            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<NxSubTexture2D> &subTexture, int entityID = -1) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<NxSubTexture2D> &subTexture, int entityID = -1) const;


            /**
             * @brief Draws a rotated quad with a specified color or texture.
             *
             * @param pos The position of the quad (2D or 3D).
             * @param size The size of the quad.
             * @param rotation The rotation angle in degrees (clockwise).
             * @param color The color of the quad (RGBA format).
             * @param texture Optional texture to apply to the quad.
             *
             * Overloaded for:
             * - 2D position (`glm::vec2`) and 3D position (`glm::vec3`).
             */
            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const glm::vec4 &color, int entityID = -1) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const glm::vec4 &color, int entityID = -1) const;

            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<NxTexture2D> &texture, int entityID = -1) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<NxTexture2D> &texture, int entityID = -1) const;

            void drawQuad(const glm::vec2 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<NxSubTexture2D> &subTexture, int entityID = -1) const;
            void drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const std::shared_ptr<NxSubTexture2D> &subTexture, int entityID = -1) const;

            /**
            * @brief Resets rendering statistics.
            *
            * Clears the draw call and quad counters in `NxRendererStats`.
            *
            * Throws:
            * - NxRendererNotInitialized if the renderer is not initialized.
            */
            void resetStats() const;

            /**
             * @brief Retrieves the current rendering statistics.
             *
             * @return A `NxRendererStats` struct containing the number of draw calls and
             *         quads rendered.
             *
             * Throws:
             * - NxRendererNotInitialized if the renderer is not initialized.
             */
            [[nodiscard]] NxRendererStats getStats() const;

            std::shared_ptr<NxRenderer2DStorage> getInternalStorage() const { return m_storage; };
        private:
            std::shared_ptr<NxRenderer2DStorage> m_storage;
            bool m_renderingScene = false;

            void flushAndReset() const;

            // Helper functions
            void generateQuadVertices(const glm::mat4 &transform, glm::vec4 color, float textureIndex, const glm::vec2 *textureCoords, int entityID) const;
            [[nodiscard]] float getTextureIndex(const std::shared_ptr<NxTexture2D> &texture) const;
    };

}




