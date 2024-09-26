#version 330 core
in vec3 vertexColor;
out vec4 FragColor;
uniform vec3 setColor;

void main(){
    vec3 tmpCol = vertexColor;
    // tmpCol.x = tmpCol.x>1? tmpCol.x-1: tmpCol.x;
    // tmpCol.y = tmpCol.y>1? tmpCol.y-1: tmpCol.y;
    // tmpCol.z = tmpCol.z>1? tmpCol.z-1: tmpCol.z;
    FragColor = vec4(tmpCol, 1.0);
}