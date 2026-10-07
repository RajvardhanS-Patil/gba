#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 resolution;
uniform float time;

void main()
{
    vec2 uv = fragTexCoord;
    
    // Slight barrel distortion
    vec2 cc = uv - 0.5;
    float dist = dot(cc, cc);
    uv = uv + cc * (dist * 0.08);

    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // Chromatic aberration
    float r = texture(texture0, uv + vec2(0.0015, 0.0)).r;
    float g = texture(texture0, uv).g;
    float b = texture(texture0, uv - vec2(0.0015, 0.0)).b;
    vec3 col = vec3(r, g, b);

    // Scanlines
    float scanline = sin(uv.y * resolution.y * 1.5 + time * 5.0) * 0.06;
    col -= scanline;

    // Subtle vignette
    float vignette = 1.0 - dist * 1.2;
    col *= clamp(vignette, 0.0, 1.0);

    // Subtle phosphor tint
    col.g *= 1.04;

    finalColor = vec4(col, 1.0) * fragColor;
}
