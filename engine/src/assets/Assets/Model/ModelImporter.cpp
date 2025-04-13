//// ModelImporter.cpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/04/2025
//  Description: Implementation file for the ModelImporter class
//
///////////////////////////////////////////////////////////////////////////////

#include "ModelImporter.hpp"

#include <array>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <components/Shapes3D.hpp>

#include "assets/AssetImporterBase.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "ModelParameters.hpp"

#include "core/exceptions/Exceptions.hpp"

namespace nexo::assets {
    
    bool ModelImporter::canRead(const ImporterInputVariant& inputVariant)
    {
        std::string extension;
        if (std::holds_alternative<ImporterFileInput>(inputVariant))
            extension = std::get<ImporterFileInput>(inputVariant).filePath.extension().string();
        if (std::holds_alternative<ImporterMemoryInput>(inputVariant)) {
            const auto& mem = std::get<ImporterMemoryInput>(inputVariant);
            if (mem.fileExtension)
                extension = mem.fileExtension.value();
        }
        const Assimp::Importer importer;
        return importer.IsExtensionSupported(extension);
    }

    void ModelImporter::importImpl(AssetImporterContext& ctx)
    {
        m_model = loadModel(ctx);
        ctx.setMainAsset(m_model);
    }

    Model* ModelImporter::loadModel(AssetImporterContext& ctx)
    {
        Model* model = new Model();

        //m_model->setData(new components::Model());
        const auto param = ctx.getParameters<ModelImportParameters>();
        int flags = aiProcess_Triangulate
                    | aiProcess_FlipUVs
                    | aiProcess_GenNormals;
        const aiScene* scene = nullptr;
        if (std::holds_alternative<ImporterFileInput>(ctx.input))
            scene = m_importer.ReadFile(std::get<ImporterFileInput>(ctx.input).filePath.string(), flags);
        if (std::holds_alternative<ImporterMemoryInput>(ctx.input)) {
            auto [memoryData, fileExtension] = std::get<ImporterMemoryInput>(ctx.input);
            scene = m_importer.ReadFileFromMemory(memoryData.data(), memoryData.size(), flags, fileExtension ? fileExtension->c_str() : nullptr);
        }
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            //log error TODO: improve error handling in importers
            delete model;
            if (scene)
                m_importer.FreeScene();
            throw core::LoadModelException(ctx.location.getFullLocation(), m_importer.GetErrorString());
        }
        auto meshNode = processNode(ctx, scene->mRootNode, scene);
        if (!meshNode) {
            delete model;
            throw core::LoadModelException(ctx.location.getFullLocation(), "Failed to process model node");
        }
        model->setData(new components::Model(meshNode));
        return model;
    }

    std::shared_ptr<components::MeshNode> ModelImporter::processNode(AssetImporterContext& ctx, aiNode const* node,
        const aiScene* scene)
    {
        static int nbNode = 0;
        nbNode++;
        auto meshNode = std::make_shared<components::MeshNode>();

        glm::mat4 nodeTransform = convertAssimpMatrixToGLM(node->mTransformation);

        meshNode->transform = nodeTransform;

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshNode->meshes.push_back(processMesh(ctx, mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            auto newNode = processNode(ctx, node->mChildren[i], scene);
            if (newNode)
                meshNode->children.push_back(newNode);
        }

        return meshNode;
    }

    components::Mesh ModelImporter::processMesh(AssetImporterContext& ctx, aiMesh* mesh, const aiScene* scene)
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

        if (float roughness = 0.0f; material->Get(AI_MATKEY_SHININESS, roughness) == AI_SUCCESS) {
            materialComponent.roughness = 1.0f - (roughness / 100.0f); // Convert glossiness to roughness
        }

        // Load Metallic
        if (float metallic = 0.0f; material->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS) {
            materialComponent.metallic = metallic;
        }

        if (float opacity = 1.0f; material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {
            materialComponent.opacity = opacity;
        }

        // Load Textures
        std::filesystem::path modelPath(std::holds_alternative<ImporterFileInput>(ctx.input) ?
            std::get<ImporterFileInput>(ctx.input).filePath : "");
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

    glm::mat4 ModelImporter::convertAssimpMatrixToGLM(const aiMatrix4x4& matrix)
    {
        return glm::mat4(
            matrix.a1, matrix.b1, matrix.c1, matrix.d1,
            matrix.a2, matrix.b2, matrix.c2, matrix.d2,
            matrix.a3, matrix.b3, matrix.c3, matrix.d3,
            matrix.a4, matrix.b4, matrix.c4, matrix.d4
        );
    }

} // namespace nexo::assets
