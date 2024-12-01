#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;
layout (location = 4) in vec3 aNormal;

uniform mat4 viewProjection;

out vec3 currentPos;
out vec2 vTexCoord;
out float vTexIndex;
out vec4 vColor;
out vec3 vNormal;

void main()
{
    currentPos = aPos;
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTexIndex = aTexIndex;
    vNormal = aNormal;
    gl_Position = viewProjection  * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColor;

#define MAX_DIR_LIGHTS 4
#define MAX_POINT_LIGHTS 8

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

in vec3 currentPos;
in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;
in vec3 vNormal;

uniform sampler2D uTexture[32];

uniform vec3 camPos;

uniform DirectionalLight dirLights[MAX_DIR_LIGHTS];
uniform int numDirLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;
uniform float ambientLight;

void main()
{
    vec4 sampler;
    switch (int(vTexIndex)) {
        case 0:  sampler = texture(uTexture[0],  vTexCoord); break;
        case 1:  sampler = texture(uTexture[1],  vTexCoord); break;
        case 2:  sampler = texture(uTexture[2],  vTexCoord); break;
        case 3:  sampler = texture(uTexture[3],  vTexCoord); break;
        case 4:  sampler = texture(uTexture[4],  vTexCoord); break;
        case 5:  sampler = texture(uTexture[5],  vTexCoord); break;
        case 6:  sampler = texture(uTexture[6],  vTexCoord); break;
        case 7:  sampler = texture(uTexture[7],  vTexCoord); break;
        case 8:  sampler = texture(uTexture[8],  vTexCoord); break;
        case 9:  sampler = texture(uTexture[9],  vTexCoord); break;
        case 10: sampler = texture(uTexture[10], vTexCoord); break;
        case 11: sampler = texture(uTexture[11], vTexCoord); break;
        case 12: sampler = texture(uTexture[12], vTexCoord); break;
        case 13: sampler = texture(uTexture[13], vTexCoord); break;
        case 14: sampler = texture(uTexture[14], vTexCoord); break;
        case 15: sampler = texture(uTexture[15], vTexCoord); break;
        case 16:  sampler = texture(uTexture[16],  vTexCoord); break;
        case 17:  sampler = texture(uTexture[17],  vTexCoord); break;
        case 18:  sampler = texture(uTexture[18],  vTexCoord); break;
        case 19:  sampler = texture(uTexture[19],  vTexCoord); break;
        case 20:  sampler = texture(uTexture[20],  vTexCoord); break;
        case 21:  sampler = texture(uTexture[21],  vTexCoord); break;
        case 22:  sampler = texture(uTexture[22],  vTexCoord); break;
        case 23:  sampler = texture(uTexture[23],  vTexCoord); break;
        case 24:  sampler = texture(uTexture[24],  vTexCoord); break;
        case 25:  sampler = texture(uTexture[25],  vTexCoord); break;
        case 26: sampler = texture(uTexture[26], vTexCoord); break;
        case 27: sampler = texture(uTexture[27], vTexCoord); break;
        case 28: sampler = texture(uTexture[28], vTexCoord); break;
        case 29: sampler = texture(uTexture[29], vTexCoord); break;
        case 30: sampler = texture(uTexture[30], vTexCoord); break;
        case 31: sampler = texture(uTexture[31], vTexCoord); break;
    }

    vec3 normal = normalize(vNormal);

    // Start with ambient light contribution
    vec3 fragColor = ambientLight * sampler.rgb * vColor.rgb;

    // Directional Lights
    for (int i = 0; i < numDirLights; ++i) {
        vec3 lightDir = normalize(-dirLights[i].direction);
        float diffuse = max(dot(normal, lightDir), 0.0);
        vec3 viewDirection = normalize(camPos - currentPos);
        vec3 reflectionDirection = reflect(-lightDir, normal);
        float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 8);
        float specular = specAmount * dirLights[i].intensity;

        fragColor += (diffuse + specular) * dirLights[i].color.rgb * sampler.rgb;
    }

    // Point Lights
    for (int i = 0; i < numPointLights; ++i) {
        vec3 lightDir = normalize(pointLights[i].position - currentPos);
        float diffuse = max(dot(normal, lightDir), 0.0);
        vec3 viewDirection = normalize(camPos - currentPos);
        vec3 reflectionDirection = reflect(-lightDir, normal);
        float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 8);
        float specular = specAmount * pointLights[i].intensity;

        float distance = length(pointLights[i].position - currentPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));

        fragColor += attenuation * (diffuse + specular) * pointLights[i].color.rgb * sampler.rgb;
    }

    FragColor = vec4(fragColor, 1.0);
}