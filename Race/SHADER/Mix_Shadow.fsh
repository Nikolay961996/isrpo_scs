#version 330 core

uniform	vec4 mAmbient;
uniform	vec4 mDiffuse;

uniform	vec4 lAmbient;
uniform	vec4 lDiffuse;
uniform	vec4 lPosition;

uniform	sampler2D tex0;
uniform	sampler2D tex1;
uniform	sampler2D tex2;
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

	float	Mask = texture (tex0, TexCoord).r;
	Mask = min(max((-0.3f + Mask) * 2.5f, 0.0), 1.0);
	vec4	MixColor = texture (tex1, TexCoord * 20.0) * Mask + texture (tex2, TexCoord * 20.0) * (1.0 - Mask);

	float	Shadowed = ShadowLookUp(); 

	vec4	Ambient = mAmbient * lAmbient;  
	vec4	Diffuse = Shadowed * mDiffuse * lDiffuse * max(dot(n_Normal, n_ToLight), 0.0f);

	vec3	Color = vec3((Diffuse + Ambient) * MixColor); 
	float	alpha = 1.0;

	gl_FragColor = vec4(Color, alpha);
}
