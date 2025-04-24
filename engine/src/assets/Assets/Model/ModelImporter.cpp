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
#include <iomanip>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "components/Shapes3D.hpp"
#include "Path.hpp"

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
            extension = mem.formatHint;
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
            auto& [memoryData, formatHint] = std::get<ImporterMemoryInput>(ctx.input);
            scene = m_importer.ReadFileFromMemory(memoryData.data(), memoryData.size(), flags, formatHint.c_str());
        }
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            //log error TODO: improve error handling in importers
            delete model;
            if (scene)
                m_importer.FreeScene();
            throw core::LoadModelException(ctx.location.getFullLocation(), m_importer.GetErrorString());
        }

        loadEmbeddedTextures(ctx, scene);
        loadMaterials(ctx, scene);

        auto meshNode = processNode(ctx, scene->mRootNode, scene);
        if (!meshNode) {
            delete model;
            throw core::LoadModelException(ctx.location.getFullLocation(), "Failed to process model node");
        }
        model->setData(new components::Model(meshNode));
        return model;
    }

    void ModelImporter::loadEmbeddedTextures(AssetImporterContext& ctx, const aiScene* scene)
    {
        m_textures.reserve(scene->mNumTextures);
        // Load embedded textures
        for (int i = 0; scene->mNumTextures; ++i) {
            aiTexture *texture = scene->mTextures[i];
            AssetImporter assetImporter;
            ImporterInputVariant inputVariant;
            if (texture->mHeight == 0) { // Compressed texture
                inputVariant = ImporterMemoryInput{
                    // Reinterpret cast to uint8_t* because this is raw memory data, not aiTexels, see assimp docs
                    .memoryData = std::vector<uint8_t>(reinterpret_cast<uint8_t *>(texture->pcData), reinterpret_cast<uint8_t *>(texture->pcData) + texture->mWidth),
                    .formatHint = std::string(texture->achFormatHint)
                };
            } else { // Uncompressed texture
                // TODO: implement RGBA8888 format
                /*inputVariant = ImporterMemoryInput{
                    .memoryData = std::vector<uint8_t>(texture->pcData, texture->pcData + (texture->mWidth * texture->mHeight * 4)),
                    .formatHint = std::string(texture->achFormatHint)
                };*/
                return;
            }

            auto assetTexture = assetImporter.importAsset<Texture>(
                ctx.genUniqueDependencyLocation<Texture>(),
                inputVariant);
            m_textures.emplace(texture->mFilename.C_Str(), assetTexture);
        }

    }

    void ModelImporter::loadMaterials(AssetImporterContext& ctx, const aiScene* scene)
    {
        m_materials.assign(scene->mNumMaterials, nullptr);

        std::filesystem::path modelPath;
        if (std::holds_alternative<ImporterFileInput>(ctx.input))
            modelPath = std::get<ImporterFileInput>(ctx.input).filePath;
        else {
            modelPath = Path::getExecutablePath();
            LOG(NEXO_WARN, "ModelImporter: Model {}: Model path not given (imported from memory), using executable path for texture lookup.", std::quoted(ctx.location.getFullLocation()));
        }
        std::filesystem::path modelDirectory = modelPath.parent_path();

        for (int matIdx = 0; matIdx < scene->mNumMaterials; ++matIdx) {
            aiMaterial const *material = scene->mMaterials[matIdx];

            const auto materialComponent = new components::Material();

            aiColor4D color;
            if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
                materialComponent->albedoColor = { color.r, color.g, color.b, color.a };
            }

            if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS) {
                materialComponent->specularColor = { color.r, color.g, color.b, color.a };
            }

            if (material->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS) {
                materialComponent->emissiveColor = { color.r, color.g, color.b };
            }

            if (float roughness = 0.0f; material->Get(AI_MATKEY_SHININESS, roughness) == AI_SUCCESS) {
                materialComponent->roughness = 1.0f - (roughness / 100.0f); // Convert glossiness to roughness
            }

            // Load Metallic
            if (float metallic = 0.0f; material->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS) {
                materialComponent->metallic = metallic;
            }

            if (float opacity = 1.0f; material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {
                materialComponent->opacity = opacity;
            }

            // Load Textures


            auto loadTexture = [&](aiTextureType type) -> AssetRef<Texture> {
                if (material->GetTextureCount(type) > 1) {
                    LOG(NEXO_WARN, "ModelImporter: Model {}: Material {} has more than one texture of type {}, only the first one will be used.", std::quoted(ctx.location.getFullLocation()), matIdx, type);
                }

                aiString aiStr;
                if (material->GetTexture(type, 0, &aiStr) == AI_SUCCESS) {
                    const char* cStr = aiStr.C_Str();
                    if (cStr[0] == '*') {
                        // Embedded texture
                        if (const auto it = m_textures.find(cStr) ; it != m_textures.end()) {
                            return it->second;
                        }
                    }
                    std::filesystem::path texturePath = (modelDirectory / cStr).lexically_normal();
                    auto texturePathStr = texturePath.string();
                    if (const auto it = m_textures.find(texturePathStr.c_str()) ; it != m_textures.end()) {
                        return it->second;
                    }
                    AssetImporter assetImporter;
                    ImporterInputVariant inputVariant = ImporterFileInput{
                        .filePath = texturePath
                    };
                    auto assetTexture = assetImporter.importAsset<Texture>(
                        ctx.genUniqueDependencyLocation<Texture>(),
                        inputVariant);
                    m_textures.emplace(texturePathStr.c_str(), assetTexture);
                    return assetTexture;
                }
                return nullptr;
            };

            materialComponent->albedoTexture = loadTexture(aiTextureType_DIFFUSE);
            materialComponent->normalMap = loadTexture(aiTextureType_NORMALS);
            materialComponent->metallicMap = loadTexture(aiTextureType_SPECULAR);  // Specular can store metallic in some cases
            materialComponent->roughnessMap = loadTexture(aiTextureType_SHININESS);
            materialComponent->emissiveMap = loadTexture(aiTextureType_EMISSIVE);

            LOG(NEXO_INFO, "Loaded material: Diffuse = {}, Normal = {}, Metallic = {}, Roughness = {}",
                materialComponent->albedoTexture ? "Yes" : "No",
                materialComponent->normalMap ? "Yes" : "No",
                materialComponent->metallicMap ? "Yes" : "No",
                materialComponent->roughnessMap ? "Yes" : "No");

            AssetCatalog::getInstance().createAsset<Material>(
                ctx.genUniqueDependencyLocation<Material>(),
                materialComponent
            );
        } // end for (int matIdx = 0; matIdx < scene->mNumMaterials; ++matIdx)
    }

    std::shared_ptr<components::MeshNode> ModelImporter::processNode(AssetImporterContext& ctx, aiNode const* node,
        const aiScene* scene)
    {
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
        std::vector<renderer::NxVertex> vertices;
        std::vector<unsigned int> indices;
        vertices.reserve(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            renderer::NxVertex vertex{};
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


        AssetRef<Material> materialComponent = nullptr;
        if (mesh->mMaterialIndex < m_materials.size()) {
            materialComponent = m_materials[mesh->mMaterialIndex];
        } else {
            LOG(NEXO_ERROR, "ModelImporter: Model {}: Mesh {} has invalid material index {}.", std::quoted(ctx.location.getFullLocation()), std::quoted(mesh->mName.C_Str()), mesh->mMaterialIndex);
        }
        if (!materialComponent) {
            LOG(NEXO_WARN, "ModelImporter: Model {}: Mesh {} has no material.", std::quoted(ctx.location.getFullLocation()), std::quoted(mesh->mName.C_Str()));
        }

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
