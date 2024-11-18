#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

// Textures
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

// Lumière attachée à la caméra
uniform vec3 light_position;
uniform vec3 light_direction;
uniform vec3 light_color;

// Spotlight configuration
uniform float cutoff;
uniform float outerCutoff;

// Vue
uniform vec3 viewPos;

void main() {
    vec3 normal = normalize(Normal);

    // Direction de la lumière
    vec3 lightDir = normalize(light_position - FragPos);
    float theta = dot(lightDir, normalize(-light_direction));
    float epsilon = cutoff - outerCutoff;
    float intensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

    // Lumière ambiante
    vec3 ambient = 0.5 * texture(texture_diffuse1, TexCoords).rgb; // Augmentez à 0.2 ou plus si nécessaire

    // Lumière diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = intensity * diff * texture(texture_diffuse1, TexCoords).rgb;

    // Lumière spéculaire
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = intensity * spec * light_color;

    // Couleur finale
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
