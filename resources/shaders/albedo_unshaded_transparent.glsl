#type vertex
#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBiTangent;

struct InstanceData {
    mat4 model;
    int  entityId;
    int materialIndex;
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

out vec2 vTexCoord;
flat out int vEntityId;
flat out int vMaterialIndex;

void main()
{
    int idx = uInstanceOffset + int(gl_InstanceID);
    mat4 model = uInstances[idx].model;
    vec4 worldPos = model * vec4(aPos, 1.0);
    vTexCoord = aTexCoord;
    vEntityId = uInstances[idx].entityId;
    vMaterialIndex = uInstances[idx].materialIndex;
    gl_Position = uViewProjection * worldPos;
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

in vec2 vTexCoord;
flat in int vEntityId;
flat in int vMaterialIndex;

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

uniform sampler2D uTexture[32];

void main()
{
    Material uMaterial = uMaterials[vMaterialIndex];
    if (texture(uTexture[uMaterial.albedoTexIndex], vTexCoord).a < 0.1)
        discard;
    vec3 color = uMaterial.albedoColor.rgb * vec3(texture(uTexture[uMaterial.albedoTexIndex], vTexCoord));
    FragColor = vec4(color, 1.0);
    EntityID = vEntityId;
}
