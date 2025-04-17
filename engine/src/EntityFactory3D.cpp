//// EntityFactory3D.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Source file for the 3D entity factory
//
///////////////////////////////////////////////////////////////////////////////

#include "EntityFactory3D.hpp"
#include "components/Light.hpp"
#include "components/Shapes3D.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "components/Camera.hpp"
#include "core/exceptions/Exceptions.hpp"
#include "Application.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <filesystem>

namespace nexo {

    ecs::Entity EntityFactory3D::createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};

        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);
        components::Material material{};
        material.albedoColor = color;
        auto cube = std::make_shared<components::Cube>();
        auto renderable = std::make_shared<components::Renderable3D>(material, cube);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newCube, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newCube, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newCube, uuid);

        return newCube;
    }

    ecs::Entity EntityFactory3D::createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, const components::Material &material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);
        auto cube = std::make_shared<components::Cube>();
        auto renderable = std::make_shared<components::Renderable3D>(material, cube);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newCube, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newCube, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newCube, uuid);
        return newCube;
    }

    ecs::Entity EntityFactory3D::createModel(const std::string &path, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);
        components::Material material{};
        std::shared_ptr<components::MeshNode> rootNode = utils::loadModel(path);
        auto model = std::make_shared<components::Model>(rootNode);
        auto renderable = std::make_shared<components::Renderable3D>(material, model);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newModel = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newModel, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newModel, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newModel, uuid);
        return newModel;
    }

    ecs::Entity EntityFactory3D::createBillboard(const glm::vec3 &pos, const glm::vec3 &size, const glm::vec4 &color)
    {
        components::TransformComponent transform{};

        transform.pos = pos;
        transform.size = size;
        components::Material material{};
        material.albedoColor = color;
        auto billboard = std::make_shared<components::BillBoard>();
        auto renderable = std::make_shared<components::Renderable3D>(material, billboard);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newBillboard, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newBillboard, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newBillboard, uuid);

        return newBillboard;
    }

    ecs::Entity EntityFactory3D::createBillboard(const glm::vec3 &pos, const glm::vec3 &size, const components::Material &material)
    {
        components::TransformComponent transform{};

        transform.pos = pos;
        transform.size = size;
        auto billboard = std::make_shared<components::BillBoard>();
        auto renderable = std::make_shared<components::Renderable3D>(material, billboard);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newBillboard, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newBillboard, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newBillboard, uuid);

        return newBillboard;
    }
}

namespace nexo::utils {
    glm::mat4 convertAssimpMatrixToGLM(const aiMatrix4x4 &matrix)
    {
        return glm::mat4(
            matrix.a1, matrix.b1, matrix.c1, matrix.d1,
            matrix.a2, matrix.b2, matrix.c2, matrix.d2,
            matrix.a3, matrix.b3, matrix.c3, matrix.d3,
            matrix.a4, matrix.b4, matrix.c4, matrix.d4
        );
    }

    components::Mesh processMesh(const std::string &path, aiMesh *mesh, const aiScene *scene)
    {
        std::vector<renderer::Vertex> vertices;
        std::vector<unsigned int> indices;
        vertices.reserve(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            renderer::Vertex vertex{};
            vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

            if (mesh->HasNormals()) {
                vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
            }

            if (mesh->mTextureCoords[0])
                vertex.texCoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            else
                vertex.texCoord = {0.0f, 0.0f};

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
        }

        aiMaterial const *material = scene->mMaterials[mesh->mMaterialIndex];

        components::Material materialComponent;

        aiColor4D color;
        if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
            materialComponent.albedoColor = { color.r, color.g, color.b, color.a };
        }

        if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
            materialComponent.specularColor = { color.r, color.g, color.b, color.a };
        }

        if (material->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS) {
            materialComponent.emissiveColor = { color.r, color.g, color.b };
        }

        float roughness = 0.0f;
        if (material->Get(AI_MATKEY_SHININESS, roughness) == AI_SUCCESS) {
            materialComponent.roughness = 1.0f - (roughness / 100.0f); // Convert glossiness to roughness
        }

        // Load Metallic
        float metallic = 0.0f;
        if (material->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS) {
            materialComponent.metallic = metallic;
        }

        float opacity = 1.0f;
        if (material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {
            materialComponent.opacity = opacity;
        }

        // Load Textures
        std::filesystem::path modelPath(path);
        std::filesystem::path modelDirectory = modelPath.parent_path();

        auto loadTexture = [&](aiTextureType type) -> std::shared_ptr<renderer::Texture2D> {
            aiString str;
            if (material->GetTexture(type, 0, &str) == AI_SUCCESS) {
                std::filesystem::path texturePath = modelDirectory / std::string(str.C_Str());
                return renderer::Texture2D::create(texturePath.string());
            }
            return nullptr;
        };

        materialComponent.albedoTexture = loadTexture(aiTextureType_DIFFUSE);
        materialComponent.normalMap = loadTexture(aiTextureType_NORMALS);
        materialComponent.metallicMap = loadTexture(aiTextureType_SPECULAR);  // Specular can store metallic in some cases
        materialComponent.roughnessMap = loadTexture(aiTextureType_SHININESS);
        materialComponent.emissiveMap = loadTexture(aiTextureType_EMISSIVE);

        LOG(NEXO_INFO, "Loaded material: Diffuse = {}, Normal = {}, Metallic = {}, Roughness = {}",
            materialComponent.albedoTexture ? "Yes" : "No",
            materialComponent.normalMap ? "Yes" : "No",
            materialComponent.metallicMap ? "Yes" : "No",
            materialComponent.roughnessMap ? "Yes" : "No");

        LOG(NEXO_INFO, "Loaded mesh {}", mesh->mName.data);

        return {mesh->mName.data, vertices, indices, materialComponent};
    }


    std::shared_ptr<components::MeshNode> processNode(const std::string &path, aiNode const *node, const aiScene *scene)
    {
        static int nbNode = 0;
        nbNode++;
        auto meshNode = std::make_shared<components::MeshNode>();

        glm::mat4 nodeTransform = convertAssimpMatrixToGLM(node->mTransformation);

        meshNode->transform = nodeTransform;

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshNode->meshes.push_back(processMesh(path, mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            auto newNode = processNode(path, node->mChildren[i], scene);
            if (newNode)
                meshNode->children.push_back(newNode);
        }

        return meshNode;
    }

    std::shared_ptr<components::MeshNode> loadModel(const std::string &path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(
            path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            THROW_EXCEPTION(core::LoadModelException, path, importer.GetErrorString());

        return processNode(path, scene->mRootNode, scene);
    }
}
