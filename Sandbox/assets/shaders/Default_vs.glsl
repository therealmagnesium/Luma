#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 fragTexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    fragTexCoord = texCoord;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.f);
}
