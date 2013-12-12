#version 400

#define RAIN 0
#define LAKE 1
#define NOSOURCE 2
#define TERRAINMOD 3
// u_terrainAttrTex Channels
// r: terrain height
// g: water height
// b: sedimentation
// a: <placeholder for d1 to be used while extracing velocity from flux>
uniform sampler2D u_terrainAttrTex;
uniform float u_deltaT;
uniform int u_waterSrc;
uniform vec2 u_manipCenter;
uniform float u_manipRadius;
uniform float u_randomSeed;

in vec2 v_Texcoord;
out vec4 out_terrainAttr;

float manipRadiusSq = u_manipRadius*u_manipRadius;

float rand(vec2 co){
  return fract(sin(u_randomSeed*dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


void main (void)
{ 
	out_terrainAttr = texture(u_terrainAttrTex, v_Texcoord.st).rgba;
	if ( u_waterSrc == LAKE)
	{
		vec2 dist = v_Texcoord-u_manipCenter;
		if( dist.x*dist.x + dist.y*dist.y < manipRadiusSq)
			out_terrainAttr.g += 0.1;
	}

	else if( u_waterSrc == RAIN)
	{
		float rainRate = 0.005;
		float r = rand(v_Texcoord);
		if ( r>0.8)
		{
			out_terrainAttr.g += u_deltaT*rainRate;
		}
	}

	else if ( u_waterSrc == TERRAINMOD)
	{
		vec2 dist = v_Texcoord-u_manipCenter;
		float distMagSq = dist.x*dist.x + dist.y*dist.y;
		if( distMagSq < manipRadiusSq)
		{
			float r = distMagSq/manipRadiusSq;
			float gaussianFalloff = (1-r)*(1-r);
			out_terrainAttr.r += 0.1*gaussianFalloff;
		}
	}
	//vec2 dist = v_Texcoord-manipCenter;
	//if( dist.x*dist.x + dist.y*dist.y < manipRadiusSq)
	//	out_terrainAttr.g += 1.0;
	//out_terrainAttr.g += 0.0001;
	//out_terrainAttr = vec4(v_Texcoord.x, v_Texcoord.y, 0, 1);
	//out_terrainAttr = vec4(1, 0, 0, 1);
}