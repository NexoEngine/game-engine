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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Render3D.hpp"
#include "Transform.hpp"
#include "renderer/RendererContext.hpp"
#include "assets/Assets/Material/Material.hpp"

namespace nexo::components {

    struct Shape3D {
        virtual ~Shape3D() = default;

        virtual void draw(std::shared_ptr<renderer::NxRendererContext> &context, const TransformComponent &transf, const Material &material, int entityID) = 0;
        [[nodiscard]] virtual std::shared_ptr<Shape3D> clone() const = 0;
    };

    struct Cube final : Shape3D {
        void draw(std::shared_ptr<renderer::NxRendererContext> &context, const TransformComponent &transf, const Material &material, const int entityID) override
        {
            const auto renderer3D = context->renderer3D;

            // lock all textures
            auto albedoTextureAsset = material.albedoTexture.lock();
            auto normalMapAsset = material.normalMap.lock();
            auto metallicMapAsset = material.metallicMap.lock();
            auto roughnessMapAsset = material.roughnessMap.lock();
            auto emissiveMapAsset = material.emissiveMap.lock();


            renderer::NxMaterial inputMaterial = {
                .albedoColor = material.albedoColor,
                .specularColor = material.specularColor,
                .emissiveColor = material.emissiveColor,
                .roughness = material.roughness,
                .metallic = material.metallic,
                .opacity = material.opacity,
                .albedoTexture = albedoTextureAsset && albedoTextureAsset->isLoaded() ? albedoTextureAsset->getData()->texture : nullptr,
                .normalMap = normalMapAsset && normalMapAsset->isLoaded() ? normalMapAsset->getData()->texture : nullptr,
                .metallicMap = metallicMapAsset && metallicMapAsset->isLoaded() ? metallicMapAsset->getData()->texture : nullptr,
                .roughnessMap = roughnessMapAsset && roughnessMapAsset->isLoaded() ? roughnessMapAsset->getData()->texture : nullptr,
                .emissiveMap = emissiveMapAsset && emissiveMapAsset->isLoaded() ? emissiveMapAsset->getData()->texture : nullptr,
                .shader = material.shader
            };

            renderer3D.drawCube(transf.pos, transf.size, transf.quat, inputMaterial, entityID);
        }

        [[nodiscard]] std::shared_ptr<Shape3D> clone() const override {
            return std::make_shared<Cube>(*this);
        }
    };

    struct Tetrahedron final : Shape3D {
        void draw(std::shared_ptr<renderer::NxRendererContext> &context, const TransformComponent &transf, const Material &material, const int entityID) override
        {
            const auto renderer3D = context->renderer3D;
            //TODO: Find a way to handle materials for tetrahedron
            renderer3D.drawTetrahedron(transf.pos, transf.size, material.albedoColor, entityID);
        }

        [[nodiscard]] std::shared_ptr<Shape3D> clone() const override {
            return std::make_shared<Tetrahedron>(*this);
        }
    };

    struct Pyramid final : Shape3D {
        void draw(std::shared_ptr<renderer::NxRendererContext> &context, const TransformComponent &transf, const Material &material, const int entityID) override
        {
            const auto renderer3D = context->renderer3D;
            //TODO: Find a way to handle materials for pyramid
            renderer3D.drawPyramid(transf.pos, transf.size, material.albedoColor, entityID);
        }

        [[nodiscard]] std::shared_ptr<Shape3D> clone() const override {
            return std::make_shared<Pyramid>(*this);
        }
    };

    struct Cylinder final : Shape3D {
        void draw(std::shared_ptr<renderer::NxRendererContext> &context, const TransformComponent &transf, const Material &material, const int entityID) override
        {
            const auto renderer3D = context->renderer3D;
            //TODO: Find a way to handle materials for cylinder
            renderer3D.drawCylinder(transf.pos, transf.size, material.albedoColor, entityID);
        }

        [[nodiscard]] std::shared_ptr<Shape3D> clone() const override {
            return std::make_shared<Cylinder>(*this);
        }
    };

    struct Sphere final : Shape3D {
        void draw(std::shared_ptr<renderer::NxRendererContext> &context, const TransformComponent &transf, const Material &material, const int entityID) override
        {
            const auto renderer3D = context->renderer3D;
            //TODO: Find a way to handle materials for sphere
            renderer3D.drawSphere(transf.pos, transf.size, material.albedoColor, entityID);
        }

        [[nodiscard]] std::shared_ptr<Shape3D> clone() const override {
            return std::make_shared<Sphere>(*this);
        }
    };

    struct Mesh {
        std::string name;
        std::vector<renderer::NxVertex> vertices;
        std::vector<unsigned int> indices;
        assets::AssetRef<assets::Material> material;
    };

    struct MeshNode {
        glm::mat4 transform{};
        std::vector<Mesh> meshes;
        std::vector<std::shared_ptr<MeshNode>> children;

        void draw(renderer::NxRenderer3D &renderer3D, const glm::mat4 &parentTransform, const int entityID) const
        {
            const glm::mat4 localTransform = parentTransform * transform;
            for (const auto &mesh : meshes)
            {
                //TODO: Implement a way to pass the transform directly to the shader
                std::vector<renderer::NxVertex> transformedVertices = mesh.vertices;
                for (auto &vertex: transformedVertices)
                    vertex.position = glm::vec3(localTransform * glm::vec4(vertex.position, 1.0f));
                {
                    const auto meshMaterialAsset = mesh.material.lock();
                    const auto albedoTextureAsset = meshMaterialAsset && meshMaterialAsset->isLoaded() ? meshMaterialAsset->getData()->albedoTexture.lock() : nullptr;

                    const auto albedoTexture = albedoTextureAsset && albedoTextureAsset->isLoaded() ? albedoTextureAsset->getData()->texture : nullptr;
                    renderer3D.drawMesh(transformedVertices, mesh.indices, albedoTexture, entityID);
                }
            }

            for (const auto &child: children)
                child->draw(renderer3D, localTransform, entityID);
        }

        [[nodiscard]] std::shared_ptr<MeshNode> clone() const {
            auto newNode = std::make_shared<MeshNode>();
            newNode->transform = transform;
            newNode->meshes = meshes;
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
        void draw(std::shared_ptr<renderer::NxRendererContext> &context, const TransformComponent &transf, [[maybe_unused]] const Material &material, const int entityID) override
        {
            auto renderer3D = context->renderer3D;
            //TODO: Pass the material to the draw mesh function
            if (root)
            {
                //TODO: find a better way than recalculating each time, maybe cache the matrix in the component ?
                const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transf.pos);

                const glm::mat4 rotationMatrix = glm::toMat4(transf.quat);

                const glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), transf.size);

                const glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scalingMatrix;

                root->draw(renderer3D, transformMatrix, entityID);
            }
        }

        [[nodiscard]] std::shared_ptr<Shape3D> clone() const override {
            return std::make_shared<Model>(root ? root->clone() : nullptr);
        }
    };

    struct BillBoard final : Shape3D {
        void draw(std::shared_ptr<renderer::NxRendererContext> &context, const TransformComponent &transf, const Material &material, const int entityID) override
        {
            const auto renderer3D = context->renderer3D;

            // lock all textures
            auto albedoTextureAsset = material.albedoTexture.lock();
            auto normalMapAsset = material.normalMap.lock();
            auto metallicMapAsset = material.metallicMap.lock();
            auto roughnessMapAsset = material.roughnessMap.lock();
            auto emissiveMapAsset = material.emissiveMap.lock();


            renderer::NxMaterial inputMaterial = {
                .albedoColor = material.albedoColor,
                .specularColor = material.specularColor,
                .emissiveColor = material.emissiveColor,
                .roughness = material.roughness,
                .metallic = material.metallic,
                .opacity = material.opacity,
                .albedoTexture = albedoTextureAsset && albedoTextureAsset->isLoaded() ? albedoTextureAsset->getData()->texture : nullptr,
                .normalMap = normalMapAsset && normalMapAsset->isLoaded() ? normalMapAsset->getData()->texture : nullptr,
                .metallicMap = metallicMapAsset && metallicMapAsset->isLoaded() ? metallicMapAsset->getData()->texture : nullptr,
                .roughnessMap = roughnessMapAsset && roughnessMapAsset->isLoaded() ? roughnessMapAsset->getData()->texture : nullptr,
                .emissiveMap = emissiveMapAsset && emissiveMapAsset->isLoaded() ? emissiveMapAsset->getData()->texture : nullptr,
                .shader = material.shader
            };

            renderer3D.drawBillboard(transf.pos, transf.size, inputMaterial, entityID);
        }

        [[nodiscard]] std::shared_ptr<Shape3D> clone() const override
        {
            return std::make_shared<BillBoard>(*this);
        }
    };
}
