#type vertex
#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBiTangent;
layout(location = 5) in int aEntityID;

uniform mat4 viewProjection;
uniform mat4 matModel;

out vec3 vFragPos;
out vec2 vTexCoord;
out vec3 vNormal;
flat out int vEntityID;

void main()
{
    vec4 worldPos = matModel * vec4(aPos, 1.0);
    vFragPos = worldPos.xyz;

    vTexCoord = aTexCoord;

    vNormal = mat3(transpose(inverse(matModel))) * aNormal;

    vEntityID = aEntityID;

    gl_Position = viewProjection * vec4(vFragPos, 1.0);
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

#define MAX_DIR_LIGHTS 4
#define MAX_POINT_LIGHTS 8
#define MAX_SPOT_LIGHTS 8

// Light definitions.
struct DirectionalLight {
    vec3 direction;
    vec4 color;
};

struct PointLight {
    vec3 position;
    vec4 color;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec4 color;
    float cutOff;
    float outerCutoff;
    float constant;
    float linear;
    float quadratic;
};

in vec3 vFragPos;
in vec2 vTexCoord;
in vec3 vNormal;
flat in int vEntityID;

uniform sampler2D uTexture[32];

uniform vec3 camPos;

uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform int numDirLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numSpotLights;
uniform vec3 ambientLight;

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
uniform Material material;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = mix(128.0, 2.0, material.roughness);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    //vec3 ambient = ambientLight * material.albedoColor.rgb * vec3(texture(uTexture[material.albedoTexIndex], vTexCoord));
    vec3 diffuse = light.color.rgb * diff * material.albedoColor.rgb * vec3(texture(uTexture[material.albedoTexIndex], vTexCoord));
    vec3 specular = light.color.rgb * spec * material.specularColor.rgb * vec3(texture(uTexture[material.specularTexIndex], vTexCoord));
    return (diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = mix(128.0, 2.0, material.roughness);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    //vec3 ambient = ambientLight * material.albedoColor.rgb * vec3(texture(uTexture[material.albedoTexIndex], vTexCoord));
    vec3 diffuse = light.color.rgb * diff * material.albedoColor.rgb * vec3(texture(uTexture[material.albedoTexIndex], vTexCoord));
    vec3 specular = light.color.rgb * spec * material.specularColor.rgb * vec3(texture(uTexture[material.specularTexIndex], vTexCoord));
    //ambient *= attenuation;
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
    float shininess = mix(128.0, 2.0, material.roughness);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    // combine results
    //vec3 ambient = ambientLight * material.albedoColor.rgb * vec3(texture(uTexture[material.albedoTexIndex], vTexCoord));
    vec3 diffuse = light.color.rgb * diff * material.albedoColor.rgb * vec3(texture(uTexture[material.albedoTexIndex], vTexCoord));
    vec3 specular = light.color.rgb * spec * material.specularColor.rgb * vec3(texture(uTexture[material.specularTexIndex], vTexCoord));
    //ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (diffuse + specular);
}

void main()
{
    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(camPos - vFragPos);
    vec3 result = vec3(0.0);
    if (texture(uTexture[material.albedoTexIndex], vTexCoord).a < 0.1)
        discard;
    vec3 ambient = ambientLight * material.albedoColor.rgb * vec3(texture(uTexture[material.albedoTexIndex], vTexCoord));
    result += ambient;

    for (int i = 0; i < numDirLights; i++)
    {
        result += CalcDirLight(dirLights[i], norm, viewDir);
    }

    for (int i = 0; i < numPointLights; i++)
    {
        result += CalcPointLight(pointLights[i], norm, vFragPos, viewDir);
    }

    for (int i = 0; i < numSpotLights; i++)
    {
        result += CalcSpotLight(spotLights[i], norm, vFragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
    EntityID = vEntityID;
}
