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
#include <array>
#include <cmath>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <gmock/gmock-matchers.h>

namespace nexo::renderer
{
    // unsigned int CYLINDER_SEGMENTS = 8; // Number of segments for the cylinder min 3
    constexpr float CYLINDER_HEIGHT = 1.0f; /**
     * @brief Generates the 3D vertex positions for a cylinder mesh.
     *
     * Creates a vector of 3D positions for a cylinder with the specified number of segments, including vertices for the top and bottom circles and their duplicates for cap triangulation. The cylinder has a height of 2.0 units, centered at the origin along the Y-axis.
     *
     * @param nbSegment Number of segments to divide the cylinder's circular faces. Must be at least 3 for a valid cylinder.
     * @return std::vector<glm::vec3> List of vertex positions for the cylinder mesh.
     */

    static std::vector<glm::vec3> generateCylinderVertices(const unsigned int nbSegment)
    {
        std::vector<glm::vec3> vertices{};

        int i = 0;
        for (unsigned int k = nbSegment-1; i < nbSegment; ++i, --k) {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, CYLINDER_HEIGHT, z);
        }
        for (unsigned int k = nbSegment-1; i < nbSegment*2; ++i, --k) {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, -CYLINDER_HEIGHT, z);
        }
        for (int k = 0; i < nbSegment*3; ++i, ++k) {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, CYLINDER_HEIGHT, z);
        }
        for (int k = 0; i < nbSegment*4; ++i, ++k) {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, -CYLINDER_HEIGHT, z);
        }
        return vertices;
    }

    /**
     * @brief Generates triangle indices for a cylinder cap using recursive triangulation.
     *
     * Adds indices to the provided vector to form a triangulated cap (top or bottom) of a cylinder mesh, handling arbitrary segment counts.
     *
     * @param indices Vector to which generated triangle indices are appended.
     * @param transformer Offset applied to each index to address the correct subset of vertices for the cap.
     * @param nbSegment Number of segments (vertices) around the cap's circumference.
     */
    static void capIndices(std::vector<unsigned int> &indices, const int transformer, const unsigned int nbSegment)
    {
        std::function<bool(int)> recurFun;
        std::function<void(int, unsigned int)> capIndicesRec;

        capIndicesRec = [&indices, &transformer, &capIndicesRec, &nbSegment](const int start, const unsigned int nbEdge) {
            if (const int step = ceil(static_cast<double>(nbEdge) / 3.0); step == 1) {
                const int tmp = (start + 2 < nbSegment) ? (start + 2) : 0;
                indices.push_back(start + transformer); indices.push_back(tmp + transformer); indices.push_back(start + 1 + transformer);
            } else {
                capIndicesRec(start, step + 1);
                if (start + 2 * step < start + nbEdge - 1) {
                    int tmp = 0;
                    if (start + 2 * step < start + nbEdge - 1) {
                        tmp = start + nbEdge - 1;
                        capIndicesRec(start + step, tmp - (start + step) + 1);
                    } else if (start + 2 * step < nbSegment) {
                        tmp = start + 2 * step;
                        capIndicesRec(start + step, step + 1);
                    }
                    tmp = tmp > nbSegment-1 ? 0 : tmp;
                    indices.push_back(start + transformer); indices.push_back(tmp + transformer); indices.push_back(start + step + transformer);
                } else {
                    const int tmp = (start + nbEdge - 1 < nbSegment) ? (start + nbEdge - 1) : 0;
                    indices.push_back(start + transformer); indices.push_back(tmp + transformer); indices.push_back(start + step + transformer);
                    if ((start + nbEdge - 1) - (start + step) > 1) {
                        capIndicesRec(start + step, step + 1);
                    }
                }
            }
        };

        constexpr int start = 0;
        const int step = ceil(static_cast<double>(nbSegment) / 3.0);
        indices.push_back(start + transformer); indices.push_back(start + 2 * step + transformer); indices.push_back(start + step + transformer);
        if (nbSegment > 3) {
            capIndicesRec(start, step + 1);
            capIndicesRec(start + step, step + 1);
        }
        if (nbSegment > 5)
            capIndicesRec(start + 2 * step, nbSegment - 2 * step + 1);
    }

    /**
     * @brief Generates index data for rendering a cylinder mesh with the specified number of segments.
     *
     * Constructs triangle indices for the cylinder's side faces and both end caps, suitable for use in an index buffer.
     *
     * @param nbSegment Number of segments around the cylinder's circumference.
     * @return std::vector<unsigned int> Index buffer for the cylinder mesh.
     */
    static std::vector<unsigned int> generateCylinderIndices(const unsigned int nbSegment)
    {
        std::vector<unsigned int> indices{};

        int i = 0;
        for (; i < nbSegment-1; ++i) {
            indices.push_back(i); indices.push_back(i + nbSegment); indices.push_back(i + 1);
            indices.push_back(i + 1); indices.push_back(i + nbSegment); indices.push_back(i + nbSegment + 1);
        }
        indices.push_back(i); indices.push_back(i + nbSegment); indices.push_back(0);
        indices.push_back(0); indices.push_back(i + nbSegment); indices.push_back(nbSegment);

        capIndices(indices, nbSegment*2, nbSegment);
        capIndices(indices, nbSegment*3, nbSegment);

        return indices;
    }

    /**
     * @brief Generates texture coordinates for a cylinder mesh.
     *
     * Computes 2D texture coordinates for all vertices of a cylinder with the specified number of segments, mapping the angular position around the cylinder to the horizontal (u) coordinate and assigning a constant vertical (v) coordinate.
     *
     * @param nbSegment Number of segments around the cylinder.
     * @return std::vector<glm::vec2> Texture coordinates for each vertex.
     */
    static std::vector<glm::vec2> generateTextureCoords(const unsigned int nbSegment)
    {
        std::vector<glm::vec2> texCoords{};
        for (int i = 0; i < nbSegment*4; ++i) {
            const float angle = static_cast<float>(i) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);

            const float u = angle / (2.0f * static_cast<float>(M_PI));
            constexpr float v = CYLINDER_HEIGHT;

            texCoords.emplace_back(u, v);
        }
        return texCoords;
    }

    /**
     * @brief Generates normal vectors for a cylinder mesh.
     *
     * Computes normals for each vertex in a cylinder mesh based on their position and segment count. Side vertices receive outward-pointing normals, while cap vertices receive normals pointing up or down.
     *
     * @param vertices The list of cylinder mesh vertices.
     * @param nbSegment The number of segments used to construct the cylinder.
     * @return std::vector<glm::vec3> The computed normal vectors for all vertices.
     */
    static std::vector<glm::vec3> generateNormals(const std::vector<glm::vec3> &vertices, const unsigned int nbSegment)
    {
        std::vector<glm::vec3> normals{};
        int i = 0;
        for (; i < nbSegment*1; ++i) {
            const glm::vec3 vector1 = vertices[i] - glm::vec3(0, 1, 0);
            normals.emplace_back(vector1);
        }
        for (; i < nbSegment*2; ++i) {
            const glm::vec3 vector2 = vertices[i] - glm::vec3(0, -1, 0);
            normals.emplace_back(vector2);
        }
        for (; i < nbSegment*3; ++i) {
            normals.emplace_back(0,1,0);
        }
        for (; i < nbSegment*4; ++i) {
            normals.emplace_back(0,-1,0);
        }
        std::ranges::copy(normals, normals.begin());
        return normals;
    }

    /**
     * @brief Returns a vertex array object (VAO) representing a 3D cylinder mesh with the specified segment count.
     *
     * Generates and caches the geometry, normals, texture coordinates, and indices for a cylinder mesh. If a VAO for the given segment count already exists, it is returned from cache. The segment count is clamped to a minimum of 3. The resulting VAO is ready for use in rendering.
     *
     * @param nbSegment Number of segments around the cylinder's circumference.
     * @return Shared pointer to the VAO containing the cylinder mesh data.
     */
    std::shared_ptr<NxVertexArray> NxRenderer3D::getCylinderVAO(unsigned int nbSegment)
    {
        if (nbSegment < 3) {
            LOG(NEXO_WARN, "Cylinder segments must be at least 3, using default value of 8.");
            nbSegment = 8;
        }
        static std::map <unsigned int, std::shared_ptr<NxVertexArray>> cylinderVaoMap;
        if (cylinderVaoMap.contains(nbSegment))
            return cylinderVaoMap[nbSegment];
        const unsigned int nbVerticesCylinder = nbSegment*4;

        cylinderVaoMap[nbSegment] = createVertexArray();
        const auto vertexBuffer = createVertexBuffer(nbVerticesCylinder * sizeof(NxVertex));
        const NxBufferLayout cylinderVertexBufferLayout = {
            {NxShaderDataType::FLOAT3, "aPos"},
            {NxShaderDataType::FLOAT2, "aTexCoord"},
            {NxShaderDataType::FLOAT3, "aNormal"},
            {NxShaderDataType::FLOAT3, "aTangent"},
            {NxShaderDataType::FLOAT3, "aBiTangent"},
            {NxShaderDataType::INT, "aEntityID"}
        };
        vertexBuffer->setLayout(cylinderVertexBufferLayout);

        const std::vector<glm::vec3> vertices = generateCylinderVertices(nbSegment);
        const std::vector<glm::vec2> texCoords = generateTextureCoords(nbSegment);
        const std::vector<glm::vec3> normals = generateNormals(vertices, nbSegment);
        std::vector<unsigned int> indices = generateCylinderIndices(nbSegment);

        std::vector<NxVertex> vertexData(nbVerticesCylinder);
        for (unsigned int i = 0; i < nbVerticesCylinder; ++i) {
            vertexData[i].position = glm::vec4(vertices[i], 1.0f);
            vertexData[i].texCoord = texCoords[i];
            vertexData[i].normal = normals[i];
        }

        vertexBuffer->setData(vertexData.data(), vertexData.size() * sizeof(NxVertex));
        cylinderVaoMap[nbSegment]->addVertexBuffer(vertexBuffer);

        const auto indexBuffer = createIndexBuffer();
        indexBuffer->setData(indices.data(), indices.size());
        cylinderVaoMap[nbSegment]->setIndexBuffer(indexBuffer);

        return cylinderVaoMap[nbSegment];
    }
}
