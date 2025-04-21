#version 450 core
out vec4 finalColor;

in vec2 fragTexCoord;
in vec3 fragNormal;

void main()
{
    vec3 N = normalize(fragNormal);
    finalColor = vec4(abs(N), 1.f);
}

