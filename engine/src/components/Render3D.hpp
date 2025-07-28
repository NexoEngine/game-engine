//// Render3D.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Header file for the render 3D components
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>

#include "assets/AssetRef.hpp"
#include "assets/Assets/Texture/Texture.hpp"

namespace nexo::components {

    struct Material {
        glm::vec4 albedoColor = glm::vec4(1.0f);
        glm::vec4 specularColor = glm::vec4(1.0f);
        glm::vec3 emissiveColor = glm::vec3(0.0f);

        bool isOpaque = true;

        float roughness = 0.0f;  // 0 = smooth, 1 = rough
        float metallic = 0.0f;   // 0 = non-metal, 1 = fully metallic
        float opacity = 1.0f;    // 1 = opaque, 0 = fully transparent

        assets::AssetRef<assets::Texture> albedoTexture = nullptr;
        assets::AssetRef<assets::Texture> normalMap = nullptr;
        assets::AssetRef<assets::Texture> metallicMap = nullptr;
        assets::AssetRef<assets::Texture> roughnessMap = nullptr;
        assets::AssetRef<assets::Texture> emissiveMap = nullptr;

        std::string shader = "Phong";
    };
}
