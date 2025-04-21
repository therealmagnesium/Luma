#version 450 core
out vec4 finalColor;

in vec2 fragTexCoord;
in vec3 fragNormal;

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 color = abs(N);
    color = pow(color, vec3(1.f / 2.2f));
    finalColor = vec4(color, 1.f);
}

