#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    vec3 ambiant = 0.8f * vec3(0.65f, 0.65f, 0.65f);

    FragColor =vec4(ambiant * vec3(texture(skybox, TexCoords)), 1.0f);
}