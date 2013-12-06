#version 400

// u_terrainAttrTex Channels
// r: terrain height
// g: water height
// b: sedimentation
// a: <placeholder for d1 to be used while extracing velocity from flux>
uniform sampler2D u_terrainAttrTex;
uniform float u_deltaT;

in vec2 v_Texcoord;
out vec4 out_terrainAttr;

void main (void)
{
	float rainRate = 100.0;
	out_terrainAttr = texture(u_terrainAttrTex, v_Texcoord.st).rgba;
	out_terrainAttr.g += rainRate*u_deltaT;
	//out_terrainAttr.g += 0.0001;
	//out_terrainAttr = vec4(v_Texcoord.x, v_Texcoord.y, 0, 1);
	//out_terrainAttr = vec4(1, 0, 0, 1);
}