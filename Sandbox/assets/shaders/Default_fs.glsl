#version 450 core
out vec4 finalColor;

in vec2 fragTexCoord;

uniform vec3 albedo;
uniform sampler2D albedoTexture;

vec3 GetObjectColor()
{
    vec3 result = albedo;

    if (textureSize(albedoTexture, 0).x > 1.f)
    {
        vec3 texel = texture(albedoTexture, fragTexCoord).xyz;
        result *= texel;
        return result;
    }

    return result;
}

void main()
{
    vec3 color = GetObjectColor();
    color = pow(color, vec3(1.f / 2.2f));
    finalColor = vec4(color, 1.f);
    //finalColor = vec4(fragTexCoord, 0.9f, 1.f);
    //finalColor = vec4(abs(normalize(fragNormal)), 1.f);
}
