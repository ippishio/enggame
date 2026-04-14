#version 330 core

out vec4 FragColor;

in vec3 SkyDirection;

uniform sampler2D skyboxTexture;

vec2 sampleHorizontalCross(vec3 direction)
{
    vec3 absDir = abs(direction);
    vec2 uv;
    vec2 cell;

    if (absDir.x >= absDir.y && absDir.x >= absDir.z)
    {
        if (direction.x > 0.0)
        {
            uv = vec2(-direction.z, direction.y) / absDir.x;
            cell = vec2(2.0, 1.0);
        }
        else
        {
            uv = vec2(direction.z, direction.y) / absDir.x;
            cell = vec2(0.0, 1.0);
        }
    }
    else if (absDir.y >= absDir.x && absDir.y >= absDir.z)
    {
        if (direction.y > 0.0)
        {
            uv = vec2(direction.x, -direction.z) / absDir.y;
            cell = vec2(1.0, 0.0);
        }
        else
        {
            uv = vec2(direction.x, direction.z) / absDir.y;
            cell = vec2(1.0, 2.0);
        }
    }
    else
    {
        if (direction.z > 0.0)
        {
            uv = vec2(direction.x, direction.y) / absDir.z;
            cell = vec2(1.0, 1.0);
        }
        else
        {
            uv = vec2(-direction.x, direction.y) / absDir.z;
            cell = vec2(3.0, 1.0);
        }
    }

    uv = uv * 0.5 + 0.5;
    return (cell + uv) / vec2(4.0, 3.0);
}

void main()
{
    vec2 atlasUV = sampleHorizontalCross(normalize(SkyDirection));
    FragColor = texture(skyboxTexture, atlasUV);
}
