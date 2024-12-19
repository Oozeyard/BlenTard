#version 330 core

layout(location = 0) in vec3 position;
layout(location = 5) in vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 objectColor;

out vec3 color;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    if (length(objectColor - vec3(0.0, 0.0, 0.0)) < 0.001) {
        color = vertexColor;
    } else {
        color = objectColor;
    }
}
