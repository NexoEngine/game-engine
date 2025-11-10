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
#include "Texture.hpp"
#include "VertexArray.hpp"

#include <array>
#include <glm/glm.hpp>

namespace nexo::renderer {
    struct NxVertex {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;

        int entityID;
    };

    /**
     * @struct NxRenderer3DStorage
     * @brief Holds internal data and resources used by NxRenderer3D.
     *
     * Members:
     * - `maxCubes`, `maxVertices`, `maxIndices`: Limits for cubes, vertices, and indices.
     * - `vertexArray`, `vertexBuffer`, `indexBuffer`: Buffers for storing cube data.
     * - `whiteTexture`: Default texture used for untextured objects.
     * - `textureShader`: Shader used for rendering.
     * - `textureSlots`: Array of texture slots for batching textures.
     * - `vertexBufferBase`, `indexBufferBase`: Base pointers for vertex and index data.
     * - `vertexBufferPtr`, `indexBufferPtr`: Current pointers for batching vertices and indices.
     * - `stats`: Rendering statistics.
     */
    struct NxRenderer3DStorage {
        static constexpr unsigned int maxTextureSlots = 32;
        std::shared_ptr<NxTexture2D> whiteTexture;

        std::vector<std::array<std::shared_ptr<NxTexture2D>, maxTextureSlots>> textureSlotsBatch;
        std::vector<unsigned int> nbTexturesInBatch;
        unsigned int currentTextureBatchIndex = 0;
        unsigned int currentTextureBindedIndex = 0;
    };

    /**
     * @class NxRenderer3D
     * @brief Provides a high-performance 3D rendering system for drawing cubes, textured objects, and meshes.
     *
     * The `NxRenderer3D` class facilitates efficient rendering of 3D objects using batching,
     * texture binding, and transformation matrices. It supports dynamic vertex and index
     * buffers, enabling high performance for drawing multiple 3D primitives.
     *
     * Features:
     * - Efficient batching for cubes and custom meshes.
     * - Support for textured and colored cubes with or without rotation.
     * - Integration with shaders for rendering effects.
     * - Dynamic handling of texture slots for multiple textures.
     *
     * Responsibilities:
     * - Manages the lifecycle of rendering scenes.
     * - Provides a high-level API for drawing cubes and meshes with colors or textures.
     * - Manages internal rendering storage for vertices, indices, and textures.
     *
     * Usage:
     * 1. Call `init()` to initialize the renderer.
     * 2. Begin a scene using `beginScene()` with a view-projection matrix and camera position.
     * 3. Use `drawCube()` or `drawMesh()` methods to draw 3D objects.
     * 4. Call `endScene()` to finalize the rendering and issue draw calls.
     * 5. Call `shutdown()` to release resources when the renderer is no longer needed.
     */
    class NxRenderer3D {
       public:
        static NxRenderer3D& get()
        {
            static NxRenderer3D instance;
            return instance;
        }
        /**
         * @brief Initializes the NxRenderer3D and allocates required resources.
         *
         * Sets up internal storage, vertex buffers, index buffers, and texture samplers.
         * Prepares the default white texture and initializes the texture shader.
         *
         * Responsibilities:
         * - Creates and configures vertex and index buffers.
         * - Allocates memory for vertex and index data.
         * - Sets up default white texture for rendering objects without textures.
         * - Configures the texture shader and binds texture samplers.
         *
         * Throws:
         * - Exceptions if buffer allocation or shader creation fails.
         *
         * Notes:
         * - Must be called before any rendering operations.
         */
        void init();

        /**
         * @brief Releases all resources and cleans up the NxRenderer3D.
         *
         * Deletes allocated vertex and index buffers and resets internal storage pointers.
         *
         * Throws:
         * - NxRendererNotInitialized if the renderer is not initialized.
         */
        void shutdown();

        /**
         * @brief Binds all currently used textures to their respective texture slots.
         *
         * Iterates through the texture slots and binds each texture to the corresponding
         * OpenGL texture unit. This prepares the textures for use in the shader during rendering.
         *
         * Throws:
         * - NxRendererNotInitialized if the renderer is not initialized.
         */
        void bindTextures() const;
        void bindTextureBatch(unsigned int batchIndex) const;
        void bindNextTextureBatch() const;

        /**
         * @brief Unbinds all currently bound textures and resets the texture slot index.
         *
         * Iterates through the texture slots and unbinds each texture from its OpenGL
         * texture unit. Resets the texture slot index to 1, keeping the default white
         * texture at slot 0.
         *
         * Throws:
         * - NxRendererNotInitialized if the renderer is not initialized.
         */
        void unbindTextures() const;

        /**
         * @brief Returns a shared Vertex Array Object (VAO) for rendering a cube.
         * @return Shared pointer to a pre-configured cube VAO.
         */
        static std::shared_ptr<NxVertexArray> getCubeVAO();

        /**
         * @brief Returns a shared Vertex Array Object (VAO) for rendering a box (used for debugging AABB)
         * @return Shared pointer to a pre-configured box VAO
         */
        static std::shared_ptr<NxVertexArray> getBoxVAO();

        /**
         * @brief Returns a shared VAO for rendering a billboard (quad facing camera).
         * @return Shared pointer to a pre-configured billboard VAO.
         */
        static std::shared_ptr<NxVertexArray> getBillboardVAO();

        /**
         * @brief Returns a shared VAO for rendering video content on a quad.
         * @return Shared pointer to a pre-configured video quad VAO.
         */
        static std::shared_ptr<NxVertexArray> getVideoVAO();

        /**
         * @brief Returns a shared VAO for rendering a tetrahedron.
         * @return Shared pointer to a pre-configured tetrahedron VAO.
         */
        static std::shared_ptr<NxVertexArray> getTetrahedronVAO();

        /**
         * @brief Returns a shared VAO for rendering a pyramid.
         * @return Shared pointer to a pre-configured pyramid VAO.
         */
        static std::shared_ptr<NxVertexArray> getPyramidVAO();

        /**
         * @brief Returns a shared VAO for rendering a cylinder with specified segments.
         * @param nbSegment Number of segments around the cylinder's circumference.
         * @return Shared pointer to a pre-configured cylinder VAO.
         */
        static std::shared_ptr<NxVertexArray> getCylinderVAO(unsigned int nbSegment);

        /**
         * @brief Returns a shared VAO for rendering a UV sphere with specified subdivisions.
         * @param nbSubdivision Number of subdivisions for the sphere's mesh resolution.
         * @return Shared pointer to a pre-configured sphere VAO.
         */
        static std::shared_ptr<NxVertexArray> getSphereVAO(unsigned int nbSubdivision);

        /**
         * @brief Provides access to the internal storage of the renderer.
         *
         * This method returns a shared pointer to the `NxRenderer3DStorage` instance,
         * which contains all internal data and resources used by the renderer.
         *
         * @return A shared pointer to the `NxRenderer3DStorage`.
         *
         * Throws:
         * - NxRendererNotInitialized if the renderer is not initialized.
         */
        [[nodiscard]] std::shared_ptr<NxRenderer3DStorage> getInternalStorage() const
        {
            return m_storage;
        }

        /**
         * @brief Returns the texture index for a given texture.
         *
         * Searches the texture slots for an existing binding. If not found, assigns a new slot.
         *
         * @param texture The texture to look up.
         * @return float The texture index.
         */
        [[nodiscard]] int getTextureIndex(const std::shared_ptr<NxTexture2D>& texture) const;

        void switchToNextTextureBatch();

       private:
        std::shared_ptr<NxRenderer3DStorage> m_storage;
        bool m_renderingScene = false;
    };
} // namespace nexo::renderer
