#version 450 core
out vec4 FragColor;

in vec2 UV;
in vec3 Norm;
in vec3 FragPos;

uniform sampler2D texture_diffuse1; // 需要光照类型不使用texture: 重学一下model这一节

void main(){
    FragColor = vec4(1.0);
}