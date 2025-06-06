#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec3 fragWorldPosition;
out vec2 fragTexCoord;
out vec3 fragNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

void main()
{
    fragWorldPosition = mat3(modelMatrix) * position;
    fragTexCoord = texCoord;
    fragNormal = mat3(normalMatrix) * normal;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.f);
}

