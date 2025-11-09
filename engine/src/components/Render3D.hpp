//// Render3D.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
        // Core PBR properties
        glm::vec4 albedoColor   = glm::vec4(1.0f);
        glm::vec3 emissiveColor = glm::vec3(0.0f);

        // PBR material properties
        float metallic     = 0.0f;  // 0 = dielectric, 1 = metallic
        float roughness    = 0.5f;  // 0 = mirror smooth, 1 = completely rough
        float ao           = 1.0f;  // Ambient occlusion factor
        float normalScale  = 1.0f;  // Normal map strength
        float opacity      = 1.0f;  // 1 = opaque, 0 = transparent

        // Material state
        bool isOpaque = true;

        // Texture references
        assets::AssetRef<assets::Texture> albedoTexture = nullptr;
        assets::AssetRef<assets::Texture> normalMap     = nullptr;
        assets::AssetRef<assets::Texture> metallicMap   = nullptr;
        assets::AssetRef<assets::Texture> roughnessMap  = nullptr;
        assets::AssetRef<assets::Texture> metallicRoughnessMap = nullptr;  // Combined MR texture
        assets::AssetRef<assets::Texture> aoMap         = nullptr;
        assets::AssetRef<assets::Texture> emissiveMap   = nullptr;
        assets::AssetRef<assets::Texture> heightMap     = nullptr;

        // Legacy properties (for backward compatibility)
        glm::vec4 specularColor = glm::vec4(1.0f);  // For legacy Blinn-Phong materials
        std::string shader = "PBR";  // Changed default to PBR

        // Serialization support
        struct Memento {
            glm::vec4 albedoColor;
            glm::vec3 emissiveColor;
            float metallic, roughness, ao, normalScale, opacity;
            bool isOpaque;
            glm::vec4 specularColor;
            std::string shader;
        };

        void restore(const Memento& memento) {
            albedoColor = memento.albedoColor;
            emissiveColor = memento.emissiveColor;
            metallic = memento.metallic;
            roughness = memento.roughness;
            ao = memento.ao;
            normalScale = memento.normalScale;
            opacity = memento.opacity;
            isOpaque = memento.isOpaque;
            specularColor = memento.specularColor;
            shader = memento.shader;
        }

        [[nodiscard]] Memento save() const {
            return {albedoColor, emissiveColor, metallic, roughness, ao,
                    normalScale, opacity, isOpaque, specularColor, shader};
        }
    };
} // namespace nexo::components
