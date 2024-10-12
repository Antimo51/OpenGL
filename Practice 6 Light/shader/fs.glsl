#version 450 core
out vec4 FragColor;

in vec2 UV;
in vec3 Norm;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main(){
    // FragColor = texture(texture_diffuse1, UV);
    // FragColor = vec4(objectColor, 1.0);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;        // ambient light

    vec3 norm = normalize(Norm);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;                   // diffuse light

    vec3 result = (ambient + diffuse) * objectColor;
    // vec3 result = ambient * objectColor; // for debug
    FragColor = vec4(result, 1.0);
}