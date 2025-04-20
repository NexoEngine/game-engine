/*

        Copyright 2024 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#type vertex
#version 430

uniform mat4 uViewProjection = mat4(1.0);
uniform float uGridSize = 100.0;
uniform vec3 uCamPos;

out vec3 FragPos;

const vec3 gridPos[4] = vec3[4](
        vec3(-1.0, 0.0, -1.0), // bottom left
        vec3(1.0, 0.0, -1.0), // bottom right
        vec3(1.0, 0.0, 1.0), // top right
        vec3(-1.0, 0.0, 1.0) // top left
    );

const int gridIndices[6] = int[6](0, 2, 1, 2, 0, 3);

void main()
{
    int Index = gridIndices[gl_VertexID];
    vec3 position = gridPos[Index] * uGridSize;

    position.x += uCamPos.x;
    position.z += uCamPos.z;

    gl_Position = uViewProjection * vec4(position, 1.0);
    FragPos = position;
}

#type fragment
/*

        Copyright 2024 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#version 420
layout(location = 0) out vec4 FragColor;
in vec3 FragPos;

uniform vec3 uMouseWorldPos;
uniform float uTime;
uniform vec3 uCamPos;
uniform float uGridSize = 100.0;
uniform float uGridMinPixelsBetweenCells = 2.0;
uniform float uGridCellSize = 0.025;
uniform vec4 uGridColorThin = vec4(0.5, 0.5, 0.5, 1.0);
uniform vec4 uGridColorThick = vec4(0.0, 0.0, 0.0, 1.0);

float log10(float x)
{
    float f = log(x) / log(10.0);
    return f;
}

float satf(float x)
{
    float f = clamp(x, 0.0, 1.0);
    return f;
}

vec2 satv(vec2 x)
{
    vec2 v = clamp(x, vec2(0.0), vec2(1.0));
    return v;
}

float max2(vec2 v)
{
    float f = max(v.x, v.y);
    return f;
}

float calculateLodAlpha(float cellSize, vec2 dudv) {
    vec2 mod_div_dudv = mod(FragPos.xz, cellSize) / dudv;
    return max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)));
}

void main()
{
    vec2 dvx = vec2(dFdx(FragPos.x), dFdy(FragPos.x));
    vec2 dvy = vec2(dFdx(FragPos.z), dFdy(FragPos.z));

    float lx = length(dvx);
    float ly = length(dvy);

    vec2 dudv = vec2(lx, ly);

    float l = length(dudv);

    float LOD = max(0.0, log10(l * uGridMinPixelsBetweenCells / uGridCellSize) + 1.0);

    float GridCellSizeLod0 = uGridCellSize * pow(10.0, floor(LOD));
    float GridCellSizeLod1 = GridCellSizeLod0 * 10.0;
    float GridCellSizeLod2 = GridCellSizeLod1 * 10.0;

    dudv *= 4.0;
    float Lod0a = calculateLodAlpha(GridCellSizeLod0, dudv);
    float Lod1a = calculateLodAlpha(GridCellSizeLod1, dudv);
    float Lod2a = calculateLodAlpha(GridCellSizeLod2, dudv);

    float LOD_fade = fract(LOD);
    vec4 Color;

    if (Lod2a > 0.0) {
        // Major grid lines with blue highlight
        Color = mix(uGridColorThick, vec4(0.6, 0.7, 0.9, 0.7), 0.3);
        Color.a *= Lod2a;
    } else if (Lod1a > 0.0) {
        // Medium grid lines with subtle color transition
        Color = mix(uGridColorThick, uGridColorThin, LOD_fade);
        Color.a *= Lod1a * 0.8; // Slightly more transparent
    } else {
        // Fine grid lines fade more with distance
        Color = uGridColorThin;
        Color.a *= (Lod0a * (1.0 - LOD_fade)) * 0.7;
    }

    float distToMouse = length(FragPos.xz - uMouseWorldPos.xz);

    // Simple size pulse for the glow radius
    float pulsePhase = uTime * 1.2;
    float pulseSize = 1.0 + 0.3 * sin(pulsePhase);
    float glowRadius = 8.0 * pulseSize;

    // Calculate how much the lines should glow based on distance
    float glowFactor = 1.0 - smoothstep(0.0, glowRadius, distToMouse);
    glowFactor = glowFactor * glowFactor; // Squared for smoother falloff

    // Colors for the glowing lines
    vec3 glowColor1 = vec3(0.9, 0.3, 0.8); // Pink
    vec3 glowColor2 = vec3(0.5, 0.3, 0.9); // Purple
    float colorMix = 0.5 + 0.5 * sin(pulsePhase * 0.5);
    vec3 finalGlowColor = mix(glowColor1, glowColor2, colorMix);

    float distance = length(FragPos.xz - uCamPos.xz);
    float normalizedDist = distance / uGridSize;

    // More gradual falloff curve for distance
    float edgeFadeStart = 0.65; // Start fading at 65% of grid size
    float edgeFadeEnd = 0.92; // Complete fade by 92% of grid size
    float distanceFactor = 1.0 - smoothstep(edgeFadeStart, edgeFadeEnd, normalizedDist);

    // Apply distance gradient to grid color (using an improved curve)
    float fadeExponent = 1.5; // Controls how quickly the fade happens (higher = sharper edge)
    float smoothFade = pow(distanceFactor, fadeExponent);

    // Apply the color gradient
    vec3 nearColor = vec3(0.4, 0.45, 0.6); // Slightly blue tint for closer grid
    vec3 farColor = vec3(0.3, 0.3, 0.5); // Subtle purple tint for distant grid
    Color.rgb = mix(farColor, nearColor, smoothFade) * Color.rgb;

    // Apply the opacity falloff (more gradual, less abrupt)
    Color.a *= smoothFade;

    // Apply the glow only to the grid lines - enhance their color and brightness
    if (Lod2a > 0.1 || Lod1a > 0.1 || Lod0a > 0.1) {
        // Scale the glow effect based on the distance fade
        float distanceAdjustedGlow = glowFactor * smoothFade;

        // Only apply to actual grid lines, not the spaces between
        Color.rgb = mix(Color.rgb, finalGlowColor, distanceAdjustedGlow * 0.8);

        // Brighten the lines (less intense at grid edges)
        float brightnessFactor = distanceAdjustedGlow * 0.4;
        Color.rgb = mix(Color.rgb, vec3(1.0), brightnessFactor * 0.4);

        // Make lines slightly more opaque when glowing (respects edge fade)
        Color.a = mix(Color.a, min(1.0, Color.a * 1.2), distanceAdjustedGlow * 0.7);
    }
    FragColor = Color;
}
