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
    unsigned int CYLINDER_SEGMENTS = 8; // Number of segments for the cylinder min 3
    constexpr float CYLINDER_HEIGHT = 1.0f; // Height of the cylinder should be 1.0f

    static std::vector<glm::vec3> generateCylinderVertices()
    {
        std::vector<glm::vec3> vertices{};

        int i = 0;
        for (unsigned int k = CYLINDER_SEGMENTS-1; i < CYLINDER_SEGMENTS; ++i, --k) {
            const float angle = static_cast<float>(k) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, CYLINDER_HEIGHT, z);
        }
        for (unsigned int k = CYLINDER_SEGMENTS-1; i < CYLINDER_SEGMENTS*2; ++i, --k) {
            const float angle = static_cast<float>(k) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, -CYLINDER_HEIGHT, z);
        }
        for (int k = 0; i < CYLINDER_SEGMENTS*3; ++i, ++k) {
            const float angle = static_cast<float>(k) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, CYLINDER_HEIGHT, z);
        }
        for (int k = 0; i < CYLINDER_SEGMENTS*4; ++i, ++k) {
            const float angle = static_cast<float>(k) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, -CYLINDER_HEIGHT, z);
        }
        return vertices;
    }

    static void capIndices(std::vector<unsigned int> &indices, const int transformer)
    {
        std::function<bool(int)> recurFun;
        std::function<void(int, int)> capIndicesRec;

        capIndicesRec = [&indices, &transformer, &capIndicesRec](const int start, const int nbSegment) {
            if (const int step = ceil(static_cast<double>(nbSegment) / 3.0); step == 1) {
                const int tmp = (start + 2 < CYLINDER_SEGMENTS) ? (start + 2) : 0;
                indices.push_back(start + transformer); indices.push_back(tmp + transformer); indices.push_back(start + 1 + transformer);
            } else {
                capIndicesRec(start, step + 1);
                if (start + 2 * step < start + nbSegment - 1) {
                    int tmp = 0;
                    if (start + 2 * step < start + nbSegment - 1) {
                        tmp = start + nbSegment - 1;
                        capIndicesRec(start + step, tmp - (start + step) + 1);
                    } else if (start + 2 * step < CYLINDER_SEGMENTS) {
                        tmp = start + 2 * step;
                        capIndicesRec(start + step, step + 1);
                    }
                    tmp = tmp > CYLINDER_SEGMENTS-1 ? 0 : tmp;
                    indices.push_back(start + transformer); indices.push_back(tmp + transformer); indices.push_back(start + step + transformer);
                } else {
                    const int tmp = (start + nbSegment - 1 < CYLINDER_SEGMENTS) ? (start + nbSegment - 1) : 0;
                    indices.push_back(start + transformer); indices.push_back(tmp + transformer); indices.push_back(start + step + transformer);
                    if ((start + nbSegment - 1) - (start + step) > 1) {
                        capIndicesRec(start + step, step + 1);
                    }
                }
            }
        };

        // capIndicesRec = [&indices, &transformer, &capIndicesRec](const int start, const int nbSegment) {
        //     if (const int step = ceil(static_cast<double>(nbSegment) / 3.0); step == 1) {
        //         const int tmp = (start + 2 < CYLINDER_SEGMENTS) ? (start + 2) : 0;
        //         indices.push_back(start + transformer);
        //         indices.push_back(tmp + transformer);
        //         indices.push_back(start + 1 + transformer);
        //     } else {
        //         capIndicesRec(start, step + 1);
        //         const int tmp = (start + nbSegment - 1 < CYLINDER_SEGMENTS) ? (start + nbSegment - 1) : 0;
        //         indices.push_back(start + transformer);
        //         indices.push_back(tmp + transformer);
        //         indices.push_back(start + step + transformer);
        //         if ((start + nbSegment - 1) - (start + step) > 1) {
        //             capIndicesRec(start + step, step + 1);
        //         }
        //     }
        // };

        constexpr int start = 0;
        const int step = ceil(static_cast<double>(CYLINDER_SEGMENTS) / 3.0);
        indices.push_back(start + transformer); indices.push_back(start + 2 * step + transformer); indices.push_back(start + step + transformer);
        if (CYLINDER_SEGMENTS > 3) {
            capIndicesRec(start, step + 1);
            capIndicesRec(start + step, step + 1);
        }
        if (CYLINDER_SEGMENTS > 5)
            capIndicesRec(start + 2 * step, CYLINDER_SEGMENTS - 2 * step + 1);
    }

    static std::vector<unsigned int> generateCylinderIndices()
    {
        std::vector<unsigned int> indices{};

        int i = 0;
        for (; i < CYLINDER_SEGMENTS-1; ++i) {
            indices.push_back(i); indices.push_back(i + CYLINDER_SEGMENTS); indices.push_back(i + 1);
            indices.push_back(i + 1); indices.push_back(i + CYLINDER_SEGMENTS); indices.push_back(i + CYLINDER_SEGMENTS + 1);
        }
        indices.push_back(i); indices.push_back(i + CYLINDER_SEGMENTS); indices.push_back(0);
        indices.push_back(0); indices.push_back(i + CYLINDER_SEGMENTS); indices.push_back(CYLINDER_SEGMENTS);

        capIndices(indices, CYLINDER_SEGMENTS*2);
        capIndices(indices, CYLINDER_SEGMENTS*3);

        return indices;
    }

    static std::vector<glm::vec2> generateTextureCoords()
    {
        std::vector<glm::vec2> texCoords{};
        for (int i = 0; i < CYLINDER_SEGMENTS*4; ++i) {
            const float angle = static_cast<float>(i) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);

            const float u = angle / (2.0f * static_cast<float>(M_PI));
            constexpr float v = CYLINDER_HEIGHT;

            texCoords.emplace_back(u, v);
        }
        return texCoords;
    }

    static std::vector<glm::vec3> generateNormals(const std::vector<glm::vec3> &vertices)
    {
        std::vector<glm::vec3> normals{};
        int i = 0;
        for (; i < CYLINDER_SEGMENTS*1; ++i) {
            const glm::vec3 vector1 = vertices[i] - glm::vec3(0, 1, 0);
            normals.emplace_back(vector1);
        }
        for (; i < CYLINDER_SEGMENTS*2; ++i) {
            const glm::vec3 vector2 = vertices[i] - glm::vec3(0, -1, 0);
            normals.emplace_back(vector2);
        }
        for (; i < CYLINDER_SEGMENTS*3; ++i) {
            normals.emplace_back(0,1,0);
        }
        for (; i < CYLINDER_SEGMENTS*4; ++i) {
            normals.emplace_back(0,-1,0);
        }
        std::ranges::copy(normals, normals.begin());
        return normals;
    }

    /**
     * @brief Creates a vertex array object (VAO) for a cylinder mesh.
     *
     * @return A shared pointer to a vertex array object containing the cylinder mesh data.
     */
    std::shared_ptr<NxVertexArray> NxRenderer3D::getCylinderVAO(const unsigned int nbSegment)
    {
        if (nbSegment < 3) {
            LOG(NEXO_WARN, "Cylinder segments must be at least 3, using default value of 8.");
            CYLINDER_SEGMENTS = 8;
        } else {
            CYLINDER_SEGMENTS = nbSegment;
        }
        const unsigned int nbVerticesCylinder = CYLINDER_SEGMENTS*4;
        static std::shared_ptr<NxVertexArray> cylinderVao = nullptr;
        if (cylinderVao)
            return cylinderVao;

        cylinderVao = createVertexArray();
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

        const std::vector<glm::vec3> vertices = generateCylinderVertices();
        const std::vector<glm::vec2> texCoords = generateTextureCoords();
        const std::vector<glm::vec3> normals = generateNormals(vertices);
        std::vector<unsigned int> indices = generateCylinderIndices();

        std::vector<NxVertex> vertexData(nbVerticesCylinder);
        for (unsigned int i = 0; i < nbVerticesCylinder; ++i) {
            vertexData[i].position = glm::vec4(vertices[i], 1.0f);
            vertexData[i].texCoord = texCoords[i];
            vertexData[i].normal = normals[i];
        }

        vertexBuffer->setData(vertexData.data(), vertexData.size() * sizeof(NxVertex));
        cylinderVao->addVertexBuffer(vertexBuffer);

        const auto indexBuffer = createIndexBuffer();
        indexBuffer->setData(indices.data(), indices.size());
        cylinderVao->setIndexBuffer(indexBuffer);

        return cylinderVao;
    }
}
