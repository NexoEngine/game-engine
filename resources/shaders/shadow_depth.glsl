// ShadowDepth.glsl
#type vertex
#version 430 core

// VERTEX
layout(location = 0) in vec3 aPos;

struct InstanceData {
    mat4 model;
    int  entityId;
    int  materialIndex;
    int  _pad0;
    int  _pad1;
};

layout(std430, binding = 0) buffer Instances {
    InstanceData uInstances[];
};

uniform mat4 uLightViewProjection;
uniform int  uInstanceOffset;

void main()
{
    int idx = uInstanceOffset + int(gl_InstanceID);
    mat4 model  = uInstances[idx].model;
    vec4 worldPos = model * vec4(aPos, 1.0);
    gl_Position = uLightViewProjection * worldPos;
}

// FRAGMENT
#type fragment
#version 430 core
void main() { }
