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
#include "components/Components.hpp"
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
        transform.rotation = rotation;
        components::Material material{};
        material.color = color;
        auto cube = std::make_shared<components::Cube>();
        auto renderable = std::make_shared<components::Renderable3D>(material, cube);
        components::RenderComponent renderComponent(renderable);

        ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newCube, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newCube, renderComponent);
        return newCube;
    }

    ecs::Entity EntityFactory3D::createModel(const std::string &path, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.rotation = rotation;
        components::Material material{};
        std::shared_ptr<components::MeshNode> rootNode = utils::loadModel(path);
        auto model = std::make_shared<components::Model>(rootNode);
        auto renderable = std::make_shared<components::Renderable3D>(material, model);
        components::RenderComponent renderComponent(renderable);

        ecs::Entity newModel = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newModel, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newModel, renderComponent);
        return newModel;
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

        // Extract vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            renderer::Vertex vertex;
            vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

            //TODO: Later
            // if (mesh->HasNormals()) {
            //     vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
            // }

            if (mesh->mTextureCoords[0])
                vertex.texCoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            else
                vertex.texCoord = {0.0f, 0.0f};

            vertices.push_back(vertex);
        }

        // Extract indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            indices.resize(indices.size() + face.mNumIndices);
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // Extract texture (if available)
        std::shared_ptr<renderer::Texture2D> texture = nullptr;
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            aiString str;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS)
            {
                std::filesystem::path modelPath(path);
                std::filesystem::path modelDirectory = modelPath.parent_path();

                std::filesystem::path texturePath = modelDirectory / std::string(str.C_Str());

                texture = renderer::Texture2D::create(texturePath.string());
                LOG(NEXO_INFO, "Loaded texture: {}", str.data);
            }
        }

        LOG(NEXO_INFO, "Loaded mesh {}", mesh->mName.data);
        return {mesh->mName.data, vertices, indices, texture, std::nullopt}; // Optional material not used yet
    }

    std::shared_ptr<components::MeshNode> processNode(const std::string &path, aiNode *node, const aiScene *scene)
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
