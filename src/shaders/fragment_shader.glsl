#version 330 core

struct Material {
    sampler2D texture_diffuse[16]; // 16 diffuse textures
};

uniform Material material;
uniform int numDiffuseTextures; // Number of diffuse textures
uniform bool selected;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// Light properties
uniform vec3 light_position;
uniform vec3 light_direction;

void main() {
    vec3 lightDir = normalize(light_position - Normal);
    float diff = max(dot(Normal, lightDir), 0.0);
    // vec3 diffuse = diff * texture(texture_diffuse1, TexCoords).rgb;

    vec4 diffuseColor = vec4(0.0);

    if (selected) {
        FragColor = vec4(1.0, 0.5, 0.0, 1.0);
        return;
    }

    if (numDiffuseTextures == 0) {
        FragColor = vec4(diff * vec3(0.8, 0.8, 0.8), 1.0);
        return;
    }

    // Manually handle each texture
    if (numDiffuseTextures > 0) diffuseColor += texture(material.texture_diffuse[0], TexCoords);
    if (numDiffuseTextures > 1) diffuseColor += texture(material.texture_diffuse[1], TexCoords);
    if (numDiffuseTextures > 2) diffuseColor += texture(material.texture_diffuse[2], TexCoords);
    if (numDiffuseTextures > 3) diffuseColor += texture(material.texture_diffuse[3], TexCoords);
    if (numDiffuseTextures > 4) diffuseColor += texture(material.texture_diffuse[4], TexCoords);
    if (numDiffuseTextures > 5) diffuseColor += texture(material.texture_diffuse[5], TexCoords);
    if (numDiffuseTextures > 6) diffuseColor += texture(material.texture_diffuse[6], TexCoords);
    if (numDiffuseTextures > 7) diffuseColor += texture(material.texture_diffuse[7], TexCoords);
    if (numDiffuseTextures > 8) diffuseColor += texture(material.texture_diffuse[8], TexCoords);
    if (numDiffuseTextures > 9) diffuseColor += texture(material.texture_diffuse[9], TexCoords);
    if (numDiffuseTextures > 10) diffuseColor += texture(material.texture_diffuse[10], TexCoords);
    if (numDiffuseTextures > 11) diffuseColor += texture(material.texture_diffuse[11], TexCoords);
    if (numDiffuseTextures > 12) diffuseColor += texture(material.texture_diffuse[12], TexCoords);
    if (numDiffuseTextures > 13) diffuseColor += texture(material.texture_diffuse[13], TexCoords);
    if (numDiffuseTextures > 14) diffuseColor += texture(material.texture_diffuse[14], TexCoords);
    if (numDiffuseTextures > 15) diffuseColor += texture(material.texture_diffuse[15], TexCoords);
    
    diffuseColor /= max(numDiffuseTextures, 1);

    FragColor = diffuseColor;
    // FragColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);
}
