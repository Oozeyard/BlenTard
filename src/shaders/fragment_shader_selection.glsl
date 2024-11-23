#version 330 core

out vec4 FragColor;

uniform vec3 objectColor; // Unique color for each object

void main() {
    FragColor = vec4(objectColor, 1.0); // Set the object color to the output color
}
