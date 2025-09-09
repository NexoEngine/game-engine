#type vertex
#version 430 core
layout(location = 0) in vec3 aPos;

uniform mat4 uViewProjection;
uniform mat4 uMatModel;

void main()
{
    vec4 worldPos = uMatModel * vec4(aPos, 1.0);
    gl_Position = uViewProjection * worldPos;
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
