#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexIndex;

uniform mat4 viewProjection;

out vec2 vTexCoord;
out float vTexIndex;
out vec4 vColor;

void main()
{
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTexIndex = aTexIndex;
    gl_Position = viewProjection  * vec4(aPos, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;

uniform sampler2D uTexture[32];

void main()
{
    vec4 sampler;
    switch (int(vTexIndex)) {
        case 0:  sampler = texture(uTexture[0],  vTexCoord); break;
        case 1:  sampler = texture(uTexture[1],  vTexCoord); break;
        case 2:  sampler = texture(uTexture[2],  vTexCoord); break;
        case 3:  sampler = texture(uTexture[3],  vTexCoord); break;
        case 4:  sampler = texture(uTexture[4],  vTexCoord); break;
        case 5:  sampler = texture(uTexture[5],  vTexCoord); break;
        case 6:  sampler = texture(uTexture[6],  vTexCoord); break;
        case 7:  sampler = texture(uTexture[7],  vTexCoord); break;
        case 8:  sampler = texture(uTexture[8],  vTexCoord); break;
        case 9:  sampler = texture(uTexture[9],  vTexCoord); break;
        case 10: sampler = texture(uTexture[10], vTexCoord); break;
        case 11: sampler = texture(uTexture[11], vTexCoord); break;
        case 12: sampler = texture(uTexture[12], vTexCoord); break;
        case 13: sampler = texture(uTexture[13], vTexCoord); break;
        case 14: sampler = texture(uTexture[14], vTexCoord); break;
        case 15: sampler = texture(uTexture[15], vTexCoord); break;
        case 16:  sampler = texture(uTexture[16],  vTexCoord); break;
        case 17:  sampler = texture(uTexture[17],  vTexCoord); break;
        case 18:  sampler = texture(uTexture[18],  vTexCoord); break;
        case 19:  sampler = texture(uTexture[19],  vTexCoord); break;
        case 20:  sampler = texture(uTexture[20],  vTexCoord); break;
        case 21:  sampler = texture(uTexture[21],  vTexCoord); break;
        case 22:  sampler = texture(uTexture[22],  vTexCoord); break;
        case 23:  sampler = texture(uTexture[23],  vTexCoord); break;
        case 24:  sampler = texture(uTexture[24],  vTexCoord); break;
        case 25:  sampler = texture(uTexture[25],  vTexCoord); break;
        case 26: sampler = texture(uTexture[26], vTexCoord); break;
        case 27: sampler = texture(uTexture[27], vTexCoord); break;
        case 28: sampler = texture(uTexture[28], vTexCoord); break;
        case 29: sampler = texture(uTexture[29], vTexCoord); break;
        case 30: sampler = texture(uTexture[30], vTexCoord); break;
        case 31: sampler = texture(uTexture[31], vTexCoord); break;
    }
    FragColor = sampler * vColor;
}