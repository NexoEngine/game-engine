//// ModelImporter.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/02/2025
//  Description: Header file for the ModelImporter class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <array>
#include <assimp/Importer.hpp>
#include <assimp/Importer.hpp>

#include "assets/AssetImporter.hpp"
#include "assets/Assets/Model/Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace nexo::assets {

    class ModelImporter final : public AssetImporter {
        public:
            ModelImporter() = default;
            ~ModelImporter() override = default;

            bool canRead(const ImporterInputVariant& inputVariant) override
            {
                const char* extension = nullptr;
                if (std::holds_alternative<ImporterFileInput>(inputVariant))
                    extension = std::get<ImporterFileInput>(inputVariant).filePath.extension().string().c_str();
                if (std::holds_alternative<ImporterMemoryInput>(inputVariant)) {
                    const auto& mem = std::get<ImporterMemoryInput>(inputVariant);
                    if (mem.fileExtension)
                        extension = mem.fileExtension->c_str();
                }
                if (!extension)
                    return false;
                const Assimp::Importer importer;
                return importer.IsExtensionSupported(extension);
            }

            void importImpl(AssetImporterContext& ctx) override
            {
                const auto model = std::make_shared<Model>();
                auto param = ctx.getParameters<ModelImportParameters>();
                int flags = aiProcess_Triangulate
                    | aiProcess_FlipUVs
                    | aiProcess_GenNormals;
                if (param.calculateTangentSpace)
                    flags |= aiProcess_CalcTangentSpace;
                if (param.joinIdenticalVertices)
                    flags |= aiProcess_JoinIdenticalVertices;
                if (param.generateSmoothNormals)
                    flags |= aiProcess_GenSmoothNormals;
                if (param.optimizeMeshes)
                    flags |= aiProcess_OptimizeMeshes;
                const aiScene* scene = nullptr;
                if (std::holds_alternative<ImporterFileInput>(ctx.input))
                    scene = model->importer.ReadFile(std::get<ImporterFileInput>(ctx.input).filePath, flags);
                if (std::holds_alternative<ImporterMemoryInput>(ctx.input)) {
                    auto memInput = std::get<ImporterMemoryInput>(ctx.input);
                    scene = model->importer.ReadFileFromMemory(memInput.memoryData.data(), memInput.memoryData.size(), flags, memInput.fileExtension ? memInput.fileExtension->c_str() : nullptr);
                }
                if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                    //log error TODO: improve error handling in importers
                    auto error = model->importer.GetErrorString();
                    LOG(NEXO_ERROR, "Error while importing model: {}: {}", ctx.location.getPath(), error);
                }
                model->data = new ModelData();
                model->data->scene = scene;
                ctx.setMainAssetData(model);
            }
    };

} // namespace nexo::assets
