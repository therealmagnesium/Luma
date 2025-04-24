#version 450 core
out vec4 finalColor;

in vec2 fragTexCoord;

uniform sampler2D albedoTexture;

void main()
{    
    vec3 texel = texture(albedoTexture, fragTexCoord).xyz;
    vec3 color = vec3(1.f) - texel;
    finalColor = vec4(color, 1.f);
}
