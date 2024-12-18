#version 450 core
in vec2 UV;
out vec4 FragColor;

uniform sampler2D tex;

void main() {
    FragColor = texture(tex, UV);
}