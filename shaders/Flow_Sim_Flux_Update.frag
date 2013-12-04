#version 400

uniform sampler2D u_terrainAttrTex;
uniform sampler2D u_fluxTex;
uniform float u_deltaT;
uniform float u_virtualPipeArea = 1.0;
uniform float u_virtualPipeLength = 1.0;
uniform float u_gridSpacingX = 1.0;
uniform float u_gridSpacingY = 1.0;

in vec2 texcoord;
out vec4 out_flux;

vec2 texSize = textureSize(u_terrainAttrTex,0);
float gravity = -9.8;
const float EPSILON = 0.000001;

float getFluxDelta(int offsetX, int offsetY)
{
	
	vec2 neighbourTexcoord = vec2(texcoord.s+ offsetX/texSize.s, texcoord.t + offsetY/texSize.t);

	float terrainHeight = texture(u_terrainAttrTex,texcoord).r;
	float waterHeight = texture(u_terrainAttrTex,texcoord).g;
	
	float neighbourTerrainHeight = texture(u_terrainAttrTex, neighbourTexcoord).r;
	float neighbourWaterHeight = texture(u_terrainAttrTex, neighbourTexcoord).g;

	float deltaH = terrainHeight + waterHeight - neighbourTerrainHeight - neighbourWaterHeight;

	return u_deltaT*u_virtualPipeArea*gravity*deltaH/u_virtualPipeLength;
}

void main (void)
{
	vec4 out_flux = texture(u_fluxTex,0).xyzw;
	//Flux left
	out_flux.x = max(0, flux.x+getFluxDelta(-1,0));
	//Flux right
	out_flux.y = max(0,flux.y+getFluxDelta(1,0));
	// Flux top
	out_flux.z = max(0,flux.z+getFluxDelta(0,1));
	//Flux bottom
	out_flux.w = max(0,flux.w+getFluxDelta(0,-1));

	//boundary conditions
	//if( texcoord.s <= 0 + EPSILON)
	//	out_flux.x = 0.0;

	//else if (texcoord.s >= 1 - EPSILON)
	//	out_flux.y = 0.0;

	//if (texcoord.t <= 0+EPSILON)
	//	out_flux.z = 0.0;

	//else if (texcoord.t >= 1-EPSILON)
	//	out_flux.w = 0.0;

	float scaleFactor = min( 1.0, 
		(texture(u_terrainAttrTex,texcoord).g*u_gridSpacingX*u_gridSpacingY) / ((out_flux.r+out_flux.g+out_flux.b+out_flux.a)*u_deltaT) );
	
	out_flux = scaleFactor*out_flux;
}