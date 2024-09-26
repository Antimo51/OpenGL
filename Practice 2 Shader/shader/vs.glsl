#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
out vec3 vertexColor;
uniform vec2 shiftPos;

void main(){
    gl_Position = vec4(aPos.xy + shiftPos, aPos.z, 1.0);
    vertexColor = (gl_Position.xyz + vec3(1, 1, 1)) / 2.0;
}