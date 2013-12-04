#version 400

// u_terrainAttrTex Channels
// r: terrain height
// g: water height
// b: sedimentation
// a: <placeholder for d1 to be used while extracing velocity from flux>
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