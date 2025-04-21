#version 450 core
out vec4 finalColor;

in vec2 fragTexCoord;

struct Material
{
    vec3 albedo;
    sampler2D albedoTexture;
};

uniform Material material;

vec3 GetObjectColor()
{
    vec3 color = material.albedo;    
    vec3 texel = texture(material.albedoTexture, fragTexCoord).xyz;

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
