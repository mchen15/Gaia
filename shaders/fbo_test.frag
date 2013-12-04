#version 400

uniform sampler2D u_terrainAttrTex;
uniform sampler2D u_fluxTex;
uniform sampler2D u_velocityTex;

in vec2 fs_Texcoord;

out vec4 fragColor;


vec4 sampleTerrainAttrTex(vec2 texcoord)
{
	return texture2D(u_terrainAttrTex, texcoord);
}

vec4 sampleFluxTex(vec2 texcoord)
{
	return texture2D(u_fluxTex, texcoord);
}

vec4 sampleVelocityTex(vec2 texcoord)
{
	return texture2D(u_velocityTex, texcoord);
}

void main(void)
{
	fragColor = sampleTerrainAttrTex(fs_Texcoord);
}
