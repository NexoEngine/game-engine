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
#include <components/Shapes3D.hpp>

#include "assets/AssetImporterBase.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "ModelParameters.hpp"

namespace nexo::assets {

    class ModelImporter final : public AssetImporterBase {
        public:
            ModelImporter() = default;
            ~ModelImporter() override = default;

            bool canRead(const ImporterInputVariant& inputVariant) override;

            void importImpl(AssetImporterContext& ctx) override;

        protected:
            Model *m_model = nullptr; //< Model being imported
            Assimp::Importer m_importer;              //< Assimp importer instance

        private:
            Model* loadModel(AssetImporterContext& ctx);
            std::shared_ptr<components::MeshNode> processNode(AssetImporterContext& ctx, aiNode const *node, const aiScene* scene);
            components::Mesh processMesh(AssetImporterContext& ctx, aiMesh* mesh, const aiScene* scene);
            static glm::mat4 convertAssimpMatrixToGLM(const aiMatrix4x4& matrix);

    };

} // namespace nexo::assets
