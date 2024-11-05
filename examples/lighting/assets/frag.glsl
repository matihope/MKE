#version 410 core

in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 camPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;


void main() {
    // Ambient light
    vec3 ambient = material.ambient * lightColor;

    // Diffuse light
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float angle = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (angle * material.diffuse);

    // Specular light
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * lightColor;


    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
