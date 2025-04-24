#version 450 core
out vec4 finalColor;

in vec3 fragWorldPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

struct DirectionalLight
{
    float intensity;
    vec3 color;
    vec3 direction;
};

const float k_ambient = 0.3f;

uniform vec3 albedo;
uniform sampler2D albedoTexture;
uniform vec3 viewWorldPosition;
uniform DirectionalLight sun;

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

float CalculateDiffuse(vec3 N, vec3 L)
{
    float kD = max(dot(N, L), k_ambient);
    return kD;
}

float CalculateSpecular(vec3 N, vec3 L, vec3 V)
{
    // Goofy ahh phong
    //vec3 R = reflect(-L, N);
    //float kS = pow(max(dot(V, R), 0.f), 32.f);

    // Blinn-phong is better
    vec3 H = normalize(L + V);
    float kS = pow(max(dot(N, H), 0.f), 32.f);

    return kS;
}

vec3 CalculateDirectionalLighting(vec3 N)
{
    vec3 L = normalize(-sun.direction);
    vec3 V = normalize(viewWorldPosition - fragWorldPosition);

    float kD = CalculateDiffuse(N, L);
    float kS = CalculateSpecular(N, L, V);

    vec3 result = (kD + kS) * sun.intensity * sun.color * GetObjectColor();
    return result;
}

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 Lo = CalculateDirectionalLighting(N);

    Lo = pow(Lo, vec3(1.f / 2.2f));
    finalColor = vec4(Lo, 1.f);
}

