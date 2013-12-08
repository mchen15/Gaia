#version 400

uniform vec3 u_up;
uniform sampler2D u_terrainAttrTex;
uniform sampler2D u_velTex;
uniform sampler2D u_normalMap;
uniform float u_Kc = 0.001;
uniform float u_Ks = 0.001;


in vec2 v_Texcoord;
out vec4 out_terrainAttr;

float sampleHeight(vec2 coord)
{
	return texture(u_terrainAttrTex, coord).r;
}

vec3 getNormalSobel()
{
	const ivec3 off = ivec3(-1.0,0.0,1.0);
	vec2 tSize = 1.0/textureSize(u_terrainAttrTex,0);
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


void main (void)
{
	float alpha = acos( dot( normalize(u_up), getNormalSobel() ));
	float C = u_Kc * sin(alpha)* length( texture(u_velTex,v_Texcoord).xy);

	out_terrainAttr = texture(u_terrainAttrTex,v_Texcoord).rgba;

	if ( C > out_terrainAttr.b)
	{
		float scaledDiff = u_Ks*(C - out_terrainAttr.b);
		out_terrainAttr.r = out_terrainAttr.r - scaledDiff;
		out_terrainAttr.b = out_terrainAttr.b + scaledDiff;
		out_terrainAttr.r = max(0,out_terrainAttr.r);
	}
	else
	{
		float scaledDiff = u_Ks*(out_terrainAttr.b-C);
		out_terrainAttr.r = out_terrainAttr.r + scaledDiff;
		out_terrainAttr.b = out_terrainAttr.b - scaledDiff;
		out_terrainAttr.b = max(0,out_terrainAttr.b);
	}
}

