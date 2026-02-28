#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 resolution;
uniform float curvature;
// uniform float time;

// Output fragment color
out vec4 finalColor;

vec2 curve(vec2 uv) {
    uv = uv * 2.0 - 1.0;
    vec2 offset = abs(uv.yx) * curvature;
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

void main() {
    vec2 uv = curve(fragTexCoord);

    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    float caOffset = 0.001;
    float r = texture(texture0, vec2(uv.x + caOffset, uv.y)).r;
    float g = texture(texture0, uv).g;
    float b = texture(texture0, vec2(uv.x - caOffset, uv.y)).b;
    float a = texture(texture0, uv).a;
    vec4 pixel = vec4(r, g, b, a);

    float scanline = sin(uv.y * resolution.y * 3.14159) * 0.5 + 0.5;
    scanline = pow(scanline, 0.3);
    pixel.rgb *= scanline * 0.3 + 0.7;

    float vignetteStrength = 0.8;
    float vignetteSize = 25.0;
    float vignette = uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y);
    vignette = clamp(pow(vignette * vignetteSize, 0.25), 0.0, 1.0);
    pixel.rgb *= mix(1.0, vignette, vignetteStrength);

    // pixel.rgb *= 0.98 + 0.2 * sin(time * 25.0);
    finalColor = pixel * fragColor * colDiffuse;
}
