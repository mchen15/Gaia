#version 400

uniform sampler2D u_heightMap;
uniform float u_heightScale;

in vec3 fs_Position;
in vec2 fs_Texcoord;

// using layout to enforce the location of each out
layout (location = 0) out vec4 out_flux;
layout (location = 1) out vec4 out_terrainAttr;
layout (location = 2) out vec4 out_velocity;

vec4 sampleHeightMap(vec2 texcoord)
{
	return texture(u_heightMap, texcoord);
}

void main (void)
{
	vec4 height = u_heightScale * sampleHeightMap(fs_Texcoord);

	out_flux = vec4(0,0,0,1);
	out_terrainAttr = vec4(height.x,0,0,1);
	out_velocity = vec4(0,0,0,1);
}