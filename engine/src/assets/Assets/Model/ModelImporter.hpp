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
            static constexpr std::array FILE_EXTENSIONS{".obj", ".fbx", ".3ds", ".blend", ".dae", ".ply", ".stl", ".x3d"};

            ModelImporter() = default;
            ~ModelImporter() override = default;

            bool canRead(const std::filesystem::path fsPath) override
            {
                const Assimp::Importer importer;
                return importer.IsExtensionSupported(fsPath.extension().string());
            }

            Model* importImpl(std::filesystem::path fsPath) override
            {
                const auto model = new Model();
                const aiScene *scene = model->importer.ReadFile(fsPath.string(),
                    aiProcess_Triangulate
                    | aiProcess_FlipUVs
                    | aiProcess_GenNormals);
                if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
                    return nullptr;
                model->data->scene = scene;
                return model;
            }
    };

} // namespace nexo::assets
