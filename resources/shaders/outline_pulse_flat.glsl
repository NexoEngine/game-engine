#type vertex
#version 430 core
layout(location = 0) in vec3 aPos;

uniform mat4 uViewProjection;
uniform mat4 uMatModel;

void main()
{
    // Calculate position in world space
    vec4 worldPos = uMatModel * vec4(aPos, 1.0);

    // Transform to clip space
    gl_Position = uViewProjection * worldPos;
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;

uniform float uTime; // For subtle animation

void main()
{
    // Base colors
    vec4 purpleColor = vec4(0.5, 0.0, 1.0, 1.0); // Purple
    vec4 blueColor = vec4(0.0, 0.4, 0.9, 1.0); // Blue

    // Color shifting effect
    float colorShift = (sin(uTime * 3.0) * 0.5 + 0.5);
    vec4 baseColor = mix(purpleColor, blueColor, colorShift);

    // Contrast pulsation (different frequency than color shift)
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
