#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientStrength;
uniform vec3 cameraPos;
float specularStrength = 1.5f;

void main()
{

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(texture(texture_specular1, TexCoord));

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 ambient = lightColor * ambientStrength;
    vec3 result = (ambient + diffuse + specular);
    vec4 color = vec4(result, 1.0f);
    vec4 tex_color = texture(texture_diffuse1, TexCoord);
    // vec4 color = vec4(TexCoord.x, TexCoord.y, 0.0, 1.0);
    if (color == vec4(0.0f, 0.0f, 0.0f, 0.0f))
    {
        color = vec4(0.7f, 0.3f, 0.2f, 0.0f);
    }
    FragColor = color * tex_color;
}