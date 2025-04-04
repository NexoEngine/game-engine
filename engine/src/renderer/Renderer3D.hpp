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
#include "components/Render3D.hpp"

#include <array>
#include <glm/glm.hpp>

namespace nexo::renderer
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;

        int entityID;
    };

    struct Material
    {
        glm::vec4 albedoColor = glm::vec4(1.0f);
        int albedoTexIndex = 0; // Default: 0 (white texture)
        glm::vec4 specularColor = glm::vec4(1.0f);
        int specularTexIndex = 0; // Default: 0 (white texture)
        glm::vec3 emissiveColor = glm::vec3(0.0f);
        int emissiveTexIndex = 0; // Default: 0 (white texture)
        float roughness = 0.5f;
        int roughnessTexIndex = 0; // Default: 0 (white texture)
        float metallic = 0.0f;
        int metallicTexIndex = 0; // Default: 0 (white texture)
        float opacity = 1.0f;
        int opacityTexIndex = 0; // Default: 0 (white texture)
    };

    //TODO: Add stats for the meshes
    struct Renderer3DStats
    {
        unsigned int drawCalls = 0;
        unsigned int cubeCount = 0;

        [[nodiscard]] unsigned int getTotalVertexCount() const { return cubeCount * 8; }
        [[nodiscard]] unsigned int getTotalIndexCount() const { return cubeCount * 36; }
    };

    /**
     * @struct Renderer3DStorage
     * @brief Holds internal data and resources used by Renderer3D.
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
    struct Renderer3DStorage
    {
        const unsigned int maxCubes = 10000;
        const unsigned int maxVertices = maxCubes * 8;
        const unsigned int maxIndices = maxCubes * 36;
        static constexpr unsigned int maxTextureSlots = 32;
        static constexpr unsigned int maxTransforms = 1024;

        std::shared_ptr<Shader> textureShader;
        std::shared_ptr<VertexArray> vertexArray;
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;
        std::shared_ptr<Texture2D> whiteTexture;

        unsigned int indexCount = 0;
        std::array<Vertex, 80000> vertexBufferBase;
        std::array<unsigned int, 360000> indexBufferBase;
        Vertex* vertexBufferPtr = nullptr;
        unsigned int* indexBufferPtr = nullptr;

        std::array<std::shared_ptr<Texture2D>, maxTextureSlots> textureSlots;
        unsigned int textureSlotIndex = 1;

        Renderer3DStats stats;
    };

    /**
     * @class Renderer3D
     * @brief Provides a high-performance 3D rendering system for drawing cubes, textured objects, and meshes.
     *
     * The `Renderer3D` class facilitates efficient rendering of 3D objects using batching,
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
    class Renderer3D
    {
    public:
        /**
         * @brief Initializes the Renderer3D and allocates required resources.
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
         * @brief Releases all resources and cleans up the Renderer3D.
         *
         * Deletes allocated vertex and index buffers and resets internal storage pointers.
         *
         * Throws:
         * - RendererNotInitialized if the renderer is not initialized.
         */
        void shutdown();

        /**
         * @brief Begins a new 3D rendering scene.
         *
         * Sets up the view-projection matrix and camera position for rendering.
         * Resets internal storage pointers for batching vertices and indices.
         *
         * @param viewProjection The combined view and projection matrix.
         * @param cameraPos The position of the camera in the scene.
         *
         * Throws:
         * - RendererNotInitialized if the renderer is not initialized.
         * - RendererSceneLifeCycleFailure if called without proper initialization.
         */
        void beginScene(const glm::mat4& viewProjection, const glm::vec3& cameraPos);

        /**
         * @brief Ends the current 3D rendering scene.
         *
         * Uploads vertex and index data to the GPU, flushes the rendering pipeline,
         * and resets buffers for the next frame.
         *
         * Throws:
         * - RendererNotInitialized if the renderer is not initialized.
         * - RendererSceneLifeCycleFailure if no scene was started with `beginScene()`.
         */
        void endScene() const;

        /**
         * @brief Draws a cube using a specified transformation and color.
         *
         * Generates the cube's vertex and index data, updates the vertex buffer with the cube's geometry,
         * and increments the cube count in the statistics.
         *
         * @param position The position of the cube.
         * @param size The dimensions of the cube.
         * @param color The color (RGBA) of the cube.
         * @param entityID An optional entity identifier (default is -1).
         *
         * @throws RendererSceneLifeCycleFailure if the renderer is not in a valid scene.
         */
        void drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color,
                      int entityID = -1) const;

        /**
         * @brief Draws a cube using a specified transformation and color.
         *
         * Generates the cube's vertex and index data, updates the vertex buffer with the cube's geometry,
         * and increments the cube count in the statistics.
         *
         * @param position The position of the cube.
         * @param size The dimensions of the cube.
         * @param rotation The rotation of the cube.
         * @param color The color (RGBA) of the cube.
         * @param entityID An optional entity identifier (default is -1).
         *
         * @throws RendererSceneLifeCycleFailure if the renderer is not in a valid scene.
         */
        void drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
                      const glm::vec4& color, int entityID = -1) const;

        /**
         * @brief Draws a cube using a specified transformation and color.
         *
         * Generates the cube's vertex and index data, updates the vertex buffer with the cube's geometry,
         * and increments the cube count in the statistics.
         *
         * @param transform The transformation matrix for the cube.
         * @param color The color (RGBA) of the cube.
         * @param entityID An optional entity identifier (default is -1).
         *
         * @throws RendererSceneLifeCycleFailure if the renderer is not in a valid scene.
         */
        void drawCube(const glm::mat4& transform, const glm::vec4& color, int entityID = -1) const;

        /**
         * @brief Draws a cube using a specified transformation and material.
         *
         * Generates the cube's vertex and index data, updates the vertex buffer with the cube's geometry,
         * and increments the cube count in the statistics.
         *
         * @param position The position of the cube.
         * @param size The dimensions of the cube.
         * @param material The material properties of the cube.
         * @param entityID An optional entity identifier (default is -1).
         *
         * @throws RendererSceneLifeCycleFailure if the renderer is not in a valid scene.
         */
        void drawCube(const glm::vec3& position, const glm::vec3& size, const components::Material& material,
                      int entityID = -1) const;

        /**
         * @brief Draws a cube using a specified transformation and material.
         *
         * Generates the cube's vertex and index data, updates the vertex buffer with the cube's geometry,
         * and increments the cube count in the statistics.
         *
         * @param position The position of the cube.
         * @param size The dimensions of the cube.
         * @param rotation The rotation of the cube (in Euler angles, in degrees).
         * @param material The material properties of the cube.
         * @param entityID An optional entity identifier (default is -1).
         *
         * @throws RendererSceneLifeCycleFailure if the renderer is not in a valid scene.
         */
        void drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
                      const components::Material& material, int entityID = -1) const;

        /**
         * @brief Draws a cube using a specified transformation and material.
         *
         * Generates the cube's vertex and index data, updates the vertex buffer with the cube's geometry,
         * and increments the cube count in the statistics.
         *
         * @param position The position of the cube.
         * @param size The dimensions of the cube.
         * @param rotation The rotation of the cube (in quaternion format).
         * @param material The material properties of the cube.
         * @param entityID An optional entity identifier (default is -1).
         *
         * @throws RendererSceneLifeCycleFailure if the renderer is not in a valid scene.
         */
        void drawCube(const glm::vec3& position, const glm::vec3& size, const glm::quat& rotation,
                      const components::Material& material, int entityID = -1) const;

        /**
         * @brief Draws a cube using a specified transformation and color.
         *
         * Generates the cube's vertex and index data, updates the vertex buffer with the cube's geometry,
         * and increments the cube count in the statistics.
         *
         * @param transform The transformation matrix for the cube.
         * @param material The material properties of the cube.
         * @param entityID An optional entity identifier (default is -1).
         *
         * @throws RendererSceneLifeCycleFailure if the renderer is not in a valid scene.
         */
        void drawCube(const glm::mat4& transform, const components::Material& material, int entityID = -1) const;


        /**
         * @brief Draws a 3D tetrahedron at the specified position and size.
         *
         * The tetrahedron can be drawn with a solid color or a texture.
         *
         * @param position The position of the tetrahedron in the 3D space.
         * @param size The dimensions of the tetrahedron.
         * @param color The color of the tetrahedron (RGBA format).
         * @param texture Optional texture to apply to the tetrahedron.
         *
         * Overloads:
         * - Draws a tetrahedron with a solid color or a texture.
         */
        void drawTetrahedron(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color,
                             int entityID = -1) const;

        /**
         * @brief Draws a custom 3D mesh.
         *
         * The mesh is defined by its vertices and indices, and optionally textured.
         *
         * @param vertices A vector of vertices defining the geometry of the mesh.
         * @param indices A vector of indices defining the connectivity of the mesh.
         * @param texture Optional texture to apply to the mesh.
         *
         * Throws:
         * - RendererSceneLifeCycleFailure if no scene was started with `beginScene()`.
         */
        void drawMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
                      const std::shared_ptr<Texture2D>& texture, int entityID = -1) const;
        void drawMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
                      const glm::vec3& position, const glm::vec3& size, const components::Material& material,
                      int entityID = -1) const;
        void drawMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
                      const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& size,
                      const components::Material& material, int entityID = -1) const;
        void drawMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
                      const glm::mat4& transform, const components::Material& material, int entityID = -1) const;

        /**
         * @brief Draws a pyramid at the specified position and size.
         *
         * The pyramid can be drawn with a solid color.
         *
         * @param position The position of the pyramid in the 3D space.
         * @param size The dimensions of the pyramid.
         * @param color The color of the pyramid (RGBA format).
         * @param entityID The ID of the entity.
         *
         * Overloads:
         * - Draws a pyramid with a solid color.
         */
        void drawPyramid(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color,
                   int entityID = -1) const;

        /**
         * @brief Resets rendering statistics.
         *
         * Clears the draw call and cube counters in `Renderer3DStats`.
         *
         * Throws:
         * - RendererNotInitialized if the renderer is not initialized.
         */
        void resetStats() const;
        /**
         * @brief Retrieves the current rendering statistics.
         *
         * @return A `Renderer3DStats` struct containing the number of draw calls and
         *         cubes rendered.
         *
         * Throws:
         * - RendererNotInitialized if the renderer is not initialized.
         */
        [[nodiscard]] Renderer3DStats getStats() const;

        std::shared_ptr<Shader>& getShader() const { return m_storage->textureShader; };

        std::shared_ptr<Renderer3DStorage> getInternalStorage() const { return m_storage; };

    private:
        std::shared_ptr<Renderer3DStorage> m_storage;
        bool m_renderingScene = false;

        /**
         * @brief Flushes the current batched data to the GPU and issues the draw call.
         *
         * Binds all active textures, draws indexed geometry, updates statistics, and unbinds resources.
         */
        void flush() const;

        /**
         * @brief Flushes the current batch and resets batching pointers.
         */
        void flushAndReset() const;

        /**
         * @brief Returns the texture index for a given texture.
         *
         * Searches the texture slots for an existing binding. If not found, assigns a new slot.
         *
         * @param texture The texture to look up.
         * @return float The texture index.
         */
        [[nodiscard]] int getTextureIndex(const std::shared_ptr<Texture2D>& texture) const;

        /**
         * @brief Sets material-related uniforms in the texture shader.
         *
         * Updates uniforms for albedo, specular, emissive, roughness, metallic, and opacity properties.
         *
         * @param material The material whose properties are to be set.
         *
         * @throws RendererNotInitialized if the renderer is not initialized.
         */
        void setMaterialUniforms(const renderer::Material& material) const;
    };
}
