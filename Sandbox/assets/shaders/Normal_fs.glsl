#version 450 core
out vec4 finalColor;

in DATA
{
    vec3 worldPosition;
    vec2 texCoord;
    vec3 normal;
} data;

uniform sampler2D normalTexture;

vec3 GetNormalVector();

void main()
{
    vec3 N = GetNormalVector();
    vec3 color = N;
    color = pow(color, vec3(1.f / 2.2f));
    finalColor = vec4(color, 1.f);
}

vec3 GetNormalVector()
{
    vec3 result = normalize(data.normal);

    if (textureSize(normalTexture, 0).x > 1)
    {
        vec3 tangentNormal = texture(normalTexture, data.texCoord).xyz * 2.0 - 1.0;

        vec3 Q1  = dFdx(data.worldPosition);
        vec3 Q2  = dFdy(data.worldPosition);
        vec2 st1 = dFdx(data.texCoord);
        vec2 st2 = dFdy(data.texCoord);

        vec3 N   = normalize(data.normal);
        vec3 T  = normalize(Q1 * st2.t - Q2 * st1.t);
        vec3 B  = -normalize(cross(N, T));
        mat3 TBN = mat3(T, B, N);

        result = normalize(TBN * tangentNormal);
    }

    return result;
}
