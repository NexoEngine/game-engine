//// GPUResources.hpp ///////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        07/11/2025
//  Description: Header file that contains types and constants for gpu resources
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace nexo::renderer {
    #define PER_VIEW_UBO "PerView"

    struct GpuPerView {
        glm::mat4 uViewProjection;
        glm::vec3 uCamPos;
        float _pad0; // to match std140 (vec3 -> 16 bytes)
    };

    #define LIGHT_UBO "LightBlock"

    constexpr int MAX_POINT_LIGHTS = 10;
    constexpr int MAX_SPOT_LIGHTS  = 10;

    struct GpuDirectionalLight {
        glm::vec3 direction;
        float _pad0;         // matches GLSL
        glm::vec4 color;
    };

    struct GpuPointLight {
        glm::vec3 position;
        float _pad1;
        glm::vec4 color;
        float constant;
        float linear;
        float quadratic;
        float _pad2;         // pad to multiple of 16 bytes
    };

    struct GpuSpotLight {
        glm::vec3 position;
        float _pad3;

        glm::vec3 direction;
        float _pad4;

        glm::vec4 color;

        float cutOff;
        float outerCutoff;
        float constant;
        float linear;

        float quadratic;
        float _pad5;
        float _pad6;
        float _pad7;
    };

    struct GpuLightBlock {
        glm::vec3 ambientLight;
        float _padAmbient;

        GpuDirectionalLight dirLight;

        GpuPointLight pointLights[MAX_POINT_LIGHTS];
        GpuSpotLight  spotLights[MAX_SPOT_LIGHTS];

        int numPointLights;
        int numSpotLights;
        int _padCount0;
        int _padCount1;
    };

    #define INSTANCE_BUFFER "ModelMatrices"

    struct GpuInstanceData {
        glm::mat4 model;
        int       entityId;
        int materialIndex;
        int       _pad0;
        int       _pad1;
    };

    #define MATERIAL_BUFFER "MaterialBuffer"

    struct GpuMaterial {
        // Base color (albedo) - 16 bytes aligned
        glm::vec4 albedoColor;          // 0..15

        // Texture indices - 16 bytes aligned
        int albedoTexIndex;             // 16..19
        int emissiveTexIndex;           // 20..23
        int metallicTexIndex;           // 24..27
        int roughnessTexIndex;          // 28..31

        // Emissive color + metallic - 16 bytes aligned
        glm::vec3 emissiveColor;        // 32..43
        float metallic;                 // 44..47

        // Surface properties - 16 bytes aligned
        float roughness;                // 48..51
        float ao;                       // 52..55
        float normalScale;              // 56..59
        float opacity;                  // 60..63

        // More texture indices - 16 bytes aligned
        int aoTexIndex;                 // 64..67
        int normalTexIndex;             // 68..71
        int opacityTexIndex;            // 72..75
        int ormTexIndex;                // 76..79

        // Explicit padding to match std430
        int _pad0[4];                   // 80..95
    };

    #define NB_RESERVED "NB_RESERVED"

    inline const std::unordered_map<std::string, unsigned int> RESERVED_BINDING_POINTS = {
        {INSTANCE_BUFFER, 0},
        {PER_VIEW_UBO, 1},
        {LIGHT_UBO, 2},
        {MATERIAL_BUFFER, 3},
        {NB_RESERVED, 4}
    };

}
