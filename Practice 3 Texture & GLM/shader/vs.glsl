#version 450 core
layout (location = 0) in vec3 aPos;         // location对应于point, color, texture coord的位置, 详见main.cpp中的三个attribute
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 myColor;
out vec2 TexCoord;
uniform mat4 transform;

void main(){
    gl_Position = transform * vec4(aPos, 1.0);
    myColor = aColor;
    TexCoord = aTexCoord;
}