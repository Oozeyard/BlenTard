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
uniform int numDiffuseTextures; // Number of diffuse textures
uniform int numNormalTextures; // Number of normal textures
uniform int numMetalicTextures; // Number of normal textures
uniform int numRoughnessTextures; // Number of normal textures

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

    // Specular
    vec3 viewDir = normalize(light_position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec;

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

    // Manually handle each texture
    if (numDiffuseTextures == 0) {
        FragColor = vec4(vec3(ambient + diffuse + specular), 1.0);
        return;
    }
    vec4 diffuseColor = vec4(0.0);
    if (numDiffuseTextures > 0) diffuseColor += texture(material.texture_diffuse[0], TexCoords);
    if (numDiffuseTextures > 1) diffuseColor += texture(material.texture_diffuse[1], TexCoords);
    if (numDiffuseTextures > 2) diffuseColor += texture(material.texture_diffuse[2], TexCoords);
    if (numDiffuseTextures > 3) diffuseColor += texture(material.texture_diffuse[3], TexCoords);
    diffuseColor /= max(numDiffuseTextures, 1);

    FragColor = diffuseColor;
}
