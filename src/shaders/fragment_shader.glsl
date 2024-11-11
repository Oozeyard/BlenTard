#version 330 core

in vec3 Normal;

out vec4 FragColor;

uniform vec3 ligth_position;
uniform vec3 ligth_direction;

void main() {
    vec3 lightDir = normalize(ligth_position - Normal);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);

    FragColor = vec4(diffuse, 1.0);
    // FragColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);
}
