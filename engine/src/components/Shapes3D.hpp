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
#include "Transform.hpp"
#include "renderer/RendererContext.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <optional>

namespace nexo::components {

    struct Shape3D {
        virtual ~Shape3D() = default;

        virtual void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf, const Material &material, int entityID) = 0;
        virtual std::shared_ptr<Shape3D> clone() const = 0;
    };

    struct Cube final : Shape3D {
        void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf, const Material &material, int entityID) override
        {
            auto renderer3D = context->renderer3D;
            renderer3D.drawCube(transf.pos, transf.size, transf.quat, material, entityID);
        }

        std::shared_ptr<Shape3D> clone() const override {
            return std::make_shared<Cube>(*this);
        }
    };

    struct Mesh {
        std::string name;
        std::vector<renderer::Vertex> vertices;
        std::vector<unsigned int> indices;
        std::optional<Material> material;
    };

    struct MeshNode {
        glm::mat4 transform;
        std::vector<Mesh> meshes;
        std::vector<std::shared_ptr<MeshNode>> children;

        void draw(renderer::Renderer3D &renderer3D, const glm::mat4 &parentTransform, int entityID) const
        {
            glm::mat4 localTransform = parentTransform * transform;
            for (const auto &mesh: meshes)
            {
                //TODO: Implement a way to pass the transform directly to the shader
                std::vector<renderer::Vertex> transformedVertices = mesh.vertices;
                for (auto &vertex: transformedVertices)
                    vertex.position = glm::vec3(localTransform * glm::vec4(vertex.position, 1.0f));
                renderer3D.drawMesh(transformedVertices, mesh.indices, mesh.material->albedoTexture, entityID);
            }

            for (const auto &child: children)
                child->draw(renderer3D, localTransform, entityID);
        }

        std::shared_ptr<MeshNode> clone() const {
            auto newNode = std::make_shared<MeshNode>();
            newNode->transform = transform;
            newNode->meshes = meshes; // Assumes Mesh is copyable.
            for (const auto &child: children)
                newNode->children.push_back(child->clone());
            return newNode;
        }
    };

    struct Model final : Shape3D {
        std::shared_ptr<MeshNode> root;
        static Material defaultMaterial;

        explicit Model(const std::shared_ptr<MeshNode> &rootNode) : root(rootNode) {};

        // NOT WORKING ANYMORE
        void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf, const Material &material, int entityID) override
        {
            auto renderer3D = context->renderer3D;
            //TODO: Pass the material to the draw mesh function
            if (root)
            {
                //TODO: find a better way than recalculating each time, maybe cache the matrix in the component ?
                const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transf.pos);

                //const glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(transf.rotation.x),
                                                        //glm::vec3(1.0f, 0.0f, 0.0f));
                //const glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(transf.rotation.y),
                                                        //glm::vec3(0.0f, 1.0f, 0.0f));
                //const glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(transf.rotation.z),
                                                        //glm::vec3(0.0f, 0.0f, 1.0f));
                const glm::mat4 rotationMatrix = glm::toMat4(transf.quat);

                const glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), transf.size);

                const glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scalingMatrix;

                root->draw(renderer3D, transformMatrix, entityID);
            }
        }

        std::shared_ptr<Shape3D> clone() const override {
            return std::make_shared<Model>(root ? root->clone() : nullptr);
        }
    };

}
