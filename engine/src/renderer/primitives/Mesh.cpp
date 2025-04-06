//// Mesh.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Source file for the mesh primitive
//
///////////////////////////////////////////////////////////////////////////////

#include "Logger.hpp"
#include "renderer/Renderer3D.hpp"
#include "renderer/RendererExceptions.hpp"

namespace nexo::renderer {

	void Renderer3D::drawMesh([[maybe_unused]] const std::vector<Vertex>& vertices, [[maybe_unused]] const std::vector<unsigned int>& indices, [[maybe_unused]] const glm::vec3& position, [[maybe_unused]] const glm::vec3& size, [[maybe_unused]] const components::Material& material, [[maybe_unused]] int entityID) const
	{

	}

    void Renderer3D::drawMesh([[maybe_unused]] const std::vector<Vertex>& vertices, [[maybe_unused]] const std::vector<unsigned int>& indices, [[maybe_unused]] const glm::vec3& position, [[maybe_unused]] const glm::vec3& rotation, [[maybe_unused]] const glm::vec3& size, [[maybe_unused]] const components::Material& material, [[maybe_unused]] int entityID) const
    {

    }

    void Renderer3D::drawMesh([[maybe_unused]] const std::vector<Vertex>& vertices, [[maybe_unused]] const std::vector<unsigned int>& indices, [[maybe_unused]] const glm::mat4& transform, [[maybe_unused]] const components::Material& material, [[maybe_unused]] int entityID) const
    {

    }

    void Renderer3D::drawMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
                              [[maybe_unused]] const std::shared_ptr<Texture2D> &texture, int entityID) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        if ((m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data()) + vertices.size() > m_storage->maxVertices ||
            m_storage->indexCount + indices.size() > m_storage->maxIndices)
        {
            //TODO: Implement the batch rendering for meshes
            LOG(NEXO_INFO, "Max number attained");
        }

        auto vertexOffset = static_cast<unsigned int>(m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data());


        for (const auto &vertex: vertices)
        {
            m_storage->vertexBufferPtr->position = vertex.position;
            //m_storage->vertexBufferPtr->color = {1.0f, 1.0f, 1.0f, 1.0f};
            m_storage->vertexBufferPtr->texCoord = vertex.texCoord;
            //m_storage->vertexBufferPtr->texIndex = textureIndex;
            m_storage->vertexBufferPtr->normal = vertex.normal;
            m_storage->vertexBufferPtr->entityID = entityID;
            m_storage->vertexBufferPtr++;
        }

        for (const auto &index : indices)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index + vertexOffset;
        }
    }
}
