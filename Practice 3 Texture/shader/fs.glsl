#version 450 core
in vec3 myColor;
in vec2 TexCoord;
out vec4 FragColor;

// sampler: 纹理对象的访问接口-采样器
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float mixVal = 0.2;

void main(){
    FragColor = mix(texture(tex1, TexCoord), texture(tex2, TexCoord), mixVal); // 线性插值
}