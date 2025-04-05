#version 330 core

in vec3 FragPos;
in vec3 FragNorm;
in vec2 TextCoord;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D textureSrc;

out vec4 FragColor;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 norm = normalize(FragNorm);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // Colors
    vec4 phong = vec4((ambient + diffuse + specular), 1.0);
    vec4 textSample = texture(textureSrc, TextCoord);
    vec4 textureColor = textSample * phong;

    // Result
    vec4 finalColor = mix(phong, textureColor, textureColor.a); 
    FragColor = finalColor;
}