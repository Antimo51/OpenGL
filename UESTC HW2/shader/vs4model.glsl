#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 verticeCol;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

vec3 dvec;
float dist;

void main(){
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    dvec = (cameraPos - gl_Position.xyz);
    dist = sqrt(dvec.x*dvec.x + dvec.y*dvec.y + dvec.z*dvec.z) / 4;
    verticeCol = vec3(dist, 0.0, 1.0 - dist);
}
