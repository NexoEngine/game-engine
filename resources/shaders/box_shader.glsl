#type vertex
#version 430 core

layout(location=0) in vec3 aPos;

uniform vec3 uColor;

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

out vec3 vColor;

void main()
{
    vColor = uColor;
    int idx = uInstanceOffset + int(gl_InstanceID);
    mat4 model = uInstances[idx].model;

    gl_Position = uViewProjection * model * vec4(aPos, 1.0);
}

#type fragment
#version 430 core

in vec3 vColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor, 1.0);
}
