#type vertex
#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBiTangent;
layout(location = 5) in int aEntityID;

uniform mat4 uViewProjection;
uniform mat4 uMatModel;

out vec2 vTexCoord;
flat out int vEntityID;

void main()
{
    vec4 worldPos = uMatModel * vec4(aPos, 1.0);
    vTexCoord = aTexCoord;
    vEntityID = aEntityID;
    gl_Position = uViewProjection * worldPos;
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

in vec2 vTexCoord;
flat in int vEntityID;

struct Material {
    vec4 albedoColor;
    int albedoTexIndex; // Default: 0 (white texture)
};
uniform Material uMaterial;

uniform sampler2D uTexture[32];

void main()
{
    if (texture(uTexture[uMaterial.albedoTexIndex], vTexCoord).a < 0.1)
        discard;
    vec3 color = uMaterial.albedoColor.rgb * vec3(texture(uTexture[uMaterial.albedoTexIndex], vTexCoord));
    FragColor = vec4(color, 1.0);
    EntityID = vEntityID;
}
