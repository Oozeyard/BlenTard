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

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Material material;
uniform int numDiffuseTextures; // Number of diffuse textures
uniform int numNormalTextures; // Number of normal textures
uniform int numMetalicTextures; // Number of normal textures
uniform int numRoughnessTextures; // Number of normal textures
uniform bool selected;

uniform Light light;
uniform mat4 lightSpaceMatrix;
uniform sampler2D shadowMap;

// Inputs from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

out vec4 FragColor;

// Function to calculate shadow
float computeShadow(vec4 fragPosLightSpace) {
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Prevent shadow outside light's range
    if (projCoords.z > 1.0) return 0.0;

    // Check if fragment is in shadow
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    // Bias to prevent shadow acne
    float bias = max(0.005 * (1.0 - dot(Normal, normalize(light.position - FragPos))), 0.0005);

    // Check if fragment is in shadow
    return currentDepth - bias > closestDepth ? 1.0 : 0.0;

}

void main() {
    if (selected) {
        FragColor = vec4(1.0, 0.5, 0.0, 1.0);
        return;
    }

    // Ambient light
    vec3 ambient = 0.1 * material.albedo * light.color * light.intensity;

    // Diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * material.albedo * light.color * light.intensity;

    // Specular light
    vec3 viewDir = normalize(-FragPos); // Assume camera at origin
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.color * light.intensity;

    // Calculate shadow factor
    float shadow = computeShadow(FragPosLightSpace);

    // Combine results
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));

    // Handle textures
    vec4 diffuseColor = vec4(1.0);
    if (numDiffuseTextures > 0) diffuseColor += texture(material.texture_diffuse[0], TexCoords);
    if (numDiffuseTextures > 1) diffuseColor += texture(material.texture_diffuse[1], TexCoords);
    if (numDiffuseTextures > 2) diffuseColor += texture(material.texture_diffuse[2], TexCoords);
    if (numDiffuseTextures > 3) diffuseColor += texture(material.texture_diffuse[3], TexCoords);
    diffuseColor /= max(numDiffuseTextures, 1);

    FragColor = vec4(lighting, 1.0) * diffuseColor;
}
