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

	//const ivec3 off(-1.0,0.0,1.0);

	//float topLeft = sampleHeight( vec2( v_Texcoord + tSize*off.xz));
	//float top = sampleHeight( vec2 (v_Texcoord + tSize*off.yz));
	//float topRight = sampleHeight( vec2( v_Texcoord + tSize*off.zz));
	//float left = sampleHeight( vec2( v_Texcoord + tSize*off.xy));
	//float right = sampleHeight( vec2( v_Texcoord + tSize*off.zy));
	//float bottomLeft = sampleHeight( vec2( v_Texcoord + tSize*off.xx));
	//float bottom = sampleHeight( vec2( v_Texcoord + tSize*off.yx));
	//float bottomRight = sampleHeight( vec2( v_Texcoord + tSize*off.zx));


	//float dx = (topRight + 2.0*right+bottomRight) - (topLeft + 2.0*left+ bottomLeft);
	//float dy = (bottomLeft + 2.0*bottom+bottomRight) - (topLeft + 2.0*top+ topRight);
	//float dz = sqrt(1- dx*dx - dy*dy);
	//normal = normalize ( vec3(dx,dy,dz));


	color = vec4(normal.xyz,1.0);
}
