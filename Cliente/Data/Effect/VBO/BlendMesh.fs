#version 330

in vec4 v_color0;
in vec2 v_texcoord0;
in float v_fogDepth;
uniform sampler2D uTexture;
uniform float u_alphaCutoff;
uniform bool uFogEnabled;
uniform vec4 uFogColor;
uniform float uFogStart;
uniform float uFogEnd;

out vec4 FragColor;

void main()
{
    vec4 texel = texture(uTexture, v_texcoord0);
    vec4 color = texel * v_color0;

    float peak = max(max(color.r, color.g), color.b);
    if (peak > 1.0)
    {
        color.rgb /= (1.0 + (peak - 1.0) * 0.65);
    }

    color.rgb = min(color.rgb, vec3(1.0));

    if (u_alphaCutoff > 0.0 && color.a < u_alphaCutoff)
    {
        discard;
    }

    if (uFogEnabled)
    {
        float fogFactor = clamp((uFogEnd - v_fogDepth) / max(uFogEnd - uFogStart, 0.0001), 0.0, 1.0);
        color = mix(uFogColor, color, fogFactor);
    }

    FragColor = color;
}