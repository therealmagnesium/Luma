#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out DATA
{
    vec3 worldPosition;
    vec2 texCoord;
    vec3 normal;
} data;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

void main()
{
    data.worldPosition = mat3(modelMatrix) * position;
    data.texCoord = texCoord;
    data.normal = mat3(normalMatrix) * normal;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.f);
}

