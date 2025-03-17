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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "assets/AssetImporterBase.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "ModelParameters.hpp"

namespace nexo::assets {

    class ModelImporter final : public AssetImporterBase {
        public:
            ModelImporter() = default;
            ~ModelImporter() override = default;

            bool canRead(const ImporterInputVariant& inputVariant) override
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

            void importImpl(AssetImporterContext& ctx) override
            {
                m_model = new Model();
                m_model->setData(new ModelData());
                const auto param = ctx.getParameters<ModelImportParameters>();
                int flags = aiProcess_Triangulate
                    | aiProcess_FlipUVs
                    | aiProcess_GenNormals;
                const aiScene* scene = nullptr;
                if (std::holds_alternative<ImporterFileInput>(ctx.input))
                    scene = m_importer.ReadFile(std::get<ImporterFileInput>(ctx.input).filePath.string(), flags);
                if (std::holds_alternative<ImporterMemoryInput>(ctx.input)) {
                    auto memInput = std::get<ImporterMemoryInput>(ctx.input);
                    scene = m_importer.ReadFileFromMemory(memInput.memoryData.data(), memInput.memoryData.size(), flags, memInput.fileExtension ? memInput.fileExtension->c_str() : nullptr);
                }
                if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                    //log error TODO: improve error handling in importers
                    auto error = m_importer.GetErrorString();
                    LOG(NEXO_ERROR, "Error while importing model: {}: {}", ctx.location.getPath(), error);
                }
                m_model->data->scene = scene;
                ctx.setMainAsset(m_model);
            }

        protected:
            Model *m_model = nullptr; //< Model being imported
            Assimp::Importer m_importer;              //< Assimp importer instance
    };

} // namespace nexo::assets
