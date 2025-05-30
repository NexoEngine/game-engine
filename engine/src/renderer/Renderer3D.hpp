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
#include "ShaderLibrary.hpp"

#include <array>
#include <glm/glm.hpp>

namespace nexo::renderer
{
    struct NxVertex
    {
        glm::vec3 position;
        glm::vec2 texCoord;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;

        int entityID;
    };

    struct NxIndexedMaterial
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

    struct NxMaterial
    {
        glm::vec4 albedoColor = glm::vec4(1.0f);
        glm::vec4 specularColor = glm::vec4(1.0f);
        glm::vec3 emissiveColor = glm::vec3(0.0f);

        float roughness = 0.0f; // 0 = smooth, 1 = rough
        float metallic = 0.0f; // 0 = non-metal, 1 = fully metallic
        float opacity = 1.0f; // 1 = opaque, 0 = fully transparent

        std::shared_ptr<NxTexture2D> albedoTexture = nullptr;
        std::shared_ptr<NxTexture2D> normalMap = nullptr;
        std::shared_ptr<NxTexture2D> metallicMap = nullptr;
        std::shared_ptr<NxTexture2D> roughnessMap = nullptr;
        std::shared_ptr<NxTexture2D> emissiveMap = nullptr;

        std::string shader;
    };

    //TODO: Add stats for the meshes
    struct NxRenderer3DStats
    {
        unsigned int drawCalls = 0;
        unsigned int cubeCount = 0;

        [[nodiscard]] unsigned int getTotalVertexCount() const { return cubeCount * 8; }
        [[nodiscard]] unsigned int getTotalIndexCount() const { return cubeCount * 36; }
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
    struct NxRenderer3DStorage
    {
        const unsigned int maxCubes = 10000;
        const unsigned int maxVertices = maxCubes * 8;
        const unsigned int maxIndices = maxCubes * 36;
        static constexpr unsigned int maxTextureSlots = 32;
        static constexpr unsigned int maxTransforms = 1024;

        glm::vec3 cameraPosition;

        std::shared_ptr<NxShader> currentSceneShader = nullptr;
        std::shared_ptr<NxVertexArray> vertexArray;
        std::shared_ptr<NxVertexBuffer> vertexBuffer;
        std::shared_ptr<NxIndexBuffer> indexBuffer;
        std::shared_ptr<NxTexture2D> whiteTexture;

        unsigned int indexCount = 0;
        std::array<NxVertex, 80000> vertexBufferBase;
        std::array<unsigned int, 360000> indexBufferBase;
        NxVertex* vertexBufferPtr = nullptr;
        unsigned int* indexBufferPtr = nullptr;

        std::array<std::shared_ptr<NxTexture2D>, maxTextureSlots> textureSlots;
        unsigned int textureSlotIndex = 1;

        NxRenderer3DStats stats;
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
    class NxRenderer3D
    {
    public:

        static NxRenderer3D& get() {
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

        void bindTextures();
        void unbindTextures();

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
         * - NxRendererNotInitialized if the renderer is not initialized.
         * - NxRendererSceneLifeCycleFailure if called without proper initialization.
         */
        void beginScene(const glm::mat4& viewProjection, const glm::vec3& cameraPos, const std::string& shader = "");

        /**
         * @brief Ends the current 3D rendering scene.
         *
         * Uploads vertex and index data to the GPU, flushes the rendering pipeline,
         * and resets buffers for the next frame.
         *
         * Throws:
         * - NxRendererNotInitialized if the renderer is not initialized.
         * - NxRendererSceneLifeCycleFailure if no scene was started with `beginScene()`.
         */
        void endScene() const;

        static std::shared_ptr<NxVertexArray> getCubeVAO();
        static std::shared_ptr<NxVertexArray> getBillboardVAO();
        static std::shared_ptr<NxVertexArray> getTetrahedronVAO();
        static std::shared_ptr<NxVertexArray> getPyramidVAO();
        static std::shared_ptr<NxVertexArray> getCylinderVAO(unsigned int nbSegment);
        static std::shared_ptr<NxVertexArray> getSphereVAO(unsigned int nbSubdivision);

        /**
         * @brief Resets rendering statistics.
         *
         * Clears the draw call and cube counters in `NxRenderer3DStats`.
         *
         * Throws:
         * - NxRendererNotInitialized if the renderer is not initialized.
         */
        void resetStats() const;
        /**
         * @brief Retrieves the current rendering statistics.
         *
         * @return A `NxRenderer3DStats` struct containing the number of draw calls and
         *         cubes rendered.
         *
         * Throws:
         * - NxRendererNotInitialized if the renderer is not initialized.
         */
        [[nodiscard]] NxRenderer3DStats getStats() const;

        [[nodiscard]] std::shared_ptr<NxShader>& getShader() const { return m_storage->currentSceneShader; };

        [[nodiscard]] std::shared_ptr<NxRenderer3DStorage> getInternalStorage() const { return m_storage; };

        /**
         * @brief Returns the texture index for a given texture.
         *
         * Searches the texture slots for an existing binding. If not found, assigns a new slot.
         *
         * @param texture The texture to look up.
         * @return float The texture index.
         */
        [[nodiscard]] int getTextureIndex(const std::shared_ptr<NxTexture2D>& texture) const;
    private:
        std::shared_ptr<NxRenderer3DStorage> m_storage;
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
         * @brief Sets material-related uniforms in the texture shader.
         *
         * Updates uniforms for albedo, specular, emissive, roughness, metallic, and opacity properties.
         *
         * @param material The material whose properties are to be set.
         *
         * @throws NxRendererNotInitialized if the renderer is not initialized.
         */
        void setMaterialUniforms(const NxIndexedMaterial& material) const;
    };
}
