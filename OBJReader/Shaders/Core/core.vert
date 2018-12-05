#version 330 core

layout ( location = 0 ) in vec3 vPos;
layout ( location = 1 ) in vec2 vText;
layout ( location = 2 ) in vec3 vNorm;

out vec2 TexCoord;
out vec3 norm;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vPos, 1.0);
	
	fragPos = vec3(model * vec4(vPos,1.0));

	TexCoord = vec2( vText.x, 1.0 - vText.y );

	norm =  mat3(transpose(inverse(model))) * vNorm;
}