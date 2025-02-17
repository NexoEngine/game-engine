//// Shapes3D.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Header for the 3D shapes
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Render3D.hpp"
#include "renderer/RendererContext.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace nexo::components {

    struct Shape3D {
        virtual ~Shape3D() = default;

        virtual void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf) = 0;
    };

    struct Cube final : Shape3D {
        void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf) override
        {
            auto renderer3D = context->renderer3D;
            //TODO: Find a way to handle materials for cube and other basic primitives
            renderer3D.drawCube(transf.pos, transf.size, {1.0f, 0.0f, 0.0f, 1.0f});
        }
    };

    struct Mesh {
        std::string name;
        std::vector<renderer::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::shared_ptr<renderer::Texture2D> texture;
        std::optional<Material> material; // Not used yet
    };

    struct MeshNode {
        glm::mat4 transform;
        std::vector<Mesh> meshes;
        std::vector<std::shared_ptr<MeshNode>> children;

        void draw(renderer::Renderer3D &renderer3D, const glm::mat4 &parentTransform) const
        {
            glm::mat4 localTransform = parentTransform * transform;
            for (const auto &mesh: meshes)
            {
                //TODO: Implement a way to pass the transform directly to the shader
                std::vector<renderer::Vertex> transformedVertices = mesh.vertices;
                for (auto &vertex: transformedVertices)
                    vertex.position = glm::vec3(localTransform * glm::vec4(vertex.position, 1.0f));
                renderer3D.drawMesh(transformedVertices, mesh.indices, mesh.texture);
            }

            for (const auto &child: children)
                child->draw(renderer3D, localTransform);
        }
    };

    struct Model final : Shape3D {
        std::shared_ptr<MeshNode> root;
        static Material defaultMaterial;

        explicit Model(const std::shared_ptr<MeshNode> &rootNode) : root(rootNode) {};

        void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf) override
        {
            auto renderer3D = context->renderer3D;
            //TODO: Pass the material to the draw mesh function
            if (root)
            {
                //TODO: find a better way than recalculating each time, maybe cache the matrix in the component ?
                const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transf.pos);

                const glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(transf.rotation.x),
                                                        glm::vec3(1.0f, 0.0f, 0.0f));
                const glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(transf.rotation.y),
                                                        glm::vec3(0.0f, 1.0f, 0.0f));
                const glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(transf.rotation.z),
                                                        glm::vec3(0.0f, 0.0f, 1.0f));
                const glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

                const glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), transf.size);

                const glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scalingMatrix;

                root->draw(renderer3D, transformMatrix);
            }
        }
    };

}
