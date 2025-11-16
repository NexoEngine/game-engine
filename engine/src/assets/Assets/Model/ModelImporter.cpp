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
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "Buffer.hpp"
#include "Path.hpp"
#include "VertexArray.hpp"
#include "assets/AssetImporterContext.hpp"
#include "math/Bounds.hpp"

#include "ModelParameters.hpp"
#include "assets/AssetImporterBase.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "renderer/Renderer3D.hpp"
#include "renderer/Texture.hpp"

#include "core/exceptions/Exceptions.hpp"

import  nexo.save.concepts;

namespace nexo::assets {

    bool ModelImporter::canRead(const ImporterInputVariant& inputVariant)
    {
        std::string extension;
        if (std::holds_alternative<ImporterFileInput>(inputVariant))
            extension = std::get<ImporterFileInput>(inputVariant).filePath.extension().string();
        if (std::holds_alternative<ImporterMemoryInput>(inputVariant)) {
            const auto& mem = std::get<ImporterMemoryInput>(inputVariant);
            extension       = mem.formatHint;
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

        const auto param     = ctx.getParameters<ModelImportParameters>();
        constexpr int flags  = aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
        const aiScene* scene = nullptr;
        if (std::holds_alternative<ImporterFileInput>(ctx.input))
            scene = m_importer.ReadFile(std::get<ImporterFileInput>(ctx.input).filePath.string(), flags);
        if (std::holds_alternative<ImporterMemoryInput>(ctx.input)) {
            auto& [memoryData, formatHint] = std::get<ImporterMemoryInput>(ctx.input);
            scene = m_importer.ReadFileFromMemory(memoryData.data(), memoryData.size(), flags, formatHint.c_str());
        }
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            // log error TODO: improve error handling in importers
            if (scene) m_importer.FreeScene();
            throw core::LoadModelException(ctx.location.getFullLocation(), m_importer.GetErrorString());
        }

        LOG(NEXO_INFO, "Scene loaded successfully:");
        LOG(NEXO_INFO, "  - {} materials", scene->mNumMaterials);
        LOG(NEXO_INFO, "  - {} embedded textures", scene->mNumTextures);
        LOG(NEXO_INFO, "  - {} meshes", scene->mNumMeshes);

        loadSceneEmbeddedTextures(ctx, scene);
        loadSceneMaterials(ctx, scene);

        auto meshNode = processNode(ctx, scene->mRootNode, scene);
        model->setData(std::make_unique<MeshNode>(meshNode));
        model->rootBounds = meshNode.modelBounds;
        model->rootSphere = math::sphereFromAABB(model->rootBounds);
        return model;
    }

    void ModelImporter::loadSceneEmbeddedTextures(AssetImporterContext& ctx, const aiScene* scene)
    {
        LOG(NEXO_INFO, "Loading embedded textures: {} textures found", scene->mNumTextures);

        m_textures.reserve(scene->mNumTextures);
        // Load embedded textures
        for (unsigned int i = 0; i < scene->mNumTextures; ++i) {
            aiTexture* texture = scene->mTextures[i];

            auto loadedTexture = loadEmbeddedTexture(ctx, texture);
            if (loadedTexture) {
                LOG(NEXO_INFO, "Successfully loaded embedded texture: {}", texture->mFilename.C_Str());
            } else {
                LOG(NEXO_ERROR, "Failed to load embedded texture: {}", texture->mFilename.C_Str());
            }
            m_textures.try_emplace(texture->mFilename.C_Str(), loadedTexture);
        }
    }

    AssetRef<Texture> ModelImporter::loadEmbeddedTexture(AssetImporterContext& ctx,
                                                         aiTexture* texture,
                                                         renderer::TextureType suggestedType)
    {
        LOG(NEXO_DEBUG, "Loading embedded texture: '{}' (suggested type: {})",
            texture->mFilename.C_Str(), static_cast<int>(suggestedType));

        if (texture->mHeight == 0) { // Compressed texture
            AssetImporter assetImporter;
            AssetImporterContext textureCtx;
            textureCtx.input = ImporterMemoryInput{
                .memoryData = std::vector<uint8_t>(reinterpret_cast<uint8_t*>(texture->pcData),
                                                   reinterpret_cast<uint8_t*>(texture->pcData) + texture->mWidth),
                .formatHint = std::string(texture->achFormatHint)
            };
            textureCtx.location = ctx.genUniqueDependencyLocation<Texture>();

            // Auto-detect texture type from filename if not specified
            renderer::TextureType detectedType = detectTextureTypeFromFilename(texture->mFilename.C_Str());
            if (detectedType == renderer::TextureType::ALBEDO && suggestedType != renderer::TextureType::ALBEDO) {
                detectedType = suggestedType;
            }

            TextureImportParameters texParams;
            texParams.textureType    = detectedType;
            texParams.generateMipmaps = true;
            textureCtx.setParameters(texParams);

            assetImporter.setCustomContext(&textureCtx);

            return assetImporter.importAsset<Texture>(textureCtx.location, textureCtx.input);
        }

        // ───── Uncompressed embedded texture (width x height, aiTexel buffer) ─────
        renderer::NxTextureFormat format;
        if (texture->achFormatHint[0] == '\0') {
            renderer::NxTextureFormatConvertArgb8ToRgba8(
                reinterpret_cast<uint8_t*>(texture->pcData),
                static_cast<unsigned long>(texture->mWidth) *
                static_cast<unsigned long>(texture->mHeight) *
                sizeof(aiTexel));
            format = renderer::NxTextureFormat::RGBA8;
        } else {
            format = convertAssimpHintToNxTextureFormat(texture->achFormatHint);
        }

        if (format == renderer::NxTextureFormat::INVALID) {
            LOG(NEXO_WARN,
                "ModelImporter: Model {}: Texture {} has an invalid format hint: {}",
                std::quoted(ctx.location.getFullLocation()),
                texture->mFilename.C_Str(),
                texture->achFormatHint);
            return nullptr;
        }

        // Type awareness from filename + suggested type
        renderer::TextureType detectedType = detectTextureTypeFromFilename(texture->mFilename.C_Str());
        if (detectedType == renderer::TextureType::ALBEDO && suggestedType != renderer::TextureType::ALBEDO) {
            detectedType = suggestedType;
        }

        // 1) Create renderer texture from the raw embedded pixels
        std::shared_ptr<renderer::NxTexture2D> rendererTexture =
            renderer::NxTexture2D::create(
                reinterpret_cast<uint8_t*>(texture->pcData),
                static_cast<unsigned int>(texture->mWidth),
                static_cast<unsigned int>(texture->mHeight),
                format,
                detectedType,
                /*generateMipmaps*/ true);

        if (!rendererTexture) {
            LOG(NEXO_ERROR,
                "ModelImporter: Failed to create renderer texture for embedded '{}'",
                texture->mFilename.C_Str());
            return nullptr;
        }

        // 2) Build TextureData like TextureImporter does
        auto textureData = std::make_unique<TextureData>();
        textureData->texture     = rendererTexture;
        textureData->textureType = detectedType;

        // 3) Register as an asset in the catalog (same pattern as materials)
        auto& catalog = AssetCatalog::getInstance();
        const auto loc = ctx.genUniqueDependencyLocation<Texture>();

        const auto textureRef = catalog.createAsset<Texture>(
            loc,
            std::move(textureData));   // forwarded to Texture's ctor

        return textureRef;
    }


    // Add this helper function
    renderer::TextureType ModelImporter::detectTextureTypeFromFilename(const std::string& filename) {
        std::string lowerName = filename;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

        if (lowerName.find("normal") != std::string::npos || lowerName.find("bump") != std::string::npos) {
            return renderer::TextureType::NORMAL;
        } else if (lowerName.find("metallicroughness") != std::string::npos || lowerName.find("metallic_roughness") != std::string::npos) {
            return renderer::TextureType::COMBINED_MR;
        } else if (lowerName.find("orm") != std::string::npos) {
            return renderer::TextureType::COMBINED_ORM;
        } else if (lowerName.find("metallic") != std::string::npos) {
            return renderer::TextureType::METALLIC;
        } else if (lowerName.find("roughness") != std::string::npos) {
            return renderer::TextureType::ROUGHNESS;
        } else if (lowerName.find("ao") != std::string::npos || lowerName.find("occlusion") != std::string::npos) {
            return renderer::TextureType::AO;
        } else if (lowerName.find("emissive") != std::string::npos || lowerName.find("emission") != std::string::npos) {
            return renderer::TextureType::EMISSIVE;
        } else if (lowerName.find("basecolor") != std::string::npos || lowerName.find("diffuse") != std::string::npos || lowerName.find("albedo") != std::string::npos) {
            return renderer::TextureType::ALBEDO;
        }

        return renderer::TextureType::ALBEDO; // Default
    }

    renderer::NxTextureFormat ModelImporter::convertAssimpHintToNxTextureFormat(const char achFormatHint[9])
    {
        if (std::memchr(achFormatHint, '\0', 9) == nullptr || std::strlen(achFormatHint) != 8) {
            return renderer::NxTextureFormat::INVALID;
        }

        // Split into channels (first 4 chars) and bit depths (next 4 chars)
        const std::string_view channels(achFormatHint, 4);
        const std::string_view bits_str(achFormatHint + 4, 4);

        // Parse active channels and their bit depths
        struct ChannelInfo {
            char code;
            int bits;
        };
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
                if (active_channels[0].code == 'r') return renderer::NxTextureFormat::R8;
                break;

            case 2:
                if (active_channels[0].code == 'r' && active_channels[1].code == 'g')
                    return renderer::NxTextureFormat::RG8;
                break;

            case 3:
                if (active_channels[0].code == 'r' && active_channels[1].code == 'g' && active_channels[2].code == 'b')
                    return renderer::NxTextureFormat::RGB8;
                break;

            case 4:
                if (active_channels[0].code == 'r' && active_channels[1].code == 'g' &&
                    active_channels[2].code == 'b' && active_channels[3].code == 'a')
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
            LOG(NEXO_WARN,
                "ModelImporter: Model {}: Model path not given (imported from memory), using executable path for "
                "texture lookup.",
                std::quoted(ctx.location.getFullLocation()));
        }
        const std::filesystem::path modelDirectory = modelPath.parent_path();

        for (unsigned int matIdx = 0; matIdx < scene->mNumMaterials; ++matIdx) {
            aiMaterial const* material = scene->mMaterials[matIdx];
            auto materialComponent = std::make_unique<components::Material>();

            LOG(NEXO_DEBUG, "Material {} texture inventory:", matIdx);
            for (int type = aiTextureType_NONE; type <= aiTextureType_TRANSMISSION; type++) {
                auto textureType = static_cast<aiTextureType>(type);
                unsigned int count = material->GetTextureCount(textureType);
                if (count > 0) {
                    aiString path;
                    if (material->GetTexture(textureType, 0, &path) == AI_SUCCESS) {
                        LOG(NEXO_DEBUG, "  Type {}: {} textures, first = '{}'", type, count, path.C_Str());
                    }
                }
            }

            // Enhanced PBR Material Property Loading
            aiColor4D color;

            // Priority 1: glTF base color first (modern PBR)
            if (material->Get("$mat.gltf.pbrMetallicRoughness.baseColorFactor", 0, 0, color) == AI_SUCCESS) {
                materialComponent->albedoColor = {color.r, color.g, color.b, color.a};
            }
            // Priority 2: Standard base color
            else if (material->Get(AI_MATKEY_BASE_COLOR, color) == AI_SUCCESS) {
                materialComponent->albedoColor = {color.r, color.g, color.b, color.a};
            }
            // Priority 3: Fallback to diffuse for legacy materials
            else if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
                materialComponent->albedoColor = {color.r, color.g, color.b, color.a};
            }

            // Enhanced Metallic Factor Loading
            if (float metallic = 0.0f; material->Get("$mat.gltf.pbrMetallicRoughness.metallicFactor", 0, 0, metallic) == AI_SUCCESS) {
                materialComponent->metallic = metallic;
            } else if (float metallic = 0.0f; material->Get(AI_MATKEY_METALLIC_FACTOR, metallic) == AI_SUCCESS) {
                materialComponent->metallic = metallic;
            }

            // Enhanced Roughness Factor Loading
            if (float roughness = 0.5f; material->Get("$mat.gltf.pbrMetallicRoughness.roughnessFactor", 0, 0, roughness) == AI_SUCCESS) {
                materialComponent->roughness = roughness;
            } else if (float roughness = 0.5f; material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) == AI_SUCCESS) {
                materialComponent->roughness = roughness;
            } else {
                // Convert legacy shininess to PBR roughness
                float shininess = 32.0f;
                if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
                    // Convert Blinn-Phong shininess to PBR roughness (approximate)
                    materialComponent->roughness = sqrt(2.0f / (shininess + 2.0f));
                    materialComponent->roughness = glm::clamp(materialComponent->roughness, 0.0f, 1.0f);
                }
            }

            // Enhanced Emissive Loading
            if (material->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS) {
                materialComponent->emissiveColor = {color.r, color.g, color.b};

                // Check for emissive strength/factor (glTF extension)
                float emissiveStrength = 1.0f;
                if (material->Get("$mat.gltf.emissiveStrength", 0, 0, emissiveStrength) == AI_SUCCESS) {
                    materialComponent->emissiveColor *= emissiveStrength;
                }
            }

            float normalScale = 1.0f;
            if (material->Get("$mat.gltf.normalTextureScale", 0, 0, normalScale) == AI_SUCCESS) {
                materialComponent->normalScale = normalScale;
            }

            // Load AO factor
            float aoFactor = 1.0f;
            if (material->Get("$mat.gltf.occlusionTexture.strength", 0, 0, aoFactor) == AI_SUCCESS) {
                materialComponent->ao = aoFactor;
            }

            if (float opacity = 1.0f; material->Get(AI_MATKEY_OPACITY, opacity) == AI_SUCCESS) {
                materialComponent->opacity = opacity;
                if (opacity < OPACITY_THRESHOLD) { // Using 0.99 to account for floating point imprecision
                    materialComponent->isOpaque = false;
                } else
                    materialComponent->opacity = 1.0f;
            }

            int blendFunc = 0;
            if (material->Get(AI_MATKEY_BLEND_FUNC, blendFunc) == AI_SUCCESS) {
                materialComponent->isOpaque = false; // Any non-default blend mode suggests transparency
            }

            // Check 2: Transparency factor (inverse of opacity in some formats)
            float transparencyFactor = 0.0f;
            if (material->Get(AI_MATKEY_TRANSPARENCYFACTOR, transparencyFactor) == AI_SUCCESS &&
                transparencyFactor > TRANSPARENCY_EPSILON) {
                materialComponent->isOpaque = false;
            }

            aiString alphaMode;
// TODO: understand why we cant access it by default
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

            LOG(NEXO_INFO, "Material {}: albedo=({},{},{},{}), metallic={}, roughness={}, ao={}, normalScale={}",
                matIdx,
                materialComponent->albedoColor.x, materialComponent->albedoColor.y, materialComponent->albedoColor.z, materialComponent->albedoColor.w,
                materialComponent->metallic, materialComponent->roughness, materialComponent->ao, materialComponent->normalScale);

            // Add material name
            aiString materialName;
            if (material->Get(AI_MATKEY_NAME, materialName) == AI_SUCCESS) {
                LOG(NEXO_INFO, "Loading material '{}' (index {})", materialName.C_Str(), matIdx);
            } else {
                LOG(NEXO_INFO, "Loading unnamed material (index {})", matIdx);
            }

            // Load Textures
            auto loadTextureWithType = [&](aiTextureType assimpType, renderer::TextureType nexoType) -> AssetRef<Texture> {

                if (material->GetTextureCount(assimpType) == 0) {
                    return nullptr;
                }

                aiString aiStr;
                if (material->GetTexture(assimpType, 0, &aiStr) == AI_SUCCESS) {
                    const char* cStr = aiStr.C_Str();
                    LOG(NEXO_DEBUG, "Found texture path: '{}'", cStr);

                    // Check for embedded textures first (GLB files)
                    if (cStr[0] == '*') {
                        // Extract index from "*0", "*1", etc.
                        int embeddedIndex = std::atoi(cStr + 1);
                        if (embeddedIndex >= 0 && embeddedIndex < static_cast<int>(scene->mNumTextures)) {
                            aiTexture* embeddedTexture = scene->mTextures[embeddedIndex];
                            // Load embedded texture with type awareness
                            auto embeddedAsset = loadEmbeddedTexture(ctx, embeddedTexture, nexoType);
                            if (embeddedAsset) {
                                LOG(NEXO_INFO, "Successfully loaded embedded texture {} as type {}",
                                    embeddedTexture->mFilename.C_Str(), static_cast<int>(nexoType));
                                //m_textures.try_emplace(embeddedKey, embeddedAsset);
                                return embeddedAsset;
                            }
                        }
                    } else if (scene->GetEmbeddedTexture(cStr)) {
                        if (const auto it = m_textures.find(cStr); it != m_textures.end()) {
                            return it->second;
                        }

                        // Find the embedded texture
                        aiTexture* embeddedTexture = const_cast<aiTexture*>(scene->GetEmbeddedTexture(cStr));
                        if (embeddedTexture) {
                            auto embeddedAsset = loadEmbeddedTexture(ctx, embeddedTexture, nexoType);
                            if (embeddedAsset) {
                                m_textures.try_emplace(cStr, embeddedAsset);
                                return embeddedAsset;
                            }
                        }
                    } else {
                        // External file (GLTF workflow) - your existing code
                        const std::filesystem::path texturePath = (modelDirectory / cStr).lexically_normal();
                        const auto texturePathStr = texturePath.string();

                        AssetImporter assetImporter;
                        AssetImporterContext textureCtx;
                        textureCtx.input    = ImporterFileInput{ .filePath = texturePath };
                        textureCtx.location = ctx.genUniqueDependencyLocation<Texture>();

                        TextureImportParameters texParams;
                        texParams.textureType    = nexoType;
                        texParams.generateMipmaps = true;
                        textureCtx.setParameters(texParams);

                        assetImporter.setCustomContext(&textureCtx);

                        auto result = assetImporter.importAsset<Texture>(textureCtx.location, textureCtx.input);
                        if (result) {
                            LOG(NEXO_INFO, "Successfully loaded external texture '{}' as type {}",
                                texturePathStr, static_cast<int>(nexoType));
                            return result;
                        }
                    }
                }

                return nullptr;
            };
            auto loadTextureWithFallbacks = [&](const std::vector<std::pair<aiTextureType, renderer::TextureType>>& typeOptions) -> AssetRef<Texture> {
                for (const auto& [assimpType, nexoType] : typeOptions) {
                    if (auto texture = loadTextureWithType(assimpType, nexoType)) {
                        return texture;
                    }
                }
                return nullptr;
            };

            // Use the type-aware loader
            materialComponent->albedoTexture = loadTextureWithFallbacks({
                {aiTextureType_BASE_COLOR, renderer::TextureType::ALBEDO},
                {aiTextureType_DIFFUSE, renderer::TextureType::ALBEDO},
                {aiTextureType_UNKNOWN, renderer::TextureType::ALBEDO}  // Sometimes stored as unknown
            });

            materialComponent->normalMap = loadTextureWithFallbacks({
                {aiTextureType_NORMAL_CAMERA, renderer::TextureType::NORMAL},
                {aiTextureType_NORMALS, renderer::TextureType::NORMAL},
                {aiTextureType_HEIGHT, renderer::TextureType::NORMAL}  // Sometimes height is used for normals
            });

            materialComponent->metallicMap = loadTextureWithFallbacks({
                {aiTextureType_METALNESS, renderer::TextureType::METALLIC},
                {aiTextureType_SPECULAR, renderer::TextureType::METALLIC},  // Legacy
                {aiTextureType_REFLECTION, renderer::TextureType::METALLIC} // Alternative
            });

            materialComponent->roughnessMap = loadTextureWithFallbacks({
                {aiTextureType_DIFFUSE_ROUGHNESS, renderer::TextureType::ROUGHNESS},
                {aiTextureType_SHININESS, renderer::TextureType::ROUGHNESS},  // Legacy conversion
                {aiTextureType_UNKNOWN, renderer::TextureType::ROUGHNESS}
            });

            materialComponent->aoMap = loadTextureWithFallbacks({
                {aiTextureType_AMBIENT_OCCLUSION, renderer::TextureType::AO},
                {aiTextureType_LIGHTMAP, renderer::TextureType::AO},
                {aiTextureType_UNKNOWN, renderer::TextureType::AO}
            });

            materialComponent->emissiveMap = loadTextureWithFallbacks({
                {aiTextureType_EMISSIVE, renderer::TextureType::EMISSIVE},
                {aiTextureType_EMISSION_COLOR, renderer::TextureType::EMISSIVE}
            });

            materialComponent->heightMap = loadTextureWithFallbacks({
                {aiTextureType_HEIGHT, renderer::TextureType::HEIGHT},
                {aiTextureType_DISPLACEMENT, renderer::TextureType::HEIGHT}
            });

            auto loadMetallicRoughnessTexture = [&]() -> AssetRef<Texture> {
                // Try multiple approaches to find combined textures
                std::vector<std::pair<std::string, renderer::TextureType>> patterns = {
                    {"metallicRoughness", renderer::TextureType::COMBINED_MR},
                    {"metallic_roughness", renderer::TextureType::COMBINED_MR},
                    {"metalRough", renderer::TextureType::COMBINED_MR},
                    {"mr.", renderer::TextureType::COMBINED_MR},
                    {"orm", renderer::TextureType::COMBINED_ORM},
                    {"occlusion", renderer::TextureType::COMBINED_ORM}
                };

                // Method 1: Check all UNKNOWN textures for naming patterns
                for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_UNKNOWN); ++i) {
                    aiString path;
                    if (material->GetTexture(aiTextureType_UNKNOWN, i, &path) == AI_SUCCESS) {
                        std::string pathStr = path.C_Str();
                        std::transform(pathStr.begin(), pathStr.end(), pathStr.begin(), ::tolower);

                        for (const auto& [pattern, textureType] : patterns) {
                            if (pathStr.find(pattern) != std::string::npos) {
                                LOG(NEXO_INFO, "Found combined texture '{}' with pattern '{}'", path.C_Str(), pattern);

                                AssetImporter assetImporter;
                                AssetImporterContext textureCtx;
                                textureCtx.input = ImporterFileInput{.filePath = (modelDirectory / path.C_Str()).lexically_normal()};
                                textureCtx.location = ctx.genUniqueDependencyLocation<Texture>();

                                TextureImportParameters texParams;
                                texParams.textureType = textureType;
                                texParams.generateMipmaps = true;
                                textureCtx.setParameters(texParams);

                                assetImporter.setCustomContext(&textureCtx);

                                try {
                                    auto result = assetImporter.importAsset<Texture>(textureCtx.location, textureCtx.input);
                                    if (result) {
                                        return result;
                                    }
                                } catch (const std::exception& e) {
                                    LOG(NEXO_WARN, "Failed to load combined texture '{}': {}", path.C_Str(), e.what());
                                }
                            }
                        }
                    }
                }

                return nullptr;
            };

            materialComponent->metallicRoughnessMap = loadMetallicRoughnessTexture();
            LOG(NEXO_INFO, "Material {} PBR Summary:", matIdx);
            LOG(NEXO_INFO, "  Albedo: ({},{},{},{}) (texture: {})",
                materialComponent->albedoColor.x, materialComponent->albedoColor.y,
                materialComponent->albedoColor.z, materialComponent->albedoColor.w,
                materialComponent->albedoTexture ? "Yes" : "No");
            LOG(NEXO_INFO, "  Normal: {} (scale: {})",
                materialComponent->normalMap ? "Yes" : "No",
                materialComponent->normalScale);
            LOG(NEXO_INFO, "  Material: Metallic={} (tex: {}), Roughness={} (tex: {})",
                materialComponent->metallic, materialComponent->metallicMap ? "Yes" : "No",
                materialComponent->roughness, materialComponent->roughnessMap ? "Yes" : "No");
            LOG(NEXO_INFO, "  Combined MR: {}", materialComponent->metallicRoughnessMap ? "Yes" : "No");
            LOG(NEXO_INFO, "  AO: {} (tex: {})", materialComponent->ao, materialComponent->aoMap ? "Yes" : "No");
            LOG(NEXO_INFO, "  Emissive: ({},{},{}) (tex: {})",
                materialComponent->emissiveColor.x, materialComponent->emissiveColor.y,
                materialComponent->emissiveColor.z,
                materialComponent->emissiveMap ? "Yes" : "No");

            const auto materialRef = AssetCatalog::getInstance().createAsset<Material>(
                ctx.genUniqueDependencyLocation<Material>(), std::move(materialComponent));
            m_materials[matIdx] = materialRef;
        } // end for (int matIdx = 0; matIdx < scene->mNumMaterials; ++matIdx)
    }

    MeshNode ModelImporter::processNode(AssetImporterContext& ctx, aiNode const* node, const aiScene* scene, const glm::mat4& parentToRoot)
    {
        auto meshNode = MeshNode{};

        meshNode.name = node->mName.C_Str();

        const glm::mat4 nodeTransform = convertAssimpMatrixToGLM(node->mTransformation);
        meshNode.transform            = nodeTransform;

        const glm::mat4 nodeToRoot = parentToRoot * nodeTransform;

        meshNode.meshes.reserve(node->mNumMeshes);

        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* m = scene->mMeshes[node->mMeshes[i]];
            Mesh out  = processMesh(ctx, m, scene);

            // local bounds union (meshes only)
            meshNode.localBounds = math::aabbUnion(meshNode.localBounds, out.localBounds);

            // model-space union: transform mesh local AABB by this nodeToRoot
            const math::AABB w = math::aabbTransform(out.localBounds, nodeToRoot);
            meshNode.modelBounds = math::aabbUnion(meshNode.modelBounds, w);

            meshNode.meshes.emplace_back(std::move(out));
        }

        meshNode.children.reserve(node->mNumChildren);

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            MeshNode child = processNode(ctx, node->mChildren[i], scene, nodeToRoot);

            meshNode.modelBounds = math::aabbUnion(meshNode.modelBounds, child.modelBounds);
            meshNode.children.emplace_back(std::move(child));
        }
        meshNode.modelSphere = math::sphereFromAABB(meshNode.modelBounds);

        return meshNode;
    }

    Mesh ModelImporter::processMesh(const AssetImporterContext& ctx, aiMesh* mesh,
                                    [[maybe_unused]] const aiScene* scene) const
    {
        std::shared_ptr<renderer::NxVertexArray> vao = renderer::createVertexArray();
        auto vertexBuffer = renderer::createVertexBuffer(mesh->mNumVertices * sizeof(renderer::NxVertex));
        const renderer::NxBufferLayout cubeVertexBufferLayout = {
            {renderer::NxShaderDataType::FLOAT3, "aPos"},       {renderer::NxShaderDataType::FLOAT2, "aTexCoord"},
            {renderer::NxShaderDataType::FLOAT3, "aNormal"},    {renderer::NxShaderDataType::FLOAT3, "aTangent"},
            {renderer::NxShaderDataType::FLOAT3, "aBiTangent"}, {renderer::NxShaderDataType::INT, "aEntityID"}};
        vertexBuffer->setLayout(cubeVertexBufferLayout);
        std::vector<renderer::NxVertex> vertices;
        std::vector<unsigned int> indices;
        vertices.reserve(mesh->mNumVertices);

        glm::vec3 minBB(+FLT_MAX, +FLT_MAX, +FLT_MAX);
        glm::vec3 maxBB(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            renderer::NxVertex vertex{};
            vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

            minBB.x = std::min(minBB.x, vertex.position.x);
            minBB.y = std::min(minBB.y, vertex.position.y);
            minBB.z = std::min(minBB.z, vertex.position.z);

            maxBB.x = std::max(maxBB.x, vertex.position.x);
            maxBB.y = std::max(maxBB.y, vertex.position.y);
            maxBB.z = std::max(maxBB.z, vertex.position.z);

            if (mesh->HasNormals()) {
                vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
            }

            if (mesh->HasTangentsAndBitangents()) {
                vertex.tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
                vertex.bitangent = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
            } else {
                // Generate default tangent space if not available
                vertex.tangent = {1.0f, 0.0f, 0.0f};
                vertex.bitangent = {0.0f, 1.0f, 0.0f};
            }

            if (mesh->mTextureCoords[0])
                vertex.texCoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            else
                vertex.texCoord = {0.0f, 0.0f};

            vertices.push_back(vertex);
        }

        glm::vec3 centerLocal = (minBB + maxBB) * 0.5f;

        math::AABB bounds{};
        bounds.min = minBB;
        bounds.max = maxBB;
        math::BSphere sphere = math::sphereFromAABB(bounds);

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
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
            LOG(NEXO_ERROR, "ModelImporter: Model {}: Mesh {} has invalid material index {}.",
                std::quoted(ctx.location.getFullLocation()), std::quoted(mesh->mName.C_Str()), mesh->mMaterialIndex);
        }
        if (!materialComponent) {
            LOG(NEXO_WARN, "ModelImporter: Model {}: Mesh {} has no material.",
                std::quoted(ctx.location.getFullLocation()), std::quoted(mesh->mName.C_Str()));
        }

        LOG(NEXO_INFO, "Loaded mesh {}", mesh->mName.C_Str());
        return {mesh->mName.C_Str(), vao, materialComponent, centerLocal, bounds, sphere};
    }

    glm::mat4 ModelImporter::convertAssimpMatrixToGLM(const aiMatrix4x4& matrix)
    {
        return {matrix.a1, matrix.b1, matrix.c1, matrix.d1, matrix.a2, matrix.b2, matrix.c2, matrix.d2,
                matrix.a3, matrix.b3, matrix.c3, matrix.d3, matrix.a4, matrix.b4, matrix.c4, matrix.d4};
    }

} // namespace nexo::assets
