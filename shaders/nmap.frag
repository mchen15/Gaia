#version 400

in vec2 v_Texcoord
uniform sampler2D u_heightMap;
uniform float u_heightScale;

out vec4 color;

float sampleHeight(vec2 texcoord)
{
	return u_heightScale*texture(u_heightScale, texcoord).r;
}

void main(void)
{	
	vec2 tSize = 1.0/textureSize(u_heightScale,0);

	float xnext = sampleHeight(vec2(v_Texcoord.s+tSize.s,v_Texcoord.t));
	float xprev = sampleHeight(vec2(v_Texcoord.s-tSize.s,v_Texcoord.t));
	float ynext = sampleHeight(vec2(v_Texcoord.s,v_Texcoord.t+tSize.t));
	float yprev = sampleHeight(vec2(v_Texcoord.s,v_Texcoord.t-tSize.t));
	vec3 ddx = (2.0, 0.0, xnext-xprev);
	vec3 ddy = (0.0, 2.0, ynext-yprev);
	vec3 normal = cross ( normalize(ddx), normalize(ddy));
	color = vec4(normal.xyz,1.0);
}
