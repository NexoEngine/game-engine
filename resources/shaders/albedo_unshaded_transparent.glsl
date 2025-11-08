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
    // padding for std430: an int is 4 bytes, but the next element (if any)
    // would need 16-byte alignment, so you can add 3 ints or a vec3 if needed later.
    int _pad0;
    int _pad1;
    int _pad2;
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

void main()
{
    int idx = uInstanceOffset + int(gl_InstanceID);
    mat4 model = uInstances[idx].model;
    vec4 worldPos = model * vec4(aPos, 1.0);
    vTexCoord = aTexCoord;
    vEntityId = uInstances[idx].entityId;
    gl_Position = uViewProjection * worldPos;
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

in vec2 vTexCoord;
flat in int vEntityId;

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
    EntityID = vEntityId;
}
