#version 400 core

uniform	vec4 mDiffuse;
uniform float mShiness;

uniform	vec4 lAmbient;
uniform	vec4 lDiffuse;
uniform	vec4 lSpecular;
uniform	vec4 lPosition;

uniform	sampler2D tex0;
uniform	sampler2D tex1;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

void main (void)
{
	vec3	n_Normal = normalize(Normal);
	vec3	n_ToLight = normalize(vec3(lPosition) - Position);
	vec3	n_ToEye = normalize (vec3(0,0,0) - Position);
	vec3	n_Reflect = normalize(reflect(-n_ToLight,n_Normal));
	
	vec4	Ambient = texture (tex0, TexCoord) * lAmbient;  
	vec4	Diffuse = texture (tex0, TexCoord) * lDiffuse * max(dot(n_Normal, n_ToLight), 0.0f);
	vec4	Specular = texture (tex1, TexCoord) * lSpecular * pow(max(dot(n_ToEye, n_Reflect), 0.0f), mShiness);

	vec3	Color = vec3(Diffuse + Ambient + Specular); 
	float	alpha = mDiffuse.a;
	
	gl_FragColor = vec4(Color, alpha);
}