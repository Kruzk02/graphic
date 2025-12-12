#version 330 core

out vec4 fragColor;

uniform float uTime;

float rand(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    float interval = 2.0;
    float seed = floor(uTime / interval);
    float t = mod(uTime, interval) / interval;

    // Top color
    vec3 topA = vec3(
        rand(vec2(seed, 1.0)),
        rand(vec2(seed, 2.0)),
        rand(vec2(seed, 3.0))
    );

    vec3 topB = vec3(
        rand(vec2(seed+1, 1.0)),
        rand(vec2(seed+1, 2.0)),
        rand(vec2(seed+1, 3.0))
    );

    // Bottom color
    vec3 bottomA = vec3(
        rand(vec2(seed+2, 1.0)),
        rand(vec2(seed+2, 2.0)),
        rand(vec2(seed+2, 3.0))
    );

    vec3 bottomB = vec3(
        rand(vec2(seed+3, 1.0)),
        rand(vec2(seed+3, 2.0)),
        rand(vec2(seed+3, 3.0))
    );

    vec3 topColor = mix(topA, topB, t);
    vec3 bottomColor = mix(bottomA, bottomB, t);

    vec3 finalColor = mix(bottomColor, topColor, gl_FragCoord.y / 720);
    fragColor = vec4(finalColor, 1.0);
}
