#version 450 core
out vec4 finalColor;

in DATA
{
    vec3 worldPosition;
    vec2 texCoord;
    vec3 normal;
} data;

struct DirectionalLight
{
    float intensity;
    vec3 color;
    vec3 direction;
};

const float k_ambient = 0.03f;
const float k_dielectricF0 = 0.04f;
const float k_PI = 3.14159265359f;

uniform vec3 viewWorldPosition;
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;

uniform sampler2D albedoTexture;
uniform sampler2D metallicTexture;
uniform sampler2D roughnessTexture;

uniform DirectionalLight sun;

vec3 GetObjectColor();
float GetMetallicValue();
float GetRoughnessValue();
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

void main()
{
    vec3 N = normalize(data.normal);
    vec3 V = normalize(viewWorldPosition - data.worldPosition);
    vec3 L = normalize(-sun.direction);
    vec3 H = normalize(V + L);

    vec3 albedoValue = GetObjectColor();
    float metallicValue = GetMetallicValue();
    float roughnessValue = GetRoughnessValue();

    vec3 F0 = vec3(k_dielectricF0); 
    F0 = mix(F0, albedoValue, metallicValue);

    float NDF = DistributionGGX(N, H, roughnessValue);   
    float G   = GeometrySmith(N, V, L, roughnessValue);      
    vec3 F    = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 numerator    = NDF * G * F; 
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallicValue);
    float NdotL = max(dot(N, L), 0.0);        

    vec3 Lo = (kD * albedoValue / k_PI + specular) * NdotL;
    Lo *= sun.intensity * sun.color;
    
    vec3 ambient = vec3(k_ambient) * albedoValue;
    vec3 result = ambient + Lo;

    result = result / (result + vec3(1.0)); // HDR tone mapping
    result = pow(result, vec3(1.f / 2.2f)); // Gamma correction
    finalColor = vec4(result, 1.f);
}

vec3 GetObjectColor()
{
    vec3 result = albedo;

    if (textureSize(albedoTexture, 0).x > 1)
    {
        vec3 texel = texture(albedoTexture, data.texCoord).xyz;
        result *= texel;
    }

    return result;
}

float GetMetallicValue() 
{
    float result = metallic;

    if (textureSize(metallicTexture, 0).x > 1)
    {
        float metallicValue = texture(metallicTexture, data.texCoord).x;
        result = metallicValue;
    }

    return result;
}

float GetRoughnessValue() 
{
    float result = roughness;

    if (textureSize(roughnessTexture, 0).x > 1)
    {
        float roughnessValue = texture(roughnessTexture, data.texCoord).x;
        result = roughnessValue;
    }

    return result;

}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = k_PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
