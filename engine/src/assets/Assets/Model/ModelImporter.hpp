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
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "assets/AssetImporterBase.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "ModelParameters.hpp"

namespace nexo::assets {

    class ModelImporter final : public AssetImporterBase {
        public:
            /**
 * @brief Constructs a new ModelImporter object.
 *
 * Initializes a new instance of the ModelImporter class with default settings.
 */
ModelImporter() = default;
            /**
 * @brief Default destructor for ModelImporter.
 *
 * Overrides the AssetImporterBase destructor.
 */
~ModelImporter() override = default;

            /**
             * @brief Determines if the input variant's file extension is supported.
             *
             * Extracts the file extension from the input variant, which may represent either a file-based input or a memory-based input.
             * For file inputs, the extension is obtained from the file path. For memory inputs, the extension is taken from the provided
             * file extension value if available. A temporary Assimp importer instance is then used to check if this extension is supported.
             *
             * @param inputVariant Variant containing file or memory input details.
             * @return true if the file extension is supported by Assimp, false otherwise.
             */
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

            /**
             * @brief Imports a 3D model using the Assimp library.
             *
             * This method initializes a new Model instance and its associated data, then attempts to load a 3D scene using Assimp.
             * It configures processing flags for triangulation, UV flipping, and normal generation, and reads the scene from either a file
             * or memory based on the input provided in the asset importer context. If the scene is null, incomplete, or missing a root node,
             * an error is logged. Upon successfully loading the scene, the model is set as the main asset in the context.
             *
             * @param ctx The asset importer context containing input data and import parameters.
             */
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
