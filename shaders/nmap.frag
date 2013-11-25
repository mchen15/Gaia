#version 400

in vec2 v_Texcoord;
uniform sampler2D u_heightMap;
uniform sampler2D u_diffuseMap;
uniform sampler2D u_normalMap;
uniform float u_heightScale;

uniform int u_toggleNormal;

out vec4 color;

float sampleHeight(vec2 texcoord)
{
	return u_heightScale*texture(u_heightMap, texcoord).r;
}

vec3 getNormalSobel()
{
	const ivec3 off = ivec3(-1.0,0.0,1.0);
	vec2 tSize = 1.0/textureSize(u_heightMap,0);
	float topLeft = sampleHeight( vec2( v_Texcoord + tSize*off.xz));
	float top = sampleHeight( vec2 (v_Texcoord + tSize*off.yz));
	float topRight = sampleHeight( vec2( v_Texcoord + tSize*off.zz));
	float left = sampleHeight( vec2( v_Texcoord + tSize*off.xy));
	float right = sampleHeight( vec2( v_Texcoord + tSize*off.zy));
	float bottomLeft = sampleHeight( vec2( v_Texcoord + tSize*off.xx));
	float bottom = sampleHeight( vec2( v_Texcoord + tSize*off.yx));
	float bottomRight = sampleHeight( vec2( v_Texcoord + tSize*off.zx));


	float dx = (topLeft + 2.0*left+ bottomLeft) - (topRight + 2.0*right+bottomRight) ;
	float dy = (topLeft + 2.0*top+ topRight) - (bottomLeft + 2.0*bottom+bottomRight);
	float dz = 4.0;

	vec3 normal = normalize ( vec3(dx,dy,dz));
	return normal;

}

vec3 getNormalCentralDifferences()
{
	vec2 tSize = 1.0/textureSize(u_heightMap,0);
	float xnext = sampleHeight(vec2(v_Texcoord.s+tSize.s,v_Texcoord.t));
	float xprev = sampleHeight(vec2(v_Texcoord.s-tSize.s,v_Texcoord.t));
	float ynext = sampleHeight(vec2(v_Texcoord.s,v_Texcoord.t+tSize.t));
	float yprev = sampleHeight(vec2(v_Texcoord.s,v_Texcoord.t-tSize.t));
	vec3 ddx = vec3(2.0*tSize.x, 0.0, xnext-xprev);
	vec3 ddy = vec3(0.0, 2.0*tSize.y, ynext-yprev);
	vec3 normal = cross ( normalize(ddx), normalize(ddy));
	return normal;
}


void main(void)
{	

	vec3 colorNormal= vec3(0.0);

	if( u_toggleNormal== 0)
	{
		vec3 normal = getNormalSobel();
		colorNormal = normal*0.5+0.5;
	}

	else
	{
		vec3 normal = texture(u_normalMap, v_Texcoord).xyz;
		colorNormal = normal;
	}

	color = vec4(colorNormal,1.0);
	//color = vec4(texture(u_diffuseMap,v_Texcoord).rgb,1);
}
