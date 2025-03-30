#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalModel;

out vec3 FragPos;
out vec3 FragNorm;
out vec2 TextCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    FragPos = vec3(model * vec4(aPos, 1.0));
    // FragNorm = mat3(transpose(inverse(model))) * aNormal;
    FragNorm = normalModel * aNormal;
    TextCoord = aTexCoord;
}