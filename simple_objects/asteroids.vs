#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * aInstanceMatrix * model * vec4(aPos, 1.0f); 
    FragPos = vec3(model* aInstanceMatrix * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(model* aInstanceMatrix))) * normal;
}