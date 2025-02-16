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

// Light definitions.
struct DirectionalLight {
    vec3 direction;
    vec4 color;
    float intensity;
};

struct PointLight {
    vec3 position;
    vec4 color;
    float intensity;
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
uniform vec3 ambientLight;

struct Material {
    vec4 albedoColor;
    int albedoTexIndex; // Default: 0 (white texture)
    vec3 specularColor;
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

void main()
{
    vec3 ambient = ambientLight * material.albedoColor.rgb;
    // diffuse
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(pointLights[0].position - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * pointLights[0].color.rgb * material.albedoColor.rgb;

    // specular
    float shininess = mix(128.0, 2.0, material.roughness);
    vec3 viewDir = normalize(camPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * pointLights[0].color.rgb;

    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0);
    EntityID = vEntityID;
}
