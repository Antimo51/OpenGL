#version 450 core
in vec3 myColor;
in vec2 TexCoord;
out vec4 FragColor;

// sampler: 纹理对象的访问接口-采样器
uniform sampler2D myTexture;

void main(){
    FragColor = texture(myTexture, TexCoord) * vec4(myColor, 1.0);
}