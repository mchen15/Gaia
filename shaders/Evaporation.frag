#version 400
// u_terrainAttrTex Channels
// r: terrain height
// g: water height
// b: sedimentation
// a: <placeholder for d1 to be used while extracing velocity from flux>
uniform sampler2D u_terrainAttrTex;
uniform float u_deltaT;
uniform float u_Ke = 0.01;

in vec2 v_Texcoord;
out vec4 out_terrainAttr;

void main (void)
{
	out_terrainAttr = texture(u_terrainAttrTex, v_Texcoord);
	out_terrainAttr.g = out_terrainAttr.g* ( 1.0 - u_Ke*u_deltaT);
	out_terrainAttr.g = max(0,out_terrainAttr.g);

	out_terrainAttr.a = 1.0;
}

