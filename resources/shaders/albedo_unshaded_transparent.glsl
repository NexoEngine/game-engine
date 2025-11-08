#type vertex
#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBiTangent;

uniform int uModelIndex;

layout(std430, binding = 0) buffer ModelMatrices {
    mat4 uMatModelBuffer[];
};

layout(std140, binding = 1) uniform PerView {
    mat4 uViewProjection;
    vec3 uCamPos;
    float _pad0;   // padding to satisfy std140 (vec3 takes 16 bytes)
};

out vec2 vTexCoord;

void main()
{
    mat4 model = uMatModelBuffer[uModelIndex];
    vec4 worldPos = model * vec4(aPos, 1.0);
    vTexCoord = aTexCoord;
    gl_Position = uViewProjection * worldPos;
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

in vec2 vTexCoord;

struct Material {
    vec4 albedoColor;
    int albedoTexIndex; // Default: 0 (white texture)
};
uniform Material uMaterial;

uniform sampler2D uTexture[32];

uniform int uEntityId;

void main()
{
    if (texture(uTexture[uMaterial.albedoTexIndex], vTexCoord).a < 0.1)
        discard;
    vec3 color = uMaterial.albedoColor.rgb * vec3(texture(uTexture[uMaterial.albedoTexIndex], vTexCoord));
    FragColor = vec4(color, 1.0);
    EntityID = uEntityId;
}
