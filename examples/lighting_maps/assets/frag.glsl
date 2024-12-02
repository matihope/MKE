#version 410 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct LightProperties {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform LightProperties light;
uniform Material material;

uniform vec3 lightPos;
uniform vec3 camPos;

void main() {
    vec3 texel = vec3(texture(material.diffuse, texCoords));
    vec3 specular_texel = vec3(texture(material.specular, texCoords));

    // Ambient light
    vec3 ambient = texel * light.ambient;

    // Diffuse light
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float angle = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * texel;

    // Specular light
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = specular_texel * spec * light.specular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
