#type vertex
#version 430 core
layout(location = 0) in vec3 aPos;

uniform int uModelIndex;

layout(std430, binding = 0) buffer ModelMatrices {
    mat4 uMatModelBuffer[];
};

layout(std140, binding = 1) uniform PerView {
    mat4 uViewProjection;
    vec3 uCamPos;
    float _pad0;   // padding to satisfy std140 (vec3 takes 16 bytes)
};

void main()
{
    mat4 model = uMatModelBuffer[uModelIndex];
    vec4 worldPos = model * vec4(aPos, 1.0);
    gl_Position = uViewProjection * worldPos;
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
