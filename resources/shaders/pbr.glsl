#type vertex
#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

struct InstanceData {
    mat4 model;
    int  entityId;
    int  materialIndex;
    // padding for std430: an int is 4 bytes, but the next element (if any)
    // would need 16-byte alignment, so you can add 3 ints or a vec3 if needed later.
    int _pad0;
    int _pad1;
};

layout(std430, binding = 0) buffer Instances {
    InstanceData uInstances[];
};

layout(std140, binding = 1) uniform PerView {
    mat4 uViewProjection;
    vec3 uCamPos;
    float _pad0;   // padding to satisfy std140 (vec3 takes 16 bytes)
};

uniform int uInstanceOffset; // per-draw, shared across instances in this batch
uniform mat4 uDirLightViewProj;

out vec3 vFragPos;
out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBitangent;
flat out int vEntityId;
flat out int vMaterialIndex;
out vec4 vLightSpacePos;

void main()
{
    int idx = uInstanceOffset + int(gl_InstanceID);
    mat4 model = uInstances[idx].model;
    vEntityId = uInstances[idx].entityId;
    vMaterialIndex = uInstances[idx].materialIndex;

    vec4 worldPos = model * vec4(aPos, 1.0);
    vFragPos = worldPos.xyz;
    vTexCoord = aTexCoord;

    // Transform normal and tangent space to world space
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    vNormal = normalize(normalMatrix * aNormal);
    vTangent = normalize(normalMatrix * aTangent);
    vBitangent = normalize(normalMatrix * aBitangent);
    vLightSpacePos = uDirLightViewProj * worldPos;

    gl_Position = uViewProjection * vec4(vFragPos, 1.0);
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

// Light definitions.
struct DirectionalLight {
    vec3 direction;
    float _pad0;  // pad vec3 to 16 bytes
    vec4 color;
};

struct PointLight {
    vec3 position;
    float _pad1;  // pad
    vec4 color;
    float constant;
    float linear;
    float quadratic;
    float _pad2;  // pad to 16-byte multiple
};

struct SpotLight {
    vec3 position;
    float _pad3;

    vec3 direction;
    float _pad4;

    vec4 color;

    float cutOff;
    float outerCutoff;
    float constant;
    float linear;

    float quadratic;
    float _pad5;
    float _pad6;
    float _pad7;
};

// Light block UBO
layout(std140, binding = 2) uniform LightBlock {
    vec3 uAmbientLight;
    float _padAmbient;

    DirectionalLight uDirLight;

    PointLight uPointLights[MAX_POINT_LIGHTS];
    SpotLight  uSpotLights[MAX_SPOT_LIGHTS];

    int uNumPointLights;
    int uNumSpotLights;
    int _padCount0;
    int _padCount1;
};

in vec3 vFragPos;
in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBitangent;
flat in int vEntityId;
flat in int vMaterialIndex;
in vec4 vLightSpacePos;

uniform sampler2D uTexture[32];

layout(std140, binding = 1) uniform PerView {
    mat4 uViewProjection;
    vec3 uCamPos;
    float _pad0;   // padding to satisfy std140 (vec3 takes 16 bytes)
};

uniform int  uDirShadowTexIndex; // index into uTexture[] used for the shadow map

struct Material {
    vec4 albedoColor;           // 0..15

    int albedoTexIndex;         // 16..19
    int emissiveTexIndex;       // 20..23
    int metallicTexIndex;       // 24..27
    int roughnessTexIndex;      // 28..31

    vec3 emissiveColor;         // 32..43
    float metallic;             // 44..47

    float roughness;            // 48..51
    float ao;                   // 52..55
    float normalScale;          // 56..59
    float opacity;              // 60..63

    int aoTexIndex;             // 64..67
    int normalTexIndex;         // 68..71
    int opacityTexIndex;        // 72..75
    int ormTexIndex;            // 76..79

    // Padding (automatically handled by std430)
    int _pad0[4];                   // 80..95
};

layout(std430, binding = 3) buffer MaterialBuffer {
    Material uMaterials[];
};

// Constants
const float PI = 3.14159265359;

// Normal Distribution Function (GGX/Trowbridge-Reitz)
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

// Geometry Function (Smith's method)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Fresnel equation (Schlick's approximation)
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// ACES tone mapping (more cinematic than Reinhard)
vec3 ACESFilm(vec3 x) {
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}

// Enhanced normal mapping with proper tangent space
vec3 getNormalFromMap(Material material, vec2 texCoords, vec3 normal, vec3 tangent, vec3 bitangent)
{
    if (material.normalTexIndex == 0) {
        return normalize(normal);
    }

    vec3 tangentNormal = texture(uTexture[material.normalTexIndex], texCoords).xyz * 2.0 - 1.0;

    // Apply normal map strength
    tangentNormal.xy *= material.normalScale;
    tangentNormal = normalize(tangentNormal);

    // Build TBN matrix with proper orthogonalization
    vec3 N = normalize(normal);
    vec3 T = normalize(tangent);
    vec3 B = normalize(bitangent);

    // Re-orthogonalize T with respect to N (Gram-Schmidt process)
    T = normalize(T - dot(T, N) * N);

    // Ensure proper handedness
    if (dot(cross(N, T), B) < 0.0) {
        T = T * -1.0;
    }

    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangentNormal);
}

vec3 CalcDirLightPBR(Material material, DirectionalLight light, vec3 N, vec3 V, vec3 albedo, float metallic, float roughness, float ao)
{
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(V + L);

    // Calculate radiance
    vec3 radiance = light.color.rgb;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    // F0 represents the base reflectivity (dielectric surfaces: ~0.04, metals: albedo color)
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;                    // Specular reflection ratio
    vec3 kD = vec3(1.0) - kS;       // Diffuse reflection ratio
    kD *= 1.0 - metallic;           // Metallic surfaces don't refract light

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    // Add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcPointLightPBR(Material material, PointLight light, vec3 N, vec3 V, vec3 fragPos, vec3 albedo, float metallic, float roughness, float ao)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 H = normalize(V + L);

    // Calculate attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    vec3 radiance = light.color.rgb * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

vec3 CalcSpotLightPBR(Material material, SpotLight light, vec3 N, vec3 V, vec3 fragPos, vec3 albedo, float metallic, float roughness, float ao)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 H = normalize(V + L);

    // Calculate attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // Spotlight intensity
    float theta = dot(L, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    vec3 radiance = light.color.rgb * attenuation * intensity;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    float NdotL = max(dot(N, L), 0.0);
    return (kD * albedo / PI + specular) * radiance * NdotL;
}

float CalcShadowFactor(vec4 lightSpacePos, vec3 normal, vec3 lightDir)
{
    // From clip space to NDC
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;

    // NDC [-1,1] -> [0,1]
    projCoords = projCoords * 0.5 + 0.5;

    // Outside light frustum → fully lit
    if (projCoords.z > 1.0 ||
        projCoords.x < 0.0 || projCoords.x > 1.0 ||
        projCoords.y < 0.0 || projCoords.y > 1.0)
    {
        return 1.0;
    }

    float currentDepth = projCoords.z;

    // Depth bias to reduce acne
    float cosTheta = max(dot(normalize(normal), normalize(-lightDir)), 0.0);
    float bias = max(0.0015 * (1.0 - cosTheta), 0.0005);

    // 5x5 PCF kernel for softer shadows
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uTexture[uDirShadowTexIndex], 0);

    for (int x = -2; x <= 2; ++x) {
        for (int y = -2; y <= 2; ++y) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            float closestDepth = texture(uTexture[uDirShadowTexIndex],
                                         projCoords.xy + offset).r;
            shadow += (currentDepth - bias > closestDepth ? 1.0 : 0.0);
        }
    }

    shadow /= 25.0;

    // 1 = fully lit, 0 = fully shadowed
    return 1.0 - shadow;
}


void main()
{
    Material material = uMaterials[vMaterialIndex];

    // Sample albedo texture
    vec3 albedo = material.albedoColor.rgb;
    if (material.albedoTexIndex > 0) {
        albedo *= texture(uTexture[material.albedoTexIndex], vTexCoord).rgb;
    }
    float metallic = material.metallic;
    float roughness = material.roughness;
    float ao = material.ao;

    // Check for combined ORM texture first (modern workflow)
    if (material.ormTexIndex > 0) {
        vec3 orm = texture(uTexture[material.ormTexIndex], vTexCoord).rgb;
        ao *= orm.r;        // Occlusion in red channel
        roughness *= orm.g; // Roughness in green channel
        metallic *= orm.b;  // Metallic in blue channel
    } else {
        // Fall back to separate textures (legacy workflow)
        if (material.metallicTexIndex > 0) {
            metallic *= texture(uTexture[material.metallicTexIndex], vTexCoord).b;
        }
        if (material.roughnessTexIndex > 0) {
            roughness *= texture(uTexture[material.roughnessTexIndex], vTexCoord).g;
        }
        if (material.aoTexIndex > 0) {
            ao *= texture(uTexture[material.aoTexIndex], vTexCoord).r;
        }
    }

    // Sample emissive texture
    vec3 emissive = material.emissiveColor;
    if (material.emissiveTexIndex > 0) {
        emissive *= texture(uTexture[material.emissiveTexIndex], vTexCoord).rgb;
    }

    // Alpha testing
    float alpha = material.opacity;
    if (material.albedoTexIndex > 0) {
        alpha *= texture(uTexture[material.albedoTexIndex], vTexCoord).a;
    }
    if (alpha < 0.1)
        discard;

    // Calculate normal with enhanced normal mapping
    vec3 N = getNormalFromMap(material, vTexCoord, vNormal, vTangent, vBitangent);
    vec3 V = normalize(uCamPos - vFragPos);

    // Calculate reflectance at normal incidence
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    // Reflectance equation
    vec3 Lo = vec3(0.0);

    // Directional light
    vec3 Ldir = normalize(-uDirLight.direction);
    float dirShadow = CalcShadowFactor(vLightSpacePos, N, Ldir);
    vec3 dirContribution = CalcDirLightPBR(material, uDirLight, N, V, albedo, metallic, roughness, ao);
    Lo += dirContribution * dirShadow;

    // Point lights
    for (int i = 0; i < uNumPointLights; i++) {
        Lo += CalcPointLightPBR(material, uPointLights[i], N, V, vFragPos, albedo, metallic, roughness, ao);
    }

    // Spot lights
    for (int i = 0; i < uNumSpotLights; i++) {
        Lo += CalcSpotLightPBR(material, uSpotLights[i], N, V, vFragPos, albedo, metallic, roughness, ao);
    }

    // Ambient lighting (simplified IBL substitute)
    vec3 ambient = uAmbientLight * albedo * ao;

    // Combine lighting with emissive
    vec3 color = ambient + Lo + emissive;

    // ACES tone mapping with exposure adjustment
    color = ACESFilm(color * 1.8);

    // Gamma correction
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, alpha);
    EntityID = vEntityId;
}
