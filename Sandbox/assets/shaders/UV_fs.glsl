#version 450 core
out vec4 finalColor;

in vec2 fragTexCoord;

void main()
{
    vec3 color = vec3(fragTexCoord, 0.9f);
    color = pow(color, vec3(1.f / 2.2f));
    finalColor = vec4(color, 1.f);
}

