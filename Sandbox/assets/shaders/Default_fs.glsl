#version 450 core
out vec4 finalColor;

in vec2 fragTexCoord;

uniform sampler2D albedoTexture;

void main()
{
    vec3 texel = texture(albedoTexture, fragTexCoord).xyz;
    //finalColor = vec4(fragTexCoord, 0.9f, 1.f);
    finalColor = vec4(texel, 1.f);
}
