#version 330 core

struct Material {
    vec3 albedo;
    vec3 specular;
    vec3 emissive;
    float shininess;
    float metalness;
    float roughness;

    sampler2D texture_diffuse[4];
    sampler2D texture_normal[4];
    sampler2D texture_metallic[4];
    sampler2D texture_roughness[4];
};

uniform Material material;
uniform bool selected;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// Light properties
uniform vec3 light_position;
uniform vec3 light_direction;

void main() {

    // Ambient 
    vec3 ambient = 0.1 * material.albedo;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.albedo;

    // Specular
    vec3 viewDir = normalize(light_position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec;

    if (selected) {
        FragColor = vec4(1.0, 0.5, 0.0, 1.0);
        return;
    }

    FragColor = vec4(vec3(ambient + diffuse + specular), 1.0);
}
