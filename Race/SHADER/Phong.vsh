#version 400 core

uniform	mat4 uProjectionMatrix;
uniform	mat4 uModelViewMatrix;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

out vec3 Position;
out vec3 Normal;

void main ()
{
	vec4 temp = uModelViewMatrix * vec4(vPosition, 1);
	
	Position = vec3(temp);
	Normal = vec3 (uModelViewMatrix * vec4 (vNormal, 0));

	gl_Position = uProjectionMatrix * temp;
}
