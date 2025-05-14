#type vertex
#version 430 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

void main()
{
    gl_Position = vec4(aPosition.xy, 0.0, 1.0);
    vTexCoord = aTexCoord;
}

#type fragment
#version 430 core
layout(location = 0) out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D uMaskTexture;
uniform sampler2D uDepthTexture;
uniform sampler2D uDepthMaskTexture;
uniform vec2 uScreenSize;
uniform float uTime;
uniform float uOutlineWidth = 5.0;

void main()
{
    float mask = texture(uMaskTexture, vTexCoord).r;
    // Exit if inside the mask
    if (mask > 0.5) {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
        return;
    }

    float minDist = uOutlineWidth * uOutlineWidth;
    const int MAX_SAMPLES = 5;
    const float MAX_DIST = uOutlineWidth;
    float maskDepth = 1.0;

    // Concentric ring sampling
    for (int i = 1; i <= MAX_SAMPLES && minDist > 1.0; i++) {
        float angle_step = 3.14159 * 0.25; // 45 degrees
        float radius = float(i) / float(MAX_SAMPLES) * MAX_DIST;

        for (float angle = 0.0; angle < 6.28318; angle += angle_step) {
            float x = cos(angle) * radius;
            float y = sin(angle) * radius;

            // Convert to texture space (textures coordinates range from 0 to 1, so dividing pixel coords by screen size does that)
            vec2 offset = vec2(x / uScreenSize.x, y / uScreenSize.y);
            float sampleMask = texture(uMaskTexture, vTexCoord + offset).r;

            if (sampleMask > 0.5) {
                float dist = dot(vec2(x, y), vec2(x, y));
                maskDepth = texture(uDepthMaskTexture, vTexCoord + offset).r;
                minDist = min(minDist, dist);
                break;
            }
        }
    }

    // Calculate outline alpha based on distance
    float alpha = 0.0;
    const float solid = 1.0; // Solid edge width
    const float fuzzy = uOutlineWidth - solid; // Fuzzy part, transparent part of the outline

    if (minDist <= uOutlineWidth * uOutlineWidth && maskDepth <= texture(uDepthTexture, vTexCoord).r) {
        // if sqrt(minDist) <= solid, we get a negative value clamped to 0.0 -> 1.0 - 0.0, full solid part
        // else, the alpha value gets smoothed along the fuzzy part (further = less opaque)
        alpha = 1.0 - clamp((sqrt(minDist) - solid) / fuzzy, 0.0, 1.0);
    } else {
        // If not close enough to mask, discard
        discard;
    }

    vec4 purpleColor = vec4(0.5, 0.0, 1.0, 1.0);
    vec4 blueColor = vec4(0.0, 0.4, 0.9, 1.0);
    // 0.5Hz oscillation between -1 and 1 then remapped to 0 and 1
    float colorShift = (sin(uTime * 3.0) * 0.5 + 0.5);
    vec4 baseColor = mix(purpleColor, blueColor, colorShift);

    // 0.365Hz oscillation between -1 and 1 then remapped to 0.5 and 1
    float contrastPulse = (sin(uTime * 2.3) * 0.5 + 0.5) * 0.5 + 0.5;

    // Increase the contrast by this pulse
    vec4 highContrastColor = vec4(
            pow(baseColor.r, contrastPulse),
            pow(baseColor.g, contrastPulse),
            pow(baseColor.b, contrastPulse),
            1.0
        );

    // Increase the brightness by a 0.63Hz pulse,remap to 0.7 and 1.0
    float brightnessPulse = sin(uTime * 4.0) * 0.15 + 0.85;

    FragColor = highContrastColor * brightnessPulse;
    FragColor.a = alpha;
}
