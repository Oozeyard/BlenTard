#version 330 core

layout(location = 0) in vec3 aPos;

uniform vec3 color;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modelModel; // Model Matrix of the selected Node
uniform mat4 model; // Model Matrix of the Gizmo

out vec3 fragColor;

void main() {
    gl_Position = projection * view * (modelModel * model) * vec4(aPos, 1.0);
    fragColor = color;
}
