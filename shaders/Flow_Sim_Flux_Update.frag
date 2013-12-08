#version 400

uniform sampler2D u_terrainAttrTex;
uniform sampler2D u_fluxTex;
uniform float u_deltaT;
uniform float u_virtualPipeArea = 1.0;
uniform float u_virtualPipeLength = 1.0;
uniform float u_gridSpacing = 1.0;
precision highp float;

in vec2 v_Texcoord;
out vec4 out_flux;

vec2 texSize = textureSize(u_terrainAttrTex,0);
const float gravity = 9.8;
const float EPSILON = 0.01;

float getFluxDelta(int offsetX, int offsetY)
{
	
	vec2 neighbourTexcoord = vec2(v_Texcoord.s + offsetX / texSize.s, v_Texcoord.t + offsetY / texSize.t);

	float terrainHeight = texture(u_terrainAttrTex, v_Texcoord).r;
	float waterHeight = texture(u_terrainAttrTex, v_Texcoord).g;
	
	float neighbourTerrainHeight = texture(u_terrainAttrTex, neighbourTexcoord).r;
	float neighbourWaterHeight = texture(u_terrainAttrTex, neighbourTexcoord).g;

	float deltaH = terrainHeight + waterHeight - neighbourTerrainHeight - neighbourWaterHeight;

	return u_deltaT*u_virtualPipeArea*gravity*deltaH/u_virtualPipeLength;
}

void main (void)
{
	out_flux = texture(u_fluxTex,v_Texcoord).xyzw;
	//Flux left
	out_flux.x = max(0, out_flux.x+getFluxDelta(-1,0));
	//Flux right
	out_flux.y = max(0,out_flux.y+getFluxDelta(1,0));
	// Flux top
	out_flux.z = max(0,out_flux.z+getFluxDelta(0,1));
	//Flux bottom
	out_flux.w = max(0,out_flux.w+getFluxDelta(0,-1));

	//boundary conditions
	if( v_Texcoord.s > 0 - EPSILON && v_Texcoord.s < 0 + EPSILON)
		out_flux.x = 0.0;

	else if (v_Texcoord.s > 1 - EPSILON && v_Texcoord.s < 1 + EPSILON)
		out_flux.y = 0.0;

	if (v_Texcoord.t > 0 - EPSILON && v_Texcoord.t < 0 + EPSILON)
		out_flux.w = 0.0;

	else if (v_Texcoord.t > 1 - EPSILON && v_Texcoord.t < 1 + EPSILON)
		out_flux.z = 0.0;

	float scaleFactor = 0.0;

	if ( (out_flux.r+out_flux.g+out_flux.b+out_flux.a) < 0.1 )
	{
		scaleFactor = 0.0;
	}
	else 
	{

	scaleFactor = min( 1.0, 
		(texture(u_terrainAttrTex,v_Texcoord).g*u_gridSpacing*u_gridSpacing) / ((out_flux.r+out_flux.g+out_flux.b+out_flux.a)*u_deltaT) );
	}
	out_flux = scaleFactor*out_flux;
}