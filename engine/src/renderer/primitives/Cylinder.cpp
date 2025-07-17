//// Cylinder.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzz zzz     zzz  zzzz               zzz  zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  zzz                zzzz  zzzz      zzzz           zzzz
//  zzz      z  zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        05/04/2025
//  Description: Source file for the cylinder primitive
//
///////////////////////////////////////////////////////////////////////////////

#include "renderer/Renderer3D.hpp"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <Logger.hpp>
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace nexo::renderer
{
    constexpr float CYLINDER_HEIGHT = 1.0f; // Height of the cylinder should be 1.0f

    static std::vector<glm::vec3> generateCylinderVertices(const unsigned int nbSegment)
    {
        std::vector<glm::vec3> vertices{};
        vertices.reserve(nbSegment * 4); // Reserve memory for all vertices (2 caps + 2 sides)

        int i = 0;
        for (unsigned int k = nbSegment - 1; i < nbSegment; ++i, --k)
        {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, CYLINDER_HEIGHT, z);
        }
        for (unsigned int k = nbSegment - 1; i < nbSegment * 2; ++i, --k)
        {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, -CYLINDER_HEIGHT, z);
        }
        for (int k = 0; i < nbSegment * 3; ++i, ++k)
        {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, CYLINDER_HEIGHT, z);
        }
        for (int k = 0; i < nbSegment * 4; ++i, ++k)
        {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, -CYLINDER_HEIGHT, z);
        }
        return vertices;
    }

    /**
     * @brief Generates indices for the caps of a cylinder mesh.
     *
     * This function calculates the indices required to form the top and bottom caps of a cylinder mesh.
     * It uses a recursive approach to divide the cap into triangles, ensuring proper tessellation.
     *
     * @param indices A reference to the vector where the generated indices will be stored.
     * @param transformer An offset applied to the indices to account for the starting position of the cap vertices. Different for top and bottom caps.
     * @param nbSegment The number of segments (or divisions) around the cylinder's circumference. (ex 3 for triangle, more if there is sub triangles.
     */
    static void capIndices(std::vector<unsigned int>& indices, const int transformer, const unsigned int nbSegment)
    {
        std::function<void(int, unsigned int)> capIndicesRec;

        capIndicesRec = [&indices, &transformer, &capIndicesRec, &nbSegment](const int start, const unsigned int nbEdge)
        {
            // Calculate the step size for dividing the cap into triangles
            // Base case: If the step size is 1, form a single triangle
            if (const int step = ceil(static_cast<double>(nbEdge) / 3.0); step == 1)
            {
                const int tmp = start + 2 < nbSegment ? start + 2 : 0;
                indices.push_back(start + transformer);
                indices.push_back(tmp + transformer);
                indices.push_back(start + 1 + transformer);
            }
            else
            {
                // Recursive case: Divide the cap into smaller sections
                capIndicesRec(start, step + 1);

                if (start + 2 * step < start + nbEdge - 1)
                {
                    int tmp = 0;
                    if (start + 2 * step < start + nbEdge - 1)
                    {
                        tmp = static_cast<int>(start + nbEdge - 1);
                        capIndicesRec(start + step, tmp - (start + step) + 1);
                    }
                    else if (start + 2 * step < nbSegment)
                    {
                        tmp = start + 2 * step;
                        capIndicesRec(start + step, step + 1);
                    }
                    tmp = tmp > nbSegment - 1 ? 0 : tmp;
                    indices.push_back(start + transformer);
                    indices.push_back(tmp + transformer);
                    indices.push_back(start + step + transformer);
                }
                else
                {
                    const int tmp = static_cast<int>(start + nbEdge - 1 < nbSegment ? start + nbEdge - 1 : 0);
                    indices.push_back(start + transformer);
                    indices.push_back(tmp + transformer);
                    indices.push_back(start + step + transformer);
                    if (start + nbEdge - 1 - (start + step) > 1)
                    {
                        capIndicesRec(start + step, step + 1);
                    }
                }
            }
        };

        // Initial setup: Define the starting point and step size
        constexpr int start = 0;
        const int step = ceil(static_cast<double>(nbSegment) / 3.0);

        // Add the first triangle to the indices
        indices.push_back(start + transformer);
        indices.push_back(start + 2 * step + transformer);
        indices.push_back(start + step + transformer);

        // Generate additional triangles: at least for 2 subsections if nbSegment > 3
        if (nbSegment > 3)
        {
            capIndicesRec(start, step + 1);
            capIndicesRec(start + step, step + 1);
        }

        // Generate additional triangles: add 3td subsections if nbSegment > 5
        if (nbSegment > 5)
            capIndicesRec(start + 2 * step, nbSegment - 2 * step + 1);
    }

    /**
     * @brief Generates the indices for a cylinder mesh.
     *
     * This function calculates the indices required to form the faces of a cylinder mesh,
     * including the side faces and the top and bottom caps. The indices are used to define
     * the triangles that make up the cylinder in a 3D rendering context.
     *
     * @param nbSegment The number of segments (or divisions) around the cylinder's circumference.
     *                  This determines the resolution of the cylinder.
     * @return A vector containing the generated indices for the cylinder mesh.
     */
    static std::vector<unsigned int> generateCylinderIndices(const unsigned int nbSegment)
    {
        std::vector<unsigned int> indices{};
        // Reserve memory for all indices (* 2 for sides * 2 for caps and * 3 because it's triangles)
        indices.reserve(nbSegment * 12);

        // Generate indices for the side faces of the cylinder
        int i = 0;
        for (; i < nbSegment - 1; ++i)
        {
            // Create two triangles for each segment
            indices.push_back(i);
            indices.push_back(i + nbSegment);
            indices.push_back(i + 1);

            indices.push_back(i + 1);
            indices.push_back(i + nbSegment);
            indices.push_back(i + nbSegment + 1);
        }
        // Handle the last segment, which wraps around to the first segment
        indices.push_back(i);
        indices.push_back(i + nbSegment);
        indices.push_back(0);

        indices.push_back(0);
        indices.push_back(i + nbSegment);
        indices.push_back(nbSegment);

        // Generate indices for the top and bottom caps of the cylinder
        capIndices(indices, static_cast<int>(nbSegment * 2), nbSegment);
        capIndices(indices, static_cast<int>(nbSegment * 3), nbSegment);

        return indices;
    }

    /**
     * @brief Generates texture coordinates for a cylinder mesh.
     *
     * This function calculates the texture coordinates (u, v) for a cylinder mesh.
     * The `u` coordinate is based on the angle around the cylinder's circumference,
     * while the `v` coordinate is constant and represents the vertical position.
     *
     * @param nbSegment The number of segments (or divisions) around the cylinder's circumference.
     *                  This determines the resolution of the cylinder.
     * @return A vector of `glm::vec2` containing the generated texture coordinates.
     */
    static std::vector<glm::vec2> generateTextureCoords(const unsigned int nbSegment)
    {
        std::vector<glm::vec2> texCoords{};
        texCoords.reserve(nbSegment * 4); // Reserve memory for all texture coordinates (one for each vertex)

        // Generate texture coordinates for cylinder sides
        for (int i = 0; i < nbSegment; ++i)
        {
            const float u = static_cast<float>(i) / static_cast<float>(nbSegment);
            texCoords.emplace_back(u, 1.0f); // Top edge
        }
        for (int i = 0; i < nbSegment; ++i)
        {
            const float u = static_cast<float>(i) / static_cast<float>(nbSegment);
            texCoords.emplace_back(u, 0.0f); // Bottom edge
        }
        // Cap vertices use radial UV mapping
        for (int i = 0; i < nbSegment * 2; ++i)
        {
            const float angle = static_cast<float>(i % nbSegment) / static_cast<float>(nbSegment) * 2.0f * static_cast<
                float>(M_PI);
            const float u = (std::cos(angle) + 1.0f) * 0.5f;
            const float v = (std::sin(angle) + 1.0f) * 0.5f;
            texCoords.emplace_back(u, v);
        }
        return texCoords;
    }


    /**
     * @brief Generates normal vectors for a cylinder mesh.
     *
     * This function calculates the normal vectors for the vertices of a cylinder mesh.
     * The normals are used in lighting calculations to determine how light interacts
     * with the surface of the cylinder.
     *
     * @param vertices A vector of `glm::vec3` representing the positions of the vertices
     *                 in the cylinder mesh.
     * @param nbSegment The number of segments (or divisions) around the cylinder's circumference.
     *                  This determines the resolution of the cylinder.
     * @return A vector of `glm::vec3` containing the calculated normal vectors for the cylinder mesh.
     */
    static std::vector<glm::vec3> generateNormals(const std::vector<glm::vec3>& vertices, const unsigned int nbSegment)
    {
        std::vector<glm::vec3> normals{};
        normals.reserve(vertices.size()); // Reserve memory for all normals (2 caps + 2 sides)
        int i = 0;

        // Generate normals for the top cap of the cylinder
        for (; i < nbSegment * 1; ++i)
        {
            const glm::vec3 vector1 = vertices[i] - glm::vec3(0, 1, 0);
            normals.emplace_back(vector1);
        }

        // Generate normals for the bottom cap of the cylinder
        for (; i < nbSegment * 2; ++i)
        {
            const glm::vec3 vector2 = vertices[i] - glm::vec3(0, -1, 0);
            normals.emplace_back(vector2);
        }

        // Generate normals for the side faces of the cylinder (top half)
        for (; i < nbSegment * 3; ++i)
        {
            normals.emplace_back(0, 1, 0);
        }

        // Generate normals for the side faces of the cylinder (bottom half)
        for (; i < nbSegment * 4; ++i)
        {
            normals.emplace_back(0, -1, 0);
        }

        return normals;
    }

    /**
     * @brief Creates or retrieves a vertex array object (VAO) for a cylinder mesh.
     *
     * This function generates a VAO for a cylinder mesh with the specified number of segments.
     * If a VAO for the given number of segments already exists, it retrieves it from a cache.
     * Otherwise, it creates a new VAO, generates the necessary vertex and index data, and stores it in the cache.
     *
     * @param nbSegment The number of segments (or divisions) around the cylinder's circumference.
     *                  Must be at least 3. If a value less than 3 is provided, it defaults to 8.
     * @return A shared pointer to the `NxVertexArray` containing the cylinder mesh data.
     */
    std::shared_ptr<NxVertexArray> NxRenderer3D::getCylinderVAO(unsigned int nbSegment)
    {
        // Ensure the number of segments is at least 3, defaulting to 8 if not.
        if (nbSegment < 3)
        {
            LOG(NEXO_WARN, "Cylinder segments must be at least 3, using default value of 8.");
            nbSegment = 8;
        }

        // Static map to cache VAOs for different segment counts.
        static std::map<unsigned int, std::shared_ptr<NxVertexArray>> cylinderVaoMap;

        // If a VAO for the given segment count already exists, return it.
        if (cylinderVaoMap.contains(nbSegment))
            return cylinderVaoMap[nbSegment];

        // Calculate the total number of vertices for the cylinder.
        const unsigned int nbVerticesCylinder = nbSegment * 4;

        // Create a new VAO and vertex buffer for the cylinder.
        cylinderVaoMap[nbSegment] = createVertexArray();
        const auto vertexBuffer = createVertexBuffer(nbVerticesCylinder * sizeof(NxVertex));

        // Define the layout of the vertex buffer.
        const NxBufferLayout cylinderVertexBufferLayout = {
            {NxShaderDataType::FLOAT3, "aPos"}, // Position attribute
            {NxShaderDataType::FLOAT2, "aTexCoord"}, // Texture coordinate attribute
            {NxShaderDataType::FLOAT3, "aNormal"}, // Normal vector attribute
            {NxShaderDataType::FLOAT3, "aTangent"}, // Tangent vector attribute
            {NxShaderDataType::FLOAT3, "aBiTangent"}, // Bi tangent vector attribute
            {NxShaderDataType::INT, "aEntityID"} // Entity ID attribute
        };
        vertexBuffer->setLayout(cylinderVertexBufferLayout);

        // Generate the vertex data for the cylinder.
        const std::vector<glm::vec3> vertices = generateCylinderVertices(nbSegment);
        const std::vector<glm::vec2> texCoords = generateTextureCoords(nbSegment);
        const std::vector<glm::vec3> normals = generateNormals(vertices, nbSegment);
        std::vector<unsigned int> indices = generateCylinderIndices(nbSegment);

        // Populate the vertex buffer with the generated data.
        std::vector<NxVertex> vertexData(nbVerticesCylinder);
        for (unsigned int i = 0; i < nbVerticesCylinder; ++i)
        {
            vertexData[i].position = glm::vec4(vertices[i], 1.0f);
            vertexData[i].texCoord = texCoords[i];
            vertexData[i].normal = normals[i];
            vertexData[i].tangent = glm::vec3(0.0f, 0.0f, 0.0f); // Default tangent
            vertexData[i].bitangent = glm::vec3(0.0f, 0.0f, 0.0f); // Default bi tangent
            vertexData[i].entityID = 0; // Default entity ID
        }
        vertexBuffer->setData(vertexData.data(), vertexData.size() * sizeof(NxVertex));
        cylinderVaoMap[nbSegment]->addVertexBuffer(vertexBuffer);

        // Create and populate the index buffer with the generated indices.
        const auto indexBuffer = createIndexBuffer();
        indexBuffer->setData(indices.data(), indices.size());
        cylinderVaoMap[nbSegment]->setIndexBuffer(indexBuffer);

        // Return the newly created VAO.
        return cylinderVaoMap[nbSegment];
    }
}
