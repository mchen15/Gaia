#version 400

uniform sampler2D u_terrainAttrTex;
uniform sampler2D u_velTex;
uniform float u_deltaT; 

in vec2 texcoord;
out vec4 out_terrainAttr;

vec2 texSize = textureSize(u_terrainAttrTex).xy;

vec2 convertTexCoordToWorld(vec2 tex)
{
	return vec2(tex.u*texSize.x ,tex.v*texSize.y);
}

vec2 convertWorldCoordToTex(vec2 world)
{
	return vec2( world.x/texSize.x , world.y/texSize.y);
}

void main (void)
{
	vec2 worldPos = convertTexCoordToWorld(texcoord.st);
	vec2 prevFrameWorldPos = worldPos - u_deltaT*( texture(u_velTex,texcoord).xy);

	vec2 prevFrameTexCoord = convertWorldCoordToTex(prevFrameWorldPos);

	out_terrainAttr = texture(u_terrainAttrTex,texcoord).rgba;
	out_terrainAttr.b = texture(u_terrainAttrTex, prevFrameTexCoord).b;
}

