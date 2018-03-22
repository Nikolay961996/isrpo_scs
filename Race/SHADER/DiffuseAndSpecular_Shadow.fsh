#version 400 core

uniform	vec4 mAmbient;
uniform	vec4 mDiffuse;
uniform	vec4 mSpecular;

uniform	vec4 lAmbient;
uniform	vec4 lDiffuse;
uniform	vec4 lSpecular;
uniform	vec4 lPosition;

uniform	sampler2D tex0;
uniform	sampler2D tex1;
uniform	sampler2D ShadowMap;

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 ShadowTexCoord;

float	ShadowLookUp (void)
{
	if (ShadowTexCoord.z > 1.0) return 1.0;

	float rezult = 0.0;
	for (float s = -3.5; s <=3.5; s++) {
		for (float t = -3.5; t <=3.5; t++) {
			vec2	texcoord = vec2(ShadowTexCoord) + 0.0003 * vec2(s,t); 
			float	LightDepth = texture(ShadowMap,texcoord).r;
			if (ShadowTexCoord.z <= LightDepth) rezult += 1.0/64.0;
		}
	}
	return rezult; 
}

void main (void)
{
	vec3	n_Normal = normalize(Normal);
	vec3	n_ToLight = normalize(vec3(lPosition) - Position);
	vec3	n_ToEye = normalize (vec3(0,0,0) - Position);
	vec3	n_Reflect = normalize(reflect(-n_ToLight,n_Normal));

	float	Shadowed = ShadowLookUp(); 
	
	vec4	Ambient = texture (tex0, TexCoord) * lAmbient;  
	vec4	Diffuse = Shadowed * texture (tex0, TexCoord) * lDiffuse * max(dot(n_Normal, n_ToLight), 0.0f);
	vec4	Specular = Shadowed * texture (tex1, TexCoord) * lSpecular * pow(max(dot(n_ToEye, n_Reflect), 0.0f), 64.0f);

	vec3	Color = vec3(Diffuse + Ambient + Specular); 
	float	alpha = mDiffuse.a;
	
	gl_FragColor = vec4(Color, alpha);
}
