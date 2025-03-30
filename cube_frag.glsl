#version 330 core

in vec3 FragPos;
in vec3 FragNorm;
in vec2 TextCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D textureSrc;

out vec4 FragColor;

void main()
{
    vec4 newLightColor = vec4(lightColor, 1.0);

    // Texturing
    vec4 objColor = vec4(objectColor, 1.0);
    vec4 textColor = texture(textureSrc, TextCoord);
    vec4 finalColor = mix(objColor, textColor, textColor.a);

    // Ambient
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * newLightColor;

    // Diffuse
    vec3 norm = normalize(FragNorm);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * newLightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec4 specular = specularStrength * spec * newLightColor;

    // Result
    vec4 result = (ambient + diffuse + specular) * finalColor;
    // FragColor = vec4(result, 1.0);
    FragColor = result;
}