#version 400

uniform vec3 u_up;
uniform sampler2D u_normalMap;
uniform sampler2D u_terrainAttrTex;
uniform float u_Kc = 0.001;
uniform float u_Ks = 0.001;
uniform sampler2D u_velTex;

in vec2 texcoord;
out vec4 out_terrainAttr;


void main (void)
{
	alpha = acos( dot( normalize(u_up), normalize( texture(u_normalMap, texcoord).xyz)));
	float C = u_Kc * sin(alpha)* length( texture(u_velTex,texcoord).xy);

	out_terrainAttr = texture(u_terrainAttrTex,texcoord).rgba;

	if ( C > out_terrainAttr.b)
	{
		float scaledDiff = u_Ks*(C - out_terrainAttr.b);
		out_terrainAttr.r = out_terrainAttr.r - scaledDiff;
		out_terrainAttr.b = out_terrainAttr.b + scaledDiff;
	}

	else
	{
		float scaledDiff = u_Ks*(out_terrainAttr.b-C);
		out_terrainAttr.r = out_terrainAttr.r + scaledDiff;
		out_terrainAttr.b = out_terrainAttr.b - scaledDiff;
	}
}

