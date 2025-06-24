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

#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "components/Shapes3D.hpp"
#include "Path.hpp"

#include "assets/AssetImporterBase.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "ModelParameters.hpp"
#include "renderer/Renderer3D.hpp"

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
        std::unique_ptr<IAsset> model = loadModel(ctx);
        ctx.setMainAsset(std::move(model));
    }

    std::unique_ptr<Model> ModelImporter::loadModel(AssetImporterContext& ctx)
    {
        auto model = std::make_unique<Model>();

        const auto param = ctx.getParameters<ModelImportParameters>();
        constexpr int flags = aiProcess_Triangulate
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
            if (scene)
                m_importer.FreeScene();
            throw core::LoadModelException(ctx.location.getFullLocation(), m_importer.GetErrorString());
        }

        loadSceneEmbeddedTextures(ctx, scene);
        loadSceneMaterials(ctx, scene);

        auto meshNode = processNode(ctx, scene->mRootNode, scene);
        model->setData(std::make_unique<MeshNode>(meshNode));
        return model;
    }

    void ModelImporter::loadSceneEmbeddedTextures(AssetImporterContext& ctx, const aiScene* scene)
    {
        m_textures.reserve(scene->mNumTextures);
        // Load embedded textures
        for (int i = 0; scene->mNumTextures; ++i) {
            aiTexture *texture = scene->mTextures[i];
            auto loadedTexture = loadEmbeddedTexture(ctx, texture);
            m_textures.try_emplace(texture->mFilename.C_Str(), loadedTexture);
        }

    }

    AssetRef<Texture> ModelImporter::loadEmbeddedTexture(AssetImporterContext& ctx, aiTexture* texture)
    {
        if (texture->mHeight == 0) { // Compressed texture
            AssetImporter assetImporter;
            const ImporterInputVariant inputVariant = ImporterMemoryInput{
                // Reinterpret cast to uint8_t* because this is raw memory data, not aiTexels, see assimp docs
                .memoryData = std::vector<uint8_t>(reinterpret_cast<uint8_t *>(texture->pcData), reinterpret_cast<uint8_t *>(texture->pcData) + texture->mWidth),
                .formatHint = std::string(texture->achFormatHint)
            };

            return assetImporter.importAsset<Texture>(
                ctx.genUniqueDependencyLocation<Texture>(),
                inputVariant);
        }
        // Uncompressed texture
        auto& catalog = AssetCatalog::getInstance();

        renderer::NxTextureFormat format;
        if (texture->achFormatHint[0] == '\0') { // if empty, then ARGB888
            renderer::NxTextureFormatConvertArgb8ToRgba8(
                reinterpret_cast<uint8_t*>(texture->pcData),
            static_cast<unsigned long>(texture->mWidth) * static_cast<unsigned long>(texture->mHeight) * sizeof(aiTexel)
            );
            format = renderer::NxTextureFormat::RGBA8;
        } else {
            format = convertAssimpHintToNxTextureFormat(texture->achFormatHint);
        }

        if (format == renderer::NxTextureFormat::INVALID) {
            LOG(NEXO_WARN, "ModelImporter: Model {}: Texture {} has an invalid format hint: {}", std::quoted(ctx.location.getFullLocation()), texture->mFilename.C_Str(), texture->achFormatHint);
            return nullptr;
        }

        return catalog.createAsset<Texture>(ctx.genUniqueDependencyLocation<Texture>(),
            reinterpret_cast<uint8_t*>(texture->pcData), texture->mWidth, texture->mHeight, format);
    }

    renderer::NxTextureFormat ModelImporter::convertAssimpHintToNxTextureFormat(const char achFormatHint[9])
    {
        if (std::memchr(achFormatHint, '\0', 9) == nullptr
            || std::strlen(achFormatHint) != 8) {
            return renderer::NxTextureFormat::INVALID;
        }

        // Split into channels (first 4 chars) and bit depths (next 4 chars)
        const std::string_view channels(achFormatHint, 4);
        const std::string_view bits_str(achFormatHint + 4, 4);

        // Parse active channels and their bit depths
        struct ChannelInfo { char code; int bits; };
        std::vector<ChannelInfo> active_channels;

        for (int i = 0; i < 4; ++i) {
            const auto ch = static_cast<char>(std::tolower(channels[i]));
            if (!(ch == 'r' || ch == 'g' || ch == 'b' || ch == 'a')) {
                return renderer::NxTextureFormat::INVALID;
            }
            if (!std::isdigit(bits_str[i])) {
                return renderer::NxTextureFormat::INVALID;
            }
            const int bits = bits_str[i] - '0';

            if (ch != '\0' && bits > 0) {
                active_channels.push_back({ch, bits});
            }
        }

        // Check all active channels have exactly 8 bits
        for (const auto& ci : active_channels) {
            if (ci.bits != 8) return renderer::NxTextureFormat::INVALID;
        }

        // Match channel patterns
        switch (active_channels.size()) {
            case 1:
                if (active_channels[0].code == 'r')
                    return renderer::NxTextureFormat::R8;
                break;

            case 2:
                if (active_channels[0].code == 'r' &&
                    active_channels[1].code == 'g')
                    return renderer::NxTextureFormat::RG8;
                break;

            case 3:
                if (active_channels[0].code == 'r' &&
                    active_channels[1].code == 'g' &&
                    active_channels[2].code == 'b')
                    return renderer::NxTextureFormat::RGB8;
                break;

            case 4:
                if (active_channels[0].code == 'r' &&
                    active_channels[1].code == 'g' &&
                    active_channels[2].code == 'b' &&
                    active_channels[3].code == 'a')
                    return renderer::NxTextureFormat::RGBA8;
                break;
            default:
                break;
        }

        return renderer::NxTextureFormat::INVALID;
    }

    void ModelImporter::loadSceneMaterials(AssetImporterContext& ctx, const aiScene* scene)
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

        for (unsigned int matIdx = 0; matIdx < scene->mNumMaterials; ++matIdx) {
            aiMaterial const *material = scene->mMaterials[matIdx];

            auto materialComponent = std::make_unique<components::Material>();

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

            if (float roughness = 0.0f; material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS) {
                materialComponent->roughness = roughness;
            }

            // Load Metallic
            if (float metallic = 0.0f; material->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS) {
                materialComponent->metallic = metallic;
            }

            if (float opacity = 1.0f; material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {
                materialComponent->opacity = opacity;
                if (opacity < 0.99f) { // Using 0.99 to account for floating point imprecision
                    materialComponent->isOpaque = false;
                }
            }

            int blendFunc = 0;
            if (material->Get(AI_MATKEY_BLEND_FUNC, blendFunc) == AI_SUCCESS) {
                materialComponent->isOpaque = false; // Any non-default blend mode suggests transparency
            }

            // Check 2: Transparency factor (inverse of opacity in some formats)
            float transparencyFactor = 0.0f;
            if (material->Get(AI_MATKEY_TRANSPARENCYFACTOR, transparencyFactor) == AI_SUCCESS) {
                if (transparencyFactor > 0.01f) { // Again with epsilon
                    materialComponent->isOpaque = false;
                }
            }

            aiString alphaMode;
            //TODO: understand why we cant access it by default
            #define AI_MATKEY_GLTF_ALPHAMODE "$mat.gltf.alphaMode"
            #define AI_MATKEY_GLTF_ALPHACUTOFF "$mat.gltf.alphaCutoff"
            if (material->Get(AI_MATKEY_GLTF_ALPHAMODE, 0, 0, alphaMode) == AI_SUCCESS) {
                std::string mode = alphaMode.C_Str();
                if (mode == "BLEND")
                    materialComponent->isOpaque = false;
                else if (mode == "MASK") {
                    float alphaCutoff = 0.5f;
                    material->Get(AI_MATKEY_GLTF_ALPHACUTOFF, 0, 0, alphaCutoff);
                }
                // OPAQUE mode - not transparent
            }

            // Load Textures
            auto loadTexture = [&](aiTextureType type) -> AssetRef<Texture> {
                if (material->GetTextureCount(type) > 1) {
                    LOG(NEXO_WARN, "ModelImporter: Model {}: Material {} has more than one texture of type {}, only the first one will be used.", std::quoted(ctx.location.getFullLocation()), matIdx, type);
                }

                aiString aiStr;
                if (material->GetTexture(type, 0, &aiStr) == AI_SUCCESS) {
                    const char* cStr = aiStr.C_Str();
                    if (cStr[0] == '*' || scene->GetEmbeddedTexture(cStr)) {
                        // Embedded texture
                        if (const auto it = m_textures.find(cStr) ; it != m_textures.end()) {
                            return it->second;
                        }
                    }
                    const std::filesystem::path texturePath = (modelDirectory / cStr).lexically_normal();
                    const auto texturePathStr = texturePath.string();
                    if (const auto it = m_textures.find(texturePathStr.c_str()) ; it != m_textures.end()) {
                        return it->second;
                    }
                    AssetImporter assetImporter;
                    const ImporterInputVariant inputVariant = ImporterFileInput{
                        .filePath = texturePath
                    };
                    auto assetTexture = assetImporter.importAsset<Texture>(
                        ctx.genUniqueDependencyLocation<Texture>(),
                        inputVariant);
                    m_textures.try_emplace(texturePathStr.c_str(), assetTexture);
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

            const auto materialRef = AssetCatalog::getInstance().createAsset<Material>(
                ctx.genUniqueDependencyLocation<Material>(),
                std::move(materialComponent)
            );
            m_materials[matIdx] = materialRef;
        } // end for (int matIdx = 0; matIdx < scene->mNumMaterials; ++matIdx)
    }

    MeshNode ModelImporter::processNode(AssetImporterContext& ctx, aiNode const* node, const aiScene* scene)
    {
        auto meshNode = MeshNode{};

        meshNode.name = node->mName.C_Str();

        const glm::mat4 nodeTransform = convertAssimpMatrixToGLM(node->mTransformation);
        meshNode.transform = nodeTransform;

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshNode.meshes.push_back(processMesh(ctx, mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            auto newNode = processNode(ctx, node->mChildren[i], scene);
            meshNode.children.push_back(newNode);
        }

        return meshNode;
    }

    Mesh ModelImporter::processMesh(AssetImporterContext& ctx, aiMesh* mesh, [[maybe_unused]] const aiScene* scene)
    {
        std::shared_ptr<renderer::NxVertexArray> vao = renderer::createVertexArray();
        auto vertexBuffer = renderer::createVertexBuffer(mesh->mNumVertices * sizeof(renderer::NxVertex));
        const renderer::NxBufferLayout cubeVertexBufferLayout = {
            {renderer::NxShaderDataType::FLOAT3, "aPos"},
            {renderer::NxShaderDataType::FLOAT2, "aTexCoord"},
            {renderer::NxShaderDataType::FLOAT3, "aNormal"},
            {renderer::NxShaderDataType::FLOAT3, "aTangent"},
            {renderer::NxShaderDataType::FLOAT3, "aBiTangent"},
            {renderer::NxShaderDataType::INT, "aEntityID"}
        };
        vertexBuffer->setLayout(cubeVertexBufferLayout);
        std::vector<renderer::NxVertex> vertices;
        std::vector<unsigned int> indices;
        vertices.reserve(mesh->mNumVertices);

        glm::vec3 minBB(+FLT_MAX, +FLT_MAX, +FLT_MAX);
        glm::vec3 maxBB(-FLT_MAX, -FLT_MAX, -FLT_MAX);


        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            renderer::NxVertex vertex{};
            vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

            minBB.x = std::min(minBB.x, vertex.position.x);
            minBB.y = std::min(minBB.y, vertex.position.y);
            minBB.z = std::min(minBB.z, vertex.position.z);

            maxBB.x = std::max(maxBB.x, vertex.position.x);
            maxBB.y = std::max(maxBB.y, vertex.position.y);
            maxBB.z = std::max(maxBB.z, vertex.position.z);

            if (mesh->HasNormals()) {
                vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
            }

            if (mesh->mTextureCoords[0])
                vertex.texCoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            else
                vertex.texCoord = {0.0f, 0.0f};

            vertices.push_back(vertex);
        }

        glm::vec3 centerLocal = (minBB + maxBB) * 0.5f;

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace face = mesh->mFaces[i];
            indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
        }

        vertexBuffer->setData(vertices.data(), static_cast<unsigned int>(vertices.size() * sizeof(renderer::NxVertex)));
        vao->addVertexBuffer(vertexBuffer);

        std::shared_ptr<renderer::NxIndexBuffer> indexBuffer = renderer::createIndexBuffer();
        indexBuffer->setData(indices.data(), static_cast<unsigned int>(indices.size()));
        vao->setIndexBuffer(indexBuffer);

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
        return {mesh->mName.data, vao, materialComponent, centerLocal};
    }

    glm::mat4 ModelImporter::convertAssimpMatrixToGLM(const aiMatrix4x4& matrix)
    {
        return {
            matrix.a1, matrix.b1, matrix.c1, matrix.d1,
            matrix.a2, matrix.b2, matrix.c2, matrix.d2,
            matrix.a3, matrix.b3, matrix.c3, matrix.d3,
            matrix.a4, matrix.b4, matrix.c4, matrix.d4
        };
    }

} // namespace nexo::assets
