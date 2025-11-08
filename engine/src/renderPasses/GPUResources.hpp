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
        // vec4 albedoColor;
        glm::vec4 albedoColor;          // 0..15

        // int albedoTexIndex;
        int  albedoTexIndex;            // 16..19
        int  _pad0[3];                  // 20..31 (pad so next vec4 is 16-byte aligned)

        // vec4 specularColor;
        glm::vec4 specularColor;        // 32..47

        // int specularTexIndex;
        int  specularTexIndex;          // 48..51
        int  _pad1[3];                  // 52..63

        // vec3 emissiveColor;
        glm::vec3 emissiveColor;        // 64..75
        float _pad2;                    // 76..79 (pad to 16-byte boundary)

        // int emissiveTexIndex;
        int  emissiveTexIndex;          // 80..83

        // float roughness;
        float roughness;                // 84..87

        // int roughnessTexIndex;
        int  roughnessTexIndex;         // 88..91

        // float metallic;
        float metallic;                 // 92..95

        // int metallicTexIndex;
        int  metallicTexIndex;          // 96..99

        // float opacity;
        float opacity;                  // 100..103

        // int opacityTexIndex;
        int  opacityTexIndex;           // 104..107

        int _pad3[1];                   // 108..111 → struct size 112 bytes
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
