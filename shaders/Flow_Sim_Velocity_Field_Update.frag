#version 400

uniform sampler2D u_terrainAttrTex;
uniform sampler2D u_fluxTex;
uniform sampler2D u_velTex;
uniform float u_deltaT;
uniform float u_gridSpacing = 1.0;

in vec2 v_Texcoord;
out vec4 out_vel;

vec2 texSize = textureSize(u_terrainAttrTex,0);

float sampleFluxL( int offsetX, int offsetY)
{
	vec2 neighbourTexcoord = vec2(v_Texcoord.s+ offsetX/texSize.s, v_Texcoord.t + offsetY/texSize.t);
	
	return texture(u_fluxTex, neighbourTexcoord).r;
}

float sampleFluxR( int offsetX, int offsetY)
{
	vec2 neighbourTexcoord = vec2(v_Texcoord.s+ offsetX/texSize.s, v_Texcoord.t + offsetY/texSize.t);
	
	return texture(u_fluxTex, neighbourTexcoord).g;
}

float sampleFluxT( int offsetX, int offsetY)
{
	vec2 neighbourTexcoord = vec2(v_Texcoord.s+ offsetX/texSize.s, v_Texcoord.t + offsetY/texSize.t);
	
	return texture(u_fluxTex, neighbourTexcoord).b;
}

float sampleFluxB( int offsetX, int offsetY)
{
	vec2 neighbourTexcoord = vec2(v_Texcoord.s+ offsetX/texSize.s, v_Texcoord.t + offsetY/texSize.t);

	return texture(u_fluxTex, neighbourTexcoord).a;
}

void main (void)
{
	float avHeight = (texture(u_terrainAttrTex,v_Texcoord).a+ texture(u_terrainAttrTex,v_Texcoord).g)/2.0;  // (d1+d2) average
	float deltaWx =  (sampleFluxR(-1,0) - sampleFluxL(0,0) + sampleFluxR(0,0) - sampleFluxL(1,0))/2.0;
	float deltaWy = (sampleFluxT(0,-1) - sampleFluxB(0,0) + sampleFluxT(0,0) - sampleFluxB(0,1))/2.0;
	
	float velX = 0.0;
	float velY = 0.0;
	if(avHeight == 0)
	{
		velX = 0.0;
		velY = 0.0;
	}

	else
	{
	velX = deltaWx/ (u_gridSpacing*avHeight);
	velY = deltaWy/ (u_gridSpacing*avHeight);
	}
	out_vel = vec4(velX,velY,0.0,1.0);
}