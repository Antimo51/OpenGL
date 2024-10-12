#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

out vec3 FragPos;
out vec3 Norm;
out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 inv;

void main(){
    FragPos = vec3(model * vec4(aPos, 1.0));
    Norm = vec3(inv * vec4(aNorm, 1.0));
    UV = aUV;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}