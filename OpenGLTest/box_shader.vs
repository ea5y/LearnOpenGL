#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct Output
{
    vec3 Normal;
    vec2 TexCoords;
    vec3 FragPos;
};
out Output output;

void main()
{
   gl_Postion = projection * view * model * vec4(position, 1.0f); 
   output.Normal = mat3(transpose(inverse(model))) * normal;
   output.TexCoords = texCoords;
   output.FragPos = vec3(model * vec4(position, 1.0f));
}
