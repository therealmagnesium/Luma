#version 450 core
out vec4 finalColor;

in vec2 fragTexCoord;
in vec3 fragNormal;

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

    if ((any(greaterThan(texel, vec3(0.f)))))
        color *= texel;

    return color;
}

void main()
{
    finalColor = vec4(GetObjectColor(), 1.f);
    //finalColor = vec4(fragTexCoord, 0.9f, 1.f);
    //finalColor = vec4(abs(normalize(fragNormal)), 1.f);
}
