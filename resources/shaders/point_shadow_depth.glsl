// point_shadow_depth.glsl

#type vertex
#version 430 core

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

uniform mat4 uShadowMatrix; // view-projection for current cube face
uniform int  uInstanceOffset;

out vec3 vFragPos;

void main()
{
    int idx = uInstanceOffset + int(gl_InstanceID);
    mat4 model = uInstances[idx].model;
    vec4 worldPos = model * vec4(aPos, 1.0);

    vFragPos    = worldPos.xyz;
    gl_Position = uShadowMatrix * worldPos;
}

#type fragment
#version 430 core

in vec3 vFragPos;

uniform vec3  uLightPos;
uniform float uFarPlane;

void main()
{
    float dist = length(vFragPos - uLightPos);

    // normalize distance to [0,1] for storage in depth
    float depth = dist / uFarPlane;
    gl_FragDepth = depth;
}
