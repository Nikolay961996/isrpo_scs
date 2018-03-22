#version 400 core

uniform	mat4 uProjectionMatrix;
uniform	mat4 uModelViewMatrix;
uniform	mat4 uLightProjectionMatrix;
uniform	mat4 uLightModelViewMatrix;

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec3 ShadowTexCoord;

void main ()
{

	Position = vec3 (uModelViewMatrix * vec4 (vPosition, 1));
	Normal = vec3 (uModelViewMatrix * vec4 (vNormal, 0));
	TexCoord = vTexCoord;

	gl_Position = uProjectionMatrix * uModelViewMatrix * vec4 (vPosition, 1);

	vec4	LPosition =  uLightProjectionMatrix * uLightModelViewMatrix * vec4 (vPosition, 1); 
	LPosition = LPosition / LPosition.w;
	ShadowTexCoord = vec3(LPosition);
	ShadowTexCoord = 0.5f * ShadowTexCoord + 0.5f; 
}
