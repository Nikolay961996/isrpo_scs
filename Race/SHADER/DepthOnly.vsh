#version 400 core

uniform	mat4 uProjectionMatrix;
uniform	mat4 uModelViewMatrix;

layout(location = 0) in vec3 vPosition;

void main ()
{
	gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(vPosition, 1);
}
