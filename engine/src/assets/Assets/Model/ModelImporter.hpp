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
#include "assets/Assets/Model/Model.hpp"
#include "assets/AssetRef.hpp"

namespace nexo::assets {

    class ModelImporter : public AssetImporterBase {
        public:
            ModelImporter() = default;
            ~ModelImporter() override = default;

            bool canRead(const ImporterInputVariant& inputVariant) override;

            void importImpl(AssetImporterContext& ctx) override;

        protected:
            std::unique_ptr<Model> loadModel(AssetImporterContext& ctx);
            void loadSceneEmbeddedTextures(AssetImporterContext& ctx, const aiScene* scene);
            AssetRef<Texture> loadEmbeddedTexture(AssetImporterContext& ctx, aiTexture *texture);
            void loadSceneMaterials(AssetImporterContext& ctx, const aiScene* scene);

            MeshNode processNode(AssetImporterContext& ctx, aiNode const *node, const aiScene* scene);
            Mesh processMesh(AssetImporterContext& ctx, aiMesh* mesh, const aiScene* scene);

            static renderer::NxTextureFormat convertAssimpHintToNxTextureFormat(const char achFormatHint[9]);
            static glm::mat4 convertAssimpMatrixToGLM(const aiMatrix4x4& matrix);

            Assimp::Importer m_importer;              //< Assimp importer instance
            std::unordered_map<const char *, AssetRef<Texture>> m_textures; //< Map of textures used in the model, std::string is the texture name (path, or *0, *1, etc. for embedded textures, see assimp)
            std::vector<AssetRef<Material>> m_materials; //< Map of materials used in the model, index is the assimp material index
    };

} // namespace nexo::assets
