#type vertex
#version 430 core

layout(location=0) in vec3 aPos;

uniform int uModelIndex;

uniform vec3 uColor;

layout(std430, binding = 0) buffer ModelMatrices {
    mat4 uMatModelBuffer[];
};

layout(std140, binding = 1) uniform PerView {
    mat4 uViewProjection;
    vec3 uCamPos;
    float _pad0;   // padding to satisfy std140 (vec3 takes 16 bytes)
};

out vec3 vColor;

void main()
{
    vColor = uColor;
    mat4 model = uMatModelBuffer[uModelIndex];

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
