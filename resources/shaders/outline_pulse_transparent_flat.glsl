#type vertex
#version 430 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uViewProjection;
uniform mat4 uMatModel;

out vec2 vTexCoord;

void main()
{
    vec4 worldPos = uMatModel * vec4(aPos.xy, 1.0, 1.0);
    vTexCoord = aTexCoord;
    gl_Position = uViewProjection * worldPos;
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;

in vec2 vTexCoord;

struct Material {
    vec4 albedoColor;
    int albedoTexIndex; // Default: 0 (white texture)
};
uniform Material uMaterial;

uniform sampler2D uTexture[31];
uniform float uTime;

void main()
{
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
