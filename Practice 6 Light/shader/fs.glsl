#version 450 core
out vec4 FragColor;

in vec2 UV;
in vec3 Norm;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;        // ambient light

    vec3 norm = normalize(Norm);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;                   // diffuse light

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 300);
    vec3 specular = specularStrength * spec * lightColor; // specular light

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}