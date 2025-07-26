//// Model.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/02/2025
//  Description: Header file for the Model class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "VertexArray.hpp"
#include "assets/Asset.hpp"
#include "assets/Assets/Material/Material.hpp"

namespace nexo::assets {

    struct Mesh {
        std::string name;
        std::shared_ptr<renderer::NxVertexArray> vao;
        AssetRef<Material> material;

        glm::vec3 localCenter = {0.0f, 0.0f, 0.0f};
    };

    struct MeshNode {
        std::string name;
        glm::mat4 transform{};
        std::vector<Mesh> meshes;
        std::vector<MeshNode> children;
    };

    /**
     * @class Model
     *
     * @brief Represents a 3D model asset.
     */
    class Model final : public Asset<MeshNode, AssetType::MODEL> {
        public:
            Model() = default;

            ~Model() override = default;
    };

}
