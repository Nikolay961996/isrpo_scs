#version 400 core

uniform samplerCube tex0;

in vec3 TexCoord;

void main (void)
{
	vec4	TexColor = texture (tex0,TexCoord);
	gl_FragColor = TexColor;
}
