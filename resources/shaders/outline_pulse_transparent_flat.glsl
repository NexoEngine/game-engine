#type vertex
#version 430 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

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
flat out int vMaterialIndex;

void main()
{
    int idx = uInstanceOffset + int(gl_InstanceID);
    mat4 model = uInstances[idx].model;
    vec4 worldPos = model * vec4(aPos.xy, 1.0, 1.0);
    vTexCoord = aTexCoord;
    vMaterialIndex = uInstances[idx].materialIndex;
    gl_Position = uViewProjection * worldPos;
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;

in vec2 vTexCoord;
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

uniform sampler2D uTexture[31];
uniform float uTime;

void main()
{
    Material uMaterial = uMaterials[vMaterialIndex];
    if (texture(uTexture[uMaterial.albedoTexIndex], vTexCoord).a < 0.1)
        discard;

    vec4 purpleColor = vec4(0.5, 0.0, 1.0, 1.0);
    vec4 blueColor = vec4(0.0, 0.4, 0.9, 1.0);

    // Color shifting effect
    float colorShift = (sin(uTime * 3.0) * 0.5 + 0.5);
    vec4 baseColor = mix(purpleColor, blueColor, colorShift);

    // Contrast pulsation
    float contrastPulse = (sin(uTime * 2.3) * 0.5 + 0.5) * 0.5 + 0.5; // Range from 0.5 to 1.0

    // Apply contrast variation
    vec4 highContrastColor = vec4(
            pow(baseColor.r, contrastPulse),
            pow(baseColor.g, contrastPulse),
            pow(baseColor.b, contrastPulse),
            1.0
        );

    // Add subtle brightness pulsation for a glowing effect
    float brightnessPulse = sin(uTime * 4.0) * 0.15 + 0.85; // Range from 0.7 to 1.0

    FragColor = highContrastColor * brightnessPulse;
}
