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

struct SpotLight
{
    float intensity;
    float cutoff;
    float outerCutoff;
    vec3 position;
    vec3 target;
    vec3 color;
};

const float k_ambient = 0.3f;

uniform vec3 albedo;
uniform vec3 viewWorldPosition;

uniform sampler2D albedoTexture;

uniform DirectionalLight sun;
uniform SpotLight spotlight;

vec3 GetObjectColor();
float CalculateDiffuse(vec3 N, vec3 L);
float CalculateSpecular(vec3 N, vec3 L, vec3 V);
vec3 CalculateLightInternal(vec3 N, vec3 V, vec3 L);
vec3 CalculateDirectionalLighting(vec3 N, vec3 V);
vec3 CalculateSpotlight(vec3 N, vec3 V);

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(viewWorldPosition - fragWorldPosition);
    vec3 Lo = vec3(0.f);
    Lo += CalculateDirectionalLighting(N, V);
    Lo += CalculateSpotlight(N, V);

    Lo = pow(Lo, vec3(1.f / 2.2f));
    finalColor = vec4(Lo, 1.f);
}

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
    float kD = max(dot(N, L), 0.f);
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

vec3 CalculateLightInternal(vec3 N, vec3 V, vec3 L)
{
    float kD = CalculateDiffuse(N, L);
    float kS = CalculateSpecular(N, L, V);

    return ((kD + kS) + k_ambient) * GetObjectColor();
}

vec3 CalculateDirectionalLighting(vec3 N, vec3 V)
{
    vec3 L = normalize(-sun.direction);
    vec3 Lc = sun.intensity * sun.color;
    vec3 Li = CalculateLightInternal(N, V, L);

    vec3 result = Li * Lc;
    return result;
}

vec3 CalculateSpotlight(vec3 N, vec3 V)
{
    vec3 L = normalize(spotlight.position - fragWorldPosition);
    vec3 Ld = normalize(spotlight.position - spotlight.target);
    vec3 Lc = spotlight.intensity * spotlight.color;
    float theta = dot(L, Ld);
    float espilon = spotlight.cutoff - spotlight.outerCutoff;
    float intensity = clamp((theta - spotlight.outerCutoff) / espilon, 0.f, 1.f);
    vec3 Li = vec3(CalculateLightInternal(N, V, L) * intensity);

    vec3 result = Li * Lc;
    return result;
}
