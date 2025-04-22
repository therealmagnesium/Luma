#version 450 core
out vec4 finalColor;

in vec2 fragTexCoord;

uniform vec3 albedo;
uniform sampler2D albedoTexture;

vec3 GetObjectColor()
{
    vec3 color = albedo;    
    vec3 texel = texture(albedoTexture, fragTexCoord).xyz;

    return color * texel;
}

void main()
{
    vec3 color = GetObjectColor();
    color = pow(color, vec3(1.f / 2.2f));
    finalColor = vec4(color, 1.f);
    //finalColor = vec4(fragTexCoord, 0.9f, 1.f);
    //finalColor = vec4(abs(normalize(fragNormal)), 1.f);
}
