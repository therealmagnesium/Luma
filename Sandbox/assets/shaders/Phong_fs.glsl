#version 450 core
out vec4 finalColor;

in vec3 fragWorldPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

struct Material
{
    vec3 albedo;
    sampler2D albedoTexture;
};

struct DirectionalLight
{
    float intensity;
    vec3 color;
    vec3 direction;
};

const float k_ambient = 0.3f;

uniform vec3 viewWorldPosition;
uniform Material material;
uniform DirectionalLight sun;

vec3 GetObjectColor()
{
    vec3 color = material.albedo;    
    vec3 texel = texture(material.albedoTexture, fragTexCoord).xyz;

    if (any(greaterThan(texel, vec3(0.f))))
        color *= texel;

    return color;
}

float CalculateDiffuse(vec3 N, vec3 L)
{
    float kD = max(dot(N, L), k_ambient);
    return kD;
}

float CalculateSpecular(vec3 N, vec3 L, vec3 V)
{
    vec3 R = reflect(-L, N);
    float kS = pow(max(dot(V, R), 0.f), 32.f);

    return kS;
}

vec3 CalculateDirectionalLighting(vec3 N)
{
    vec3 L = normalize(-sun.direction);
    vec3 V = normalize(viewWorldPosition - fragWorldPosition);

    float kD = CalculateDiffuse(N, L);
    float kS = CalculateSpecular(N, L, V);

    vec3 result = (kD + kS) * sun.intensity * GetObjectColor();
    return result;
}

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 Lo = CalculateDirectionalLighting(N);

    finalColor = vec4(Lo, 1.f);
}

