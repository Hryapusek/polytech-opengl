#version 330 core

layout(location = 0) in vec3 aPos;    // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;    // Position of the light
uniform vec3 viewPos;     // Position of the camera

out vec3 FragPos;         // Position of the fragment in world space
out vec3 Normal;          // Normal at the fragment position

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal; // Correct normal direction
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
