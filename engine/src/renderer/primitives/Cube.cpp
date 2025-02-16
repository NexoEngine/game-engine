//// Cube.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Source file for the cube primitive
//
///////////////////////////////////////////////////////////////////////////////

#include "renderer/Renderer3D.hpp"
#include "renderer/RendererExceptions.hpp"

#include <algorithm>
#include <array>
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace nexo::renderer {

	// 8 unique vertices for a cube (one per corner)
	constexpr glm::vec3 cubePositions[8] = {
            {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}
        };

        constexpr unsigned int cubeIndices[36] = {
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            4, 5, 6, 6, 7, 4,
            // Bottom face
            0, 1, 5, 5, 4, 0,
            // Top face
            3, 2, 6, 6, 7, 3,
            // Left face
            0, 3, 7, 7, 4, 0,
            // Right face
            1, 2, 6, 6, 5, 1
        };

        static const glm::vec3 vertexNormals[8] = {
            glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)), // Vertex 0
            glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)),  // Vertex 1
            glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)),   // Vertex 2
            glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)),  // Vertex 3
            glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)),  // Vertex 4
            glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)),   // Vertex 5
            glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),    // Vertex 6
            glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f))    // Vertex 7
        };

        constexpr glm::vec2 textureCoords[4] = {
            {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 1.0f}, {0.0f, 1.0f}
        };


    static void genCubeMesh(std::array<glm::vec3, 36> &vertices, std::array<glm::vec2, 36> &texCoords, std::array<glm::vec3, 36> &normals)
    {
    	float x = 0.5f;
     	float y = 0.5f;
     	float z = 0.5f;

      	glm::vec3 a0 = glm::vec3(+x, +y, +z); // 0.5, 0.5, 0.5 -> Front face en haut a droite
		glm::vec3 a1 = glm::vec3(-x, +y, +z); // -0.5, 0.5, 0.5 -> Front face en haut a gauche
		glm::vec3 a2 = glm::vec3(-x, -y, +z); // -0.5, -0.5, 0.5 -> Front face en bas a gauche
		glm::vec3 a3 = glm::vec3(+x, -y, +z); // 0.5, -0.5, 0.5 -> Front face en bas a droite
		glm::vec3 a4 = glm::vec3(+x, +y, -z); // 0.5, 0.5, -0.5 -> Back face en haut a droite
		glm::vec3 a5 = glm::vec3(-x, +y, -z); // -0.5, 0.5, -0.5 -> Back face en haut a gauche
		glm::vec3 a6 = glm::vec3(-x, -y, -z); // -0.5, -0.5, -0.5 -> Back face en bas a gauche
		glm::vec3 a7 = glm::vec3(+x, -y, -z); // 0.5, -0.5, -0.5 -> Back face en bas a droite

		glm::vec3 verts[] = {
				a1, a2, a3, a3, a0, a1,
				a2, a6, a7, a7, a3, a2,
				a6, a5, a4, a4, a7, a6,
				a5, a1, a0, a0, a4, a5,
				a0, a3, a7, a7, a4, a0,
				a5, a6, a2, a2, a1, a5
		};

		std::copy(std::begin(verts), std::end(verts), vertices.begin());

		glm::vec2 texc[] = {
			glm::vec2(0,1), glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1),
			glm::vec2(0,1), glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1),
			glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1), glm::vec2(0,1), glm::vec2(0,0), glm::vec2(1,0),
			glm::vec2(0,1), glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1),
			glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,1), glm::vec2(1,1), glm::vec2(0,1), glm::vec2(0,0),
			glm::vec2(1,1), glm::vec2(0,1), glm::vec2(0,0), glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,1),
		};

		std::copy(std::begin(texc), std::end(texc), texCoords.begin());
		glm::vec3 norm[36];

		std::cout << "PRINTING NORMALS" << std::endl;
		for (int i = 0; i < 36; i += 3)
		{
			glm::vec3 normal = glm::normalize(
				glm::cross(
					glm::vec3(verts[i + 1]) - glm::vec3(verts[i]),
					glm::vec3(verts[i + 2]) - glm::vec3(verts[i])));

			norm[i] = normal;
			norm[i + 1] = normal;
			norm[i + 2] = normal;
			std::cout << "Normal for face : " << i / 3 << " { " << norm[i].x << ", " << norm[i].y << ", " << norm[i].z << " }" << std::endl;
		}

		std::copy(std::begin(norm), std::end(norm), normals.begin());
    }

	void Renderer3D::drawCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec4 &color, int entityID) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");

        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                    glm::scale(glm::mat4(1.0f), size);

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = color;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 36> verts;
        std::array<glm::vec2, 36> texCoords;
        std::array<glm::vec3, 36> normals;
        std::array<unsigned int, 36> indices;

        genCubeMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 36; ++i)
            indices[i] = i;

        // Vertex data
        auto vertexOffset = static_cast<unsigned int>(m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data());
        for (unsigned int i = 0; i < 36; ++i)
            {
                m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
                m_storage->vertexBufferPtr->texCoord = texCoords[i];
                m_storage->vertexBufferPtr->normal = normals[i];
                //m_storage->vertexBufferPtr->tangent = cubeTangents[i];
                //m_storage->vertexBufferPtr->bitangent = cubeBitangents[i];
                m_storage->vertexBufferPtr->entityID = entityID;
                m_storage->vertexBufferPtr++;
            }

        // Index data
        std::ranges::for_each(indices, [this, vertexOffset](unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });


        // Update stats
        m_storage->stats.cubeCount++;
        const auto vertexDataSize = static_cast<unsigned int>(
            reinterpret_cast<std::byte*>(m_storage->vertexBufferPtr) -
            reinterpret_cast<std::byte*>(m_storage->vertexBufferBase.data())
        );


        m_storage->vertexBuffer->setData(m_storage->vertexBufferBase.data(), vertexDataSize);

        m_storage->indexBuffer->setData(m_storage->indexBufferBase.data(), m_storage->indexCount);
        flushAndReset();
    }

    void Renderer3D::drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3 &rotation, const glm::vec4& color, int entityID) const
    {
	    if (!m_renderingScene)
	        THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");

		glm::quat rotationQuat = glm::quat(glm::radians(rotation));
		glm::mat4 rotationMat = glm::toMat4(rotationQuat);

	    const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
									rotationMat *
                                    glm::scale(glm::mat4(1.0f), size);

        if ((m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data()) + 8 > m_storage->maxVertices ||
            m_storage->indexCount + 36 > m_storage->maxIndices)
        {
            flushAndReset();
        }

        // Vertex data
        auto vertexOffset = static_cast<unsigned int>(m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data());
        for (unsigned int i = 0; i < 8; ++i)
        {
            m_storage->vertexBufferPtr->position = transform * glm::vec4(cubePositions[i], 1.0f);
            //m_storage->vertexBufferPtr->color = color;
            //m_storage->vertexBufferPtr->texCoord = textureCoords[i % 4];
            //m_storage->vertexBufferPtr->texIndex = 0.0f; // White texture
            //m_storage->vertexBufferPtr->normal = vertexNormals[i];
            m_storage->vertexBufferPtr->entityID = entityID;
            //m_storage->vertexBufferPtr->transformIndex = transformIndex;
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(cubeIndices, [this, vertexOffset](unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index + vertexOffset;
        });
    }

    void Renderer3D::drawCube(const glm::mat4& transform, const glm::vec4& color, int entityID) const
    {
	    if (!m_renderingScene)
	        THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");

        if ((m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data()) + 8 > m_storage->maxVertices ||
            m_storage->indexCount + 36 > m_storage->maxIndices)
        {
            flushAndReset();
        }

        // Vertex data
        auto vertexOffset = static_cast<unsigned int>(m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data());
        for (unsigned int i = 0; i < 8; ++i)
        {
            m_storage->vertexBufferPtr->position = transform * glm::vec4(cubePositions[i], 1.0f);
            //m_storage->vertexBufferPtr->color = color;
            //m_storage->vertexBufferPtr->texCoord = textureCoords[i % 4];
            //m_storage->vertexBufferPtr->texIndex = 0.0f; // White texture
            ///m_storage->vertexBufferPtr->normal = vertexNormals[i];
            m_storage->vertexBufferPtr->entityID = entityID;
           // m_storage->vertexBufferPtr->transformIndex = transformIndex;
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(cubeIndices, [this, vertexOffset](unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index + vertexOffset;
        });
    }

    //TODO: implement material
    void Renderer3D::drawCube(const glm::vec3& position, const glm::vec3& size, const components::Material &material, int entityID) const
    {
	    if (!m_renderingScene)
	        THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");
    }

    //TODO: implement material
    void Renderer3D::drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, const components::Material &material, int entityID) const
    {
	    if (!m_renderingScene)
	        THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");
    }

    //TODO: implement material
    void Renderer3D::drawCube(const glm::mat4& transform, const components::Material &material, int entityID) const
    {
	    if (!m_renderingScene)
	        THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");
    }
}
