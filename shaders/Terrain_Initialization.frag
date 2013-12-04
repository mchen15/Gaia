#version 400

in vec3 fs_Position;

// using layout to enforce the location of each out
layout (location = 0) out vec4 out_flux;
layout (location = 1) out vec4 out_terrainAttr;
layout (location = 2) out vec4 out_velocity;

void main (void)
{
	out_flux = vec4(1,0,0,1);
	out_terrainAttr = vec4(0,1,0,1);
	out_velocity = vec4(0,0,1,1);
}