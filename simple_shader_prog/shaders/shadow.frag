#version 330 core

in vec3 FragPos;    // Position of the fragment
in vec3 Normal;     // Normal at the fragment

uniform vec3 lightPos;   // Position of the light
uniform vec3 lightColor; // Color of the light
uniform vec3 objectColor; // Color of the object
uniform vec3 viewPos;    // Position of the camera

out vec4 FragColor;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.3; // Increase ambient light
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Final color
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
