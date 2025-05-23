//// Sphere.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzz  zzzzz
//  zzz zzz     zzz  zzzz               zzz  zzzz       zzzz          zzzz
//  zzz   zzz   zzz  zzzzzzzzz             zzzz        zzzz            zzzz
//  zzz    zzz  zzz  zzzz               zzzz  zzzz      zzzz          zzzz
//  zzz      zz zzz  zzzzzzzzzzzzz    zzzz       zzzz      zzzzz  zzzzz
//
//  Author:      Marie Giacomel
//  Date:        06/05/2025
//  Description: Source file for the sphere primitive
//
///////////////////////////////////////////////////////////////////////////////

#include "renderer/Renderer3D.hpp"
#include "renderer/RendererExceptions.hpp"

#include <algorithm>
#include <array>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923 // π/2
#endif
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <Logger.hpp>
#include <map>
#include <set>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace nexo::renderer
{
    unsigned int VERTEX_NUMBER = 12;
    // Number of vertex for the sphere min 12 then VERTEX_NUMBER += (old) VERTEX_NUMBER + (old) nb of faces - 2

    static void normalizeVertices(std::vector<glm::vec3>& vertices)
    {
        for (auto& vertex : vertices)
        {
            vertex = glm::normalize(vertex);
        }
    }

    static std::vector<glm::vec3> generateSphereVertices()
    {
        std::vector<glm::vec3> vertices{};
        const float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
        float a = 1.0f;
        float b = 1.0f / phi;

        vertices.emplace_back(0, b, -a); // 0
        vertices.emplace_back(b, a, 0); // 1
        vertices.emplace_back(-b, a, 0); // 2
        vertices.emplace_back(0, b, a); // 3
        vertices.emplace_back(0, -b, a); // 4
        vertices.emplace_back(-a, 0, b); // 5
        vertices.emplace_back(0, -b, -a); // 6
        vertices.emplace_back(a, 0, -b); // 7
        vertices.emplace_back(a, 0, b); // 8
        vertices.emplace_back(-a, 0, -b); // 9
        vertices.emplace_back(b, -a, 0); // 10
        vertices.emplace_back(-b, -a, 0); // 11

        // Normalize the vertices to create a unit sphere
        normalizeVertices(vertices);

        return vertices;
    }

    // unique vertices for a sphere
    const std::vector<glm::vec3> spherePositions = generateSphereVertices();

    static std::vector<unsigned int> generateSphereIndices()
    {
        std::vector<unsigned int> indices{};

        indices.push_back(2);
        indices.push_back(1);
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(5);
        indices.push_back(4);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(8);
        indices.push_back(3);
        indices.push_back(7);
        indices.push_back(6);
        indices.push_back(0);
        indices.push_back(6);
        indices.push_back(9);
        indices.push_back(0);
        indices.push_back(11);
        indices.push_back(10);
        indices.push_back(4);
        indices.push_back(10);
        indices.push_back(11);
        indices.push_back(6);
        indices.push_back(9);
        indices.push_back(5);
        indices.push_back(2);
        indices.push_back(5);
        indices.push_back(9);
        indices.push_back(11);
        indices.push_back(8);
        indices.push_back(7);
        indices.push_back(1);
        indices.push_back(7);
        indices.push_back(8);
        indices.push_back(10);
        indices.push_back(2);
        indices.push_back(5);
        indices.push_back(3);
        indices.push_back(8);
        indices.push_back(1);
        indices.push_back(3);
        indices.push_back(9);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(7);
        indices.push_back(0);
        indices.push_back(11);
        indices.push_back(9);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(10);
        indices.push_back(6);
        indices.push_back(5);
        indices.push_back(11);
        indices.push_back(4);
        indices.push_back(10);
        indices.push_back(8);
        indices.push_back(4);

        return indices;
    }

    struct Vec3Comparator
    {
        bool operator()(const glm::vec3& a, const glm::vec3& b) const
        {
            if (a.x != b.x) return a.x < b.x;
            if (a.y != b.y) return a.y < b.y;
            return a.z < b.z;
        }
    };

    static void loopSubdivision(std::vector<unsigned int>& indices, std::vector<glm::vec3>& vertices,
                                const unsigned int nbSubdivisions)
    {
        for (unsigned int i = 0; i < nbSubdivisions; ++i)
        {
            std::set<std::pair<unsigned int, unsigned int>> edges;
            std::vector<unsigned int> newIndices{};
            std::map<glm::vec3, unsigned int, Vec3Comparator> newVertices{};

            for (int j = 0; j < indices.size(); j += 3)
            {
                const unsigned int v1 = indices[j];
                const unsigned int v2 = indices[j + 1];
                const unsigned int v3 = indices[j + 2];

                // Add edges to the set
                edges.insert({std::min(v1, v2), std::max(v1, v2)});
                edges.insert({std::min(v2, v3), std::max(v2, v3)});
                edges.insert({std::min(v1, v3), std::max(v1, v3)});

                const glm::vec3 m1_pos = (vertices[v1] + vertices[v2]) / 2.0f;
                const glm::vec3 m2_pos = (vertices[v2] + vertices[v3]) / 2.0f;
                const glm::vec3 m3_pos = (vertices[v1] + vertices[v3]) / 2.0f;

                if (!newVertices.contains(m1_pos))
                {
                    vertices.emplace_back(m1_pos);
                    newVertices.insert({m1_pos, vertices.size() - 1});
                }
                if (!newVertices.contains(m2_pos))
                {
                    vertices.emplace_back(m2_pos);
                    newVertices.insert({m2_pos, vertices.size() - 1});
                }
                if (!newVertices.contains(m3_pos))
                {
                    vertices.emplace_back(m3_pos);
                    newVertices.insert({m3_pos, vertices.size() - 1});
                }

                auto m1 = newVertices[m1_pos];
                auto m2 = newVertices[m2_pos];
                auto m3 = newVertices[m3_pos];

                // Triangle 1: v1, m1, m3
                newIndices.push_back(v1);
                newIndices.push_back(m1);
                newIndices.push_back(m3);
                // Triangle 2: m1, v2, m2
                newIndices.push_back(m1);
                newIndices.push_back(v2);
                newIndices.push_back(m2);
                // Triangle 3: m3, m2, v3
                newIndices.push_back(m3);
                newIndices.push_back(m2);
                newIndices.push_back(v3);
                // Triangle 4: m1, m2, m3
                newIndices.push_back(m1);
                newIndices.push_back(m2);
                newIndices.push_back(m3);
            }

            normalizeVertices(vertices);
            indices = std::move(newIndices);
        }
        VERTEX_NUMBER = vertices.size();
    }

    static std::vector<glm::vec2> generateTextureCoords(const std::vector<glm::vec3>& vertices)
    {
        std::vector<glm::vec2> texCoords{};

        for (int i = 0; i < VERTEX_NUMBER; ++i)
        {
            const glm::vec3 p = vertices[i];

            float u = (atan2(p.z, p.x) + M_PI) / (2 * M_PI);
            float v = acos(p.y) / M_PI;
            // v = 1.0f - v; // uncomment if reversed

            texCoords.emplace_back(u, v);
        }
        return texCoords;
    }


    /**
    * @brief Generates the vertex, texture coordinate, and normal data for a sphere mesh.
    *
    * Fills the provided arrays with 36 vertices, texture coordinates, and normals for a sphere.
    *
    * @param vertices Array to store generated vertex positions.
    * @param texCoords Array to store generated texture coordinates.
    * @param normals Array to store generated normals.
    * @param indices Array to store generated indices.
    * @param nbSubdivisions Number of subdivisions for the sphere mesh.
    */
    static void genSphereMesh(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& texCoords,
                              std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices,
                              const unsigned int nbSubdivisions)
    {
        vertices = generateSphereVertices();
        indices = generateSphereIndices();

        loopSubdivision(indices, vertices, nbSubdivisions);

        for (auto vec : vertices)
        {
            const glm::vec3 vector1 = vec - glm::vec3(0, 0, 0);
            normals.emplace_back(vector1);
        }
        texCoords = generateTextureCoords(vertices);

        std::ranges::copy(normals, normals.begin());
    }

    void Renderer3D::drawSphere(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color,
                                const unsigned int nbSubdivisions, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }

        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = color;
        setMaterialUniforms(mat);

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices{};

        genSphereMesh(verts, texCoords, normals, indices, nbSubdivisions);

        // Vertex data
        for (unsigned int i = 0; i < VERTEX_NUMBER; ++i)
        {
            m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
            m_storage->vertexBufferPtr->texCoord = texCoords[i];
            m_storage->vertexBufferPtr->normal = normals[i];
            m_storage->vertexBufferPtr->entityID = entityID;
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawSphere(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
                                const glm::vec4& color, const unsigned int nbSubdivisions, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }

        const glm::quat rotationQuat = glm::radians(rotation);
        const glm::mat4 rotationMat = glm::toMat4(rotationQuat);

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            rotationMat *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = color;
        setMaterialUniforms(mat);

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices{};

        genSphereMesh(verts, texCoords, normals, indices, nbSubdivisions);

        // Vertex data
        for (unsigned int i = 0; i < VERTEX_NUMBER; ++i)
        {
            m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
            m_storage->vertexBufferPtr->texCoord = texCoords[i];
            m_storage->vertexBufferPtr->normal = normals[i];
            m_storage->vertexBufferPtr->entityID = entityID;
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawSphere(const glm::mat4& transform, const glm::vec4& color, const unsigned int nbSubdivisions,
                                const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }


        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = color;
        setMaterialUniforms(mat);

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices{};

        genSphereMesh(verts, texCoords, normals, indices, nbSubdivisions);

        // Vertex data
        for (unsigned int i = 0; i < VERTEX_NUMBER; ++i)
        {
            m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
            m_storage->vertexBufferPtr->texCoord = texCoords[i];
            m_storage->vertexBufferPtr->normal = normals[i];
            m_storage->vertexBufferPtr->entityID = entityID;
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawSphere(const glm::vec3& position, const glm::vec3& size, const components::Material& material,
                                const unsigned int nbSubdivisions, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }

        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices{};

        genSphereMesh(verts, texCoords, normals, indices, nbSubdivisions);

        // Vertex data
        for (unsigned int i = 0; i < VERTEX_NUMBER; ++i)
        {
            m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
            m_storage->vertexBufferPtr->texCoord = texCoords[i];
            m_storage->vertexBufferPtr->normal = normals[i];
            m_storage->vertexBufferPtr->entityID = entityID;
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });


        // Update stats
        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawSphere(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
                                const components::Material& material, const unsigned int nbSubdivisions,
                                const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }

        const glm::quat rotationQuat = glm::radians(rotation);
        const glm::mat4 rotationMat = glm::toMat4(rotationQuat);
        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            rotationMat *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices{};

        genSphereMesh(verts, texCoords, normals, indices, nbSubdivisions);

        // Vertex data
        for (unsigned int i = 0; i < VERTEX_NUMBER; ++i)
        {
            m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
            m_storage->vertexBufferPtr->texCoord = texCoords[i];
            m_storage->vertexBufferPtr->normal = normals[i];
            m_storage->vertexBufferPtr->entityID = entityID;
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });


        // Update stats
        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawSphere(const glm::vec3& position, const glm::vec3& size, const glm::quat& rotation,
                                const components::Material& material, const unsigned int nbSubdivisions,
                                const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }

        const glm::mat4 rotationMat = glm::toMat4(rotation);
        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            rotationMat *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices{};

        genSphereMesh(verts, texCoords, normals, indices, nbSubdivisions);

        // Vertex data
        for (unsigned int i = 0; i < VERTEX_NUMBER; ++i)
        {
            m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
            // m_storage->vertexBufferPtr->texCoord = texCoords[i];
            // m_storage->vertexBufferPtr->normal = normals[i];
            m_storage->vertexBufferPtr->entityID = entityID;
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });


        // Update stats
        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawSphere(const glm::mat4& transform, const components::Material& material,
                                const unsigned int nbSubdivisions, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices{};

        genSphereMesh(verts, texCoords, normals, indices, nbSubdivisions);

        // Vertex data
        for (unsigned int i = 0; i < VERTEX_NUMBER; ++i)
        {
            m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
            m_storage->vertexBufferPtr->texCoord = texCoords[i];
            m_storage->vertexBufferPtr->normal = normals[i];
            m_storage->vertexBufferPtr->entityID = entityID;
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }
}
