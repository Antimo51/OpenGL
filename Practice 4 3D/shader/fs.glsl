#version 450 core
in vec2 TexCoord;
in vec3 setCol;
out vec4 FragColor;

uniform sampler2D tex;
uniform samplerCube cubeTex;

void main(){
    FragColor = texture(tex, TexCoord);
    // FragColor = vec4(setCol, 1.0);
}