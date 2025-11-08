#type vertex
#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

uniform mat4 uMatModel;

layout(std140, binding = 1) uniform PerView {
    mat4 uViewProjection;
    vec3 uCamPos;
    float _pad0;   // padding to satisfy std140 (vec3 takes 16 bytes)
};

out vec3 vFragPos;
out vec2 vTexCoord;
out vec3 vNormal;

void main()
{
    vec4 worldPos = uMatModel * vec4(aPos, 1.0);
    vFragPos = worldPos.xyz;

    vTexCoord = aTexCoord;

    vNormal = mat3(transpose(inverse(uMatModel))) * aNormal;

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

uniform sampler2D uTexture[32];

layout(std140, binding = 1) uniform PerView {
    mat4 uViewProjection;
    vec3 uCamPos;
    float _pad0;   // padding to satisfy std140 (vec3 takes 16 bytes)
};

struct Material {
    vec4 albedoColor;
    int albedoTexIndex; // Default: 0 (white texture)
    vec4 specularColor;
    int specularTexIndex; // Default: 0 (white texture)
    vec3 emissiveColor;
    int emissiveTexIndex; // Default: 0 (white texture)
    float roughness;
    int roughnessTexIndex; // Default: 0 (white texture)
    float metallic;
    int metallicTexIndex; // Default: 0 (white texture)
    float opacity;
    int opacityTexIndex; // Default: 0 (white texture)
};
uniform Material uMaterial;

uniform int uEntityId;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = mix(128.0, 2.0, uMaterial.roughness);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 diffuse = light.color.rgb * diff * uMaterial.albedoColor.rgb * vec3(texture(uTexture[uMaterial.albedoTexIndex], vTexCoord));
    vec3 specular = light.color.rgb * spec * uMaterial.specularColor.rgb * vec3(texture(uTexture[uMaterial.specularTexIndex], vTexCoord));
    return (diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = mix(128.0, 2.0, uMaterial.roughness);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 diffuse = light.color.rgb * diff * uMaterial.albedoColor.rgb * vec3(texture(uTexture[uMaterial.albedoTexIndex], vTexCoord));
    vec3 specular = light.color.rgb * spec * uMaterial.specularColor.rgb * vec3(texture(uTexture[uMaterial.specularTexIndex], vTexCoord));
    diffuse *= attenuation;
    specular *= attenuation;
    return (diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = mix(128.0, 2.0, uMaterial.roughness);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 diffuse = light.color.rgb * diff * uMaterial.albedoColor.rgb * vec3(texture(uTexture[uMaterial.albedoTexIndex], vTexCoord));
    vec3 specular = light.color.rgb * spec * uMaterial.specularColor.rgb * vec3(texture(uTexture[uMaterial.specularTexIndex], vTexCoord));
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (diffuse + specular);
}

void main()
{
    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(uCamPos - vFragPos);
    vec3 result = vec3(0.0);
    if (texture(uTexture[uMaterial.albedoTexIndex], vTexCoord).a < 0.1)
        discard;
    vec3 ambient = uAmbientLight * uMaterial.albedoColor.rgb * vec3(texture(uTexture[uMaterial.albedoTexIndex], vTexCoord));
    result += ambient;

    result += CalcDirLight(uDirLight, norm, viewDir);

    for (int i = 0; i < uNumPointLights; i++)
    {
        result += CalcPointLight(uPointLights[i], norm, vFragPos, viewDir);
    }

    for (int i = 0; i < uNumSpotLights; i++)
    {
        result += CalcSpotLight(uSpotLights[i], norm, vFragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
    EntityID = uEntityId;
}
