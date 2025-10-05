#type vertex
#version 430 core

layout(location=0) in vec3 aPos;

uniform mat4 uViewProjection;
uniform mat4 uMatModel;
uniform vec3 uColor;

out vec3 vColor;

void main()
{
    vColor = uColor;
    gl_Position = uViewProjection * uMatModel * vec4(aPos, 1.0);
}

#type fragment
#version 430 core

in vec3 vColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(vColor, 1.0);
}
