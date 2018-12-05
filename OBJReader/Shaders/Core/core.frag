#version 330 core

in vec2 TexCoord;
in vec3 norm;
in vec3 fragPos;

out vec4 color;

uniform sampler2D texture1;

uniform float ambientStrength = 0.2;

uniform vec3 lightColor;

uniform vec3 kd; 
uniform vec3 ka;
uniform vec3 ks;
uniform float ns;
uniform float ni;

uniform vec3 lightPos;

uniform vec3 viewPos;


void main(){
	
    vec3 ambient = ambientStrength * lightColor;

	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(lightPos - fragPos);  

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec3 specular = ns * spec * lightColor;  

	vec4 tex1 = texture( texture1, TexCoord );
    vec3 result = (ambient + diffuse + specular) * kd;
    color = vec4(result, 1.0)*tex1;


}