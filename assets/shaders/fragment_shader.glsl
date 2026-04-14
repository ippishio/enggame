#version 330 core
out vec4 FragColor;

#define MAX_LIGHTS 8

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
struct PointLight
{
    vec3 position;
    vec3 color;
    float ambientStrength;
};

uniform int lightCount;
uniform PointLight lights[MAX_LIGHTS];
uniform vec3 cameraPos;
uniform vec3 skyLightColor;
uniform vec3 groundLightColor;
uniform vec3 sunDirection;
uniform vec3 sunColor;
uniform float sunStrength;
const float specularStrength = 0.35f;

void main()
{
    vec3 albedo = vec3(texture(texture_diffuse1, TexCoord));
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    float skyFactor = clamp(norm.y * 0.5f + 0.5f, 0.0f, 1.0f);
    vec3 hemisphericLight = mix(groundLightColor, skyLightColor, skyFactor) * albedo;
    vec3 lighting = hemisphericLight;

    vec3 sunDir = normalize(-sunDirection);
    vec3 sunReflect = reflect(-sunDir, norm);
    float sunDiffuse = max(dot(norm, sunDir), 0.0f);
    float sunSpecular = pow(max(dot(viewDir, sunReflect), 0.0f), 32.0f);
    lighting += sunStrength * (sunDiffuse * sunColor * albedo + specularStrength * sunSpecular * sunColor);

    for (int i = 0; i < lightCount; i++)
    {
        vec3 lightDir = normalize(lights[i].position - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);

        float diff = max(dot(norm, lightDir), 0.0f);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
        float distanceToLight = length(lights[i].position - FragPos);
        float attenuation = 1.0f / (1.0f + 0.09f * distanceToLight + 0.032f * distanceToLight * distanceToLight);

        vec3 ambient = lights[i].ambientStrength * lights[i].color * albedo;
        vec3 diffuse = diff * lights[i].color * albedo;
        vec3 specular = specularStrength * spec * lights[i].color;
        lighting += (ambient + diffuse + specular) * attenuation;
    }

    if (lightCount == 0)
    {
        lighting = hemisphericLight + sunStrength * sunDiffuse * sunColor * albedo;
    }

    FragColor = vec4(lighting, 1.0f);
}
