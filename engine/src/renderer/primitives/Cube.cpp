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

    constexpr glm::vec2 textureCoords[4] = {
        {0.0f, 0.0f}, {1.0f, 0.0f},
        {1.0f, 1.0f}, {0.0f, 1.0f}
    };


	/**
	* @brief Generates the vertex, texture coordinate, and normal data for a cube mesh.
	*
	* Fills the provided arrays with 36 vertices, texture coordinates, and normals for a cube.
	*
	* @param vertices Array to store generated vertex positions.
	* @param texCoords Array to store generated texture coordinates.
	* @param normals Array to store generated normals.
	*/
    static void genCubeMesh(std::array<glm::vec3, 36> &vertices, std::array<glm::vec2, 36> &texCoords, std::array<glm::vec3, 36> &normals)
    {
    	float x = 0.5f;
     	float y = 0.5f;
     	float z = 0.5f;

      	glm::vec3 a0 = {+x, +y, +z}; // 0.5, 0.5, 0.5 -> Front face top right
		glm::vec3 a1 = {-x, +y, +z}; // -0.5, 0.5, 0.5 -> Front face top left
		glm::vec3 a2 = {-x, -y, +z}; // -0.5, -0.5, 0.5 -> Front face bottom left
		glm::vec3 a3 = {+x, -y, +z}; // 0.5, -0.5, 0.5 -> Front face bottom right
		glm::vec3 a4 = {+x, +y, -z}; // 0.5, 0.5, -0.5 -> Back face top right
		glm::vec3 a5 = {-x, +y, -z}; // -0.5, 0.5, -0.5 -> Back face top left
		glm::vec3 a6 = {-x, -y, -z}; // -0.5, -0.5, -0.5 -> Back face bottom left
		glm::vec3 a7 = {+x, -y, -z}; // 0.5, -0.5, -0.5 -> Back face bottom right

		glm::vec3 verts[] = {
				a1, a2, a3, a3, a0, a1,
				a2, a6, a7, a7, a3, a2,
				a6, a5, a4, a4, a7, a6,
				a5, a1, a0, a0, a4, a5,
				a0, a3, a7, a7, a4, a0,
				a5, a6, a2, a2, a1, a5
		};

		std::ranges::copy(verts, vertices.begin());

		glm::vec2 texc[] = {
			glm::vec2(0,1), glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1),
			glm::vec2(0,1), glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1),
			glm::vec2(0,1), glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1),
			glm::vec2(0,1), glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1),
			glm::vec2(0,1), glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1),
			glm::vec2(0,1), glm::vec2(0,0), glm::vec2(1,0), glm::vec2(1,0), glm::vec2(1,1), glm::vec2(0,1),
		};

		std::ranges::copy(texc, texCoords.begin());
		glm::vec3 norm[36];

		for (int i = 0; i < 36; i += 3)
		{
			glm::vec3 normal = glm::normalize(
				glm::cross(
					glm::vec3(verts[i + 1]) - glm::vec3(verts[i]),
					glm::vec3(verts[i + 2]) - glm::vec3(verts[i])));

			norm[i] = normal;
			norm[i + 1] = normal;
			norm[i + 2] = normal;
		}

		std::ranges::copy(norm, normals.begin());
    }

	void NxRenderer3D::drawCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec4 &color, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        }

        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                    glm::scale(glm::mat4(1.0f), size);

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
        mat.albedoColor = color;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 36> verts{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};
        std::array<unsigned int, 36> indices{};

        genCubeMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 36; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }

    void NxRenderer3D::drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3 &rotation, const glm::vec4& color, const int entityID) const
    {
	    if (!m_renderingScene)
	    {
	        THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");
	    }

		const glm::quat rotationQuat = glm::radians(rotation);
		const glm::mat4 rotationMat = glm::toMat4(rotationQuat);

	    const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
									rotationMat *
                                    glm::scale(glm::mat4(1.0f), size);

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
        mat.albedoColor = color;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 36> verts{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};
        std::array<unsigned int, 36> indices{};

        genCubeMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 36; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }

    void NxRenderer3D::drawCube(const glm::mat4& transform, const glm::vec4& color, const int entityID) const
    {
	    if (!m_renderingScene)
	    {
	        THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");
	    }


		m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
        mat.albedoColor = color;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 36> verts{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};
        std::array<unsigned int, 36> indices{};

        genCubeMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 36; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }

    void NxRenderer3D::drawCube(const glm::vec3& position, const glm::vec3& size, const NxMaterial& material, const int entityID) const
    {
	    if (!m_renderingScene)
	    {
	        THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");
	    }

		// Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                    glm::scale(glm::mat4(1.0f), size);

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 36> verts{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};
        std::array<unsigned int, 36> indices{};

        genCubeMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 36; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });


        // Update stats
        m_storage->stats.cubeCount++;
    }

    void NxRenderer3D::drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, const NxMaterial& material, const int entityID) const
    {
	    if (!m_renderingScene)
	    {
	        THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");
	    }

		const glm::quat rotationQuat = glm::radians(rotation);
		const glm::mat4 rotationMat = glm::toMat4(rotationQuat);
		// Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
        							rotationMat *
                                    glm::scale(glm::mat4(1.0f), size);

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 36> verts{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};
        std::array<unsigned int, 36> indices{};

        genCubeMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 36; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });


        // Update stats
        m_storage->stats.cubeCount++;
    }

    void NxRenderer3D::drawCube(const glm::vec3 &position, const glm::vec3 &size, const glm::quat &rotation, const NxMaterial& material, const int entityID) const
    {
	    if (!m_renderingScene)
	    {
	        THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");

	    }

		const glm::mat4 rotationMat = glm::toMat4(rotation);
		// Transform matrix
       const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
       							rotationMat *
                                   glm::scale(glm::mat4(1.0f), size);

       m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

       NxIndexedMaterial mat;
       mat.albedoColor = material.albedoColor;
       mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
       mat.specularColor = material.specularColor;
       mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
       setMaterialUniforms(mat);

       std::array<glm::vec3, 36> verts{};
       std::array<glm::vec2, 36> texCoords{};
       std::array<glm::vec3, 36> normals{};
       std::array<unsigned int, 36> indices{};

       genCubeMesh(verts, texCoords, normals);
       for (unsigned int i = 0; i < 36; ++i)
           indices[i] = i;

       // Vertex data
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
       std::ranges::for_each(indices, [this](const unsigned int index) {
           m_storage->indexBufferBase[m_storage->indexCount++] = index;
       });


       // Update stats
       m_storage->stats.cubeCount++;
    }

    void NxRenderer3D::drawCube(const glm::mat4& transform, const NxMaterial& material, const int entityID) const
    {
	    if (!m_renderingScene)
	    {
	        THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");
	    }

	    m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 36> verts{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};
        std::array<unsigned int, 36> indices{};

        genCubeMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 36; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });


        // Update stats
        m_storage->stats.cubeCount++;
    }
}
