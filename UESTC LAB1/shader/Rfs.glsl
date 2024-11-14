#version 450 core
out vec4 FragColor;

uniform vec3 setCol;

void main() {
    FragColor = vec4(setCol, 1.0f);
}