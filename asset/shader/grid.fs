#version 330 core
in vec3 worldPos;
out vec4 FragColor;

float grid(vec2 coord)
{
    vec2 g = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);
    return 1.0 - min(min(g.x, g.y), 1.0);
}

void main()
{
    float minor = grid(worldPos.xz);
    float major = grid(worldPos.xz / 10.0);

    float line = max(minor * 0.4, major * 0.8);

    float dist = length(worldPos.xz);
    float fade = 1.0 - clamp(dist / 100.0, 0.0, 1.0);
    line *= fade;

    vec3 color = mix(vec3(0.1), vec3(0.6), line);
    FragColor = vec4(color, 1.0);
}
