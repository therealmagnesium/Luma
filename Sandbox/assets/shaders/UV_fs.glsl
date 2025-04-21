#version 450 core
out vec4 finalColor;

in vec2 fragTexCoord;

void main()
{
    finalColor = vec4(fragTexCoord, 0.9f, 1.f);
}

