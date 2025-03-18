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
 * @brief Default constructor for the ModelImporter class.
 *
 * Instantiates a new ModelImporter object using the compiler-generated default initialization.
 */
ModelImporter() = default;
            /**
 * @brief Destroys the ModelImporter instance and releases associated resources.
 *
 * As a default destructor, it ensures proper cleanup when a ModelImporter object is destroyed.
 */
~ModelImporter() override = default;

            /**
             * @brief Determines whether the file extension from the input variant is supported by Assimp.
             *
             * Extracts the file extension from the input variant—which can represent either a file-based or memory-based input—and
             * checks if it is recognized by Assimp.
             *
             * @param inputVariant The variant that holds input data with an associated file extension.
             * @return true if Assimp supports the file extension; false otherwise.
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
             * This method creates a new Model instance with associated ModelData, then retrieves import
             * parameters and configures processing flags for Assimp. It attempts to load the model scene
             * either from a file path or memory, depending on the input type provided in the import
             * context. If the scene is invalid, incomplete, or missing a root node, an error is logged.
             * On success, the scene is stored in the model data and the model is registered as the main asset
             * within the provided context.
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
