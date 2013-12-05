#version 400

uniform sampler2D u_terrainAttrTex;
uniform sampler2D u_fluxTex;
uniform float u_deltaT;
uniform float u_gridSpacingX = 1.0;
uniform float u_gridSpacingY = 1.0;

in vec2 texcoord;
out vec4 out_terrainAttrTex;

vec2 texSize = textureSize(u_terrainAttrTex,0);

float sampleFluxL( int offsetX, int offsetY)
{
	vec2 neighbourTexcoord = vec2(texcoord.s+ offsetX/texSize.s, texcoord.t + offsetY/texSize.t);
	
	return texture(u_fluxTex, neighbourTexcoord).r;
}

float sampleFluxR( int offsetX, int offsetY)
{
	vec2 neighbourTexcoord = vec2(texcoord.s+ offsetX/texSize.s, texcoord.t + offsetY/texSize.t);
	
	return texture(u_fluxTex, neighbourTexcoord).g;
}

float sampleFluxT( int offsetX, int offsetY)
{
	vec2 neighbourTexcoord = vec2(texcoord.s+ offsetX/texSize.s, texcoord.t + offsetY/texSize.t);
	
	return texture(u_fluxTex, neighbourTexcoord).b;
}

float sampleFluxB( int offsetX, int offsetY)
{
	vec2 neighbourTexcoord = vec2(texcoord.s+ offsetX/texSize.s, texcoord.t + offsetY/texSize.t);

	return texture(u_fluxTex, neighbourTexcoord).a;
}


void main (void)
{
	float deltaVolume = u_deltaT*( sampleFluxR(-1,0) + sampleFluxT(0,-1) + sampleFluxL(1,0) + sampleFluxB(0,1) 
		                           - sampleFluxR(0,0) -sampleFluxT(0,0) - sampleFluxL(0,0) - sampleFluxB(0,0) );
	out_terrainAttrTex = texture(u_terrainAttrTex,texcoord);
	out_terrainAttrTex.a = out_terrainAttrTex.g; // save d1
	out_terrainAttrTex.g += deltaVolume / (u_gridSpacingX*u_gridSpacingY);
}