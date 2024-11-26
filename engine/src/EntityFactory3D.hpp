//// EntityFactory3D.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Header for the 3D entity factory
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "ecs/ECSExceptions.hpp"
#include "components/Components.hpp"

namespace nexo
{
    class EntityFactory3D
    {
    public:
        static ecs::Entity createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                      glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f});
        static ecs::Entity createModel(const std::string& path, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation);
    };
}

namespace nexo::utils
{
    std::shared_ptr<components::MeshNode> loadModel(const std::string& path);
    std::shared_ptr<components::MeshNode> processNode(const std::string &path, aiNode const *node, const aiScene* scene);
    components::Mesh processMesh(const std::string &path, aiMesh* mesh, const aiScene* scene);
    glm::mat4 convertAssimpMatrixToGLM(const aiMatrix4x4& matrix);
}
