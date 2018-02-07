#version 400 core

uniform	vec4 mAmbient;
uniform	vec4 mDiffuse;

uniform	vec4 lAmbient;
uniform	vec4 lDiffuse;
uniform	vec4 lPosition;

uniform	sampler2D tex0;
uniform	sampler2D tex1;
uniform	sampler2D tex2;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

void main (void)
{
	vec3	n_Normal = normalize(Normal);
	vec3	n_ToLight = normalize(vec3(lPosition) - Position);

	float	Mask = texture (tex0, TexCoord).r;
	Mask = min(max((-0.3f + Mask) * 2.5f, 0.0), 1.0);
	vec4	MixColor = texture (tex1, TexCoord * 20.0) * Mask + texture (tex2, TexCoord * 20.0) * (1.0 - Mask);
	
	vec4	Ambient = mAmbient * lAmbient;  
	vec4	Diffuse = mDiffuse * lDiffuse * max(dot(n_Normal, n_ToLight), 0.0f);

	vec3	Color = vec3((Diffuse + Ambient) * MixColor); 
	float	alpha = 1.0;

	gl_FragColor = vec4(Color, alpha);
}
