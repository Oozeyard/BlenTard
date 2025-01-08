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
uniform bool editMode;
uniform bool editable;
uniform vec3 objectColor;

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

    if (selected) {
        FragColor = vec4(1.0, 0.5, 0.0, 1.0);
        return;
    }

    if (editMode) {
        FragColor = vec4(objectColor, 1.0);
        return;
    }

    if (editable) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    FragColor = vec4(vec3(ambient + diffuse), 1.0);
}
