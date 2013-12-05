#version 400

uniform sampler2D u_terrainAttrTex;
uniform sampler2D u_velTex;
uniform float u_deltaT; 

in vec2 v_Texcoord;
out vec4 out_terrainAttr;

vec2 texSize = textureSize(u_terrainAttrTex,0).xy;

vec2 convertTexCoordToWorld(vec2 tex)
{
	return vec2(tex.x*texSize.x ,tex.y*texSize.y);
}

vec2 convertWorldCoordToTex(vec2 world)
{
	return vec2( world.x/texSize.x , world.y/texSize.y);
}

void main (void)
{
	vec2 worldPos = convertTexCoordToWorld(v_Texcoord.st);
	vec2 prevFrameWorldPos = worldPos - u_deltaT*( texture(u_velTex,v_Texcoord).xy);

	vec2 prevFrameTexCoord = convertWorldCoordToTex(prevFrameWorldPos);

	out_terrainAttr = texture(u_terrainAttrTex,v_Texcoord).rgba;
	out_terrainAttr.b = texture(u_terrainAttrTex, prevFrameTexCoord).b;
}

