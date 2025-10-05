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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "assets/AssetImporterBase.hpp"
#include "assets/AssetRef.hpp"
#include "assets/Assets/Model/Model.hpp"

namespace nexo::assets {

    constexpr float OPACITY_THRESHOLD    = 0.99f;
    constexpr float TRANSPARENCY_EPSILON = 0.01f;

    class ModelImporter : public AssetImporterBase {
       public:
        ModelImporter()           = default;
        ~ModelImporter() override = default;

        /**
         * @brief Check if the importer can read the given input variant.
         *
         * This function checks if the Assimp library supports the file extension
         * or format hint provided in the input variant.
         *
         * @param inputVariant The input variant containing either a file path or memory data with a format hint.
         * @return true if the importer can read the input variant, false otherwise.
         */
        bool canRead(const ImporterInputVariant& inputVariant) override;

        /**
         * @brief Import the model from the given context.
         *
         * This function loads the model using Assimp, processes its nodes and meshes,
         * and populates the Model asset with the loaded data.
         *
         * @param ctx The asset importer context containing input data and parameters.
         */
        void importImpl(AssetImporterContext& ctx) override;

       protected:
        /**
         * @brief Load the model from the given context.
         *
         * This function uses Assimp to read the model file or memory data,
         * processes embedded textures and materials, and constructs the model's
         * mesh hierarchy.
         * @param ctx The asset importer context containing input data and parameters.
         * @return A unique pointer to the loaded Model asset.
         * @throws core::LoadModelException if the model cannot be loaded.
         */
        std::unique_ptr<Model> loadModel(AssetImporterContext& ctx);

        /**
         * @brief Load embedded textures from the Assimp scene.
         *
         * This function iterates over the embedded textures in the Assimp scene
         * and loads them into the asset catalog.
         * @param ctx The asset importer context.
         * @param scene The Assimp scene containing the embedded textures.
         */
        void loadSceneEmbeddedTextures(AssetImporterContext& ctx, const aiScene* scene);

        /**
         * @brief Load materials from the Assimp scene.
         *
         * This function iterates over the materials in the Assimp scene,
         * extracts their properties and associated textures, and loads them
         * into the asset catalog.
         * @param ctx The asset importer context.
         * @param texture The embedded texture to load.
         * @return An AssetRef to the loaded Texture asset.
         */
        AssetRef<Texture> loadEmbeddedTexture(AssetImporterContext& ctx, aiTexture* texture);

        /**
         * @brief Load materials from the Assimp scene.
         *
         * This function iterates over the materials in the Assimp scene,
         * extracts their properties and associated textures, and loads them
         * into the asset catalog.
         * @param ctx The asset importer context.
         * @param scene The Assimp scene containing the materials.
         */
        void loadSceneMaterials(AssetImporterContext& ctx, const aiScene* scene);

        /**  * @brief Process an Assimp node and its children recursively.
         *
         * This function processes the given Assimp node, extracts its meshes,
         * and recursively processes its child nodes to build a hierarchical
         * representation of the model.
         *
         * @param ctx The asset importer context.
         * @param node The Assimp node to process.
         * @param scene The Assimp scene containing the node.
         * @return A MeshNode representing the processed node and its children.
         */
        MeshNode processNode(AssetImporterContext& ctx, aiNode const* node, const aiScene* scene, const glm::mat4& parentToRoot = glm::mat4(1.0f));

        /**
         * @brief Process an Assimp mesh and convert it to the engine's Mesh format.
         *
         * This function extracts vertex data, indices, and material information
         * from the given Assimp mesh and constructs a Mesh object compatible with
         * the engine's rendering system.
         *
         * @param ctx The asset importer context.
         * @param mesh The Assimp mesh to process.
         * @param scene The Assimp scene containing the mesh.
         * @return A Mesh representing the processed Assimp mesh.
         */
        Mesh processMesh(const AssetImporterContext& ctx, aiMesh* mesh, const aiScene* scene) const;

        /**
         * @brief Convert Assimp texture format hint to engine texture format.
         *
         * This function maps the Assimp texture format hint to the corresponding
         * texture format used by the engine's rendering system.
         *
         * @param achFormatHint The Assimp texture format hint (e.g., "png", "jpg").
         * @return The corresponding NxTextureFormat, or NxTextureFormat::INVALID if unsupported.
         */
        static renderer::NxTextureFormat convertAssimpHintToNxTextureFormat(const char achFormatHint[9]);

        /**
         * @brief Convert an Assimp matrix to a GLM matrix.
         *
         * This function converts a 4x4 matrix from Assimp's aiMatrix4x4 format
         * to GLM's glm::mat4 format.
         *
         * @param matrix The Assimp matrix to convert.
         * @return The converted GLM matrix.
         */
        static glm::mat4 convertAssimpMatrixToGLM(const aiMatrix4x4& matrix);

        Assimp::Importer m_importer; //< Assimp importer instance
        std::unordered_map<const char*, AssetRef<Texture>>
            m_textures; //< Map of textures used in the model, std::string is the texture name (path, or *0, *1, etc.
                        // for embedded textures, see assimp)
        std::vector<AssetRef<Material>>
            m_materials; //< Map of materials used in the model, index is the assimp material index
    };

} // namespace nexo::assets
