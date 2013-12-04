#version 400

uniform sampler2D u_terrainAttrTex;
uniform float deltaT;

in vec2 texcoord;
out vec4 out_terrainAttr;

void main (void)
{
	float rainRate = 1.0;
	out_terrainAttr = texture(u_terrainAttrTex,texcoord.st).rgba;
	out_terrainAttr.g += rainRate*deltaT;
}